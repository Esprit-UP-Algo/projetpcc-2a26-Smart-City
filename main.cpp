#include <QApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QDebug>

#include "mainwindow.h"       // 👈 this is your login window
#include "welcomedialog.h"
#include "dashboardwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 🔗 Optional Oracle connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("OracleXE");   // DSN name
    db.setUserName("NEXORA");
    db.setPassword("nexora_pw");

    if (!db.open()) {
        qDebug() << "❌ Oracle connection failed:" << db.lastError().text();
    } else {
        qDebug() << "✅ Connected to Oracle (NEXORA / XEPDB1)";
    }

    // 1️⃣ Show the login (MainWindow)
    MainWindow w;
    w.show();

    return a.exec();
}
