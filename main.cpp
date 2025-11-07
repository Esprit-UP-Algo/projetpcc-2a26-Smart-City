#include "mainwindow.h"
#include "connection.h"
#include "databasemanager.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << "========================================";
    qDebug() << "=== DÉMARRAGE DE L'APPLICATION NEXORA ===";
    qDebug() << "========================================\n";

    // ===== Récupération de l'instance unique du Singleton Connection =====
    qDebug() << "[Main] Récupération de l'instance unique du Singleton Connection...";
    Connection& connectionInstance = Connection::getInstance();
    qDebug() << "[Main] Instance Connection récupérée\n";

    // ===== Tentative d'établir la connexion =====
    qDebug() << "[Main] Tentative d'établissement de la connexion à la base de données...";
    bool connectionSuccess = connectionInstance.createConnection();

    MainWindow w;

    if (connectionSuccess) {
        qDebug() << "[Main] ✓ Connexion réussie!\n";

        // ===== Initialisation du DatabaseManager (qui utilise la connexion existante) =====
        qDebug() << "[Main] Initialisation du DatabaseManager Singleton...";
        DatabaseManager::instance(); // Force la création de l'instance
        qDebug() << "[Main] DatabaseManager initialisé\n";

        // Afficher la fenêtre principale
        qDebug() << "[Main] Affichage de la fenêtre principale...";
        w.show();

        QMessageBox::information(nullptr, QObject::tr("Connexion Réussie"),
                                 QObject::tr("✓ Connexion établie avec succès à la base de données Oracle.\n\n"
                                             "L'application utilise le pattern Singleton pour garantir:\n"
                                             "• Une seule instance de Connection\n"
                                             "• Une seule instance de DatabaseManager\n"
                                             "• Une connexion unique partagée dans toute l'application\n\n"
                                             "Cliquez sur OK pour continuer."),
                                 QMessageBox::Ok);

    } else {
        qDebug() << "[Main] ✗ ÉCHEC de la connexion\n";
        // Si la connexion échoue, afficher un message d'erreur
        QMessageBox::critical(nullptr, QObject::tr("Erreur de Connexion"),
                              QObject::tr("✗ Impossible de se connecter à la base de données.\n\n"
                                          "Vérifiez que:\n"
                                          "1. Oracle XE est démarré\n"
                                          "2. Les pilotes ODBC sont installés\n"
                                          "3. Les informations de connexion sont correctes\n\n"
                                          "L'application va se fermer."),
                              QMessageBox::Ok);
        return 1; // Quitter l'application avec code d'erreur
    }

    qDebug() << "\n[Main] Entrée dans la boucle d'événements Qt...";
    int result = a.exec();

    // ===== Fermeture propre à la fin de l'application =====
    qDebug() << "\n[Main] Fermeture de l'application...";
    qDebug() << "[Main] Fermeture de la connexion...";
    connectionInstance.closeConnection();

    qDebug() << "\n========================================";
    qDebug() << "===   FIN DE L'APPLICATION NEXORA    ===";
    qDebug() << "========================================";

    return result;
}
