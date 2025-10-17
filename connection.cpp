#include "connection.h"

Connection::Connection()
{
    // On choisit le pilote ODBC
    db = QSqlDatabase::addDatabase("QODBC");

    // On configure les paramètres de connexion
    db.setDatabaseName("source_projet2A");  // Nom de la source ODBC
    db.setUserName("nesrine");              // Nom d'utilisateur Oracle
    db.setPassword("esprit25");             // Mot de passe Oracle
}

bool Connection::createconnect()
{
    if (db.open()) {
        qDebug() << "Connexion réussie à la base Oracle via ODBC.";
        return true;
    } else {
        qDebug() << "Erreur de connexion :" << db.lastError().text();
        QMessageBox::critical(nullptr, "Erreur", "Connexion à la base échouée !");
        return false;
    }
}

void Connection::closeConnection()
{
    db.close();
    qDebug() << "Connexion fermée.";
}
