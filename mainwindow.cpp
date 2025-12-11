#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "welcomedialog.h"
#include "nexoradashboard.h"
#include "thememanager.h"
#include "languagemanager.h"
#include "settingsmanager.h"
#include "databasemanager.h"

#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QSequentialAnimationGroup>
#include <QPixmap>
#include <QMessageBox>
#include <QDebug>
#include <QCoreApplication>
#include <QKeyEvent>
#include <QSqlQuery>
#include <QSqlError>
#include <QScreen>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , isLoggingIn(false)
    , m_currentDashboard(nullptr)
    , m_lastServiceId(-1)   // 🔹 IMPORTANT
{
    ui->setupUi(this);

    // Settings scope
    if (QCoreApplication::organizationName().isEmpty())
        QCoreApplication::setOrganizationName("Nexora");
    if (QCoreApplication::applicationName().isEmpty())
        QCoreApplication::setApplicationName("Qtadmin");

    // Initialize theme & language
    ThemeManager::instance().initialize();
    LanguageManager::instance().initialize();
    ThemeManager::instance().applyTheme(this);

    connect(&ThemeManager::instance(), &ThemeManager::themeChanged,
            this, [this](AppTheme){ applyTheme(); });

    connect(&LanguageManager::instance(), &LanguageManager::languageChanged,
            this, [this](const QString&){ reloadTranslations(); });

    reloadTranslations();

    // USERS LOADED HERE (pour rôle + displayName)
    setupUsers();

    // Logo
    QPixmap logo(":/resources/nexorab.png");
    if (ui->logoLabel && !logo.isNull()) {
        ui->logoLabel->setPixmap(logo.scaled(220, 220, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        ui->logoLabel->setAlignment(Qt::AlignCenter);
        ui->logoLabel->setScaledContents(true);
    }

    setWindowTitle("Smart City - Connexion");
    setFixedSize(1000, 600);  // Set exact professional size
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);  // Disable maximize button
    
    // Center the window on screen
    QScreen *screen = QApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->geometry();
        int x = (screenGeometry.width() - width()) / 2;
        int y = (screenGeometry.height() - height()) / 2;
        move(x, y);
    }

    if (ui->errorLabel) ui->errorLabel->hide();

    connect(ui->emailLineEdit, &QLineEdit::returnPressed,
            this, &MainWindow::performLogin);
    connect(ui->passwordLineEdit, &QLineEdit::returnPressed,
            this, &MainWindow::performLogin);

    // Fade illustration
    if (ui->illustrationLabel) {
        auto *op = new QGraphicsOpacityEffect(ui->illustrationLabel);
        ui->illustrationLabel->setGraphicsEffect(op);

        auto *fade = new QPropertyAnimation(op, "opacity");
        fade->setDuration(1500);
        fade->setStartValue(0.0);
        fade->setEndValue(1.0);
        fade->setEasingCurve(QEasingCurve::InOutQuad);
        fade->start(QAbstractAnimation::DeleteWhenStopped);
    }
    // Auto-resize window to fit login card perfectly
    QTimer::singleShot(0, this, [this]() {
        adjustSize();        // shrink to layout's real content
        setFixedSize(size()); // lock size so user cannot stretch it
        setMinimumSize(size());
    });
}


MainWindow::~MainWindow()
{
    if (m_currentDashboard)
        m_currentDashboard->deleteLater();
    delete ui;
}

/*──────────────────────────────────────────────
 *  BOUTONS
 *─────────────────────────────────────────────*/

void MainWindow::on_loginButton_clicked()
{
    performLogin();
}

void MainWindow::on_fullscreenButton_clicked()
{
    isFullScreen() ? showMaximized() : showFullScreen();
}

/*──────────────────────────────────────────────
 *  LOGIN FLOW
 *─────────────────────────────────────────────*/

void MainWindow::performLogin()
{
    if (isLoggingIn) return;
    isLoggingIn = true;

    QString username = ui->emailLineEdit->text().trimmed();
    QString password = ui->passwordLineEdit->text();

    ui->errorLabel->hide();

    if (username.isEmpty() || password.isEmpty()) {
        showErrorMessage("Veuillez remplir tous les champs.");
        shakeWidget(ui->emailLineEdit);
        shakeWidget(ui->passwordLineEdit);
        isLoggingIn = false;
        return;
    }

    // On remet le serviceId à -1 au cas où
    m_lastServiceId = -1;

    if (validateCredentials(username, password)) {

        UserRole role = getUserRole(username);
        QString userName = getUserDisplayName(username);

        qDebug() << "✅ Login Success →" << username
                 << "| service_id =" << m_lastServiceId
                 << "| Role =" << static_cast<int>(role);

        openDashboardWithRole(role, username, userName);

    } else {
        showErrorMessage("Incorrect ! Nom d'utilisateur ou mot de passe invalide.");
        shakeWidget(ui->emailLineEdit);
        shakeWidget(ui->passwordLineEdit);
    }

    isLoggingIn = false;
}

void MainWindow::showErrorMessage(const QString &message)
{
    ui->errorLabel->setText(message);
    ui->errorLabel->show();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if ((event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) &&
        (ui->emailLineEdit->hasFocus() ||
         ui->passwordLineEdit->hasFocus() ||
         ui->loginButton->hasFocus()))
    {
        performLogin();
        return;
    }
    QMainWindow::keyPressEvent(event);
}

void MainWindow::shakeWidget(QWidget *widget)
{
    if (!widget) return;

    QRect original = widget->geometry();
    auto *group = new QSequentialAnimationGroup(this);

    int distance = 10;
    int duration = 40;

    for (int i = 0; i < 4; i++) {
        auto *r = new QPropertyAnimation(widget, "geometry");
        r->setDuration(duration);
        QRect m = original;
        m.moveLeft(original.left() + distance);
        r->setEndValue(m);
        group->addAnimation(r);

        auto *l = new QPropertyAnimation(widget, "geometry");
        l->setDuration(duration);
        QRect n = original;
        n.moveLeft(original.left() - distance);
        l->setEndValue(n);
        group->addAnimation(l);
    }

    auto *reset = new QPropertyAnimation(widget, "geometry");
    reset->setDuration(duration);
    reset->setEndValue(original);
    group->addAnimation(reset);

    group->start(QAbstractAnimation::DeleteWhenStopped);
}

/*──────────────────────────────────────────────
 *  UTILISATEURS / RÔLES
 *─────────────────────────────────────────────*/

void MainWindow::setupUsers()
{
    m_users.clear();

    m_users.append({"smartcity",         "0000", UserRole::GENERAL_ADMIN,       "Administrateur Général"});
    m_users.append({"serviceClients",    "0000", UserRole::SERVICE_CLIENTS,     "Service Clients"});
    m_users.append({"serviceFinancier",  "0000", UserRole::SERVICE_FINANCIER,   "Service Financier"});
    m_users.append({"serviceLogistique", "0000", UserRole::SERVICE_LOGISTIQUE,  "Service Logistique"});
    m_users.append({"serviceInformatique","0000",UserRole::SERVICE_INFORMATIQUE,"Service Informatique"});
}

// 🔹 NOUVELLE VERSION : vérifie le mot de passe + récupère SERVICE_ID dans Oracle
bool MainWindow::validateCredentials(const QString &username, const QString &password)
{
    QSqlDatabase db = DatabaseManager::instance().database();
    if (!db.isOpen()) {
        qWarning() << "❌ validateCredentials: database not open";
        return false;
    }

    QSqlQuery q(db);
    q.prepare(R"(
        SELECT PASSWORD_HASH, SERVICE_ID
        FROM USERS
        WHERE USERNAME = :u
    )");
    q.bindValue(":u", username);

    if (!q.exec()) {
        qWarning() << "❌ validateCredentials SQL error:" << q.lastError().text();
        return false;
    }

    if (!q.next()) {
        // aucun user avec ce username
        qDebug() << "⚠️ validateCredentials: username not found in USERS:" << username;
        return false;
    }

    QString dbHash = q.value(0).toString();
    int serviceId  = q.value(1).toInt();
    QString enteredHash = SettingsManager::instance().hashPassword(password);

    qDebug() << "?? ORACLE USER FOUND:";
    qDebug() << "   USERNAME   =" << username;
    qDebug() << "   HASH(DB)   =" << dbHash;
    qDebug() << "   HASH(ENTR) =" << enteredHash;
    qDebug() << "   SERVICE_ID =" << serviceId;

    if (dbHash == enteredHash) {
        m_lastServiceId = serviceId;   // 🔹 On retient le service pour ce login
        return true;
    }

    return false;
}

// 🔹 Utilise m_lastServiceId s’il est valide, sinon retombe sur la table m_users
UserRole MainWindow::getUserRole(const QString &username) const
{
    if (m_lastServiceId >= 1) {
        return roleFromServiceId(m_lastServiceId);
    }

    // fallback (ne devrait pas arriver)
    for (const UserInfo &u : m_users)
        if (u.email == username)
            return u.role;

    return UserRole::NONE;
}

QString MainWindow::getUserDisplayName(const QString &username) const
{
    for (const UserInfo &u : m_users)
        if (u.email == username)
            return u.displayName;

    return "Utilisateur Inconnu";
}

// 🔹 mapping SERVICE_ID → UserRole
UserRole MainWindow::roleFromServiceId(int serviceId) const
{
    switch (serviceId) {
    case 1: return UserRole::GENERAL_ADMIN;
    case 2: return UserRole::SERVICE_CLIENTS;
    case 3: return UserRole::SERVICE_FINANCIER;
    case 4: return UserRole::SERVICE_LOGISTIQUE;
    case 5: return UserRole::SERVICE_INFORMATIQUE;
    default: return UserRole::NONE;
    }
}

/*──────────────────────────────────────────────
 *  DASHBOARD / LOGOUT
 *─────────────────────────────────────────────*/

void MainWindow::openDashboardWithRole(UserRole role,
                                       const QString &userEmail,
                                       const QString &userName)
{
    if (m_currentDashboard) {
        disconnect(m_currentDashboard, nullptr, this, nullptr);
        m_currentDashboard->close();
        m_currentDashboard->deleteLater();
        m_currentDashboard = nullptr;
    }

    WelcomeDialog *welcome = new WelcomeDialog(role, userEmail, userName, this);

    if (welcome->exec() == QDialog::Accepted) {
        hide();

        m_currentDashboard = new NexoraDashboard(role, userEmail, userName, nullptr);

        connect(m_currentDashboard, &NexoraDashboard::loginRequested,
                this, &MainWindow::onLoginRequested);

        connect(m_currentDashboard, &QObject::destroyed,
                this, [this]() { m_currentDashboard = nullptr; });

        m_currentDashboard->show();
        m_currentDashboard->raise();
        m_currentDashboard->activateWindow();
    }

    welcome->deleteLater();
}

void MainWindow::applyTheme()
{
    ThemeManager::instance().applyTheme(this);
}

void MainWindow::reloadTranslations()
{
    if (ui)
        ui->retranslateUi(this);
}

void MainWindow::onLoginRequested()
{
    if (m_currentDashboard) {
        disconnect(m_currentDashboard, nullptr, this, nullptr);
        m_currentDashboard->close();
        m_currentDashboard->deleteLater();
        m_currentDashboard = nullptr;
    }

    ui->emailLineEdit->clear();
    ui->passwordLineEdit->clear();
    ui->errorLabel->hide();
    ui->emailLineEdit->setFocus();

    show();
    raise();
    activateWindow();
}
