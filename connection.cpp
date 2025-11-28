#include "connection.h"

bool Connection::createConnection() {
    if (isInitialized && db.isOpen()) {
        qDebug() << "[Connection] Déjà connecté ✅";
        return true;
    }

    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection)) {
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    }

    db = QSqlDatabase::addDatabase("QODBC");

    // ✅ SERVICE NAME CORRECT : "xe" (d'après votre lsnrctl status)
    db.setDatabaseName("Driver={Oracle in XE};Dbq=localhost:1521/xe;Uid=NEXORA;Pwd=nexora124;");

    db.setUserName("NEXORA");
    db.setPassword("nexora124");

    if (!db.open()) {
        QString error = db.lastError().text();
        qCritical() << "[Connection] ❌ ÉCHEC:" << error;

        QMessageBox::critical(nullptr, "❌ Erreur Oracle XE",
                              QString("Impossible de se connecter à OracleXE.\n\n"
                                      "Erreur: %1\n\n"
                                      "VÉRIFICATIONS:\n"
                                      "1. Service OracleServiceXE DÉMARRÉ (services.msc)\n"
                                      "2. lsnrctl status → Service 'xe' READY ✓\n"
                                      "3. Testez: sqlplus NEXORA/nexora124@localhost:1521/xe\n"
                                      "4. Port 1521 LISTENING (netstat -an | find \"1521\")\n\n"
                                      "Si le test 3 fonctionne, le code fonctionne."
                                      ).arg(error));
        return false;
    }

    isInitialized = true;
    qDebug() << "✅ CONNEXION ORACLE XE RÉUSSIE !";
    qDebug() << "[Connection] Service:" << db.databaseName();
    qDebug() << "[Connection] Utilisateur:" << db.userName();
    return true;
}

void Connection::closeConnection() {
    if (db.isOpen()) {
        db.close();
        QSqlDatabase::removeDatabase(db.connectionName());
        isInitialized = false;
        qDebug() << "[Connection] Déconnexion réussie";
    }
}

QSqlDatabase Connection::getDatabase() const {
    return db;
}
