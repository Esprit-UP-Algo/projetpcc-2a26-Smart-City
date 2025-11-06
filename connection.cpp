#include "connection.h"

Connection::Connection()
{
    db = QSqlDatabase::addDatabase("QODBC");
    qDebug() << "[Connection] Constructeur appelé";
}

Connection::~Connection()
{
    closeConnection();
    qDebug() << "[Connection] Destructeur appelé";
}

bool Connection::createConnection()
{
    if (db.isOpen()) {
        qDebug() << "[Connection] Connexion déjà ouverte";
        return true;
    }

    QStringList driverNames;
    driverNames << "Oracle in OracleXE"
                << "Oracle in XE"
                << "Oracle in instantclient_21_3"
                << "Oracle in instantclient_19_3"
                << "Oracle in instantclient_12_2"
                << "Microsoft ODBC for Oracle"
                << "OracleODBC-19c";

    QString workingConfig;
    for (const QString &driver : driverNames) {
        QString connectionString = QString(
                                       "DRIVER={%1};"
                                       "SERVER=localhost;"
                                       "PORT=1521;"
                                       "SERVICE_NAME=XE;"
                                       "UID=nexora;"
                                       "PWD=nexora124;"
                                       ).arg(driver);

        qDebug() << "[Connection] Essai avec le pilote:" << driver;
        db.setDatabaseName(connectionString);

        if (db.open()) {
            workingConfig = driver;
            qDebug() << "[Connection] CONNEXION RÉUSSIE avec:" << driver;
            break;
        } else {
            qDebug() << "[Connection] Échec avec" << driver;
            qDebug() << "[Connection] Erreur:" << db.lastError().databaseText();
        }
    }

    if (db.isOpen()) {
        qDebug() << "[Connection] Connexion réussie à Oracle via ODBC.";
        QMessageBox::information(nullptr, "Succès",
                                 QString("Connexion établie avec succès !\n\nPilote utilisé : %1").arg(workingConfig));
        return true;
    } else {
        qDebug() << "[Connection] ÉCHEC : Aucun pilote trouvé";
        QMessageBox::critical(nullptr, "Erreur",
                              "Connexion échouée !\n\n"
                              "1. Oracle XE doit être démarré\n"
                              "2. Pilote ODBC Oracle installé\n"
                              "3. Identifiants corrects\n\n"
                              "Voir console pour détails.");
        return false;
    }
}

void Connection::closeConnection()
{
    if (db.isOpen()) {
        db.close();
        qDebug() << "[Connection] Connexion fermée proprement.";
    }
}

QSqlDatabase Connection::getDatabase() const
{
    return db;
}
