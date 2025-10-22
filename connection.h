#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtSql>
#include <QDebug>
#include <QDate>

class Connection
{
public:
    Connection();

    // Connexion Oracle via ODBC
    static bool connecterOracle();

    // Création de la table Incident

};

#endif // CONNECTION_H
