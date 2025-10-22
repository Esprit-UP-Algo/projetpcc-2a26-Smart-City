#include "connection.h"

Connection::Connection()
{
}

// Connexion Oracle XE via ODBC
bool Connection::connecterOracle()
{
    // Ajouter une connexion ODBC
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("XE_ODBC");   // Nom de ta source ODBC
    db.setUserName("SYSTEM");          // Ton utilisateur Oracle
    db.setPassword("wajd123");       // Mot de passe

    if(!db.open()) {
        qDebug() << "Erreur de connexion:" << db.lastError().text();
        return false;
    }
    qDebug() << "Connexion réussie !";
    return true;
}

// Création de la table Incident si elle n'existe pas
