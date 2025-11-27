#include "nexoradashboard.h"
#include "ui_nexoradashboard.h"

#include "residentspage.h"
#include "financespage.h"
#include "incidentspage.h"
#include "transportpage.h"
#include "locauxpage.h"
#include "settingspage.h"
#include "admindatabasepage.h"
#include "admindbinfopage.h"
#include "adminfilespage.h"
#include "databasemanager.h"
#include "thememanager.h"
#include "languagemanager.h"

#include <QButtonGroup>
#include <QMessageBox>
#include <QDebug>

NexoraDashboard::NexoraDashboard(UserRole userRole,
                                 const QString &userEmail,
                                 const QString &userName,
                                 QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::NexoraDashboard)
    , m_userRole(userRole)
    , m_userEmail(userEmail)
    , m_userName(userName)
    , pageStack(nullptr)
    , residentsPage(nullptr)
    , financesPage(nullptr)
    , incidentsPage(nullptr)
    , transportsPage(nullptr)
    , locauxPage(nullptr)
    , settingsPage(nullptr)
    , adminDbPage(nullptr)
    , adminInfoPage(nullptr)
    , adminFilesPage(nullptr)
    , pageTransition(nullptr)
    , pageOpacity(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("Smart City - Menu Principal");

    DatabaseManager::instance();   // Initialize DB

    // Setup UI system
    setupUI();
    setupNavigation();
    setupPages();
    setupAnimations();
    setupRoleBasedAccess();

    // Hook global theme/language and apply immediately
    hookGlobalManagers();
    applyTheme();
    reloadTranslations();

    showMaximized();
}

NexoraDashboard::~NexoraDashboard()
{
    delete ui;
}

//
// ────────────────────────────────────────────────
//   UI Setup
// ────────────────────────────────────────────────
//
void NexoraDashboard::setupUI()
{
    pageStack = ui->pageStackedWidget;
}

//
// ────────────────────────────────────────────────
//   Button Navigation
// ────────────────────────────────────────────────
//
void NexoraDashboard::setupNavigation()
{
    QButtonGroup *navGroup = new QButtonGroup(this);
    navGroup->setExclusive(true);

    navGroup->addButton(ui->residentsBtn);
    navGroup->addButton(ui->financesBtn);
    navGroup->addButton(ui->incidentsBtn);
    navGroup->addButton(ui->transportsBtn);
    navGroup->addButton(ui->locauxBtn);
    navGroup->addButton(ui->adminInfoBtn);
    navGroup->addButton(ui->adminFilesBtn);
    navGroup->addButton(ui->settingsBtn);

    connect(ui->residentsBtn, &QPushButton::clicked, this, &NexoraDashboard::onResidentsClicked);
    connect(ui->financesBtn, &QPushButton::clicked, this, &NexoraDashboard::onFinancesClicked);
    connect(ui->incidentsBtn, &QPushButton::clicked, this, &NexoraDashboard::onIncidentsClicked);
    connect(ui->transportsBtn, &QPushButton::clicked, this, &NexoraDashboard::onTransportsClicked);
    connect(ui->locauxBtn, &QPushButton::clicked, this, &NexoraDashboard::onLocauxClicked);
    connect(ui->adminInfoBtn, &QPushButton::clicked, this, &NexoraDashboard::onAdminInfoClicked);
    connect(ui->adminFilesBtn, &QPushButton::clicked, this, &NexoraDashboard::onAdminFilesClicked);
    connect(ui->settingsBtn, &QPushButton::clicked, this, &NexoraDashboard::onSettingsClicked);

    connect(ui->logoutBtn, &QPushButton::clicked, this, &NexoraDashboard::onLogoutClicked);
}

//
// ────────────────────────────────────────────────
//   Pages Setup
// ────────────────────────────────────────────────
//
void NexoraDashboard::setupPages()
{
    residentsPage  = new ResidentsPage();
    financesPage   = new FinancesPage();
    incidentsPage  = new IncidentsPage();
    transportsPage = new TransportPage();
    locauxPage     = new LocauxPage();
    settingsPage   = new SettingsPage(m_userRole, m_userEmail);
    adminDbPage    = nullptr;
    adminInfoPage  = nullptr;
    adminFilesPage = nullptr;

    pageStack->addWidget(residentsPage);   // 0
    pageStack->addWidget(financesPage);    // 1
    pageStack->addWidget(incidentsPage);   // 2
    pageStack->addWidget(transportsPage);  // 3
    pageStack->addWidget(locauxPage);      // 4
    pageStack->addWidget(settingsPage);    // 5
    
    // Admin pages will be added when needed based on user role
}

//
// ────────────────────────────────────────────────
//   Animations
// ────────────────────────────────────────────────
//
void NexoraDashboard::setupAnimations()
{
    pageOpacity = new QGraphicsOpacityEffect(pageStack);
    pageStack->setGraphicsEffect(pageOpacity);

    pageTransition = new QPropertyAnimation(pageOpacity, "opacity");
    pageTransition->setDuration(200);
}

void NexoraDashboard::showPage(int pageIndex)
{
    pageStack->setCurrentIndex(pageIndex);
}

//
// ────────────────────────────────────────────────
//   Role-based Access
// ────────────────────────────────────────────────
//
void NexoraDashboard::setupRoleBasedAccess()
{
    configureButtonVisibility();

    switch (m_userRole)
    {
    case UserRole::ADMIN:
        ui->residentsBtn->setChecked(true);
        showPage(0);
        break;

    case UserRole::RESIDENT:
        ui->residentsBtn->setChecked(true);
        showPage(0);
        break;

    case UserRole::FINANCE:
        ui->financesBtn->setChecked(true);
        showPage(1);
        break;

    case UserRole::INCIDENT:
        ui->incidentsBtn->setChecked(true);
        showPage(2);
        break;

    case UserRole::TRANSPORT:
        ui->transportsBtn->setChecked(true);
        showPage(3);
        break;

    case UserRole::LOCAUX:
        ui->locauxBtn->setChecked(true);
        showPage(4);
        break;

    case UserRole::ADMIN_SETTINGS:
        // Show admin database info page by default for database admin
        if (!adminInfoPage) {
            adminInfoPage = new AdminDatabaseInfoPage(this);
            pageStack->addWidget(adminInfoPage);
        }
        ui->adminInfoBtn->setChecked(true);
        pageStack->setCurrentWidget(adminInfoPage);
        break;

    default:
        returnToLogin();
        break;
    }
}

void NexoraDashboard::configureButtonVisibility()
{
    // Hide all navigation buttons by default
    ui->residentsBtn->hide();
    ui->financesBtn->hide();
    ui->incidentsBtn->hide();
    ui->transportsBtn->hide();
    ui->locauxBtn->hide();
    ui->adminInfoBtn->hide();
    ui->adminFilesBtn->hide();
    ui->settingsBtn->hide();

    switch (m_userRole)
    {
    case UserRole::ADMIN:
        ui->residentsBtn->show();
        ui->financesBtn->show();
        ui->incidentsBtn->show();
        ui->transportsBtn->show();
        ui->locauxBtn->show();
        ui->adminInfoBtn->show();
        ui->adminFilesBtn->show();
        // Don't show settings for admin users
        break;

    case UserRole::RESIDENT:
        ui->residentsBtn->show();
        ui->settingsBtn->show();
        break;

    case UserRole::FINANCE:
        ui->financesBtn->show();
        ui->settingsBtn->show();
        break;

    case UserRole::INCIDENT:
        ui->incidentsBtn->show();
        ui->settingsBtn->show();
        break;

    case UserRole::TRANSPORT:
        ui->transportsBtn->show();
        ui->settingsBtn->show();
        break;

    case UserRole::LOCAUX:
        ui->locauxBtn->show();
        ui->settingsBtn->show();
        break;

    case UserRole::ADMIN_SETTINGS:
        // Show only admin buttons for database admin
        ui->adminInfoBtn->show();
        ui->adminFilesBtn->show();
        // Don't show settings for admin settings users
        break;

    default:
        break;
    }
}

//
// ────────────────────────────────────────────────
//   Button slots
// ────────────────────────────────────────────────
//
void NexoraDashboard::onResidentsClicked()
{
    if (m_userRole == UserRole::RESIDENT || m_userRole == UserRole::ADMIN)
        showPage(0);
}

void NexoraDashboard::onFinancesClicked()
{
    if (m_userRole == UserRole::FINANCE || m_userRole == UserRole::ADMIN)
        showPage(1);
}

void NexoraDashboard::onIncidentsClicked()
{
    if (m_userRole == UserRole::INCIDENT || m_userRole == UserRole::ADMIN)
        showPage(2);
}

void NexoraDashboard::onTransportsClicked()
{
    if (m_userRole == UserRole::TRANSPORT || m_userRole == UserRole::ADMIN)
        showPage(3);
}

void NexoraDashboard::onLocauxClicked()
{
    if (m_userRole == UserRole::LOCAUX || m_userRole == UserRole::ADMIN)
        showPage(4);
}

void NexoraDashboard::onSettingsClicked()
{
    if (m_userRole == UserRole::ADMIN_SETTINGS) {
        if (!adminDbPage) {
            adminDbPage = new AdminDatabasePage(this);
            pageStack->addWidget(adminDbPage);
        }
        pageStack->setCurrentWidget(adminDbPage);
    } else {
        showPage(5);
    }
}

void NexoraDashboard::onAdminInfoClicked()
{
    if (m_userRole == UserRole::ADMIN_SETTINGS || m_userRole == UserRole::ADMIN) {
        if (!adminInfoPage) {
            adminInfoPage = new AdminDatabaseInfoPage(this);
            pageStack->addWidget(adminInfoPage);
        }
        pageStack->setCurrentWidget(adminInfoPage);
    }
}

void NexoraDashboard::onAdminFilesClicked()
{
    if (m_userRole == UserRole::ADMIN_SETTINGS || m_userRole == UserRole::ADMIN) {
        if (!adminFilesPage) {
            adminFilesPage = new AdminFilesPage(this);
            pageStack->addWidget(adminFilesPage);
        }
        pageStack->setCurrentWidget(adminFilesPage);
    }
}

//
// ────────────────────────────────────────────────
//   Logout
// ────────────────────────────────────────────────
//
void NexoraDashboard::onLogoutClicked()
{
    QMessageBox::StandardButton reply =
        QMessageBox::question(this, tr("Déconnexion"),
                              tr("Voulez-vous vraiment vous déconnecter ?"),
                              QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
        returnToLogin();
}

void NexoraDashboard::returnToLogin()
{
    emit loginRequested();
}

void NexoraDashboard::hookGlobalManagers()
{
    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, [this](AppTheme){ applyTheme(); });
    connect(&LanguageManager::instance(), &LanguageManager::languageChanged, this, [this](const QString&){ reloadTranslations(); });
}

void NexoraDashboard::applyTheme()
{
    ThemeManager::instance().applyTheme(this);
}

void NexoraDashboard::reloadTranslations()
{
    if (ui) ui->retranslateUi(this);
}
