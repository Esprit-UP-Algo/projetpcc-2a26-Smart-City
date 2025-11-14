#include "connection.h"

// Crée et ouvre la connexion
bool Connection::createConnection() {
    // Utilisation de l'instance singleton
    db = QSqlDatabase::addDatabase("QODBC");

    // 🔹 IMPORTANT : le nom entre parenthèses DOIT être exactement celui défini dans ton DSN (ODBC Data Source)
    db.setDatabaseName("Driver={Oracle in instantclient_23_8};Dbq=localhost:1521/XEPDB1;Uid=NEXORA;Pwd=nexora124;");
    db.setUserName("NEXORA");
    db.setPassword("nexora124");

    if (!db.open()) {
        qDebug() << "[Connection] Échec de connexion";
        qDebug() << "[Connection] Erreur:" << db.lastError().text();
        QMessageBox::critical(nullptr, "Erreur de Connexion",
                              "Impossible de se connecter à la base de données.\n\nDétails: " + db.lastError().text());
        return false;
    }

    qDebug() << "[Connection] Connexion réussie ✅";
    return true;
}

// Ferme la connexion
void Connection::closeConnection() {
    if (db.isOpen()) {
        db.close();
        qDebug() << "[Connection] Connexion fermée";
    }
}

// Retourne l'objet QSqlDatabase
QSqlDatabase Connection::getDatabase() const {
    return db;
}
