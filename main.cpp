#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include "connection.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Configuration de l'application
    a.setApplicationName("Nexora Smart City");
    a.setApplicationVersion("1.0");
    a.setOrganizationName("Nexora");

    qDebug() << "🚀 Démarrage de l'application Nexora Smart City...";

    // Initialisation de la connexion base de données
    Connection& connection = Connection::getInstance();

    if (!connection.createConnection()) {
        qDebug() << "❌ Arrêt de l'application: connexion base de données échouée";
        QMessageBox::critical(nullptr,
                              "Erreur Critique",
                              "Impossible de se connecter à la base de données.\n"
                              "L'application va se fermer.");
        return -1;
    }

    qDebug() << "✅ Connexion Oracle établie avec succès";

    // Création et affichage de la fenêtre principale
    MainWindow mainWindow;
    mainWindow.show();

    qDebug() << "🏠 Fenêtre principale affichée";

    return a.exec();
}
