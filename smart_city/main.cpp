#include <QApplication>
#include <QDebug>
#include "connection.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Utiliser le nouveau pattern singleton
    Connection& c = Connection::getInstance();

    if (!c.createConnection()) {
        qDebug() << "❌ Oracle connection failed";
        return -1; // Quitter si connexion échoue
    } else {
        qDebug() << "✅ Connected to Oracle (NEXORA / XE)";
    }

    MainWindow loginWindow;
    loginWindow.show();

    return a.exec();
}
