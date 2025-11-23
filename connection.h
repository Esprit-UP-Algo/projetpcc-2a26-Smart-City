#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

class Connection {
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
    QSqlDatabase getDatabase() const;

private:
    QSqlDatabase db;
    bool isInitialized = false;
    Connection() {}
    ~Connection() { closeConnection(); }
    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;
};

#endif
