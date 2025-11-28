#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>

class Connection
{
public:
    static Connection& getInstance() {
        static Connection instance;
        return instance;
    }

    static bool isOpen() {
        return getInstance().db.isOpen();
    }

    bool createConnection();
    void closeConnection();

    // CORRECTION : Retourner une référence au lieu d'une copie
    QSqlDatabase& getDatabase() { return db; }  // ✅ Version non-const
    const QSqlDatabase& getDatabase() const { return db; }  // ✅ Version const

private:
    QSqlDatabase db;

    // Constructeur privé pour singleton
    Connection() = default;
    ~Connection() = default;

    // Empêcher la copie
    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;
};

#endif // CONNECTION_H
