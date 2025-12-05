#include "nexoradashboard.h"
#include "ui_nexoradashboard.h"

#include "residentspage.h"
#include "financespage.h"
#include "incidentspage.h"
#include "transportpage.h"
#include "locauxpage.h"
#include "settingspage.h"
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
    , adminInfoPage(nullptr)
    , adminFilesPage(nullptr)
    , pageTransition(nullptr)
    , pageOpacity(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("Smart City - Menu Principal");

    // Initialise la connexion BD (singleton)
    DatabaseManager::instance();

    // Setup UI system
    setupUI();
    setupNavigation();
    setupPages();
    setupAnimations();
    setupRoleBasedAccess();

    // Thème / Langue globaux
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
//   Navigation (Sidebar Buttons)
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

    connect(ui->residentsBtn,   &QPushButton::clicked, this, &NexoraDashboard::onResidentsClicked);
    connect(ui->financesBtn,    &QPushButton::clicked, this, &NexoraDashboard::onFinancesClicked);
    connect(ui->incidentsBtn,   &QPushButton::clicked, this, &NexoraDashboard::onIncidentsClicked);
    connect(ui->transportsBtn,  &QPushButton::clicked, this, &NexoraDashboard::onTransportsClicked);
    connect(ui->locauxBtn,      &QPushButton::clicked, this, &NexoraDashboard::onLocauxClicked);
    connect(ui->adminInfoBtn,   &QPushButton::clicked, this, &NexoraDashboard::onAdminInfoClicked);
    connect(ui->adminFilesBtn,  &QPushButton::clicked, this, &NexoraDashboard::onAdminFilesClicked);
    connect(ui->settingsBtn,    &QPushButton::clicked, this, &NexoraDashboard::onSettingsClicked);

    connect(ui->logoutBtn,      &QPushButton::clicked, this, &NexoraDashboard::onLogoutClicked);
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

    // Indexes fixes
    pageStack->addWidget(residentsPage);   // 0
    pageStack->addWidget(financesPage);    // 1
    pageStack->addWidget(incidentsPage);   // 2
    pageStack->addWidget(transportsPage);  // 3
    pageStack->addWidget(locauxPage);      // 4
    pageStack->addWidget(settingsPage);    // 5

    // Admin pages (IT + General Admin) are added on demand
    adminInfoPage  = nullptr;
    adminFilesPage = nullptr;
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
    if (!pageStack)
        return;

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
    updateUserDisplayInfo();

    switch (m_userRole)
    {
    case UserRole::GENERAL_ADMIN:
        // Full access, start on Residents
        ui->residentsBtn->setChecked(true);
        showPage(0);
        break;

    case UserRole::SERVICE_CLIENTS:
        // Residents + Incidents + Settings
        ui->residentsBtn->setChecked(true);
        showPage(0);
        break;

    case UserRole::SERVICE_FINANCIER:
        // Finance + Settings
        ui->financesBtn->setChecked(true);
        showPage(1);
        break;

    case UserRole::SERVICE_LOGISTIQUE:
        // Transports + Locaux + Settings
        ui->transportsBtn->setChecked(true);
        showPage(3);
        break;

    case UserRole::SERVICE_INFORMATIQUE:
        // IT admin → Admin Info page by default
        if (!adminInfoPage) {
            adminInfoPage = new AdminDatabaseInfoPage(this);
            pageStack->addWidget(adminInfoPage);
        }
        ui->adminInfoBtn->setChecked(true);
        pageStack->setCurrentWidget(adminInfoPage);
        break;

    default:
        // No role → back to login
        returnToLogin();
        break;
    }
}

void NexoraDashboard::configureButtonVisibility()
{
    // Hide all nav buttons first
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
    case UserRole::GENERAL_ADMIN:
        // Full access
        ui->residentsBtn->show();
        ui->financesBtn->show();
        ui->incidentsBtn->show();
        ui->transportsBtn->show();
        ui->locauxBtn->show();
        ui->adminInfoBtn->show();
        ui->adminFilesBtn->show();
        ui->settingsBtn->show();
        break;

    case UserRole::SERVICE_CLIENTS:
        // Residents + Incidents + Settings
        ui->residentsBtn->show();
        ui->incidentsBtn->show();
        ui->settingsBtn->show();
        break;

    case UserRole::SERVICE_FINANCIER:
        // Finance + Settings
        ui->financesBtn->show();
        ui->settingsBtn->show();
        break;

    case UserRole::SERVICE_LOGISTIQUE:
        // Transports + Locaux + Settings
        ui->transportsBtn->show();
        ui->locauxBtn->show();
        ui->settingsBtn->show();
        break;

    case UserRole::SERVICE_INFORMATIQUE:
        // IT admin: DB info + Files + Settings
        ui->adminInfoBtn->show();
        ui->adminFilesBtn->show();
        ui->settingsBtn->show();
        break;

    default:
        break;
    }
}

//
// ────────────────────────────────────────────────
//   Button Slots
// ────────────────────────────────────────────────
//
void NexoraDashboard::onResidentsClicked()
{
    if (m_userRole == UserRole::GENERAL_ADMIN ||
        m_userRole == UserRole::SERVICE_CLIENTS)
    {
        showPage(0);
    }
}

void NexoraDashboard::onFinancesClicked()
{
    if (m_userRole == UserRole::GENERAL_ADMIN ||
        m_userRole == UserRole::SERVICE_FINANCIER)
    {
        showPage(1);
    }
}

void NexoraDashboard::onIncidentsClicked()
{
    if (m_userRole == UserRole::GENERAL_ADMIN ||
        m_userRole == UserRole::SERVICE_CLIENTS)
    {
        showPage(2);
    }
}

void NexoraDashboard::onTransportsClicked()
{
    if (m_userRole == UserRole::GENERAL_ADMIN ||
        m_userRole == UserRole::SERVICE_LOGISTIQUE)
    {
        showPage(3);
    }
}

void NexoraDashboard::onLocauxClicked()
{
    if (m_userRole == UserRole::GENERAL_ADMIN ||
        m_userRole == UserRole::SERVICE_LOGISTIQUE)
    {
        showPage(4);
    }
}

void NexoraDashboard::onSettingsClicked()
{
    // All roles that can see the button have access to SettingsPage (index 5)
    showPage(5);
}

void NexoraDashboard::onAdminInfoClicked()
{
    if (m_userRole == UserRole::GENERAL_ADMIN ||
        m_userRole == UserRole::SERVICE_INFORMATIQUE)
    {
        if (!adminInfoPage) {
            adminInfoPage = new AdminDatabaseInfoPage(this);
            pageStack->addWidget(adminInfoPage);
        }
        pageStack->setCurrentWidget(adminInfoPage);
    }
}

void NexoraDashboard::onAdminFilesClicked()
{
    if (m_userRole == UserRole::GENERAL_ADMIN ||
        m_userRole == UserRole::SERVICE_INFORMATIQUE)
    {
        if (!adminFilesPage) {
            adminFilesPage = new AdminFilesPage(this);
            pageStack->addWidget(adminFilesPage);
        }
        pageStack->setCurrentWidget(adminFilesPage);
    }
}

//
// ────────────────────────────────────────────────
//   User Display
// ────────────────────────────────────────────────
//
void NexoraDashboard::updateUserDisplayInfo()
{
    QString roleName;
    QString roleDescription;
    
    switch (m_userRole)
    {
    case UserRole::GENERAL_ADMIN:
        roleName = tr("Administrateur Général");
        roleDescription = tr("Accès complet à toutes les fonctionnalités");
        break;
    case UserRole::SERVICE_CLIENTS:
        roleName = tr("Service Clients");
        roleDescription = tr("Gestion des résidents et incidents");
        break;
    case UserRole::SERVICE_FINANCIER:
        roleName = tr("Service Financier");
        roleDescription = tr("Gestion financière et budgétaire");
        break;
    case UserRole::SERVICE_LOGISTIQUE:
        roleName = tr("Service Logistique");
        roleDescription = tr("Transport et gestion des locaux");
        break;
    case UserRole::SERVICE_INFORMATIQUE:
        roleName = tr("Service Informatique");
        roleDescription = tr("Administration système et base de données");
        break;
    default:
        roleName = tr("Utilisateur");
        roleDescription = tr("Rôle non défini");
        break;
    }
    
    // Create HTML formatted text with icon and role information
    QString userDisplayHtml = QString(
        "<html><head/><body>"
        "<p align=\"center\">"
        "<img src=\":/resources/admin.png\" width=\"14\" height=\"14\"/><br/>"
        "<span style=\"font-weight:600; font-size:11px; color:#ffffff;\">%1</span><br/>"
        "<span style=\"font-size:9px; color:#e0e0e0;\">%2</span>"
        "</p></body></html>"
    ).arg(roleName, roleDescription);
    
    if (ui->userLabel) {
        ui->userLabel->setText(userDisplayHtml);
    }
    
    // Set tooltip with full user information
    QString tooltipText = QString(tr("Utilisateur: %1\nEmail: %2\nRôle: %3"))
                         .arg(m_userName, m_userEmail, roleName);
    if (ui->userLabel) {
        ui->userLabel->setToolTip(tooltipText);
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

//
// ────────────────────────────────────────────────
//   Theme + Language
// ────────────────────────────────────────────────
//
void NexoraDashboard::hookGlobalManagers()
{
    connect(&ThemeManager::instance(), &ThemeManager::themeChanged,
            this, [this](AppTheme){ applyTheme(); });

    connect(&LanguageManager::instance(), &LanguageManager::languageChanged,
            this, [this](const QString&){ reloadTranslations(); });
}

void NexoraDashboard::applyTheme()
{
    ThemeManager::instance().applyTheme(this);
}

void NexoraDashboard::reloadTranslations()
{
    if (ui)
        ui->retranslateUi(this);
}
