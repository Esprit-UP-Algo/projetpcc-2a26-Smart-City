#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>

class Connection
{
private:
    QSqlDatabase db;  // Objet de type QSqlDatabase (membre de la classe)

public:
    Connection();
    bool createconnect();
    void closeConnection();  // Méthode pour fermer la connexion
};

#endif // CONNECTION_H
