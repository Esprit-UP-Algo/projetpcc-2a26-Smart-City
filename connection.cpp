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
    db.setUserName("wajd");          // Ton utilisateur Oracle
    db.setPassword("wajd123");       // Mot de passe

    if(!db.open()) {
        qDebug() << "Erreur de connexion:" << db.lastError().text();
        return false;
    }
    qDebug() << "Connexion réussie !";
    return true;
}

// Création de la table Incident si elle n'existe pas
void Connection::creerTableIncident()
{
    QSqlQuery query;
    if(!query.exec("CREATE TABLE Incident ("
                    "ID_Incident NUMBER PRIMARY KEY, "
                    "Type VARCHAR2(50) NOT NULL, "
                    "Localisation VARCHAR2(100) NOT NULL, "
                    "Date_Incident DATE NOT NULL, "
                    "Description VARCHAR2(255), "
                    "Statut VARCHAR2(20))")) {
        qDebug() << "Erreur création table:" << query.lastError().text();
    } else {
        qDebug() << "Table Incident prête !";
    }
}

// Insertion d'un incident
void Connection::insererIncident(int id, QString type, QString loc, QDate date, QString desc, QString statut)
{
    QSqlQuery query;
    query.prepare("INSERT INTO Incident (ID_Incident, Type, Localisation, Date_Incident, Description, Statut) "
                  "VALUES (:id, :type, :loc, :date, :desc, :statut)");
    query.bindValue(":id", id);
    query.bindValue(":type", type);
    query.bindValue(":loc", loc);
    query.bindValue(":date", date);
    query.bindValue(":desc", desc);
    query.bindValue(":statut", statut);

    if(!query.exec()) {
        qDebug() << "Erreur insertion:" << query.lastError().text();
    } else {
        qDebug() << "Incident ajouté avec succès !";
    }
}

// Affichage des incidents dans la console
void Connection::afficherIncidents()
{
    QSqlQuery query("SELECT * FROM Incident");
    if(!query.exec()) {
        qDebug() << "Erreur lecture table:" << query.lastError().text();
        return;
    }

    while(query.next()) {
        qDebug() << "ID:" << query.value("ID_Incident").toInt()
        << "Type:" << query.value("Type").toString()
        << "Localisation:" << query.value("Localisation").toString()
        << "Date:" << query.value("Date_Incident").toDate()
        << "Description:" << query.value("Description").toString()
        << "Statut:" << query.value("Statut").toString();
    }
}
