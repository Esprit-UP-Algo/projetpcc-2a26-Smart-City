#include <QApplication>
#include <QDebug>
#include "connection.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Récupère l'instance unique du singleton
    Connection &c = Connection::createInstance();
    if (!c.createConnection()) {
        qDebug() << "❌ Oracle connection failed";
    } else {
        qDebug() << "✅ Connected to Oracle (NEXORA / XEPDB1)";
    }

    MainWindow w;
    w.show();

    return a.exec();
}
