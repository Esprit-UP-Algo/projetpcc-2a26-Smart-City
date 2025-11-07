#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class Connection {
public:
    // Retourne l'instance unique (Meyers singleton)
    static Connection& createInstance();

    // Configure et ouvre la connexion (renomme pour correspondre aux slides)
    bool createConnection();

private:
    QSqlDatabase db; // attribut persistant (instance unique)

    Connection();
    ~Connection();

    // Empêcher la copie et l'affectation
    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;
};

#endif // CONNECTION_H
