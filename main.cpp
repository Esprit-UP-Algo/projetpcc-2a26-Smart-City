#include "mainwindow.h"
#include "connection.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Créer une instance de la classe Connection
    Connection c;

    // Tenter d'établir la connexion
    bool test = c.createconnect();

    MainWindow w;

    if (test) {
        // Si la connexion réussit, afficher la fenêtre principale
        w.show();
        QMessageBox::information(nullptr, QObject::tr("Connexion"),
                                 QObject::tr("Connexion établie avec succès.\n"
                                             "Cliquez sur Cancel pour quitter."),
                                 QMessageBox::Cancel);
    } else {
        // Si la connexion échoue, afficher un message d'erreur
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                              QObject::tr("Impossible de se connecter à la base de données.\n"
                                          "L'application va se fermer."),
                              QMessageBox::Cancel);
        return 1; // Quitter l'application avec code d'erreur
    }

    int result = a.exec();

    // Fermer la connexion proprement avant de quitter
    c.closeConnection();

    return result;
}
