#include "connection.h"

bool Connection::createConnection() {

    db = QSqlDatabase::addDatabase("QODBC");

    // 🟢 IMPORTANT : uniquement le nom du DSN !
    db.setDatabaseName("OracleXE");

    db.setUserName("NEXORA");
    db.setPassword("nexora124");

    if (!db.open()) {
        qDebug() << "[Connection] Échec :" << db.lastError().text();
        QMessageBox::critical(nullptr, "Erreur de Connexion",
                              "Impossible de se connecter à la base de données.\n\n" + db.lastError().text());
        return false;
    }

    qDebug() << "Connexion Oracle réussie ✔";
    return true;
}

void Connection::closeConnection() {
    if (db.isOpen()) db.close();
}
