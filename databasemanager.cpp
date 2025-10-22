#include "databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(":memory:");
    if (!m_db.open()) {
        qWarning() << "[DB] Failed to open in-memory DB:" << m_db.lastError().text();
        return;
    }
    qInfo() << "[DB] In-memory database opened successfully.";
    ensureSchema();
}

DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager inst;
    return inst;
}

QSqlDatabase DatabaseManager::database() const { return m_db; }

// -------------------- Residents --------------------
bool DatabaseManager::addResident(const QVariantMap &) { return true; }
bool DatabaseManager::updateResident(const QString &, const QVariantMap &) { return true; }
bool DatabaseManager::deleteResident(const QString &) { return true; }
QList<QVariantMap> DatabaseManager::getAllResidents() { return {}; }
QVariantMap DatabaseManager::getResident(const QString &) { return {}; }
QList<QVariantMap> DatabaseManager::searchResidents(const QString &) { return {}; }
QVariantMap DatabaseManager::getStatistics() { return {{"total",0}}; }

// -------------------- Transactions --------------------
bool DatabaseManager::addTransaction(const QVariantMap &) { return true; }
bool DatabaseManager::updateTransaction(const QString &, const QVariantMap &) { return true; }
bool DatabaseManager::deleteTransaction(const QString &) { return true; }
QList<QVariantMap> DatabaseManager::getAllTransactions() { return {}; }
QVariantMap DatabaseManager::getTransaction(const QString &) { return {}; }
QList<QVariantMap> DatabaseManager::searchTransactions(const QString &) { return {}; }
QVariantMap DatabaseManager::getFinancialStatistics(int,int) { return {{"revenu",0},{"depense",0}}; }
QVariantMap DatabaseManager::getMonthlyEvolution(int) { return {{"mois","Janvier"},{"revenu",0},{"depense",0}}; }
double DatabaseManager::getTotalRevenue(int,int) { return 0; }
double DatabaseManager::getTotalExpenses(int,int) { return 0; }

// -------------------- Incidents --------------------
bool DatabaseManager::addIncident(const QVariantMap &data)
{
    QSqlQuery q(m_db);
    q.prepare("INSERT INTO incidents (categorie, description, etat, date) VALUES (?, ?, ?, ?)");
    q.addBindValue(data.value("categorie"));
    q.addBindValue(data.value("description"));
    q.addBindValue(data.value("etat"));
    q.addBindValue(data.value("date"));
    return q.exec();
}
bool DatabaseManager::updateIncident(int id, const QVariantMap &data)
{
    QSqlQuery q(m_db);
    q.prepare("UPDATE incidents SET categorie=?, description=?, etat=?, date=? WHERE id=?");
    q.addBindValue(data.value("categorie"));
    q.addBindValue(data.value("description"));
    q.addBindValue(data.value("etat"));
    q.addBindValue(data.value("date"));
    q.addBindValue(id);
    return q.exec();
}

bool DatabaseManager::deleteIncident(int id)
{
    QSqlQuery q(m_db);
    q.prepare("DELETE FROM incidents WHERE id=?");
    q.addBindValue(id);
    return q.exec();
}
QList<QVariantMap> DatabaseManager::getAllIncidents()
{
    QList<QVariantMap> list;
    QSqlQuery q("SELECT * FROM incidents ORDER BY id DESC", m_db);
    while(q.next()) {
        QVariantMap m;
        m["id"] = q.value("id").toInt();
        m["categorie"] = q.value("categorie").toString();
        m["description"] = q.value("description").toString();
        m["etat"] = q.value("etat").toString();
        m["date"] = q.value("date").toString();
        list << m;
    }
    return list;
}

QVariantMap DatabaseManager::getIncident(int id)
{
    QVariantMap incident;
    QSqlQuery q(m_db);
    q.prepare("SELECT * FROM incidents WHERE id=?");
    q.addBindValue(id);
    if (q.exec() && q.next()) {
        incident["id"] = q.value("id").toInt();
        incident["categorie"] = q.value("categorie").toString();
        incident["description"] = q.value("description").toString();
        incident["etat"] = q.value("etat").toString();
        incident["date"] = q.value("date").toString();
    }
    return incident;
}

// -------------------- Vehicles --------------------
bool DatabaseManager::addVehicule(const QVariantMap &) { return true; }

// -------------------- Schema --------------------
void DatabaseManager::ensureSchema()
{
    QSqlQuery q(m_db);
    q.exec("CREATE TABLE residents (cin TEXT, nom TEXT, prenom TEXT, appartement TEXT, statut TEXT)");
    q.exec("CREATE TABLE transactions (code TEXT, date TEXT, type TEXT, montant REAL, description TEXT)");
    q.exec("CREATE TABLE incidents (id INTEGER PRIMARY KEY AUTOINCREMENT, categorie TEXT, description TEXT, etat TEXT, date TEXT)");
    q.exec("CREATE TABLE vehicules (id INTEGER PRIMARY KEY AUTOINCREMENT, matricule TEXT, type TEXT, proprietaire TEXT, capacite INTEGER, zone TEXT, horaire TEXT, statut TEXT, date TEXT, temps_utilise REAL)");
    if (q.lastError().isValid())
        qWarning() << "[DB] Schema warning:" << q.lastError().text();
}
