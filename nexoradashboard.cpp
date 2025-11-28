#include "nexoradashboard.h"
#include "ui_nexoradashboard.h"
#include "residentspage.h"
#include "financespage.h"
#include "incidentspage.h"
#include "transportpage.h"
#include "locauxpage.h"
#include "settingspage.h"
#include "databasemanager.h"
#include "mainwindow.h"
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QFrame>
#include <QPushButton>
#include <QMessageBox>
#include <QApplication>

NexoraDashboard::NexoraDashboard(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::NexoraDashboard)
    , pageStack(nullptr)
    , residentsPage(nullptr)
    , financesPage(nullptr)
    , incidentsPage(nullptr)
    , locauxPage(nullptr)
    , settingsPage(nullptr)
    , pageTransition(nullptr)
    , pageOpacity(nullptr)
{
    ui->setupUi(this);
    
    // Initialize database
    DatabaseManager::instance();
    
    setupUI();
    setupNavigation();
    setupPages();
    setupAnimations();
    
    // Show fullscreen
    showMaximized();
    
    // Start with residents page by default and set button as checked
    ui->residentsBtn->setChecked(true);
    showPage(0, "👥 Résidents");
}

NexoraDashboard::~NexoraDashboard()
{
    delete ui;
}

void NexoraDashboard::setupUI()
{
    setWindowTitle("Smart City - Menu Principal");
    
    // Use the page stack widget from UI directly
    pageStack = ui->pageStackedWidget;
    
    // Configure the page stack styling
    pageStack->setStyleSheet(R"(
        QStackedWidget {
            background: #F5F5F5;
            border: none;
            margin: 0px;
            padding: 20px;
        }
    )");
}

void NexoraDashboard::setupNavigation()
{
    // Create button group for exclusive selection
    QButtonGroup *navGroup = new QButtonGroup(this);
    navGroup->addButton(ui->residentsBtn);
    navGroup->addButton(ui->financesBtn);
    navGroup->addButton(ui->incidentsBtn);
    navGroup->addButton(ui->transportsBtn);
    navGroup->addButton(ui->locauxBtn);
    navGroup->addButton(ui->settingsBtn);
    navGroup->setExclusive(true);
    
    // Connect sidebar buttons to navigation
    connect(ui->residentsBtn, &QPushButton::clicked, this, &NexoraDashboard::onResidentsClicked);
    connect(ui->financesBtn, &QPushButton::clicked, this, &NexoraDashboard::onFinancesClicked);
    connect(ui->incidentsBtn, &QPushButton::clicked, this, &NexoraDashboard::onIncidentsClicked);
    connect(ui->transportsBtn, &QPushButton::clicked, this, &NexoraDashboard::onTransportsClicked);
    connect(ui->locauxBtn, &QPushButton::clicked, this, &NexoraDashboard::onLocauxClicked);
    connect(ui->settingsBtn, &QPushButton::clicked, this, &NexoraDashboard::onSettingsClicked);
    
    // Connect logout button
    connect(ui->logoutBtn, &QPushButton::clicked, this, &NexoraDashboard::onLogoutClicked);
}

void NexoraDashboard::setupPages()
{
    // Create module pages
    residentsPage = new ResidentsPage();
    financesPage = new FinancesPage();
    incidentsPage = new IncidentsPage();
    transportsPage = new TransportPage();
    locauxPage = new LocauxPage();
    settingsPage = new SettingsPage();
    
    // Add pages to stack
    pageStack->addWidget(residentsPage);               // Index 0
    pageStack->addWidget(financesPage);                // Index 1
    pageStack->addWidget(incidentsPage);               // Index 2
    pageStack->addWidget(transportsPage);               // Index 3
    pageStack->addWidget(locauxPage);                  // Index 4
    pageStack->addWidget(settingsPage);                // Index 5
}

void NexoraDashboard::setupAnimations()
{
    // Create opacity effect for smooth page transitions
    pageOpacity = new QGraphicsOpacityEffect(pageStack);
    pageStack->setGraphicsEffect(pageOpacity);
    
    // Create page transition animation
    pageTransition = new QPropertyAnimation(pageOpacity, "opacity");
    pageTransition->setDuration(250);
    pageTransition->setEasingCurve(QEasingCurve::InOutQuad);
}

void NexoraDashboard::showPage(int pageIndex, const QString &pageTitle)
{
    Q_UNUSED(pageTitle) // Suppress unused parameter warning
    
    // Switch to the requested page with animation
    pageTransition->stop();
    pageTransition->setStartValue(1.0);
    pageTransition->setEndValue(0.0);
    
    connect(pageTransition, &QPropertyAnimation::finished, this, [this, pageIndex]() {
        pageStack->setCurrentIndex(pageIndex);
        pageTransition->disconnect();
        pageTransition->setStartValue(0.0);
        pageTransition->setEndValue(1.0);
        pageTransition->start();
    });
    
    pageTransition->start();
}

// Navigation slots - now they show individual pages
void NexoraDashboard::onResidentsClicked()
{
    showPage(0, "👥 Résidents");
}

void NexoraDashboard::onFinancesClicked()
{
    showPage(1, "💰 Finances");
}

void NexoraDashboard::onIncidentsClicked()
{
    showPage(2, "🚨 Incidents");
}

void NexoraDashboard::onTransportsClicked()
{
    showPage(3, "🚗 Transport");
}

void NexoraDashboard::onLocauxClicked()
{
    showPage(4, "🏢 Locaux");
}

void NexoraDashboard::onSettingsClicked()
{
    showPage(5, "⚙️ Paramètres");
}

void NexoraDashboard::onLogoutClicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, 
        tr("Déconnexion"),
        tr("Êtes-vous sûr de vouloir vous déconnecter ?"),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        // Create and show new login window
        MainWindow *loginWindow = new MainWindow();
        loginWindow->show();
        
        // Close current dashboard
        this->close();
        this->deleteLater();
    }
}
