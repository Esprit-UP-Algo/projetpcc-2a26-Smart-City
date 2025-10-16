#include "incidentspage.h"
#include <QApplication>
#include <QDebug>
#include "connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 1️⃣ Connexion à Oracle
    if (!Connection::connecterOracle()) {
        qDebug() << "Impossible de se connecter à Oracle";
        return -1; // quitter l'application si la connexion échoue
    }

    // 2️⃣ Créer la table Incident si elle n'existe pas
    Connection::creerTableIncident();

    // 3️⃣ Lancer la fenêtre principale
    IncidentsPage w;
    w.setWindowTitle("Module Finances - NEXORA");
    w.setMinimumSize(1200, 700);
    w.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    w.show();

    return a.exec();
}
