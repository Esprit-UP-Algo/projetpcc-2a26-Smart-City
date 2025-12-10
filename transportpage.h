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

private:
    Ui::TransportPage *ui;
    QSystemTrayIcon *m_trayIcon;
    QTimer *m_maintenanceCheckTimer;
    QSet<QString> m_knownMaintenanceVehicles;
};

#endif // TRANSPORTPAGE_H
