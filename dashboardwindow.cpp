#include "dashboardwindow.h"
#include "ui_dashboardwindow.h"
#include "residentspage.h"
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

    // Initialize database
    DatabaseManager::instance();

    // Sidebar buttons (only residents + settings)
    QButtonGroup *navButtonGroup = new QButtonGroup(this);
    navButtonGroup->addButton(ui->dashboardButton);
    navButtonGroup->addButton(ui->residentsButton);
    navButtonGroup->addButton(ui->settingsButton);
    navButtonGroup->setExclusive(true);

    ui->dashboardButton->setChecked(true);
    ui->titleLabel->setText("Tableau de bord");

    // Opacity animation
    contentOpacity = new QGraphicsOpacityEffect(ui->scrollAreaWidgetContents);
    ui->scrollAreaWidgetContents->setGraphicsEffect(contentOpacity);

    setupPages();
}

void DashboardWindow::setupPages()
{
    // Main stacked widget
    stackedWidget = new QStackedWidget();

    // --- Dashboard Page ---
    QWidget *dashboardPage = new QWidget();
    QVBoxLayout *dashLayout = new QVBoxLayout(dashboardPage);
    dashLayout->setSpacing(20);
    dashLayout->setContentsMargins(20, 20, 20, 20);

    QLabel *welcomeLabel = new QLabel("🏙️ Bienvenue sur NEXORA Smart City Dashboard");
    welcomeLabel->setStyleSheet("font-size: 28px; font-weight: bold; color: #2C3E50;");
    dashLayout->addWidget(welcomeLabel);

    // Stats cards container
    QWidget *statsWidget = new QWidget();
    QHBoxLayout *statsLayout = new QHBoxLayout(statsWidget);
    statsLayout->setSpacing(20);

    DatabaseManager &db = DatabaseManager::instance();
    int totalResidents = db.getAllResidents().size();

    QLabel *residentsCard = new QLabel(QString("👥 Résidents\n\n%1\n\nactifs").arg(totalResidents));
    residentsCard->setStyleSheet("background-color: white; padding: 30px; border-radius: 15px; font-size: 18px; font-weight: bold; color: #4A90E2;");
    residentsCard->setAlignment(Qt::AlignCenter);
    statsLayout->addWidget(residentsCard);

    dashLayout->addWidget(statsWidget);

    QLabel *actionsTitle = new QLabel("⚡ Actions Rapides");
    actionsTitle->setStyleSheet("font-size: 22px; font-weight: bold; color: #2C3E50;");
    dashLayout->addWidget(actionsTitle);

    QWidget *actionsWidget = new QWidget();
    QHBoxLayout *actionsLayout = new QHBoxLayout(actionsWidget);
    actionsLayout->setSpacing(15);

    QPushButton *addResidentBtn = new QPushButton("👤 Nouveau Résident");
    addResidentBtn->setStyleSheet("background-color: #4A90E2; color: white; padding: 20px; border-radius: 10px; font-size: 16px; font-weight: bold;");
    addResidentBtn->setMinimumHeight(80);
    actionsLayout->addWidget(addResidentBtn);

    dashLayout->addWidget(actionsWidget);
    dashLayout->addStretch();

    // --- Residents Page ---
    residentsPage = new ResidentsPage();

    // --- Settings Page ---
    settingsPage = new SettingsPage();

    // Add pages
    stackedWidget->addWidget(dashboardPage);  // 0
    stackedWidget->addWidget(residentsPage);  // 1
    stackedWidget->addWidget(settingsPage);   // 2

    // Place into scroll area
    QVBoxLayout *contentLayout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->layout());
    if (!contentLayout)
        contentLayout = new QVBoxLayout(ui->scrollAreaWidgetContents);

    QLayoutItem *item;
    while ((item = contentLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    contentLayout->addWidget(stackedWidget);
}

DashboardWindow::~DashboardWindow()
{
    delete ui;
}

void DashboardWindow::animatePageTransition(const QString &newTitle)
{
    QPropertyAnimation *fadeOut = new QPropertyAnimation(contentOpacity, "opacity");
    fadeOut->setDuration(200);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.3);

    QPropertyAnimation *fadeIn = new QPropertyAnimation(contentOpacity, "opacity");
    fadeIn->setDuration(300);
    fadeIn->setStartValue(0.3);
    fadeIn->setEndValue(1.0);

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

void DashboardWindow::on_settingsButton_clicked()
{
    animatePageTransition("Paramètres");
    stackedWidget->setCurrentIndex(2);
}

void DashboardWindow::on_exitButton_clicked()
{
    if (QMessageBox::question(this, "Quitter", "Voulez-vous quitter NEXORA ?",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        QApplication::quit();
    }
}
void DashboardWindow::on_financesButton_clicked() {}
void DashboardWindow::on_transportsButton_clicked() {}
void DashboardWindow::on_incidentsButton_clicked() {}
void DashboardWindow::on_locauxButton_clicked() {}
void DashboardWindow::on_exportPdfButton_clicked() {}
void DashboardWindow::on_exportCsvButton_clicked() {}
void DashboardWindow::on_notificationsButton_clicked() {}
void DashboardWindow::on_searchButton_clicked() {}
void DashboardWindow::on_searchLineEdit_returnPressed() {}

