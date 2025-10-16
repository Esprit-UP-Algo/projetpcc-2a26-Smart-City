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
    static void creerTableIncident();

    // Gestion des incidents
    static void insererIncident(int id, QString type, QString loc, QDate date, QString desc, QString statut);
    static void afficherIncidents();
};

#endif // CONNECTION_H
