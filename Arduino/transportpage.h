#ifndef TRANSPORTPAGE_H
#define TRANSPORTPAGE_H

#include <QWidget>
#include <QVariant>
#include <QVariantMap>
#include <QList>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSystemTrayIcon>
#include <QRandomGenerator>
#include <QInputDialog>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui {
class TransportPage;
}
QT_END_NAMESPACE

class TransportPage : public QWidget
{
    Q_OBJECT

public:
    explicit TransportPage(QWidget *parent = nullptr);
    ~TransportPage();

    void setVehicules(const QList<QVariantMap> &rows);
    void showEmptyTable();
    int sortColumnForIndex(int index) const;

signals:
    void refreshRequested();
    void arduinoResponse(const QString &message); // Signal pour Arduino
    void transactionCreated(); // Signal pour notifier financespage

public slots:
    void onVehiclePaymentRequested(); // Slot pour Arduino
    void onVehiclePaymentRequestedWithCode(const QString &codeVehicule); // Slot avec code spécifique

private slots:
    void onTabAdd();
    void onTabList();
    void onTabStats();
    void onSaveForm();
    void onCancelForm();
    void onFilterTextChanged(const QString &text);
    void onSortIndexChanged(int index);
    void onExportPdf();
    void onEditClicked();
    void onDeleteClicked();
    void onIdTextChanged();
    void onCapaciteTextChanged();
    void onDataChanged();
    void onAfficherStatistiques();
    void onHoraireTextChanged();
    void onHistoriqueClicked();
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void checkMaintenanceVehicles();
    void showMaintenanceNotification(const QString &vehicleId);

private:
    void setupTableHeaders();
    void setupUiBehavior();
    void setupValidation();
    void clearForm();
    void loadAllVehicules();
    bool validateForm();
    bool validateId(const QString &id);
    bool validateCapacite(const QString &capacite);
    bool validateHoraire(const QString &horaire);
    void showError(const QString &message);
    void hideError();
    void loadVehicule(const QVariantMap &v);
    void calculateAndDisplayStatistics();
    double calculerKilometrage(const QString& type, double tempsUtilise) const;
    void calculerPourcentagesKilometrage();
    void afficherGraphiqueKilometrage();
    void afficherHistoriqueComplet();
    void creerVehiculesExemple(); // Créer des véhicules d'exemple

    // Méthodes pour l'historique
    void sauvegarderHistorique(const QVariantMap &vehicule, const QString &action);
    void sauvegarderHistoriqueModification(const QVariantMap &ancienVehicule, const QVariantMap &nouveauVehicule);
    void initialiserFichierHistorique();
    QList<QVariantMap> getHistoriqueComplet();
    QString getFichierHistorique() const;
    void verifierContenuHistorique(); // AJOUTEZ CETTE LIGNE

    // Méthodes pour les notifications avancées
    void setupNotificationSystem();
    void setupTrayIcon();
    void playNotificationSound();
    void showPopupNotification(const QString &title, const QString &message);
    void logMaintenanceEvent(const QString &vehicleId);
    bool isNewMaintenanceEvent(const QString &vehicleId);
    void chargerCodesUniquesFinance();
    void mettreAJourHistoriqueHTML();
    void genererHistoriqueHTML(const QList<QVariantMap> &historique);
    
    // Méthodes pour paiement Arduino
    double calculerMontantPaiement(const QString &typeVehicule) const;
    bool creerTransactionPaiementVehicule(const QString &codeVehicule, double montant);
    QString genererCodeTransaction() const;
    void logPaiementVehicule(const QString &codeVehicule, double montant, const QString &codeTransaction);
    void testerPaiementArduino(); // Test manuel
    void simulerPaiementArduino(); // Simulation automatique
    void traiterCodeVehiculeArduino(const QString &codeVehicule); // Traitement du code Arduino
    void afficherMessagePaiement(const QString &titre, const QString &message, bool succes); // Affichage des messages
    void creerChampSaisieArduino(); // Créer le champ de saisie Arduino
    void onArduinoInputChanged(const QString &text); // Gestion de la saisie Arduino
    void simulerLectureArduino(); // Simulation lecture Arduino
    void forceUpdateTransactionsList(); // Force mise à jour transactions
    
    // 🔥 NOUVELLES MÉTHODES POUR VALIDATION STRICTE
    bool validerFormatCode(const QString &code) const; // Validation format A123
    QString determinerTypeVehicule(const QString &code) const; // A=Bus, B=Metro, etc.
    QString determinerZoneVehicule(const QString &code) const; // Zone selon code
    void envoyerErreurArduino(const QString &erreur); // Envoyer ERROR: à Arduino
    void envoyerSuccesArduino(const QString &message); // Envoyer SUCCESS: à Arduino
    bool creerTransactionTransport(const QString &codeVehicule, double montant); // Transaction avec code TRP-
    bool mettreAJourVehiculeApresRecharge(const QString &codeVehicule); // UPDATE vehicule statut=actif
    
    // Communication série Arduino physique
    void setupArduinoSerialCommunication(); // Configuration port série
    void lireMessagesArduino(); // Lecture des messages du clavier matriciel
    void simulerArduinoPhysique(); // Simulation pour tests

private:
    Ui::TransportPage *ui;
    QSystemTrayIcon *m_trayIcon;
    QTimer *m_maintenanceCheckTimer;
    QTimer *m_arduinoSimulationTimer;
    QSet<QString> m_knownMaintenanceVehicles;
    
    // Widgets Arduino (désactivés)
    QWidget *m_arduinoWidget;
    QLineEdit *m_arduinoInput;
    
    // Communication série Arduino physique
    QSerialPort *m_serialPort;
};

#endif // TRANSPORTPAGE_H
