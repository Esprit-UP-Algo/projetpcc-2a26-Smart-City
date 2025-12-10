#include "connection.h"

bool Connection::createConnection() {
    db = QSqlDatabase::addDatabase("QODBC");

    // ⭐ Utilise TON DSN existant (celui qui marche dans ton test ODBC)
    db.setDatabaseName("OracleXE");
    db.setUserName("NEXORA");
    db.setPassword("nexora124");

    if (!db.open()) {
        qDebug() << "[Connection] Échec de connexion";
        qDebug() << "[Connection] Erreur:" << db.lastError().text();
        QMessageBox::critical(nullptr, "Erreur de Connexion",
                              "Impossible de se connecter à la base de données.\n\nDétails: "
                                  + db.lastError().text());
        return false;
    }

    qDebug() << "[Connection] Connexion réussie ✅";
    return true;
}

void Connection::closeConnection() {
    if (db.isOpen()) {
        db.close();
        qDebug() << "[Connection] Connexion fermée";
    }
}

QSqlDatabase Connection::getDatabase() const {
    return db;
}
