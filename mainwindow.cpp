#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connection.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if (!Connection::connecterOracle()) {
        qDebug() << "Impossible de se connecter à Oracle";
    } else {
        qDebug() << "Connexion réussie !";
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}
