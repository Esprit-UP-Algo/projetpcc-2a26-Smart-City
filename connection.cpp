#include "connection.h"
#include <QDebug>

Connection::Connection()
{
    // Initialise l'attribut db avec le driver ODBC
    db = QSqlDatabase::addDatabase("QODBC");
}

Connection::~Connection()
{
    // Ferme la connexion si elle est encore ouverte
    if (db.isOpen())
        db.close();
}

// Meyers singleton : instance locale statique, thread-safe en C++11+
Connection& Connection::createInstance()
{
    static Connection instance;
    return instance;
}

bool Connection::createConnection()
{
    // Configure la connexion (valeurs reprises du projet)
    db.setDatabaseName("OracleXE");
    db.setUserName("NEXORA");
    db.setPassword("nexora124");

    if (db.open()) {
        qDebug() << "Connexion établie";
        return true;
    }

    qDebug() << "Échec de la connexion :" << db.lastError().text();
    return false;
}
