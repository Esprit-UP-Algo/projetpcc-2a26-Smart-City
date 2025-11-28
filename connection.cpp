#include "connection.h"

bool Connection::createConnection()
{
    qDebug() << "🔍 Drivers disponibles:" << QSqlDatabase::drivers();

    QStringList drivers = QSqlDatabase::drivers();
    QString connectionName = "oracle_nexora_connection";

    // Priorité au driver Oracle natif QOCI
    if (drivers.contains("QOCI")) {
        qDebug() << "🚀 Utilisation du driver QOCI (Oracle natif)";
        db = QSqlDatabase::addDatabase("QOCI", connectionName);
        db.setHostName("localhost");
        db.setPort(1521);
        db.setDatabaseName("XE");
        db.setUserName("NEXORA");
        db.setPassword("nexora124");
    }
    else if (drivers.contains("QODBC")) {
        qDebug() << "🔧 Tentative avec driver QODBC";

        db = QSqlDatabase::addDatabase("QODBC", connectionName);

        // Essayer différentes configurations
        QStringList odbcConfigs = {
            "Driver={Oracle dans OraDB21Home1};DBQ=localhost:1521/XE;",
            "Driver={Oracle in XE};DBQ=localhost:1521/XE;",
            "Driver={Oracle ODBC Driver};DBQ=localhost:1521/XE;",
            "DSN=XE;"
        };

        for (const QString &config : odbcConfigs) {
            db.setDatabaseName(config);
            db.setUserName("NEXORA");
            db.setPassword("nexora124");

            if (db.open()) {
                qDebug() << "✅ Connexion ODBC réussie avec:" << config;
                return true;
            }
        }

        // Si aucune configuration ODBC ne fonctionne
        QString error = db.lastError().text();
        qDebug() << "❌ Toutes les tentatives ODBC ont échoué:" << error;
        QMessageBox::critical(nullptr, "Erreur ODBC",
                              "Échec de toutes les configurations ODBC.\n"
                              "Détails: " + error);
        return false;
    }
    else {
        qDebug() << "❌ Aucun driver Oracle trouvé!";
        QMessageBox::critical(nullptr, "Erreur Driver",
                              "Aucun driver Oracle (QOCI) disponible.\n\n"
                              "Installez Oracle Instant Client pour avoir le driver QOCI.\n"
                              "Drivers disponibles: " + drivers.join(", "));
        return false;
    }

    // Tentative de connexion
    if (!db.open()) {
        QString error = db.lastError().text();
        qDebug() << "❌ Échec de connexion:" << error;

        QString errorMessage = "Impossible de se connecter à Oracle XE.\n\n"
                               "Détails techniques:\n" + error +
                               "\n\nSolutions possibles:\n"
                               "1. Vérifiez que Oracle XE est démarré\n"
                               "2. Testez dans SQL Developer: NEXORA/nexora124@localhost:1521/XE\n"
                               "3. Vérifiez le mot de passe";

        QMessageBox::critical(nullptr, "Erreur de Connexion", errorMessage);
        return false;
    }

    qDebug() << "✅ Connexion Oracle réussie!";

    // Test de la connexion
    QSqlQuery testQuery(db);
    if (testQuery.exec("SELECT name FROM v$database")) {
        if (testQuery.next()) {
            qDebug() << "📊 Base de données:" << testQuery.value(0).toString();
        }
    }

    // Test de la table locaux
    if (testQuery.exec("SELECT COUNT(*) FROM locaux")) {
        if (testQuery.next()) {
            qDebug() << "🏢 Nombre de locaux:" << testQuery.value(0).toInt();
        }
    } else {
        qDebug() << "⚠️ Table locaux:" << testQuery.lastError().text();
    }

    return true;
}

void Connection::closeConnection()
{
    if (db.isOpen()) {
        QString connectionName = db.connectionName();
        db.close();
        QSqlDatabase::removeDatabase(connectionName);
        qDebug() << "🔴 Connexion fermée";
    }
}

// SUPPRIMER ces lignes - les fonctions sont maintenant inline dans le .h
// QSqlDatabase Connection::getDatabase() const
// {
//     return db;
// }
