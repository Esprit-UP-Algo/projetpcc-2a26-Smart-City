#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label_2->setAttribute(Qt::WA_TranslucentBackground);
    ui->label_2->setStyleSheet("background: transparent;");
    ui->label_2->setAutoFillBackground(false);
    ui->tableWidget->setColumnWidth(0, 120);  // Colonne CIN = 120px
    ui->tableWidget->setColumnWidth(1, 200);  // Colonne Nom et Prénom = 200px
    ui->tableWidget->setColumnWidth(2, 100);
    ui->tableWidget->setColumnWidth(3, 120);  // Colonne CIN = 120px
    ui->tableWidget->setColumnWidth(4, 100);  // Colonne Nom et Prénom = 200px
    ui->tableWidget->setColumnWidth(5, 200);


    // 👉 Bouton "Ajouter transaction" → page 0
    connect(ui->btnAjouterTransaction, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(0);
    });

    // 👉 Bouton "Liste" → page 1
    connect(ui->btnListe, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(1);
    });

    // 👉 Bouton "Statistique" → page 2
    connect(ui->btnStatistique, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(2);
        // ou : ui->stackedWidget->setCurrentWidget(ui->pageStatistique);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

