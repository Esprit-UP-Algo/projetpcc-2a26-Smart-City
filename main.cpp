#include <QApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QDebug>

#include "mainwindow.h"       // ta page de login
#include "welcomedialog.h"
#include "dashboardwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 🔹 Liste des drivers disponibles
    qDebug() << "Drivers disponibles :" << QSqlDatabase::drivers();

    // 🔹 Connexion Oracle via driver direct (64-bit)
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");

    // Remplace le driver par le nom exact que tu as dans ODBC 64-bit
    db.setDatabaseName("Driver={Oracle in InstantClient_23_8};DBQ=localhost:1521/XE;UID=SYSTEM;PWD=wajd123;");


    if (!db.open()) {
        qDebug() << "❌ Oracle connection failed:" << db.lastError().text();
    } else {
        qDebug() << "✅ Connected to Oracle (NEXORA / XE)";
    }

    // 🔹 Ouvre la page de login (MainWindow)
    MainWindow w;
    w.show();

    return a.exec();
}
