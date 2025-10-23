#include <QApplication>
#include <QDebug>
#include "connection.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Connection c;
    if (!c.createconnect()) {
        qDebug() << "❌ Oracle connection failed";
    } else {
        qDebug() << "✅ Connected to Oracle (NEXORA / XEPDB1)";
    }

    MainWindow w;
    w.show();

    return a.exec();
}
