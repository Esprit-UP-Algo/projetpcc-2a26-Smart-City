#include "dashboardwindow.h"
#include "ui_dashboardwindow.h"
#include "residentspage.h"
#include "financespage.h"
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
    QPixmap logo(":/nexora.png");
    ui->logoLabel->setPixmap(logo.scaled(220, 220, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->logoLabel->setAlignment(Qt::AlignCenter);
    ui->logoLabel->setScaledContents(true);

    setWindowTitle("NEXORA - Smart City Dashboard");
    showFullScreen();

    // Initialisation base de données
    DatabaseManager::instance();

    // Groupe de boutons latéraux (navigation)
    QButtonGroup *navButtonGroup = new QButtonGroup(this);
    navButtonGroup->addButton(ui->dashboardButton);
    navButtonGroup->addButton(ui->residentsButton);
    navButtonGroup->addButton(ui->financesButton);
    navButtonGroup->setExclusive(true);

    // Tableau de bord sélectionné par défaut
    ui->dashboardButton->setChecked(true);
    ui->titleLabel->setText("Tableau de bord");

    // Effet de fondu sur le contenu
    contentOpacity = new QGraphicsOpacityEffect(ui->scrollAreaWidgetContents);
    ui->scrollAreaWidgetContents->setGraphicsEffect(contentOpacity);

    // Charger les pages
    setupPages();
}

void DashboardWindow::setupPages()
{
    // Création du QStackedWidget
    stackedWidget = new QStackedWidget();

    // ----- PAGE TABLEAU DE BORD -----
    QWidget *dashboardPage = new QWidget();
    QVBoxLayout *dashLayout = new QVBoxLayout(dashboardPage);
    dashLayout->setSpacing(20);
    dashLayout->setContentsMargins(20, 20, 20, 20);

    // Titre de bienvenue
    QLabel *welcomeLabel = new QLabel("🏙️ Bienvenue sur NEXORA Smart City Dashboard");
    welcomeLabel->setStyleSheet("font-size: 28px; font-weight: bold; color: #2C3E50; margin-bottom: 10px;");
    dashLayout->addWidget(welcomeLabel);

    // ---- Statistiques ----
    QWidget *statsWidget = new QWidget();
    QHBoxLayout *statsLayout = new QHBoxLayout(statsWidget);
    statsLayout->setSpacing(20);

    DatabaseManager &db = DatabaseManager::instance();
    int totalResidents = db.getAllResidents().size();
    int totalTransactions = db.getAllTransactions().size();

    // Carte : Résidents
    QLabel *residentsCard = new QLabel(QString("👥 Résidents\n\n%1\n\nactifs").arg(totalResidents));
    residentsCard->setStyleSheet("background-color: white; padding: 30px; border-radius: 15px; "
                                 "font-size: 18px; font-weight: bold; color: #4A90E2; min-height: 150px;");
    residentsCard->setAlignment(Qt::AlignCenter);
    statsLayout->addWidget(residentsCard);

    // Carte : Finances
    QLabel *transactionsCard = new QLabel(QString("💰 Transactions\n\n%1\n\nenregistrées").arg(totalTransactions));
    transactionsCard->setStyleSheet("background-color: white; padding: 30px; border-radius: 15px; "
                                    "font-size: 18px; font-weight: bold; color: #9B59B6; min-height: 150px;");
    transactionsCard->setAlignment(Qt::AlignCenter);
    statsLayout->addWidget(transactionsCard);

    dashLayout->addWidget(statsWidget);

    // ---- Actions rapides ----
    QLabel *actionsTitle = new QLabel("⚡ Actions Rapides");
    actionsTitle->setStyleSheet("font-size: 22px; font-weight: bold; color: #2C3E50; margin-top: 20px;");
    dashLayout->addWidget(actionsTitle);

    QWidget *actionsWidget = new QWidget();
    QGridLayout *actionsLayout = new QGridLayout(actionsWidget);
    actionsLayout->setSpacing(15);

    // Bouton : Nouveau résident
    QPushButton *addResidentBtn = new QPushButton("👤 Nouveau Résident");
    addResidentBtn->setStyleSheet("background-color: #4A90E2; color: white; padding: 20px; "
                                  "border-radius: 10px; font-size: 16px; font-weight: bold;");
    addResidentBtn->setMinimumHeight(80);

    // Bouton : Nouvelle transaction
    QPushButton *addTransactionBtn = new QPushButton("💳 Nouvelle Transaction");
    addTransactionBtn->setStyleSheet("background-color: #9B59B6; color: white; padding: 20px; "
                                     "border-radius: 10px; font-size: 16px; font-weight: bold;");
    addTransactionBtn->setMinimumHeight(80);

    actionsLayout->addWidget(addResidentBtn, 0, 0);
    actionsLayout->addWidget(addTransactionBtn, 0, 1);

    dashLayout->addWidget(actionsWidget);
    dashLayout->addStretch();

    // ----- PAGES -----
    residentsPage = new ResidentsPage();
    financesPage = new FinancesPage();
    settingsPage = new SettingsPage();

    stackedWidget->addWidget(dashboardPage);   // index 0
    stackedWidget->addWidget(residentsPage);   // index 1
    stackedWidget->addWidget(financesPage);    // index 2
    stackedWidget->addWidget(settingsPage);    // index 3

    // Remplacer le contenu du scrollArea par le stackedWidget
    QVBoxLayout *contentLayout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->layout());
    if (!contentLayout) {
        contentLayout = new QVBoxLayout(ui->scrollAreaWidgetContents);
    }

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

// ----- ANIMATION -----
void DashboardWindow::animatePageTransition(const QString &newTitle)
{
    QPropertyAnimation *fadeOut = new QPropertyAnimation(contentOpacity, "opacity");
    fadeOut->setDuration(200);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.3);
    fadeOut->setEasingCurve(QEasingCurve::InOutQuad);

    QPropertyAnimation *fadeIn = new QPropertyAnimation(contentOpacity, "opacity");
    fadeIn->setDuration(300);
    fadeIn->setStartValue(0.3);
    fadeIn->setEndValue(1.0);
    fadeIn->setEasingCurve(QEasingCurve::InOutQuad);

    connect(fadeOut, &QPropertyAnimation::finished, [this, newTitle, fadeIn]() {
        ui->titleLabel->setText(newTitle);
        fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
    });

    fadeOut->start(QAbstractAnimation::DeleteWhenStopped);
}

// ----- BOUTONS -----
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
void DashboardWindow::on_settingsButton_clicked()
{
    animatePageTransition("Paramètres");
    stackedWidget->setCurrentIndex(3);  // Index de la page "Paramètres" dans setupPages()
}


// ----- FONCTIONS COMMUNES -----
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

void DashboardWindow::on_exitButton_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Quitter NEXORA",
        "Êtes-vous sûr de vouloir quitter l'application ?",
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        QApplication::quit();
    }
}
