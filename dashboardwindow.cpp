#include "dashboardwindow.h"
#include "ui_dashboardwindow.h"
#include "incidentspage.h"
#include "transportspage.h"
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

    // --- Logo ---
    QPixmap logo(":/nexora.png");
    ui->logoLabel->setPixmap(logo.scaled(220, 220, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->logoLabel->setAlignment(Qt::AlignCenter);
    ui->logoLabel->setScaledContents(true);

    setWindowTitle("NEXORA - Smart City Dashboard");
    showFullScreen();

    // --- Base de données ---
    DatabaseManager::instance();

    // --- Groupe de boutons pour la navigation ---
    QButtonGroup *navButtonGroup = new QButtonGroup(this);
    navButtonGroup->addButton(ui->dashboardButton);
    navButtonGroup->addButton(ui->incidentsButton);
    navButtonGroup->addButton(ui->transportsButton);
    navButtonGroup->addButton(ui->settingsButton);
    navButtonGroup->setExclusive(true);

    ui->dashboardButton->setChecked(true);
    ui->titleLabel->setText("Tableau de bord");

    // --- Effet d'opacité pour l'animation de transition ---
    contentOpacity = new QGraphicsOpacityEffect(ui->scrollAreaWidgetContents);
    ui->scrollAreaWidgetContents->setGraphicsEffect(contentOpacity);

    setupPages();
}

void DashboardWindow::setupPages()
{
    stackedWidget = new QStackedWidget();

    // --- Page d'accueil (tableau de bord) ---
    QWidget *dashboardPage = new QWidget();
    QVBoxLayout *dashLayout = new QVBoxLayout(dashboardPage);
    dashLayout->setSpacing(20);
    dashLayout->setContentsMargins(20, 20, 20, 20);

    QLabel *welcomeLabel = new QLabel("🏙️ Bienvenue sur NEXORA Smart City Dashboard");
    welcomeLabel->setStyleSheet("font-size: 28px; font-weight: bold; color: #2C3E50;");
    dashLayout->addWidget(welcomeLabel);

    QWidget *statsWidget = new QWidget();
    QHBoxLayout *statsLayout = new QHBoxLayout(statsWidget);
    statsLayout->setSpacing(20);

    // --- Statistiques simplifiées ---
    QLabel *incidentsCard = new QLabel("🚨 Incidents\n\n0\n\nactifs");
    incidentsCard->setStyleSheet("background-color: white; padding: 30px; border-radius: 15px; font-size: 18px; font-weight: bold; color: #E74C3C; min-height: 150px;");
    incidentsCard->setAlignment(Qt::AlignCenter);
    statsLayout->addWidget(incidentsCard);

    QLabel *transportsCard = new QLabel("🚗 Véhicules\n\n0\n\ndans la flotte");
    transportsCard->setStyleSheet("background-color: white; padding: 30px; border-radius: 15px; font-size: 18px; font-weight: bold; color: #3498DB; min-height: 150px;");
    transportsCard->setAlignment(Qt::AlignCenter);
    statsLayout->addWidget(transportsCard);

    dashLayout->addWidget(statsWidget);

    QLabel *actionsTitle = new QLabel("⚡ Actions Rapides");
    actionsTitle->setStyleSheet("font-size: 22px; font-weight: bold; color: #2C3E50;");
    dashLayout->addWidget(actionsTitle);

    QWidget *actionsWidget = new QWidget();
    QGridLayout *actionsLayout = new QGridLayout(actionsWidget);
    actionsLayout->setSpacing(15);

    // --- Boutons d’action rapide ---
    QPushButton *addIncidentBtn = new QPushButton("🚨 Déclarer un Incident");
    addIncidentBtn->setStyleSheet("background-color: #E74C3C; color: white; padding: 20px; border-radius: 10px; font-size: 16px; font-weight: bold;");
    addIncidentBtn->setMinimumHeight(80);

    QPushButton *addVehicleBtn = new QPushButton("🚙 Ajouter un Véhicule");
    addVehicleBtn->setStyleSheet("background-color: #3498DB; color: white; padding: 20px; border-radius: 10px; font-size: 16px; font-weight: bold;");
    addVehicleBtn->setMinimumHeight(80);

    actionsLayout->addWidget(addIncidentBtn, 0, 0);
    actionsLayout->addWidget(addVehicleBtn, 0, 1);

    dashLayout->addWidget(actionsWidget);
    dashLayout->addStretch();

    // --- Création des pages principales ---
    incidentsPage = new IncidentsPage();
    transportsPage = new TransportsPage();
    settingsPage = new SettingsPage();

    // --- Ajout des pages au QStackedWidget ---
    stackedWidget->addWidget(dashboardPage);   // Index 0
    stackedWidget->addWidget(incidentsPage);   // Index 1
    stackedWidget->addWidget(transportsPage);  // Index 2
    stackedWidget->addWidget(settingsPage);    // Index 3

    // --- Ajout dans la zone de contenu ---
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

// --- Navigation entre les pages ---
void DashboardWindow::on_dashboardButton_clicked()
{
    animatePageTransition("Tableau de bord");
    stackedWidget->setCurrentIndex(0);
}

void DashboardWindow::on_incidentsButton_clicked()
{
    animatePageTransition("Gestion des Incidents");
    stackedWidget->setCurrentIndex(1);
}

void DashboardWindow::on_transportsButton_clicked()
{
    animatePageTransition("Gestion des Transports");
    stackedWidget->setCurrentIndex(2);
}

void DashboardWindow::on_settingsButton_clicked()
{
    animatePageTransition("Paramètres");
    stackedWidget->setCurrentIndex(3);
}

// --- Boutons du header (facultatifs) ---
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
    if (QMessageBox::question(this, "Quitter NEXORA", "Êtes-vous sûr de vouloir quitter ?") == QMessageBox::Yes)
        QApplication::quit();
}
