#include "databasemanager.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent)
{
    // Utiliser la connexion Oracle existante
    QSqlDatabase db = QSqlDatabase::database();

    if (!db.isOpen()) {
        qWarning() << "[DB] Database is not open!";
        return;
    }

    qInfo() << "[DB] Using existing Oracle database connection.";
    ensureSchema();
}

void DatabaseManager::ensureSchema()
{
    QSqlQuery q;

    // Créer les tables si elles n'existent pas
    // Table residents
    q.exec("CREATE TABLE residents ("
           "cin VARCHAR2(20) PRIMARY KEY, "
           "nom VARCHAR2(100), "
           "prenom VARCHAR2(100), "
           "appartement VARCHAR2(50), "
           "statut VARCHAR2(50))");

    // Table transactions
    q.exec("CREATE TABLE transactions ("
           "code VARCHAR2(20) PRIMARY KEY, "
           "date_trans VARCHAR2(20), "
           "type VARCHAR2(50), "
           "montant NUMBER(10,2), "
           "description VARCHAR2(255))");

    // Table incidents
    q.exec("CREATE TABLE incidents ("
           "id NUMBER GENERATED ALWAYS AS IDENTITY PRIMARY KEY, "
           "categorie VARCHAR2(100), "
           "description VARCHAR2(500), "
           "etat VARCHAR2(50), "
           "date_incident VARCHAR2(20))");

    // Table vehicules
    q.exec("CREATE TABLE vehicules ("
           "id VARCHAR2(20) PRIMARY KEY, "
           "type VARCHAR2(50), "
           "capacite NUMBER, "
           "zone VARCHAR2(100), "
           "horaire VARCHAR2(50), "
           "statut VARCHAR2(50), "
           "date_ajout VARCHAR2(20), "
           "temps_utilise NUMBER(10,1))");

    if (q.lastError().isValid() && !q.lastError().text().contains("name is already used")) {
        qWarning() << "[DB] Schema warning:" << q.lastError().text();
    }
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

    if (!q.exec()) {
        qWarning() << "[DB] Error adding resident:" << q.lastError().text();
        return false;
    }
    return true;
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

    if (!q.exec()) {
        qWarning() << "[DB] Error updating resident:" << q.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::deleteResident(const QString &cin)
{
    QSqlQuery q;
    q.prepare("DELETE FROM residents WHERE cin=?");
    q.addBindValue(cin);

    if (!q.exec()) {
        qWarning() << "[DB] Error deleting resident:" << q.lastError().text();
        return false;
    }
    return true;
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
    q.prepare("INSERT INTO transactions (code, date_trans, type, montant, description) VALUES (?, ?, ?, ?, ?)");
    q.addBindValue(data.value("code"));
    q.addBindValue(data.value("date"));
    q.addBindValue(data.value("type"));
    q.addBindValue(data.value("montant"));
    q.addBindValue(data.value("description"));

    if (!q.exec()) {
        qWarning() << "[DB] Error adding transaction:" << q.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::updateTransaction(const QString &code, const QVariantMap &data)
{
    QSqlQuery q;
    q.prepare("UPDATE transactions SET date_trans=?, type=?, montant=?, description=? WHERE code=?");
    q.addBindValue(data.value("date"));
    q.addBindValue(data.value("type"));
    q.addBindValue(data.value("montant"));
    q.addBindValue(data.value("description"));
    q.addBindValue(code);

    if (!q.exec()) {
        qWarning() << "[DB] Error updating transaction:" << q.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::deleteTransaction(const QString &code)
{
    QSqlQuery q;
    q.prepare("DELETE FROM transactions WHERE code=?");
    q.addBindValue(code);

    if (!q.exec()) {
        qWarning() << "[DB] Error deleting transaction:" << q.lastError().text();
        return false;
    }
    return true;
}

QList<QVariantMap> DatabaseManager::getAllTransactions()
{
    QList<QVariantMap> list;
    QSqlQuery q("SELECT * FROM transactions");

    while (q.next()) {
        QVariantMap m;
        m["code"] = q.value("code");
        m["date"] = q.value("date_trans");
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
        m["date"] = q.value("date_trans");
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
            m["date"] = q.value("date_trans");
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
    q.prepare("INSERT INTO incidents (categorie, description, etat, date_incident) VALUES (?, ?, ?, ?)");
    q.addBindValue(data.value("categorie"));
    q.addBindValue(data.value("description"));
    q.addBindValue(data.value("etat"));
    q.addBindValue(data.value("date"));

    if (!q.exec()) {
        qWarning() << "[DB] Error adding incident:" << q.lastError().text();
        return false;
    }
    return true;
}

// -------------------- Vehicles --------------------
bool DatabaseManager::addVehicule(const QVariantMap &data)
{
    QSqlQuery q;
    q.prepare("INSERT INTO vehicules (id, type, capacite, zone, horaire, statut, date_ajout, temps_utilise) "
              "VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
    q.addBindValue(data.value("id"));
    q.addBindValue(data.value("type"));
    q.addBindValue(data.value("capacite"));
    q.addBindValue(data.value("zone"));
    q.addBindValue(data.value("horaire"));
    q.addBindValue(data.value("statut"));
    q.addBindValue(data.value("date"));
    q.addBindValue(data.value("temps_utilise"));

    if (!q.exec()) {
        qWarning() << "[DB] Error adding vehicule:" << q.lastError().text();
        return false;
    }

    qDebug() << "[DB] Vehicule added successfully:" << data.value("id").toString();
    return true;
}

bool DatabaseManager::updateVehicule(const QString &id, const QVariantMap &data)
{
    QSqlQuery q;
    q.prepare("UPDATE vehicules SET type=?, capacite=?, zone=?, horaire=?, statut=?, "
              "date_ajout=?, temps_utilise=? WHERE id=?");
    q.addBindValue(data.value("type"));
    q.addBindValue(data.value("capacite"));
    q.addBindValue(data.value("zone"));
    q.addBindValue(data.value("horaire"));
    q.addBindValue(data.value("statut"));
    q.addBindValue(data.value("date"));
    q.addBindValue(data.value("temps_utilise"));
    q.addBindValue(id);

    if (!q.exec()) {
        qWarning() << "[DB] Error updating vehicule:" << q.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::deleteVehicule(const QString &id)
{
    QSqlQuery q;
    q.prepare("DELETE FROM vehicules WHERE id=?");
    q.addBindValue(id);

    if (!q.exec()) {
        qWarning() << "[DB] Error deleting vehicule:" << q.lastError().text();
        return false;
    }
    return true;
}

QList<QVariantMap> DatabaseManager::getAllVehicules()
{
    QList<QVariantMap> list;
    QSqlQuery q("SELECT * FROM vehicules ORDER BY id");

    while (q.next()) {
        QVariantMap m;
        m["id"] = q.value("id");
        m["type"] = q.value("type");
        m["capacite"] = q.value("capacite");
        m["zone"] = q.value("zone");
        m["horaire"] = q.value("horaire");
        m["statut"] = q.value("statut");
        m["date"] = q.value("date_ajout");
        m["temps_utilise"] = q.value("temps_utilise");
        list << m;
    }

    qDebug() << "[DB] Retrieved" << list.size() << "vehicules";
    return list;
}

QVariantMap DatabaseManager::getVehicule(const QString &id)
{
    QVariantMap m;
    QSqlQuery q;
    q.prepare("SELECT * FROM vehicules WHERE id=?");
    q.addBindValue(id);

    if (q.exec() && q.next()) {
        m["id"] = q.value("id");
        m["type"] = q.value("type");
        m["capacite"] = q.value("capacite");
        m["zone"] = q.value("zone");
        m["horaire"] = q.value("horaire");
        m["statut"] = q.value("statut");
        m["date"] = q.value("date_ajout");
        m["temps_utilise"] = q.value("temps_utilise");
    }
    return m;
}

QList<QVariantMap> DatabaseManager::searchVehicules(const QString &term)
{
    QList<QVariantMap> list;
    QSqlQuery q;
    q.prepare("SELECT * FROM vehicules WHERE id LIKE ? OR type LIKE ? OR zone LIKE ?");
    QString searchTerm = "%" + term + "%";
    q.addBindValue(searchTerm);
    q.addBindValue(searchTerm);
    q.addBindValue(searchTerm);

    if (q.exec()) {
        while (q.next()) {
            QVariantMap m;
            m["id"] = q.value("id");
            m["type"] = q.value("type");
            m["capacite"] = q.value("capacite");
            m["zone"] = q.value("zone");
            m["horaire"] = q.value("horaire");
            m["statut"] = q.value("statut");
            m["date"] = q.value("date_ajout");
            m["temps_utilise"] = q.value("temps_utilise");
            list << m;
        }
    }
    return list;
}
