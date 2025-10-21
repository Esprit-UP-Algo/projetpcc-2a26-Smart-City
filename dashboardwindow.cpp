#include "dashboardwindow.h"
#include "ui_dashboardwindow.h"
#include "residentspage.h"
#include "financespage.h"
#include "incidentspage.h"
#include "transportspage.h"
#include "locauxpage.h"
#include "settingspage.h"
#include "databasemanager.h"
#include <QButtonGroup>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QParallelAnimationGroup>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QApplication>

DashboardWindow::DashboardWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DashboardWindow)
{
    ui->setupUi(this);

    setWindowTitle("NEXORA - Smart City Dashboard");
    showFullScreen();

    // Initialize database
    DatabaseManager::instance();

    // Create button group for sidebar navigation
    QButtonGroup *navButtonGroup = new QButtonGroup(this);
    navButtonGroup->addButton(ui->dashboardButton);
    navButtonGroup->addButton(ui->residentsButton);
    navButtonGroup->addButton(ui->financesButton);
    navButtonGroup->addButton(ui->transportsButton);
    navButtonGroup->addButton(ui->incidentsButton);
    navButtonGroup->addButton(ui->locauxButton);
    navButtonGroup->addButton(ui->settingsButton);
    navButtonGroup->setExclusive(true);

    // Set dashboard as default selected
    ui->dashboardButton->setChecked(true);
    ui->titleLabel->setText("Tableau de bord");

    // Setup opacity effect for content animation
    contentOpacity = new QGraphicsOpacityEffect(ui->scrollAreaWidgetContents);
    ui->scrollAreaWidgetContents->setGraphicsEffect(contentOpacity);

    // Setup pages
    setupPages();
}

void DashboardWindow::setupPages()
{
    // Create stacked widget to hold different pages
    stackedWidget = new QStackedWidget();

    // Create dashboard page with statistics
    QWidget *dashboardPage = new QWidget();
    QVBoxLayout *dashLayout = new QVBoxLayout(dashboardPage);
    dashLayout->setSpacing(20);
    dashLayout->setContentsMargins(20, 20, 20, 20);

    // Welcome title
    QLabel *welcomeLabel = new QLabel("🏙️ Bienvenue sur NEXORA Smart City Dashboard");
    welcomeLabel->setStyleSheet("font-size: 28px; font-weight: bold; color: #2C3E50; margin-bottom: 10px;");
    dashLayout->addWidget(welcomeLabel);

    // Stats container
    QWidget *statsWidget = new QWidget();
    QHBoxLayout *statsLayout = new QHBoxLayout(statsWidget);
    statsLayout->setSpacing(20);

    // Get statistics from database
    DatabaseManager &db = DatabaseManager::instance();
    int totalResidents = db.getAllResidents().size();
    int totalTransactions = db.getAllTransactions().size();

    // Residents stat card
    QLabel *residentsCard = new QLabel(QString("👥 Résidents\n\n%1\n\nactifs").arg(totalResidents));
    residentsCard->setStyleSheet("background-color: white; padding: 30px; border-radius: 15px; font-size: 18px; font-weight: bold; color: #4A90E2; min-height: 150px;");
    residentsCard->setAlignment(Qt::AlignCenter);
    statsLayout->addWidget(residentsCard);

    // Transactions stat card
    QLabel *transactionsCard = new QLabel(QString("💰 Transactions\n\n%1\n\nenregistrées").arg(totalTransactions));
    transactionsCard->setStyleSheet("background-color: white; padding: 30px; border-radius: 15px; font-size: 18px; font-weight: bold; color: #9B59B6; min-height: 150px;");
    transactionsCard->setAlignment(Qt::AlignCenter);
    statsLayout->addWidget(transactionsCard);

    // Incidents stat card
    QLabel *incidentsCard = new QLabel("🚨 Incidents\n\n0\n\nactifs");
    incidentsCard->setStyleSheet("background-color: white; padding: 30px; border-radius: 15px; font-size: 18px; font-weight: bold; color: #E74C3C; min-height: 150px;");
    incidentsCard->setAlignment(Qt::AlignCenter);
    statsLayout->addWidget(incidentsCard);

    // Transports stat card
    QLabel *transportsCard = new QLabel("🚗 Véhicules\n\n0\n\ndans la flotte");
    transportsCard->setStyleSheet("background-color: white; padding: 30px; border-radius: 15px; font-size: 18px; font-weight: bold; color: #3498DB; min-height: 150px;");
    transportsCard->setAlignment(Qt::AlignCenter);
    statsLayout->addWidget(transportsCard);
    
    // Locaux stat card
    QLabel *locauxCard = new QLabel("🏢 Locaux\n\n5\n\nenregistrés");
    locauxCard->setStyleSheet("background-color: white; padding: 30px; border-radius: 15px; font-size: 18px; font-weight: bold; color: #1ABC9C; min-height: 150px;");
    locauxCard->setAlignment(Qt::AlignCenter);
    statsLayout->addWidget(locauxCard);

    dashLayout->addWidget(statsWidget);

    // Quick actions section
    QLabel *actionsTitle = new QLabel("⚡ Actions Rapides");
    actionsTitle->setStyleSheet("font-size: 22px; font-weight: bold; color: #2C3E50; margin-top: 20px;");
    dashLayout->addWidget(actionsTitle);

    QWidget *actionsWidget = new QWidget();
    QGridLayout *actionsLayout = new QGridLayout(actionsWidget);
    actionsLayout->setSpacing(15);

    // Quick action buttons
    QPushButton *addResidentBtn = new QPushButton("👤 Nouveau Résident");
    addResidentBtn->setStyleSheet("background-color: #4A90E2; color: white; padding: 20px; border-radius: 10px; font-size: 16px; font-weight: bold;");
    addResidentBtn->setMinimumHeight(80);

    QPushButton *addTransactionBtn = new QPushButton("💳 Nouvelle Transaction");
    addTransactionBtn->setStyleSheet("background-color: #9B59B6; color: white; padding: 20px; border-radius: 10px; font-size: 16px; font-weight: bold;");
    addTransactionBtn->setMinimumHeight(80);

    QPushButton *addIncidentBtn = new QPushButton("🚨 Déclarer Incident");
    addIncidentBtn->setStyleSheet("background-color: #E74C3C; color: white; padding: 20px; border-radius: 10px; font-size: 16px; font-weight: bold;");
    addIncidentBtn->setMinimumHeight(80);

    QPushButton *addVehicleBtn = new QPushButton("🚙 Ajouter Véhicule");
    addVehicleBtn->setStyleSheet("background-color: #3498DB; color: white; padding: 20px; border-radius: 10px; font-size: 16px; font-weight: bold;");
    addVehicleBtn->setMinimumHeight(80);
    
    QPushButton *addLocalBtn = new QPushButton("🏢 Nouveau Local");
    addLocalBtn->setStyleSheet("background-color: #1ABC9C; color: white; padding: 20px; border-radius: 10px; font-size: 16px; font-weight: bold;");
    addLocalBtn->setMinimumHeight(80);

    actionsLayout->addWidget(addResidentBtn, 0, 0);
    actionsLayout->addWidget(addTransactionBtn, 0, 1);
    actionsLayout->addWidget(addIncidentBtn, 1, 0);
    actionsLayout->addWidget(addVehicleBtn, 1, 1);
    actionsLayout->addWidget(addLocalBtn, 0, 2);

    dashLayout->addWidget(actionsWidget);
    dashLayout->addStretch();

    // Create residents page
    residentsPage = new ResidentsPage();

    // Create finances page
    financesPage = new FinancesPage();

    // Create incidents page
    incidentsPage = new IncidentsPage();

    // Create transports page
    transportsPage = new TransportsPage();

    // Create locaux page
    locauxPage = new LocauxPage();

    // Create settings page
    settingsPage = new SettingsPage();

    // Add pages to stacked widget
    stackedWidget->addWidget(dashboardPage);     // Index 0
    stackedWidget->addWidget(residentsPage);      // Index 1
    stackedWidget->addWidget(financesPage);       // Index 2
    stackedWidget->addWidget(incidentsPage);      // Index 3
    stackedWidget->addWidget(transportsPage);     // Index 4
    stackedWidget->addWidget(locauxPage);         // Index 5
    stackedWidget->addWidget(settingsPage);       // Index 6

    // Replace the scroll area contents with stacked widget
    QVBoxLayout *contentLayout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->layout());
    if (!contentLayout) {
        contentLayout = new QVBoxLayout(ui->scrollAreaWidgetContents);
    }

    // Clear existing content
    QLayoutItem *item;
    while ((item = contentLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    contentLayout->addWidget(stackedWidget);
    contentLayout->setContentsMargins(0, 0, 0, 0);
}

DashboardWindow::~DashboardWindow()
{
    delete ui;
}

void DashboardWindow::animatePageTransition(const QString &newTitle)
{
    // Fade out animation
    QPropertyAnimation *fadeOut = new QPropertyAnimation(contentOpacity, "opacity");
    fadeOut->setDuration(200);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.3);
    fadeOut->setEasingCurve(QEasingCurve::InOutQuad);

    // Fade in animation
    QPropertyAnimation *fadeIn = new QPropertyAnimation(contentOpacity, "opacity");
    fadeIn->setDuration(300);
    fadeIn->setStartValue(0.3);
    fadeIn->setEndValue(1.0);
    fadeIn->setEasingCurve(QEasingCurve::InOutQuad);

    // Connect animations
    connect(fadeOut, &QPropertyAnimation::finished, [this, newTitle, fadeIn]() {
        ui->titleLabel->setText(newTitle);
        fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
    });

    fadeOut->start(QAbstractAnimation::DeleteWhenStopped);
}

void DashboardWindow::on_dashboardButton_clicked()
{
    animatePageTransition("Tableau de bord");
    stackedWidget->setCurrentIndex(0);
}

void DashboardWindow::on_residentsButton_clicked()
{
    animatePageTransition("Résidents");
    stackedWidget->setCurrentIndex(1);
}

void DashboardWindow::on_financesButton_clicked()
{
    animatePageTransition("Finances");
    stackedWidget->setCurrentIndex(2);
}

void DashboardWindow::on_transportsButton_clicked()
{
    animatePageTransition("Transports");
    stackedWidget->setCurrentIndex(4);
}

void DashboardWindow::on_incidentsButton_clicked()
{
    animatePageTransition("Incidents");
    stackedWidget->setCurrentIndex(3);
}

void DashboardWindow::on_locauxButton_clicked()
{
    animatePageTransition("Locaux");
    stackedWidget->setCurrentIndex(5);
}

void DashboardWindow::on_settingsButton_clicked()
{
    animatePageTransition("Paramètres");
    stackedWidget->setCurrentIndex(6);
}

void DashboardWindow::on_exportPdfButton_clicked()
{
    QMessageBox::information(this, "Export PDF", "Export PDF en cours...");
}

void DashboardWindow::on_exportCsvButton_clicked()
{
    QMessageBox::information(this, "Export CSV", "Export CSV en cours...");
}

void DashboardWindow::on_notificationsButton_clicked()
{
    QMessageBox::information(this, "Notifications", "3 nouvelles notifications");
}

void DashboardWindow::on_searchButton_clicked()
{
    QString searchText = ui->searchLineEdit->text().trimmed();

    if (searchText.isEmpty()) {
        QMessageBox::warning(this, "Recherche", "Veuillez entrer un terme de recherche");
        return;
    }

    // Perform actual search in database
    DatabaseManager &db = DatabaseManager::instance();

    QString results = "🔍 Résultats de recherche pour: \"" + searchText + "\"\n\n";
    int totalResults = 0;

    // Search in residents
    QList<QVariantMap> residents = db.searchResidents(searchText);
    if (!residents.isEmpty()) {
        results += QString("👥 Résidents (%1 résultat(s)):\n").arg(residents.size());
        for (int i = 0; i < qMin(3, residents.size()); i++) {
            results += QString("  • %1 %2 - Apt %3\n")
                           .arg(residents[i]["nom"].toString())
                           .arg(residents[i]["prenom"].toString())
                           .arg(residents[i]["numero_appartement"].toString());
        }
        if (residents.size() > 3) {
            results += QString("  ... et %1 autre(s)\n").arg(residents.size() - 3);
        }
        results += "\n";
        totalResults += residents.size();
    }

    // Search in transactions
    QList<QVariantMap> transactions = db.searchTransactions(searchText);
    if (!transactions.isEmpty()) {
        results += QString("💰 Finances (%1 résultat(s)):\n").arg(transactions.size());
        for (int i = 0; i < qMin(3, transactions.size()); i++) {
            results += QString("  • %1 - %2 DT (%3)\n")
                           .arg(transactions[i]["code"].toString())
                           .arg(transactions[i]["montant"].toDouble())
                           .arg(transactions[i]["type"].toString());
        }
        if (transactions.size() > 3) {
            results += QString("  ... et %1 autre(s)\n").arg(transactions.size() - 3);
        }
        results += "\n";
        totalResults += transactions.size();
    }

    // Add module suggestions
    if (totalResults == 0) {
        results += "❌ Aucun résultat trouvé.\n\n";
        results += "💡 Suggestions:\n";
        results += "  • Vérifiez l'orthographe\n";
        results += "  • Essayez des termes plus généraux\n";
        results += "  • Utilisez la recherche dans chaque module\n";
    } else {
        results += QString("✅ Total: %1 résultat(s) trouvé(s)\n\n").arg(totalResults);
        results += "💡 Cliquez sur le module concerné pour voir plus de détails.";
    }

    QMessageBox::information(this, "Recherche Globale", results);

    // Clear search field
    ui->searchLineEdit->clear();
}

void DashboardWindow::on_exitButton_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Quitter NEXORA",
        "Êtes-vous sûr de vouloir quitter l'application?",
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        QApplication::quit();
    }
}

void DashboardWindow::on_searchLineEdit_returnPressed()
{
    // Trigger search when Enter is pressed
    on_searchButton_clicked();
}
