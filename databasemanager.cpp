#include "databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QMessageBox>

DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent)
{
    // Connexion Oracle via ODBC — DSN = OracleXE
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("OracleXE");   // Ton DSN exact dans ODBC
    db.setUserName("NEXORA");
    db.setPassword("nexora124");

    if (db.open()) {
        qInfo() << "✅ Oracle connection succeeded!";
    } else {
        qWarning() << "❌ Oracle connection failed:" << db.lastError().text();
        QMessageBox::critical(nullptr, "Erreur Oracle",
                              "Impossible de se connecter à la base Oracle.\n"
                              "Vérifie le DSN (OracleXE), le login et le mot de passe.");
    }
}

QSqlDatabase DatabaseManager::database() const
{
    return db;
}

//
// ==================== MODULE RÉSIDENTS ====================
//
bool DatabaseManager::addResident(const QVariantMap &data)
{
    if (!db.isOpen()) {
        qWarning() << "⚠️ Database not open!";
        QMessageBox::warning(nullptr, "Erreur", "Base Oracle non connectée.");
        return false;
    }

    QSqlQuery q;
    q.prepare("INSERT INTO RESIDENTS "
              "(CIN, NOM, PRENOM, SEXE, TELEPHONE, EMAIL, DATE_ENTREE, APPARTEMENT, ETAGE, STATUT) "
              "VALUES (:cin, :nom, :prenom, :sexe, :telephone, :email, "
              "NVL(TO_DATE(:date_entree, 'YYYY-MM-DD'), SYSDATE), "
              ":appartement, :etage, :statut)");

    q.bindValue(":cin", data.value("cin"));
    q.bindValue(":nom", data.value("nom"));
    q.bindValue(":prenom", data.value("prenom"));
    q.bindValue(":sexe", data.value("sexe"));
    q.bindValue(":telephone", data.value("telephone"));
    q.bindValue(":email", data.value("email"));
    q.bindValue(":date_entree", data.value("date_entree"));
    q.bindValue(":appartement", data.value("appartement"));
    q.bindValue(":etage", data.value("etage"));
    q.bindValue(":statut", data.value("statut"));

    if (!q.exec()) {
        QString err = q.lastError().text();
        qWarning() << "❌ Failed to insert resident:" << err;
        QMessageBox::critical(nullptr, "Erreur Oracle",
                              "Impossible d'ajouter le résident à la base.\n\n" + err);
        return false;
    }

    qInfo() << "✅ Resident added successfully!";
    return true;
}

bool DatabaseManager::updateResident(const QString &cin, const QVariantMap &data)
{
    QSqlQuery q;
    q.prepare("UPDATE RESIDENTS SET "
              "NOM=:nom, PRENOM=:prenom, SEXE=:sexe, TELEPHONE=:telephone, EMAIL=:email, "
              "APPARTEMENT=:appartement, ETAGE=:etage, STATUT=:statut "
              "WHERE CIN=:cin");

    q.bindValue(":nom", data.value("nom"));
    q.bindValue(":prenom", data.value("prenom"));
    q.bindValue(":sexe", data.value("sexe"));
    q.bindValue(":telephone", data.value("telephone"));
    q.bindValue(":email", data.value("email"));
    q.bindValue(":appartement", data.value("appartement"));
    q.bindValue(":etage", data.value("etage"));
    q.bindValue(":statut", data.value("statut"));
    q.bindValue(":cin", cin);

    if (!q.exec()) {
        qWarning() << "❌ Failed to update resident:" << q.lastError().text();
        QMessageBox::critical(nullptr, "Erreur Oracle",
                              "Impossible de modifier le résident.\n\n" + q.lastError().text());
        return false;
    }
    return true;
}

bool DatabaseManager::deleteResident(const QString &cin)
{
    QSqlQuery q;
    q.prepare("DELETE FROM RESIDENTS WHERE CIN=:cin");
    q.bindValue(":cin", cin);
    if (!q.exec()) {
        qWarning() << "❌ Failed to delete resident:" << q.lastError().text();
        QMessageBox::critical(nullptr, "Erreur Oracle",
                              "Impossible de supprimer le résident.\n\n" + q.lastError().text());
        return false;
    }
    return true;
}

QList<QVariantMap> DatabaseManager::getAllResidents()
{
    QList<QVariantMap> list;
    QSqlQuery q("SELECT CIN, NOM, PRENOM, SEXE, TELEPHONE, EMAIL, "
                "TO_CHAR(DATE_ENTREE,'YYYY-MM-DD') AS DATE_ENTREE, "
                "APPARTEMENT, ETAGE, STATUT FROM RESIDENTS ORDER BY ID DESC");

    while (q.next()) {
        QVariantMap m;
        m["cin"] = q.value("CIN");
        m["nom"] = q.value("NOM");
        m["prenom"] = q.value("PRENOM");
        m["sexe"] = q.value("SEXE");
        m["telephone"] = q.value("TELEPHONE");
        m["email"] = q.value("EMAIL");
        m["date_entree"] = q.value("DATE_ENTREE");
        m["appartement"] = q.value("APPARTEMENT");
        m["etage"] = q.value("ETAGE");
        m["statut"] = q.value("STATUT");
        list << m;
    }
    return list;
}

QVariantMap DatabaseManager::getResident(const QString &cin)
{
    QVariantMap m;
    QSqlQuery q;
    q.prepare("SELECT CIN, NOM, PRENOM, SEXE, TELEPHONE, EMAIL, "
              "TO_CHAR(DATE_ENTREE,'YYYY-MM-DD') AS DATE_ENTREE, "
              "APPARTEMENT, ETAGE, STATUT FROM RESIDENTS WHERE CIN=:cin");
    q.bindValue(":cin", cin);

    if (q.exec() && q.next()) {
        m["cin"] = q.value("CIN");
        m["nom"] = q.value("NOM");
        m["prenom"] = q.value("PRENOM");
        m["sexe"] = q.value("SEXE");
        m["telephone"] = q.value("TELEPHONE");
        m["email"] = q.value("EMAIL");
        m["date_entree"] = q.value("DATE_ENTREE");
        m["appartement"] = q.value("APPARTEMENT");
        m["etage"] = q.value("ETAGE");
        m["statut"] = q.value("STATUT");
    }
    return m;
}

QList<QVariantMap> DatabaseManager::searchResidents(const QString &term)
{
    QList<QVariantMap> list;
    QSqlQuery q;
    q.prepare("SELECT CIN, NOM, PRENOM, APPARTEMENT FROM RESIDENTS "
              "WHERE LOWER(NOM) LIKE LOWER(:t) OR LOWER(PRENOM) LIKE LOWER(:t)");
    q.bindValue(":t", "%" + term + "%");

    if (q.exec()) {
        while (q.next()) {
            QVariantMap m;
            m["cin"] = q.value("CIN");
            m["nom"] = q.value("NOM");
            m["prenom"] = q.value("PRENOM");
            m["appartement"] = q.value("APPARTEMENT");
            list << m;
        }
    }
    return list;
}

QVariantMap DatabaseManager::getStatistics()
{
    QVariantMap stats;
    QSqlQuery q("SELECT COUNT(*) FROM RESIDENTS");
    if (q.next())
        stats["total"] = q.value(0).toInt();
    return stats;
}

//
// ==================== MODULE TRANSACTIONS ====================
//
bool DatabaseManager::addTransaction(const QVariantMap &data)
{
    QSqlQuery q;
    q.prepare("INSERT INTO TRANSACTIONS (CODE, DATE, TYPE, MONTANT, DESCRIPTION) "
              "VALUES (:code, TO_DATE(:date,'YYYY-MM-DD'), :type, :montant, :description)");
    q.bindValue(":code", data.value("code"));
    q.bindValue(":date", data.value("date"));
    q.bindValue(":type", data.value("type"));
    q.bindValue(":montant", data.value("montant"));
    q.bindValue(":description", data.value("description"));

    if (!q.exec()) {
        qWarning() << "❌ Failed to insert transaction:" << q.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::updateTransaction(const QString &code, const QVariantMap &data)
{
    QSqlQuery q;
    q.prepare("UPDATE TRANSACTIONS SET "
              "DATE=TO_DATE(:date,'YYYY-MM-DD'), TYPE=:type, MONTANT=:montant, DESCRIPTION=:description "
              "WHERE CODE=:code");
    q.bindValue(":date", data.value("date"));
    q.bindValue(":type", data.value("type"));
    q.bindValue(":montant", data.value("montant"));
    q.bindValue(":description", data.value("description"));
    q.bindValue(":code", code);
    return q.exec();
}

bool DatabaseManager::deleteTransaction(const QString &code)
{
    QSqlQuery q;
    q.prepare("DELETE FROM TRANSACTIONS WHERE CODE=:code");
    q.bindValue(":code", code);
    return q.exec();
}

QList<QVariantMap> DatabaseManager::getAllTransactions()
{
    QList<QVariantMap> list;
    QSqlQuery q("SELECT CODE, TO_CHAR(DATE,'YYYY-MM-DD') AS DATE, TYPE, MONTANT, DESCRIPTION FROM TRANSACTIONS");

    while (q.next()) {
        QVariantMap m;
        m["code"] = q.value("CODE");
        m["date"] = q.value("DATE");
        m["type"] = q.value("TYPE");
        m["montant"] = q.value("MONTANT");
        m["description"] = q.value("DESCRIPTION");
        list << m;
    }
    return list;
}

QVariantMap DatabaseManager::getTransaction(const QString &code)
{
    QVariantMap m;
    QSqlQuery q;
    q.prepare("SELECT CODE, TO_CHAR(DATE,'YYYY-MM-DD') AS DATE, TYPE, MONTANT, DESCRIPTION "
              "FROM TRANSACTIONS WHERE CODE=:code");
    q.bindValue(":code", code);
    if (q.exec() && q.next()) {
        m["code"] = q.value("CODE");
        m["date"] = q.value("DATE");
        m["type"] = q.value("TYPE");
        m["montant"] = q.value("MONTANT");
        m["description"] = q.value("DESCRIPTION");
    }
    return m;
}

QList<QVariantMap> DatabaseManager::searchTransactions(const QString &term)
{
    QList<QVariantMap> list;
    QSqlQuery q;
    q.prepare("SELECT CODE, TO_CHAR(DATE,'YYYY-MM-DD') AS DATE, TYPE, MONTANT, DESCRIPTION "
              "FROM TRANSACTIONS WHERE LOWER(TYPE) LIKE LOWER(:t) OR LOWER(DESCRIPTION) LIKE LOWER(:t)");
    q.bindValue(":t", "%" + term + "%");

    if (q.exec()) {
        while (q.next()) {
            QVariantMap m;
            m["code"] = q.value("CODE");
            m["date"] = q.value("DATE");
            m["type"] = q.value("TYPE");
            m["montant"] = q.value("MONTANT");
            m["description"] = q.value("DESCRIPTION");
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

//
// ==================== MODULE INCIDENTS ====================
//
bool DatabaseManager::addIncident(const QVariantMap &data)
{
    QSqlQuery q;
    q.prepare("INSERT INTO INCIDENTS (CATEGORIE, DESCRIPTION, ETAT, DATE) "
              "VALUES (:categorie, :description, :etat, TO_DATE(:date,'YYYY-MM-DD'))");
    q.bindValue(":categorie", data.value("categorie"));
    q.bindValue(":description", data.value("description"));
    q.bindValue(":etat", data.value("etat"));
    q.bindValue(":date", data.value("date"));
    return q.exec();
}

//
// ==================== MODULE VÉHICULES ====================
//
bool DatabaseManager::addVehicule(const QVariantMap &data)
{
    QSqlQuery q;
    q.prepare("INSERT INTO VEHICULES (MATRICULE, TYPE, PROPRIETAIRE) "
              "VALUES (:matricule, :type, :proprietaire)");
    q.bindValue(":matricule", data.value("matricule"));
    q.bindValue(":type", data.value("type"));
    q.bindValue(":proprietaire", data.value("proprietaire"));
    return q.exec();
}
