#include "settingspage.h"
#include "ui_settingspage.h"
#include "settingsmanager.h"
#include <QCryptographicHash>

#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QDateTime>
#include <QInputDialog>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>

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
    if (ui->settingsStackedWidget) {
        ui->settingsStackedWidget->setCurrentIndex(0);
    }
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

    // Sync thème courant (string utilisée pour radios)
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
    connect(ui->databaseButton,      &QPushButton::clicked, this, &SettingsPage::on_databaseButton_clicked);
    connect(ui->aboutButton,         &QPushButton::clicked, this, &SettingsPage::on_aboutButton_clicked);

    // Actions
    connect(ui->saveProfileButton,   &QPushButton::clicked, this, &SettingsPage::on_saveProfileButton_clicked);
    connect(ui->changePasswordButton,&QPushButton::clicked, this, &SettingsPage::on_changePasswordButton_clicked);

    connect(ui->backupButton,        &QPushButton::clicked, this, &SettingsPage::on_backupButton_clicked);
    connect(ui->restoreButton,       &QPushButton::clicked, this, &SettingsPage::on_restoreButton_clicked);
    connect(ui->clearDataButton,     &QPushButton::clicked, this, &SettingsPage::on_clearDataButton_clicked);

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
 *  SETTERS PUBLICS
 * ───────────────────────────────────────────── */

void SettingsPage::setUserRole(UserRole role)
{
    m_userRole = role;
    updateUIForRole();
    updateNotificationsForRole();
    updateAboutSection();
}

void SettingsPage::setCurrentUser(const QString &user)
{
    m_currentUser = user;
    loadUserData();
}

/* ─────────────────────────────────────────────
 *  LOGIQUE SELON RÔLE
 * ───────────────────────────────────────────── */

void SettingsPage::updateUIForRole()
{
    QString moduleName;

    switch (m_userRole)
    {
    case UserRole::ADMIN:          moduleName = tr("Administration Principale"); break;
    case UserRole::ADMIN_SETTINGS: moduleName = tr("Administration Système");    break;
    case UserRole::RESIDENT:       moduleName = tr("Résidents");                 break;
    case UserRole::FINANCE:        moduleName = tr("Finance");                   break;
    case UserRole::INCIDENT:       moduleName = tr("Incidents");                 break;
    case UserRole::TRANSPORT:      moduleName = tr("Transport");                 break;
    case UserRole::LOCAUX:         moduleName = tr("Locaux");                    break;
    default:                       moduleName = tr("Général");                   break;
    }

    if (ui->titleLabel)
        ui->titleLabel->setText(tr("⚙️ Paramètres — ") + moduleName);

    // Accès DB réservé aux admins
    if (ui->databaseButton) {
        ui->databaseButton->setVisible(
            m_userRole == UserRole::ADMIN || m_userRole == UserRole::ADMIN_SETTINGS
            );
    }
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
    case UserRole::ADMIN:
    case UserRole::ADMIN_SETTINGS:
        if (ui->residentsNotifCheckBox)    ui->residentsNotifCheckBox->setVisible(true);
        if (ui->transactionsNotifCheckBox) ui->transactionsNotifCheckBox->setVisible(true);
        if (ui->incidentsNotifCheckBox)    ui->incidentsNotifCheckBox->setVisible(true);
        if (ui->transportsNotifCheckBox)   ui->transportsNotifCheckBox->setVisible(true);
        break;

    case UserRole::RESIDENT:
        if (ui->residentsNotifCheckBox) ui->residentsNotifCheckBox->setVisible(true);
        break;

    case UserRole::FINANCE:
        if (ui->transactionsNotifCheckBox) ui->transactionsNotifCheckBox->setVisible(true);
        break;

    case UserRole::INCIDENT:
        if (ui->incidentsNotifCheckBox) ui->incidentsNotifCheckBox->setVisible(true);
        break;

    case UserRole::TRANSPORT:
        if (ui->transportsNotifCheckBox) ui->transportsNotifCheckBox->setVisible(true);
        break;

    case UserRole::LOCAUX:
        if (ui->residentsNotifCheckBox) ui->residentsNotifCheckBox->setVisible(true);
        break;

    default:
        break;
    }
}

void SettingsPage::updateAboutSection()
{
    if (!ui) return;

    QString moduleName;

    switch (m_userRole)
    {
    case UserRole::ADMIN:          moduleName = tr("Administration Principale"); break;
    case UserRole::ADMIN_SETTINGS: moduleName = tr("Administration Système");    break;
    case UserRole::RESIDENT:       moduleName = tr("Résidents");                 break;
    case UserRole::FINANCE:        moduleName = tr("Finance");                   break;
    case UserRole::INCIDENT:       moduleName = tr("Incidents");                 break;
    case UserRole::TRANSPORT:      moduleName = tr("Transport");                 break;
    case UserRole::LOCAUX:         moduleName = tr("Locaux");                    break;
    default:                       moduleName = tr("Général");                   break;
    }

    if (ui->versionLabel)
        ui->versionLabel->setText(tr("Version 1.0.0"));

    if (ui->descriptionLabel) {
        ui->descriptionLabel->setText(
            QStringLiteral(
                "<html><head/><body>"
                "<p align=\"center\"><span style=\" font-size:16px;\">%1</span></p>"
                "<p align=\"center\"><span style=\" font-size:14px; color:#7f8c8d;\">%2 %3</span></p>"
                "</body></html>"
                )
                .arg(tr("Plateforme de gestion intelligente pour Smart Cities"))
                .arg(tr("Module"))
                .arg(moduleName)
            );
    }

    if (ui->featuresLabel) {
        QString features = tr("Fonctionnalités:\n\n");

        switch (m_userRole)
        {
        case UserRole::ADMIN:
            features += "📊 " + tr("Statistiques en temps réel") + "\n";
            features += "🛡️ " + tr("Gestion des accès et sécurité") + "\n";
            features += "🗄️ " + tr("Maintenance base de données") + "\n";
            break;
        case UserRole::RESIDENT:
            features += "👥 " + tr("Gestion des résidents") + "\n";
            features += "🔎 " + tr("Recherche avancée") + "\n";
            break;
        case UserRole::FINANCE:
            features += "💳 " + tr("Suivi des transactions") + "\n";
            features += "📈 " + tr("Analyse et scoring") + "\n";
            break;
        case UserRole::INCIDENT:
            features += "🚨 " + tr("Déclaration et suivi des incidents") + "\n";
            features += "🗺️ " + tr("Localisation sur carte") + "\n";
            break;
        case UserRole::TRANSPORT:
            features += "🚌 " + tr("Gestion de flotte") + "\n";
            features += "🔔 " + tr("Notifications de maintenance") + "\n";
            break;
        case UserRole::LOCAUX:
            features += "🏢 " + tr("Gestion des locaux") + "\n";
            features += "📄 " + tr("Exports PDF et rapports") + "\n";
            break;
        default:
            features += "📦 " + tr("Fonctionnalités générales de la plateforme") + "\n";
            break;
        }

        ui->featuresLabel->setText(features);
    }
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
        ui->phoneLineEdit->setText("+33 1 23 45 67 89");

    if (ui->enableNotificationsCheckBox)
        ui->enableNotificationsCheckBox->setChecked(true);
}

void SettingsPage::on_saveProfileButton_clicked()
{
    if (!ui || !ui->emailLineEdit) return;

    const QString email = ui->emailLineEdit->text().trimmed();

    if (!email.contains("@")) {
        showError(tr("Veuillez entrer une adresse email valide."));
        return;
    }

    showSuccess(tr("Profil mis à jour avec succès !"));
    emit userProfileUpdated();
}

/* ─────────────────────────────────────────────
 *  MOT DE PASSE
 * ───────────────────────────────────────────── */

void SettingsPage::on_changePasswordButton_clicked()
{
    bool ok;
    QString oldPass = QInputDialog::getText(
        this, tr("Ancien mot de passe"),
        tr("Entrez l'ancien mot de passe :"),
        QLineEdit::Password, "", &ok);

    if (!ok) return;

    QString storedHash =
        SettingsManager::instance().getPasswordForUser(m_currentUser);

    if (storedHash.isEmpty())
        storedHash = SettingsManager::instance().hashPassword("0000");

    QString enteredHash =
        QString(QCryptographicHash::hash(oldPass.toUtf8(),
                                         QCryptographicHash::Sha256).toHex());

    if (enteredHash != storedHash) {
        showError(tr("Mot de passe incorrect."));
        return;
    }

    QString newPass = QInputDialog::getText(
        this, tr("Nouveau mot de passe"),
        tr("Entrez un nouveau mot de passe :"),
        QLineEdit::Password, "", &ok);

    if (!ok || newPass.length() < 4) {
        showError(tr("Le mot de passe doit contenir 4 caractères minimum."));
        return;
    }

    SettingsManager::instance().setPasswordForUser(m_currentUser, newPass);
    showSuccess(tr("Mot de passe changé avec succès !"));
}


/* ─────────────────────────────────────────────
 *  BASE DE DONNÉES
 * ───────────────────────────────────────────── */

void SettingsPage::on_databaseButton_clicked()
{
    if (m_userRole != UserRole::ADMIN && m_userRole != UserRole::ADMIN_SETTINGS) {
        showError(tr("Vous n'avez pas l'accès admin."));
        return;
    }

    switchToPage(4);
    highlightMenuButton(ui->databaseButton);
}

void SettingsPage::on_backupButton_clicked()
{
    showSuccess(tr("Base sauvegardée (mode interface uniquement)."));
}

void SettingsPage::on_restoreButton_clicked()
{
    showSuccess(tr("Restauration exécutée (mode interface uniquement)."));
}

void SettingsPage::on_clearDataButton_clicked()
{
    showSuccess(tr("Nettoyage terminé (mode interface uniquement)."));
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
        // Revenir à la langue courante
        {
            const QString cur = LanguageManager::instance().currentLanguage();
            ui->languageComboBox->setCurrentIndex(cur == "en" ? 1 : 0);
        }
        return;
    default:
        return;
    }

    LanguageManager::instance().setLanguage(langCode);
    if (ui) ui->retranslateUi(this);
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

    // Mises à jour spécifiques à chaque page
    switch (index) {
    case 0: // Profil
        loadUserData();
        break;
    case 2: // Notifications
        updateNotificationsForRole();
        break;
    case 5: // À propos
        updateAboutSection();
        break;
    default:
        break;
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

    if (ui->profileButton)       ui->profileButton->setStyleSheet(defaultStyle);
    if (ui->themeButton)         ui->themeButton->setStyleSheet(defaultStyle);
    if (ui->notificationsButton) ui->notificationsButton->setStyleSheet(defaultStyle);
    if (ui->securityButton)      ui->securityButton->setStyleSheet(defaultStyle);
    if (ui->databaseButton)      ui->databaseButton->setStyleSheet(defaultStyle);
    if (ui->aboutButton)         ui->aboutButton->setStyleSheet(defaultStyle);
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
    switchToPage(5);
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
