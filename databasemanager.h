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
    static DatabaseManager& instance() {
        static DatabaseManager instance;
        return instance;
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

    // ====== Incidents ======
    bool addIncident(const QVariantMap &data);

    // ====== Véhicules ======
    bool addVehicule(const QVariantMap &data);

private:
    QSqlDatabase db;
};

#endif // DATABASEMANAGER_H
