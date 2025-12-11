#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

class Connection {
public:

    // Singleton : accès global
    static Connection& getInstance() {
        static Connection instance;
        return instance;
    }

    // Vérifie si la connexion Oracle est ouverte
    static bool isOpen() {
        return getInstance().db.isOpen();
    }

    // Ouvre la connexion
    bool createConnection();

    // Ferme la connexion
    void closeConnection();

    // Donne la base active
    QSqlDatabase getDatabase() const {
        return db;
    }

private:
    QSqlDatabase db;

    // Constructeur privé (singleton)
    Connection() {}

    // Destructeur
    ~Connection() {}

    // Interdit la copie
    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;
};

#endif // CONNECTION_H
