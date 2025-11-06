#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QList>
#include <QVariantMap>

class DatabaseManager : public QObject
{
    Q_OBJECT

public:
    static DatabaseManager& instance()
    {
        static DatabaseManager instance;
        return instance;
    }

    QSqlDatabase database() const;

    // ----------- Transactions ----------
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

    // ----------- Residents ----------
    bool addResident(const QVariantMap &data);
    bool updateResident(const QString &cin, const QVariantMap &data);
    bool deleteResident(const QString &cin);
    QList<QVariantMap> getAllResidents();
    QVariantMap getResident(const QString &cin);
    QList<QVariantMap> searchResidents(const QString &term);
    QVariantMap getStatistics();

    // ----------- Incidents ----------
    bool addIncident(const QVariantMap &data);
    bool updateIncident(int id, const QVariantMap &data);
    bool deleteIncident(int id);
    QList<QVariantMap> getAllIncidents();
    QVariantMap getIncident(int id);
    QList<QVariantMap> searchIncidents(const QString &term);
    QVariantMap getIncidentStatistics();

    // ----------- Vehicles ----------
    bool addVehicule(const QVariantMap &data);
    bool updateVehicule(const QString &id, const QVariantMap &data);
    bool deleteVehicule(const QString &id);
    QList<QVariantMap> getAllVehicules();
    QVariantMap getVehicule(const QString &id);
    QList<QVariantMap> searchVehicules(const QString &term);

private:
    explicit DatabaseManager(QObject *parent = nullptr);
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;
    void ensureSchema();
};

#endif // DATABASEMANAGER_H
