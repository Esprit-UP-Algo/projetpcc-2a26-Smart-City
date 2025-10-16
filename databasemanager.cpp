#include "databasemanager.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent)
{
    // In-memory DB (temporary only)
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");

    if (!db.open()) {
        qWarning() << "[DB] Failed to open in-memory DB:" << db.lastError().text();
        return;
    }

    qInfo() << "[DB] In-memory database opened successfully.";
    ensureSchema();
}

void DatabaseManager::ensureSchema()
{
    QSqlQuery q;

    q.exec("CREATE TABLE residents (cin TEXT, nom TEXT, prenom TEXT, appartement TEXT, statut TEXT)");
    q.exec("CREATE TABLE transactions (code TEXT, date TEXT, type TEXT, montant REAL, description TEXT)");
    q.exec("CREATE TABLE incidents (id INTEGER PRIMARY KEY AUTOINCREMENT, categorie TEXT, description TEXT, etat TEXT, date TEXT)");
    q.exec("CREATE TABLE vehicules (id INTEGER PRIMARY KEY AUTOINCREMENT, matricule TEXT, type TEXT, proprietaire TEXT, capacite INTEGER, zone TEXT, horaire TEXT, statut TEXT, date TEXT, temps_utilise REAL)");

    if (q.lastError().isValid())
        qWarning() << "[DB] Schema warning:" << q.lastError().text();
}

QSqlDatabase DatabaseManager::database() const
{
    return QSqlDatabase::database();
}

// -------------------- Residents --------------------
bool DatabaseManager::addResident(const QVariantMap &data)
{
    QSqlQuery q;
    q.prepare("INSERT INTO residents (cin, nom, prenom, appartement, statut) VALUES (?, ?, ?, ?, ?)");
    q.addBindValue(data.value("cin"));
    q.addBindValue(data.value("nom"));
    q.addBindValue(data.value("prenom"));
    q.addBindValue(data.value("appartement"));
    q.addBindValue(data.value("statut"));
    return q.exec();
}

bool DatabaseManager::updateResident(const QString &cin, const QVariantMap &data)
{
    QSqlQuery q;
    q.prepare("UPDATE residents SET nom=?, prenom=?, appartement=?, statut=? WHERE cin=?");
    q.addBindValue(data.value("nom"));
    q.addBindValue(data.value("prenom"));
    q.addBindValue(data.value("appartement"));
    q.addBindValue(data.value("statut"));
    q.addBindValue(cin);
    return q.exec();
}

bool DatabaseManager::deleteResident(const QString &cin)
{
    QSqlQuery q;
    q.prepare("DELETE FROM residents WHERE cin=?");
    q.addBindValue(cin);
    return q.exec();
}

QList<QVariantMap> DatabaseManager::getAllResidents()
{
    QList<QVariantMap> list;
    QSqlQuery q("SELECT * FROM residents");
    while (q.next()) {
        QVariantMap m;
        m["cin"] = q.value("cin");
        m["nom"] = q.value("nom");
        m["prenom"] = q.value("prenom");
        m["appartement"] = q.value("appartement");
        m["statut"] = q.value("statut");
        list << m;
    }
    return list;
}

QVariantMap DatabaseManager::getResident(const QString &cin)
{
    QVariantMap m;
    QSqlQuery q;
    q.prepare("SELECT * FROM residents WHERE cin=?");
    q.addBindValue(cin);
    if (q.exec() && q.next()) {
        m["cin"] = q.value("cin");
        m["nom"] = q.value("nom");
        m["prenom"] = q.value("prenom");
        m["appartement"] = q.value("appartement");
        m["statut"] = q.value("statut");
    }
    return m;
}

QList<QVariantMap> DatabaseManager::searchResidents(const QString &term)
{
    QList<QVariantMap> list;
    QSqlQuery q;
    q.prepare("SELECT * FROM residents WHERE nom LIKE ? OR prenom LIKE ?");
    q.addBindValue("%" + term + "%");
    q.addBindValue("%" + term + "%");
    if (q.exec()) {
        while (q.next()) {
            QVariantMap m;
            m["cin"] = q.value("cin");
            m["nom"] = q.value("nom");
            m["prenom"] = q.value("prenom");
            m["appartement"] = q.value("appartement");
            m["statut"] = q.value("statut");
            list << m;
        }
    }
    return list;
}

QVariantMap DatabaseManager::getStatistics()
{
    QVariantMap stats;
    QSqlQuery q("SELECT COUNT(*) FROM residents");
    if (q.next()) stats["total"] = q.value(0).toInt();
    return stats;
}

// -------------------- Transactions --------------------
bool DatabaseManager::addTransaction(const QVariantMap &data)
{
    QSqlQuery q;
    q.prepare("INSERT INTO transactions (code, date, type, montant, description) VALUES (?, ?, ?, ?, ?)");
    q.addBindValue(data.value("code"));
    q.addBindValue(data.value("date"));
    q.addBindValue(data.value("type"));
    q.addBindValue(data.value("montant"));
    q.addBindValue(data.value("description"));
    return q.exec();
}

bool DatabaseManager::updateTransaction(const QString &code, const QVariantMap &data)
{
    QSqlQuery q;
    q.prepare("UPDATE transactions SET date=?, type=?, montant=?, description=? WHERE code=?");
    q.addBindValue(data.value("date"));
    q.addBindValue(data.value("type"));
    q.addBindValue(data.value("montant"));
    q.addBindValue(data.value("description"));
    q.addBindValue(code);
    return q.exec();
}

bool DatabaseManager::deleteTransaction(const QString &code)
{
    QSqlQuery q;
    q.prepare("DELETE FROM transactions WHERE code=?");
    q.addBindValue(code);
    return q.exec();
}

QList<QVariantMap> DatabaseManager::getAllTransactions()
{
    QList<QVariantMap> list;
    QSqlQuery q("SELECT * FROM transactions");
    while (q.next()) {
        QVariantMap m;
        m["code"] = q.value("code");
        m["date"] = q.value("date");
        m["type"] = q.value("type");
        m["montant"] = q.value("montant");
        m["description"] = q.value("description");
        list << m;
    }
    return list;
}

QVariantMap DatabaseManager::getTransaction(const QString &code)
{
    QVariantMap m;
    QSqlQuery q;
    q.prepare("SELECT * FROM transactions WHERE code=?");
    q.addBindValue(code);
    if (q.exec() && q.next()) {
        m["code"] = q.value("code");
        m["date"] = q.value("date");
        m["type"] = q.value("type");
        m["montant"] = q.value("montant");
        m["description"] = q.value("description");
    }
    return m;
}

QList<QVariantMap> DatabaseManager::searchTransactions(const QString &term)
{
    QList<QVariantMap> list;
    QSqlQuery q;
    q.prepare("SELECT * FROM transactions WHERE type LIKE ? OR description LIKE ?");
    q.addBindValue("%" + term + "%");
    q.addBindValue("%" + term + "%");
    if (q.exec()) {
        while (q.next()) {
            QVariantMap m;
            m["code"] = q.value("code");
            m["date"] = q.value("date");
            m["type"] = q.value("type");
            m["montant"] = q.value("montant");
            m["description"] = q.value("description");
            list << m;
        }
    }
    return list;
}

QVariantMap DatabaseManager::getFinancialStatistics(int, int)
{
    QVariantMap stats;
    stats["revenu"] = 0;
    stats["depense"] = 0;
    return stats;
}

QVariantMap DatabaseManager::getMonthlyEvolution(int)
{
    QVariantMap evo;
    evo["mois"] = "Janvier";
    evo["revenu"] = 0;
    evo["depense"] = 0;
    return evo;
}

double DatabaseManager::getTotalRevenue(int, int) { return 0; }
double DatabaseManager::getTotalExpenses(int, int) { return 0; }

// -------------------- Incidents --------------------
bool DatabaseManager::addIncident(const QVariantMap &data)
{
    QSqlQuery q;
    q.prepare("INSERT INTO incidents (categorie, description, etat, date) VALUES (?, ?, ?, ?)");
    q.addBindValue(data.value("categorie"));
    q.addBindValue(data.value("description"));
    q.addBindValue(data.value("etat"));
    q.addBindValue(data.value("date"));
    return q.exec();
}

// -------------------- Vehicles --------------------
bool DatabaseManager::addVehicule(const QVariantMap &data)
{
    QSqlQuery q;
    q.prepare("INSERT INTO vehicules (matricule, type, proprietaire) VALUES (?, ?, ?)");
    q.addBindValue(data.value("matricule"));
    q.addBindValue(data.value("type"));
    q.addBindValue(data.value("proprietaire"));
    return q.exec();
}
