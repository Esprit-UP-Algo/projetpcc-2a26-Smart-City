#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 👉 Bouton "Ajouter transaction" → page 0
    connect(ui->btnAjouterVehicule, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(1);
    });

    // 👉 Bouton "Liste" → page 1
    connect(ui->btnStatistique, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(2);
    });

    // 👉 Bouton "Statistique" → page 2
    connect(ui->btnListe, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(0);
        // ou : ui->stackedWidget->setCurrentWidget(ui->pageStatistique);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
