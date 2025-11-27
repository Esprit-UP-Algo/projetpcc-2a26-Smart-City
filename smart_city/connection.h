#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

class Connection {
public:
    // Singleton
    static Connection& getInstance() {
        static Connection instance;
        return instance;
    }

    // Vérifie si la connexion est ouverte
    static bool isOpen() {
        return getInstance().db.isOpen();
    }

    // Crée et ouvre la connexion
    bool createConnection();

    // Ferme la connexion
    void closeConnection();

    // Retourne l'objet QSqlDatabase
    QSqlDatabase getDatabase() const;

private:
    QSqlDatabase db;

    Connection() {}   // constructeur vide
    ~Connection() {}  // destructeur vide

    // Empêche la copie
    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;
};

#endif // CONNECTION_H
