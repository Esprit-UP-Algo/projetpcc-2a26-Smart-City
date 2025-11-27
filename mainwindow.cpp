#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "welcomedialog.h"
#include "nexoradashboard.h"
#include "thememanager.h"
#include "languagemanager.h"
#include "settingsmanager.h"

#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QSequentialAnimationGroup>
#include <QPixmap>
#include <QMessageBox>
#include <QDebug>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , isLoggingIn(false)
    , m_currentDashboard(nullptr)
{
    ui->setupUi(this);

    //
    // ─── SETTINGS SCOPE ───────────────────────────────────────────
    //
    if (QCoreApplication::organizationName().isEmpty())
        QCoreApplication::setOrganizationName("Nexora");

    if (QCoreApplication::applicationName().isEmpty())
        QCoreApplication::setApplicationName("Qtadmin");

    //
    // ─── INITIALIZE THEME & LANGUAGE ──────────────────────────────
    //
    ThemeManager::instance().initialize();
    LanguageManager::instance().initialize();

    // Apply theme now
    ThemeManager::instance().applyTheme(this);

    // React to theme changes
    connect(&ThemeManager::instance(), &ThemeManager::themeChanged,
            this, [this](AppTheme) { applyTheme(); });

    // React to language changes
    connect(&LanguageManager::instance(), &LanguageManager::languageChanged,
            this, [this](const QString&) { reloadTranslations(); });

    reloadTranslations(); // apply stored language

    //
    // ─── INITIALISATION LOCALE ─────────────────────────────────────
    //
    setupUsers();

    // Set Logo
    QPixmap logo(":/resources/nexorab.png");
    if (ui->logoLabel && !logo.isNull()) {
        ui->logoLabel->setPixmap(
            logo.scaled(220, 220, Qt::KeepAspectRatio, Qt::SmoothTransformation)
            );
        ui->logoLabel->setAlignment(Qt::AlignCenter);
        ui->logoLabel->setScaledContents(true);
    }

    setWindowTitle("Smart City - Connexion");
    showMaximized();

    if (ui->errorLabel) ui->errorLabel->hide();

    //
    // ─── ENTER KEY LOGIN ───────────────────────────────────────────
    //
    connect(ui->emailLineEdit, &QLineEdit::returnPressed, this, &MainWindow::performLogin);
    connect(ui->passwordLineEdit, &QLineEdit::returnPressed, this, &MainWindow::performLogin);

    //
    // ─── FADE IN ILLUSTRATION ──────────────────────────────────────
    //
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
}

MainWindow::~MainWindow()
{
    if (m_currentDashboard)
        m_currentDashboard->deleteLater();
    delete ui;
}

//
// ─────────────────────────────────────────────────────────────
//   LOGIN BUTTON
// ─────────────────────────────────────────────────────────────
//
void MainWindow::on_loginButton_clicked()
{
    performLogin();
}

//
// ─────────────────────────────────────────────────────────────
//   MAIN LOGIN LOGIC
// ─────────────────────────────────────────────────────────────
//
void MainWindow::performLogin()
{
    if (isLoggingIn) return;
    isLoggingIn = true;

    QString email = ui->emailLineEdit->text().trimmed();
    QString password = ui->passwordLineEdit->text();

    ui->errorLabel->hide();

    if (email.isEmpty() || password.isEmpty()) {
        showErrorMessage("Veuillez remplir tous les champs.");
        shakeWidget(ui->emailLineEdit);
        shakeWidget(ui->passwordLineEdit);
        isLoggingIn = false;
        return;
    }

    if (validateCredentials(email, password)) {

        UserRole role = getUserRole(email);
        QString userName = getUserDisplayName(email);

        qDebug() << "Login Success →" << email << "| Role =" << (int)role;

        openDashboardWithRole(role, email, userName);

    } else {
        showErrorMessage("Incorrect ! Nom d'utilisateur ou mot de passe invalide.");
        shakeWidget(ui->emailLineEdit);
        shakeWidget(ui->passwordLineEdit);
    }

    isLoggingIn = false;
}

//
// ─────────────────────────────────────────────────────────────
//   ERROR MESSAGE
// ─────────────────────────────────────────────────────────────
//
void MainWindow::showErrorMessage(const QString &message)
{
    ui->errorLabel->setText(message);
    ui->errorLabel->show();
}

//
// ─────────────────────────────────────────────────────────────
//   ENTER KEY
// ─────────────────────────────────────────────────────────────
//
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

//
// ─────────────────────────────────────────────────────────────
//   SHAKE EFFECT
// ─────────────────────────────────────────────────────────────
//
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

//
// ─────────────────────────────────────────────────────────────
//   FULLSCREEN TOGGLE
// ─────────────────────────────────────────────────────────────
//
void MainWindow::on_fullscreenButton_clicked()
{
    isFullScreen() ? showMaximized() : showFullScreen();
}

//
// ─────────────────────────────────────────────────────────────
//   USER DATABASE
// ─────────────────────────────────────────────────────────────
//
void MainWindow::setupUsers()
{
    m_users.clear();
    m_users.append({"smart city","0000",UserRole::ADMIN,"Administrateur Principal"});
    m_users.append({"resident"  ,"0000",UserRole::RESIDENT,"Gestionnaire Résidents"});
    m_users.append({"finance"   ,"0000",UserRole::FINANCE,"Gestionnaire Finances"});
    m_users.append({"incident"  ,"0000",UserRole::INCIDENT,"Gestionnaire Incidents"});
    m_users.append({"transport" ,"0000",UserRole::TRANSPORT,"Gestionnaire Transports"});
    m_users.append({"locaux"    ,"0000",UserRole::LOCAUX,"Gestionnaire Locaux"});
    m_users.append({"admin"     ,"0000",UserRole::ADMIN_SETTINGS,"Administrateur Système"});
}

bool MainWindow::validateCredentials(const QString &email, const QString &password)
{
    // Prefer persisted hash if available
    QString storedHash = SettingsManager::instance().getPasswordForUser(email);
    if (!storedHash.isEmpty()) {
        QString enteredHash = SettingsManager::instance().hashPassword(password);
        return storedHash == enteredHash;
    }
    // Fallback to static list
    for (const UserInfo &u : m_users)
        if (u.email == email && u.password == password)
            return true;
    return false;
}



UserRole MainWindow::getUserRole(const QString &email) const
{
    for (const UserInfo &u : m_users)
        if (u.email == email)
            return u.role;
    return UserRole::NONE;
}

QString MainWindow::getUserDisplayName(const QString &email) const
{
    for (const UserInfo &u : m_users)
        if (u.email == email)
            return u.displayName;

    return "Utilisateur Inconnu";
}

//
// ─────────────────────────────────────────────────────────────
//   OPEN DASHBOARD
// ─────────────────────────────────────────────────────────────
//
void MainWindow::openDashboardWithRole(UserRole role, const QString &userEmail, const QString &userName)
{
    if (m_currentDashboard) {
        disconnect(m_currentDashboard, nullptr, this, nullptr);
        m_currentDashboard->close();
        m_currentDashboard->deleteLater();
        m_currentDashboard = nullptr;
    }

    WelcomeDialog *welcome = new WelcomeDialog(role, userEmail, userName, this);

    if (welcome->exec() == QDialog::Accepted) {

        hide();  // Hide login window

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

//
// ─────────────────────────────────────────────────────────────
//   APPLY THEME
// ─────────────────────────────────────────────────────────────
//
void MainWindow::applyTheme()
{
    ThemeManager::instance().applyTheme(this);
}

//
// ─────────────────────────────────────────────────────────────
//   RELOAD LANGUAGE
// ─────────────────────────────────────────────────────────────
//
void MainWindow::reloadTranslations()
{
    if (ui)
        ui->retranslateUi(this);
}

//
// ─────────────────────────────────────────────────────────────
//   LOGOUT → RETURN TO LOGIN WINDOW
// ─────────────────────────────────────────────────────────────
//
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
