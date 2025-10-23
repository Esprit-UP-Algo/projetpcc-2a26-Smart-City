#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{
    bool test = false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("OracleXE");  // Your DSN name
    db.setUserName("NEXORA");        // Your username
    db.setPassword("nexora124");     // Your password

    if (db.open()) {
        test = true;
    }

    return test;
}
