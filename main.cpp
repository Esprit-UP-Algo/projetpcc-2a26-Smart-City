#include "mainwindow.h"
#include "connection.h"      // ➕ Ajout de l’en-tête de connexion
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // ➕ Création et test de la connexion à la base
    Connection c;
    bool test = c.createconnect();

    if (test) {
        QMessageBox::information(nullptr, "Connexion", "Connexion réussie à la base de données !");
        MainWindow w;
        w.show();
        return a.exec();
    } else {
        QMessageBox::critical(nullptr, "Erreur", "Connexion à la base de données échouée !");
        return -1; // ❌ Stoppe le programme si la connexion échoue
    }
}
