#include "settingspage.h"
#include "ui_settingspage.h"
#include "settingsmanager.h"
#include "databasemanager.h"
#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QAbstractAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsDropShadowEffect>

#include <QCryptographicHash>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QDateTime>
#include <QInputDialog>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>

#include <QDialog>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QSqlQuery>
#include <QSqlError>

/* ─────────────────────────────────────────────
 *  CONSTRUCTEUR / DESTRUCTEUR
 * ───────────────────────────────────────────── */

SettingsPage::SettingsPage(UserRole userRole,
                           const QString &currentUser,
                           QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SettingsPage)
    , m_userRole(userRole)
    , m_currentUser(currentUser)
{
    ui->setupUi(this);

    setupUI();
    setupConnections();
    updateUIForRole();
    updateNotificationsForRole();
    updateAboutSection();
    loadUserData();

    // Live update thème & langue (globaux)
    connect(&ThemeManager::instance(), &ThemeManager::themeChanged,
            this, [this](AppTheme){
                ThemeManager::instance().applyTheme(window());
            });

    connect(&LanguageManager::instance(), &LanguageManager::languageChanged,
            this, [this](const QString &){
                if (ui) ui->retranslateUi(this);
                updateUIForRole();
                updateNotificationsForRole();
                updateAboutSection();
            });

    // Page par défaut : Profil
    if (ui->settingsStackedWidget)
        ui->settingsStackedWidget->setCurrentIndex(0);

    highlightMenuButton(ui->profileButton);
}

SettingsPage::~SettingsPage()
{
    delete ui;
}

/* ─────────────────────────────────────────────
 *  INIT UI
 * ───────────────────────────────────────────── */

void SettingsPage::setupUI()
{
    // Appliquer le thème global
    ThemeManager::instance().applyTheme(window());

    // Sync thème courant
    m_currentTheme =
        (ThemeManager::instance().currentTheme() == AppTheme::Light
             ? "Clair" : "Sombre");

    // Sync langue courante → combobox
    if (ui->languageComboBox) {
        const QString lang = LanguageManager::instance().currentLanguage();
        ui->languageComboBox->setCurrentIndex(lang == "en" ? 1 : 0);
    }
}

void SettingsPage::setupConnections()
{
    // Menu gauche
    connect(ui->profileButton,       &QPushButton::clicked, this, &SettingsPage::on_profileButton_clicked);
    connect(ui->themeButton,         &QPushButton::clicked, this, &SettingsPage::on_themeButton_clicked);
    connect(ui->notificationsButton, &QPushButton::clicked, this, &SettingsPage::on_notificationsButton_clicked);
    connect(ui->securityButton,      &QPushButton::clicked, this, &SettingsPage::on_securityButton_clicked);
    connect(ui->aboutButton,         &QPushButton::clicked, this, &SettingsPage::on_aboutButton_clicked);

    // Actions
    connect(ui->saveProfileButton,   &QPushButton::clicked, this, &SettingsPage::on_saveProfileButton_clicked);
    connect(ui->changePasswordButton,&QPushButton::clicked, this, &SettingsPage::on_changePasswordButton_clicked);

    // Thème
    connect(ui->lightThemeRadio,     &QRadioButton::clicked, this, &SettingsPage::on_lightThemeRadio_clicked);
    connect(ui->darkThemeRadio,      &QRadioButton::clicked, this, &SettingsPage::on_darkThemeRadio_clicked);

    // Langue
    connect(ui->languageComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            &SettingsPage::on_languageComboBox_currentIndexChanged);
}

/* ─────────────────────────────────────────────
 *  LOGIQUE SELON RÔLE
 * ───────────────────────────────────────────── */

void SettingsPage::updateUIForRole()
{
    QString moduleName;

    switch (m_userRole)
    {
    case UserRole::GENERAL_ADMIN:
        moduleName = tr("Administration Générale");
        break;
    case UserRole::SERVICE_CLIENTS:
        moduleName = tr("Service Clients");
        break;
    case UserRole::SERVICE_FINANCIER:
        moduleName = tr("Service Financier");
        break;
    case UserRole::SERVICE_LOGISTIQUE:
        moduleName = tr("Service Logistique");
        break;
    case UserRole::SERVICE_INFORMATIQUE:
        moduleName = tr("Service Informatique");
        break;
    default:
        moduleName = tr("Général");
        break;
    }

    if (ui->titleLabel)
        ui->titleLabel->setText(tr("⚙️ Paramètres — ") + moduleName);
}

void SettingsPage::updateNotificationsForRole()
{
    if (!ui) return;

    if (ui->residentsNotifCheckBox)    ui->residentsNotifCheckBox->setVisible(false);
    if (ui->transactionsNotifCheckBox) ui->transactionsNotifCheckBox->setVisible(false);
    if (ui->incidentsNotifCheckBox)    ui->incidentsNotifCheckBox->setVisible(false);
    if (ui->transportsNotifCheckBox)   ui->transportsNotifCheckBox->setVisible(false);

    switch (m_userRole)
    {
    case UserRole::GENERAL_ADMIN:
        ui->residentsNotifCheckBox->setVisible(true);
        ui->transactionsNotifCheckBox->setVisible(true);
        ui->incidentsNotifCheckBox->setVisible(true);
        ui->transportsNotifCheckBox->setVisible(true);
        break;

    case UserRole::SERVICE_CLIENTS:
        ui->residentsNotifCheckBox->setVisible(true);
        ui->incidentsNotifCheckBox->setVisible(true);
        break;

    case UserRole::SERVICE_FINANCIER:
        ui->transactionsNotifCheckBox->setVisible(true);
        break;

    case UserRole::SERVICE_LOGISTIQUE:
        ui->transportsNotifCheckBox->setVisible(true);
        break;

    case UserRole::SERVICE_INFORMATIQUE:
        // Service informatique gets no notifications from modules
        break;

    default:
        break;
    }
}

void SettingsPage::updateAboutSection()
{
    if (!ui) return;

    QString title;
    QString text;

    switch (m_userRole)
    {
    case UserRole::GENERAL_ADMIN:
        title = tr("À propos — Administration Générale");
        text =
            "🔹 Supervision de tous les modules NEXORA\n"
            "🔹 Gestion des rôles et accès\n"
            "🔹 Vision globale Smart City\n"
            "🔹 Statistiques avancées et maintenabilité";
        break;

    case UserRole::SERVICE_CLIENTS:
        title = tr("À propos — Service Clients");
        text =
            "👥 Gestion des Résidents\n"
            "🚨 Suivi des Incidents\n"
            "📞 Support & Relation Citoyenne\n"
            "🔍 Recherche & Historique rapide";
        break;

    case UserRole::SERVICE_FINANCIER:
        title = tr("À propos — Service Financier");
        text =
            "💰 Gestion des transactions\n"
            "📈 Analyse financière\n"
            "🧾 Contrôle budgétaire\n"
            "📄 Exports automatiques";
        break;

    case UserRole::SERVICE_LOGISTIQUE:
        title = tr("À propos — Service Logistique");
        text =
            "🚗 Gestion des véhicules\n"
            "🏢 Gestion des locaux\n"
            "🧭 Optimisation des ressources\n"
            "🛠️ Planification & maintenance";
        break;

    case UserRole::SERVICE_INFORMATIQUE:
        title = tr("À propos — Service Informatique");
        text =
            "🛡️ Maintenance Système\n"
            "🗄️ Supervision Base de Données\n"
            "⚙️ Sécurité & Monitoring\n"
            "📂 Gestion des sauvegardes";
        break;

    default:
        title = tr("À propos");
        text = tr("Plateforme NEXORA — Smart City Management");
        break;
    }

    ui->descriptionLabel->setText(
        QString("<b style='font-size:18px;'>%1</b><br><br>%2")
            .arg(title).arg(text)
        );
}

/* ─────────────────────────────────────────────
 *  PROFIL
 * ───────────────────────────────────────────── */

void SettingsPage::loadUserData()
{
    if (!ui) return;

    if (ui->usernameLineEdit)
        ui->usernameLineEdit->setText(m_currentUser);

    if (ui->emailLineEdit)
        ui->emailLineEdit->setText(m_currentUser + "@nexora.city");

    if (ui->phoneLineEdit)
        ui->phoneLineEdit->setText("+216 XX XXX XXX");

    if (ui->enableNotificationsCheckBox)
        ui->enableNotificationsCheckBox->setChecked(true);

    // Populate role field based on current user role
    if (ui->roleLineEdit) {
        QString roleName;
        switch (m_userRole)
        {
        case UserRole::GENERAL_ADMIN:
            roleName = tr("Administrateur Général");
            break;
        case UserRole::SERVICE_CLIENTS:
            roleName = tr("Service Clients");
            break;
        case UserRole::SERVICE_FINANCIER:
            roleName = tr("Service Financier");
            break;
        case UserRole::SERVICE_LOGISTIQUE:
            roleName = tr("Service Logistique");
            break;
        case UserRole::SERVICE_INFORMATIQUE:
            roleName = tr("Service Informatique");
            break;
        default:
            roleName = tr("Utilisateur");
            break;
        }
        ui->roleLineEdit->setText(roleName);
    }
}

void SettingsPage::on_saveProfileButton_clicked()
{
    const QString email = ui->emailLineEdit->text().trimmed();

    if (!email.contains("@")) {
        showError(tr("Veuillez entrer une adresse email valide."));
        return;
    }

    showSuccess(tr("Profil mis à jour avec succès !"));
    emit userProfileUpdated();
}

/* ─────────────────────────────────────────────
 *  MOT DE PASSE (Oracle + UI pro)
 * ───────────────────────────────────────────── */
void SettingsPage::on_changePasswordButton_clicked()
{
    // === CLEAN & PROFESSIONAL DIALOG ===
    QDialog dialog(this);
    dialog.setModal(true);
    dialog.setFixedSize(500, 380);
    dialog.setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    dialog.setStyleSheet(
        "QDialog {"
        "background: rgba(255,255,255,0.15);"
        "border-radius: 20px;"
        "border: 1px solid rgba(255,255,255,0.2);"
        "backdrop-filter: blur(25px);"
        "}"
        );

    // Shadow
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(&dialog);
    shadow->setBlurRadius(40);
    shadow->setOffset(0, 6);
    shadow->setColor(QColor(0, 0, 0, 120));
    dialog.setGraphicsEffect(shadow);

    // === HEADER (SMALLER + CLEANER) ===
    QWidget *header = new QWidget();
    header->setFixedHeight(60);
    header->setStyleSheet(
        "QWidget {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        " stop:0 #5A6FF0, stop:1 #8C5BE8);"
        "border-top-left-radius: 20px;"
        "border-top-right-radius: 20px;"
        "}"
        );

    QHBoxLayout *headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(18, 10, 18, 10);

    QLabel *title = new QLabel("Modifier votre mot de passe");
    title->setStyleSheet(
        "font-size: 20px;"
        "font-weight: 600;"
        "color: white;"
        );

    QLabel *subtitle = new QLabel("Sécurité renforcée • Nexora Smart City");
    subtitle->setStyleSheet(
        "font-size: 11px;"
        "color: rgba(255,255,255,0.85);"
        );

    QVBoxLayout *titleBox = new QVBoxLayout();
    titleBox->addWidget(title);
    titleBox->addWidget(subtitle);

    headerLayout->addLayout(titleBox);
    headerLayout->addStretch();

    // === CLEAN INPUT FIELD ===
    auto createField = [&](const QString &placeholder) {
        QLineEdit *edit = new QLineEdit();
        edit->setPlaceholderText(placeholder);
        edit->setEchoMode(QLineEdit::Password);
        edit->setFixedHeight(44);
        edit->setStyleSheet(
            "QLineEdit {"
            "background: rgba(255,255,255,0.92);"
            "border-radius: 12px;"
            "border: 1px solid #d0d0d0;"
            "padding-left: 12px;"
            "font-size: 15px;"
            "}"
            "QLineEdit:focus {"
            "border-color: #5A6FF0;"
            "background: white;"
            "}"
            );
        return edit;
    };

    QLineEdit *oldPass = createField("Ancien mot de passe");
    QLineEdit *newPass = createField("Nouveau mot de passe");
    QLineEdit *confirmPass = createField("Confirmer le mot de passe");

    // === BUTTONS (CLEAN MINIMAL STYLE) ===
    QPushButton *cancelBtn = new QPushButton("Annuler");
    cancelBtn->setFixedHeight(42);
    cancelBtn->setStyleSheet(
        "QPushButton {"
        "background: transparent;"
        "color: #5A6FF0;"
        "font-size: 15px;"
        "padding: 6px 18px;"
        "border-radius: 10px;"
        "border: 1px solid rgba(90,111,240,0.4);"
        "}"
        "QPushButton:hover { background: rgba(90,111,240,0.1); }"
        );
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    QPushButton *saveBtn = new QPushButton("Enregistrer");
    saveBtn->setFixedHeight(42);
    saveBtn->setStyleSheet(
        "QPushButton {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        " stop:0 #5A6FF0, stop:1 #8C5BE8);"
        "color: white;"
        "font-size: 15px;"
        "font-weight: 600;"
        "padding: 6px 22px;"
        "border-radius: 10px;"
        "}"
        "QPushButton:hover { opacity: 0.92; }"
        );

    // === LAYOUT ===
    QVBoxLayout *contentLayout = new QVBoxLayout();
    contentLayout->setContentsMargins(30, 25, 30, 25);
    contentLayout->setSpacing(16);

    QLabel *infoText = new QLabel(
        "Veuillez confirmer votre ancien mot de passe avant d'en définir un nouveau."
        );
    infoText->setWordWrap(true);
    infoText->setStyleSheet("font-size: 13px; color: #FFFFFF;");

    contentLayout->addWidget(infoText);
    contentLayout->addWidget(oldPass);
    contentLayout->addWidget(newPass);
    contentLayout->addWidget(confirmPass);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(cancelBtn);
    btnLayout->addStretch();
    btnLayout->addWidget(saveBtn);

    contentLayout->addSpacing(12);
    contentLayout->addLayout(btnLayout);

    // === ASSEMBLE FULL LAYOUT ===
    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(header);
    mainLayout->addLayout(contentLayout);

    // === FADE ANIMATION ===
    dialog.setWindowOpacity(0);
    QPropertyAnimation *fade = new QPropertyAnimation(&dialog, "windowOpacity");
    fade->setDuration(220);
    fade->setStartValue(0);
    fade->setEndValue(1);
    fade->start(QAbstractAnimation::DeleteWhenStopped);

    // === PASSWORD LOGIC ===
    connect(saveBtn, &QPushButton::clicked, [&]() {
        QString storedHash = SettingsManager::instance().getPasswordForUser(m_currentUser);
        if (storedHash.isEmpty()) storedHash = SettingsManager::instance().hashPassword("0000");

        if (SettingsManager::instance().hashPassword(oldPass->text()) != storedHash) {
            oldPass->setStyleSheet(oldPass->styleSheet() + "border:2px solid red;");
            return;
        }
        if (newPass->text() != confirmPass->text() || newPass->text().length() < 4) {
            confirmPass->setStyleSheet(confirmPass->styleSheet() + "border:2px solid red;");
            return;
        }

        SettingsManager::instance().setPasswordForUser(m_currentUser, newPass->text());
        dialog.accept();
    });

    dialog.exec();
}




/* ─────────────────────────────────────────────
 *  THÈME
 * ───────────────────────────────────────────── */

void SettingsPage::on_lightThemeRadio_clicked()
{
    applyTheme("Clair");
    emit themeChanged("Clair");
}

void SettingsPage::on_darkThemeRadio_clicked()
{
    applyTheme("Sombre");
    emit themeChanged("Sombre");
}

void SettingsPage::applyTheme(const QString &themeName)
{
    m_currentTheme = themeName;

    AppTheme t = (themeName == "Sombre" ? AppTheme::Dark : AppTheme::Light);
    ThemeManager::instance().setTheme(t);
    ThemeManager::instance().applyTheme(window());

    showSuccess(tr("Thème changé."));
}

/* ─────────────────────────────────────────────
 *  LANGUE
 * ───────────────────────────────────────────── */

void SettingsPage::on_languageComboBox_currentIndexChanged(int index)
{
    if (!ui || !ui->languageComboBox) return;

    QString langCode;
    switch (index) {
    case 0: langCode = "fr"; break;
    case 1: langCode = "en"; break;
    case 2:
        QMessageBox::information(this, tr("Langue"),
                                 tr("La langue Arabe sera bientôt disponible."));
        {
            const QString cur = LanguageManager::instance().currentLanguage();
            ui->languageComboBox->setCurrentIndex(cur == "en" ? 1 : 0);
        }
        return;
    default:
        return;
    }

    LanguageManager::instance().setLanguage(langCode);
    updateUIForRole();
    updateNotificationsForRole();
    updateAboutSection();

    showSuccess(tr("Langue changée."));
    emit languageChanged(langCode);
}

/* ─────────────────────────────────────────────
 *  NAVIGATION ENTRE PAGES
 * ───────────────────────────────────────────── */

void SettingsPage::switchToPage(int index)
{
    if (!ui || !ui->settingsStackedWidget) return;

    ui->settingsStackedWidget->setCurrentIndex(index);

    switch (index) {
    case 0: loadUserData(); break;
    case 2: updateNotificationsForRole(); break;
    case 4: updateAboutSection(); break;
    default: break;
    }
}

void SettingsPage::resetMenuButtons()
{
    QString defaultStyle =
        "QPushButton {"
        " background:#ECF0F1; color:#2C3E50;"
        " border:none; border-radius:10px;"
        " padding:10px; text-align:left; padding-left:20px;"
        " font-size:15px; font-weight:bold;"
        "}"
        "QPushButton:hover { background:#4A90E2; color:white; }";

    ui->profileButton->setStyleSheet(defaultStyle);
    ui->themeButton->setStyleSheet(defaultStyle);
    ui->notificationsButton->setStyleSheet(defaultStyle);
    ui->securityButton->setStyleSheet(defaultStyle);
    ui->aboutButton->setStyleSheet(defaultStyle);
}

void SettingsPage::highlightMenuButton(QPushButton *button)
{
    resetMenuButtons();

    if (!button) return;

    button->setStyleSheet(
        "QPushButton {"
        " background:#4A90E2; color:white;"
        " border:none; border-radius:10px;"
        " padding:10px; text-align:left; padding-left:20px;"
        " font-size:15px; font-weight:bold;"
        "}"
        "QPushButton:hover { background:#3A7BC8; }"
        );
}

/* ─────────────────────────────────────────────
 *  MENU GAUCHE : SLOTS
 * ───────────────────────────────────────────── */

void SettingsPage::on_profileButton_clicked()
{
    switchToPage(0);
    highlightMenuButton(ui->profileButton);
}

void SettingsPage::on_themeButton_clicked()
{
    switchToPage(1);
    highlightMenuButton(ui->themeButton);
}

void SettingsPage::on_notificationsButton_clicked()
{
    switchToPage(2);
    highlightMenuButton(ui->notificationsButton);
}

void SettingsPage::on_securityButton_clicked()
{
    switchToPage(3);
    highlightMenuButton(ui->securityButton);
}

void SettingsPage::on_aboutButton_clicked()
{
    switchToPage(4);
    highlightMenuButton(ui->aboutButton);
}

/* ─────────────────────────────────────────────
 *  MESSAGES
 * ───────────────────────────────────────────── */

void SettingsPage::showSuccess(const QString &message)
{
    QMessageBox::information(this, tr("Succès"), message);
}

void SettingsPage::showError(const QString &message)
{
    QMessageBox::warning(this, tr("Erreur"), message);
}
