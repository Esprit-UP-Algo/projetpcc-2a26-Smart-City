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
    explicit DatabaseManager(QObject *parent = nullptr);
    static DatabaseManager& instance(); // singleton

    QSqlDatabase database() const;

    // -------------------- Residents --------------------
    bool addResident(const QVariantMap &data);
    bool updateResident(const QString &cin, const QVariantMap &data);
    bool deleteResident(const QString &cin);
    QList<QVariantMap> getAllResidents();
    QVariantMap getResident(const QString &cin);
    QList<QVariantMap> searchResidents(const QString &term);
    QVariantMap getStatistics();

    // -------------------- Transactions --------------------
    bool addTransaction(const QVariantMap &data);
    bool updateTransaction(const QString &code, const QVariantMap &data);
    bool deleteTransaction(const QString &code);
    QList<QVariantMap> getAllTransactions();
    QVariantMap getTransaction(const QString &code);
    QList<QVariantMap> searchTransactions(const QString &term);
    QVariantMap getFinancialStatistics(int month, int year);
    QVariantMap getMonthlyEvolution(int month);
    double getTotalRevenue(int month, int year);
    double getTotalExpenses(int month, int year);

    // -------------------- Incidents --------------------
    bool addIncident(const QVariantMap &data);
    bool updateIncident(int id, const QVariantMap &data);
    bool deleteIncident(int id);
    QList<QVariantMap> getAllIncidents();
    QVariantMap getIncident(int id);

    // -------------------- Vehicles --------------------
    bool addVehicule(const QVariantMap &data);

private:
    void ensureSchema();
    QSqlDatabase m_db;
};

#endif // DATABASEMANAGER_H
