#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QVariantMap>
#include <QList>

class DatabaseManager : public QObject
{
    Q_OBJECT

public:
    static DatabaseManager& instance() {
        static DatabaseManager inst;
        return inst;
    }

    QSqlDatabase database() const;

    // ====== Résidents ======
    bool addResident(const QVariantMap &data);
    bool updateResident(const QString &cin, const QVariantMap &data);
    bool deleteResident(const QString &cin);
    QList<QVariantMap> getAllResidents();
    QVariantMap getResident(const QString &cin);
    QList<QVariantMap> searchResidents(const QString &term);
    QVariantMap getStatistics();

    // ====== Transactions ======
    bool addTransaction(const QVariantMap &data);
    bool updateTransaction(const QString &code, const QVariantMap &data);
    bool deleteTransaction(const QString &code);
    QList<QVariantMap> getAllTransactions();
    QVariantMap getTransaction(const QString &code);
    QList<QVariantMap> searchTransactions(const QString &term);
    QVariantMap getFinancialStatistics(int year, int month);
    QVariantMap getMonthlyEvolution(int year);
    double getTotalRevenue(int year, int month);
    double getTotalExpenses(int year, int month);
    QVariantMap getResidentByCIN(const QString &cin);
    QList<QVariantMap> getTransactionsForResidentMonth(const QString &cin, int year, int month);

    // ====== Incidents ======
    bool addIncident(const QVariantMap &data);
    bool updateIncident(int id, const QVariantMap &data);
    bool deleteIncident(int id);
    QList<QVariantMap> getAllIncidents();
    QVariantMap getIncident(int id);
    QList<QVariantMap> searchIncidents(const QString &term);
    QVariantMap getIncidentsStatistics();

    // ====== Véhicules (Parking) ======
    bool addVehicule(const QVariantMap &data);

    // ====== Locaux ======
    bool addLocal(const QVariantMap &data);
    bool updateLocal(const QString &id, const QVariantMap &data);
    bool deleteLocal(const QString &id);
    QList<QVariantMap> getAllLocaux();
    QVariantMap getLocal(const QString &id);
    QList<QVariantMap> searchLocaux(const QString &term);

    // ====== Transport ======
    bool addTransportVehicule(const QVariantMap &data);
    bool updateTransportVehicule(const QString &id, const QVariantMap &data);
    bool deleteTransportVehicule(const QString &id);
    QList<QVariantMap> getAllTransportVehicules();
    QVariantMap getTransportVehicule(const QString &id);
    QList<QVariantMap> searchTransportVehicules(const QString &term);
    QVariantMap getTransportStatistics();
    // Alias pour compatibilité avec TransportPage
    inline QList<QVariantMap> getAllVehicules() { return getAllTransportVehicules(); }
    inline QVariantMap getVehicule(const QString &id) { return getTransportVehicule(id); }
    inline bool updateVehicule(const QString &id, const QVariantMap &data) { return updateTransportVehicule(id, data); }
    inline bool deleteVehicule(const QString &id) { return deleteTransportVehicule(id); }
    inline bool ajouterVehiculeTransport(const QString &id, const QString &type, const QString &zone, const QString &statut) {
        QVariantMap data;
        data["id_vehicule"] = id;
        data["type"] = type;
        data["zone"] = zone;
        data["statut"] = statut;
        return addTransportVehicule(data);
    }
    
    // Méthodes additionnelles pour compatibilité
    bool vehiculeExists(const QString &id);
    QVariantMap getVehiculeByID(const QString &id);

signals:
    void vehiculeAdded();
    void vehiculeUpdated();
    void vehiculeDeleted();
    void dataChanged();

private:
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager() = default;
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    QSqlDatabase db;
};

#endif // DATABASEMANAGER_H
