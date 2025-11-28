#include "databasemanager.h"
#include "connection.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QMessageBox>
#include <QDate>

DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent)
{
    // Utiliser la connexion singleton
    db = Connection::getInstance().getDatabase();

    if (db.isOpen()) {
        qInfo() << "✅ DatabaseManager initialisé avec connexion active";
    } else {
        qWarning() << "❌ DatabaseManager: connexion non établie";
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
        QMessageBox::warning(nullptr, "Erreur", "Connexion Oracle fermée.");
        return false;
    }

    QSqlQuery q(db);
    q.prepare(R"(
        INSERT INTO RESIDENTS
        (CIN, NOM, PRENOM, DATE_NAISSANCE, ADRESSE, TELEPHONE, EMAIL, STATUT, SATISFACTION)
        VALUES (:cin, :nom, :prenom,
                TO_DATE(:date_naissance, 'YYYY-MM-DD'),
                :adresse, :telephone, :email, :statut, :satisfaction)
    )");

    q.bindValue(":cin", data.value("cin"));
    q.bindValue(":nom", data.value("nom"));
    q.bindValue(":prenom", data.value("prenom"));
    q.bindValue(":date_naissance", data.value("date_naissance"));
    q.bindValue(":adresse", data.value("adresse"));
    q.bindValue(":telephone", data.value("telephone"));
    q.bindValue(":email", data.value("email"));
    q.bindValue(":statut", data.value("statut"));
    q.bindValue(":satisfaction", data.value("satisfaction"));

    qInfo() << "[DEBUG SQL]" << q.lastQuery();

    if (!q.exec()) {
        QMessageBox::critical(nullptr, "Erreur Oracle",
                              "Impossible d'ajouter le résident.\n\n" + q.lastError().text());
        qWarning() << "❌ addResident:" << q.lastError().text();
        return false;
    }

    db.commit();
    qInfo() << "✅ Résident ajouté avec succès dans Oracle.";
    return true;
}

bool DatabaseManager::updateResident(const QString &cin, const QVariantMap &data)
{
    if (!db.isOpen()) {
        QMessageBox::warning(nullptr, "Erreur", "Connexion Oracle fermée.");
        return false;
    }

    QSqlQuery q(db);
    q.prepare(R"(
        UPDATE RESIDENTS SET
        NOM=:nom,
        PRENOM=:prenom,
        DATE_NAISSANCE=TO_DATE(:date_naissance,'YYYY-MM-DD'),
        ADRESSE=:adresse,
        TELEPHONE=:telephone,
        EMAIL=:email,
        STATUT=:statut,
        SATISFACTION=:satisfaction
        WHERE CIN=:cin
    )");

    q.bindValue(":nom", data.value("nom"));
    q.bindValue(":prenom", data.value("prenom"));
    q.bindValue(":date_naissance", data.value("date_naissance"));
    q.bindValue(":adresse", data.value("adresse"));
    q.bindValue(":telephone", data.value("telephone"));
    q.bindValue(":email", data.value("email"));
    q.bindValue(":statut", data.value("statut"));
    q.bindValue(":satisfaction", data.value("satisfaction"));
    q.bindValue(":cin", cin);

    qInfo() << "[DEBUG UPDATE]" << q.lastQuery();

    if (!q.exec()) {
        QMessageBox::critical(nullptr, "Erreur Oracle",
                              "Impossible de modifier le résident.\n\n" + q.lastError().text());
        qWarning() << "❌ updateResident:" << q.lastError().text();
        return false;
    }

    db.commit();
    qInfo() << "✅ Résident mis à jour avec succès dans Oracle.";
    return true;
}

bool DatabaseManager::deleteResident(const QString &cin)
{
    QSqlQuery q(db);
    q.prepare("DELETE FROM RESIDENTS WHERE CIN=:cin");
    q.bindValue(":cin", cin);
    if (!q.exec()) {
        QMessageBox::critical(nullptr, "Erreur Oracle",
                              "Impossible de supprimer le résident.\n\n" + q.lastError().text());
        return false;
    }
    db.commit();
    return true;
}

QList<QVariantMap> DatabaseManager::getAllResidents()
{
    QList<QVariantMap> list;
    QSqlQuery q(db);
    q.exec(R"(
        SELECT CIN, NOM, PRENOM,
               TO_CHAR(DATE_NAISSANCE,'YYYY-MM-DD') AS DATE_NAISSANCE,
               ADRESSE, TELEPHONE, EMAIL, STATUT, SATISFACTION
        FROM RESIDENTS ORDER BY DATE_NAISSANCE DESC
    )");
    while (q.next()) {
        QVariantMap m;
        m["cin"] = q.value("CIN");
        m["nom"] = q.value("NOM");
        m["prenom"] = q.value("PRENOM");
        m["date_naissance"] = q.value("DATE_NAISSANCE");
        m["adresse"] = q.value("ADRESSE");
        m["telephone"] = q.value("TELEPHONE");
        m["email"] = q.value("EMAIL");
        m["statut"] = q.value("STATUT");
        m["satisfaction"] = q.value("SATISFACTION");
        list << m;
    }
    return list;
}

QVariantMap DatabaseManager::getResident(const QString &cin)
{
    QVariantMap m;
    QSqlQuery q(db);
    q.prepare(R"(
        SELECT CIN, NOM, PRENOM,
               TO_CHAR(DATE_NAISSANCE,'YYYY-MM-DD') AS DATE_NAISSANCE,
               ADRESSE, TELEPHONE, EMAIL, STATUT, SATISFACTION
        FROM RESIDENTS WHERE CIN=:cin
    )");
    q.bindValue(":cin", cin);
    if (q.exec() && q.next()) {
        m["cin"] = q.value("CIN");
        m["nom"] = q.value("NOM");
        m["prenom"] = q.value("PRENOM");
        m["date_naissance"] = q.value("DATE_NAISSANCE");
        m["adresse"] = q.value("ADRESSE");
        m["telephone"] = q.value("TELEPHONE");
        m["email"] = q.value("EMAIL");
        m["statut"] = q.value("STATUT");
        m["satisfaction"] = q.value("SATISFACTION");
    }
    return m;
}

QList<QVariantMap> DatabaseManager::searchResidents(const QString &term)
{
    QList<QVariantMap> list;
    QSqlQuery q(db);
    q.prepare(R"(
        SELECT CIN, NOM, PRENOM, ADRESSE
        FROM RESIDENTS
        WHERE LOWER(NOM) LIKE LOWER(:t) OR LOWER(PRENOM) LIKE LOWER(:t)
           OR LOWER(ADRESSE) LIKE LOWER(:t)
    )");
    q.bindValue(":t", "%" + term + "%");
    if (q.exec()) {
        while (q.next()) {
            QVariantMap m;
            m["cin"] = q.value("CIN");
            m["nom"] = q.value("NOM");
            m["prenom"] = q.value("PRENOM");
            m["adresse"] = q.value("ADRESSE");
            list << m;
        }
    }
    return list;
}

QVariantMap DatabaseManager::getStatistics()
{
    QVariantMap stats;
    QSqlQuery q(db);
    q.exec("SELECT COUNT(*) AS TOTAL FROM RESIDENTS");
    if (q.next())
        stats["total"] = q.value("TOTAL").toInt();
    return stats;
}

//
// ==================== MODULE TRANSACTIONS ====================
//
bool DatabaseManager::addTransaction(const QVariantMap &data)
{
    if (!db.isOpen()) {
        QMessageBox::warning(nullptr, "Erreur", "Connexion Oracle fermée.");
        return false;
    }

    auto normalizeDate = [](const QVariant &value) -> QString {
        if (value.canConvert<QDate>()) {
            return value.toDate().toString("yyyy-MM-dd");
        }
        QString str = value.toString().trimmed();
        if (!str.isEmpty()) {
            return QDate::fromString(str, "yyyy-MM-dd").isValid()
            ? str
            : QDate::fromString(str, "dd/MM/yyyy").toString("yyyy-MM-dd");
        }
        return QDate::currentDate().toString("yyyy-MM-dd");
    };

    QString code = data.value("code_unique", data.value("code")).toString().trimmed();
    QString type = data.value("type").toString().trimmed();
    QString categorie = data.value("categorie").toString().trimmed();
    QString description = data.value("description").toString().trimmed();
    double montant = data.value("montant").toDouble();
    QString date = normalizeDate(data.value("date_transaction", data.value("date")));
    QString cinResident = data.value("cin_resident").toString().trimmed();

    if (code.isEmpty() || type.isEmpty() || date.isEmpty()) {
        QMessageBox::warning(nullptr, "Validation",
                             "Code, type et date de transaction sont obligatoires.");
        return false;
    }

    QSqlQuery q(db);
    q.prepare(R"(
        INSERT INTO TRANSACTIONS (CODE_UNIQUE, MONTANT, "TYPE", CATEGORIE, DESCRIPTION, DATE_TRANSACTION, CIN_RESIDENT)
        VALUES (:code, :montant, :type, :categorie, :description, TO_DATE(:date,'YYYY-MM-DD'), :cin)
    )");

    q.bindValue(":code", code);
    q.bindValue(":montant", montant);
    q.bindValue(":type", type);
    q.bindValue(":categorie", categorie);
    q.bindValue(":description", description);
    q.bindValue(":date", date);
    if (cinResident.isEmpty())
        q.bindValue(":cin", QVariant());
    else
        q.bindValue(":cin", cinResident);

    if (!q.exec()) {
        qWarning() << "❌ addTransaction:" << q.lastError().text();
        QMessageBox::critical(nullptr, "Erreur Oracle",
                              "Impossible d'ajouter la transaction.\n\n" + q.lastError().text());
        return false;
    }
    db.commit();
    qInfo() << "✅ Transaction ajoutée avec succès dans Oracle.";
    return true;
}

QList<QVariantMap> DatabaseManager::getAllTransactions()
{
    QList<QVariantMap> list;
    if (!db.isOpen())
        return list;

    QSqlQuery q(db);
    if (!q.exec(R"(
            SELECT CODE_UNIQUE, MONTANT, "TYPE", CATEGORIE, DESCRIPTION,
                   TO_CHAR(DATE_TRANSACTION,'YYYY-MM-DD') AS DATE_TRANSACTION,
                   CIN_RESIDENT
            FROM TRANSACTIONS
            ORDER BY DATE_TRANSACTION DESC
        )")) {
        qWarning() << "❌ getAllTransactions:" << q.lastError().text();
        return list;
    }

    while (q.next()) {
        QVariantMap m;
        m["code_unique"] = q.value("CODE_UNIQUE");
        m["montant"] = q.value("MONTANT");
        m["type"] = q.value("TYPE");
        m["categorie"] = q.value("CATEGORIE");
        m["description"] = q.value("DESCRIPTION");
        m["date_transaction"] = q.value("DATE_TRANSACTION");
        m["cin_resident"] = q.value("CIN_RESIDENT");
        list << m;
    }
    return list;
}

QVariantMap DatabaseManager::getTransaction(const QString &code)
{
    QVariantMap m;
    if (!db.isOpen())
        return m;

    QSqlQuery q(db);
    q.prepare(R"(
        SELECT CODE_UNIQUE, MONTANT, "TYPE", CATEGORIE, DESCRIPTION,
               TO_CHAR(DATE_TRANSACTION,'YYYY-MM-DD') AS DATE_TRANSACTION,
               CIN_RESIDENT
        FROM TRANSACTIONS
        WHERE CODE_UNIQUE=:code
    )");
    q.bindValue(":code", code);
    if (q.exec() && q.next()) {
        m["code_unique"] = q.value("CODE_UNIQUE");
        m["montant"] = q.value("MONTANT");
        m["type"] = q.value("TYPE");
        m["categorie"] = q.value("CATEGORIE");
        m["description"] = q.value("DESCRIPTION");
        m["date_transaction"] = q.value("DATE_TRANSACTION");
        m["cin_resident"] = q.value("CIN_RESIDENT");
    }
    return m;
}

bool DatabaseManager::updateTransaction(const QString &code, const QVariantMap &data)
{
    if (!db.isOpen())
        return false;

    auto normalizeDate = [](const QVariant &value) -> QString {
        if (value.canConvert<QDate>())
            return value.toDate().toString("yyyy-MM-dd");
        QString str = value.toString().trimmed();
        if (str.isEmpty())
            return QDate::currentDate().toString("yyyy-MM-dd");
        QDate parsed = QDate::fromString(str, "yyyy-MM-dd");
        if (!parsed.isValid())
            parsed = QDate::fromString(str, "dd/MM/yyyy");
        return parsed.isValid() ? parsed.toString("yyyy-MM-dd")
                                : QDate::currentDate().toString("yyyy-MM-dd");
    };

    QString type = data.value("type").toString().trimmed();

    QSqlQuery q(db);
    q.prepare(R"(
        UPDATE TRANSACTIONS SET
            MONTANT=:montant,
            "TYPE"=:type,
            CATEGORIE=:categorie,
            DESCRIPTION=:description,
            DATE_TRANSACTION=TO_DATE(:date,'YYYY-MM-DD'),
            CIN_RESIDENT=:cin
        WHERE CODE_UNIQUE=:code
    )");

    q.bindValue(":montant", data.value("montant").toDouble());
    q.bindValue(":type", type);
    q.bindValue(":categorie", data.value("categorie").toString().trimmed());
    q.bindValue(":description", data.value("description").toString().trimmed());
    q.bindValue(":date", normalizeDate(data.value("date_transaction", data.value("date"))));
    QString cinResident = data.value("cin_resident").toString().trimmed();
    if (cinResident.isEmpty())
        q.bindValue(":cin", QVariant());
    else
        q.bindValue(":cin", cinResident);
    q.bindValue(":code", code);

    if (!q.exec()) {
        qWarning() << "❌ updateTransaction:" << q.lastError().text();
        QMessageBox::critical(nullptr, "Erreur Oracle", q.lastError().text());
        return false;
    }

    db.commit();
    return true;
}

bool DatabaseManager::deleteTransaction(const QString &code)
{
    if (!db.isOpen())
        return false;

    QSqlQuery q(db);
    q.prepare("DELETE FROM TRANSACTIONS WHERE CODE_UNIQUE=:code");
    q.bindValue(":code", code);
    if (!q.exec()) {
        qWarning() << "❌ deleteTransaction:" << q.lastError().text();
        QMessageBox::critical(nullptr, "Erreur Oracle", q.lastError().text());
        return false;
    }
    db.commit();
    return true;
}

QList<QVariantMap> DatabaseManager::searchTransactions(const QString &term)
{
    QList<QVariantMap> list;
    if (!db.isOpen())
        return list;

    QSqlQuery q(db);
    q.prepare(R"(
        SELECT CODE_UNIQUE, MONTANT, "TYPE", CATEGORIE, DESCRIPTION,
               TO_CHAR(DATE_TRANSACTION,'YYYY-MM-DD') AS DATE_TRANSACTION,
               CIN_RESIDENT
        FROM TRANSACTIONS
        WHERE LOWER(CODE_UNIQUE) LIKE LOWER(:term)
           OR LOWER("TYPE") LIKE LOWER(:term)
           OR LOWER(CATEGORIE) LIKE LOWER(:term)
           OR LOWER(DESCRIPTION) LIKE LOWER(:term)
           OR LOWER(CIN_RESIDENT) LIKE LOWER(:term)
    )");
    q.bindValue(":term", "%" + term + "%");
    if (q.exec()) {
        while (q.next()) {
            QVariantMap m;
            m["code_unique"] = q.value("CODE_UNIQUE");
            m["montant"] = q.value("MONTANT");
            m["type"] = q.value("TYPE");
            m["categorie"] = q.value("CATEGORIE");
            m["description"] = q.value("DESCRIPTION");
            m["date_transaction"] = q.value("DATE_TRANSACTION");
            m["cin_resident"] = q.value("CIN_RESIDENT");
            list << m;
        }
    }
    return list;
}

QVariantMap DatabaseManager::getFinancialStatistics(int year, int month)
{
    QVariantMap stats;
    if (!db.isOpen())
        return stats;

    QSqlQuery q(db);
    q.prepare(R"(
        SELECT NVL(SUM(CASE WHEN "TYPE"='Revenu' THEN MONTANT ELSE 0 END),0) AS REVENU,
               NVL(SUM(CASE WHEN "TYPE"='Dépense' THEN MONTANT ELSE 0 END),0) AS DEPENSE
        FROM TRANSACTIONS
        WHERE EXTRACT(YEAR FROM DATE_TRANSACTION)=:y
          AND EXTRACT(MONTH FROM DATE_TRANSACTION)=:m
    )");
    q.bindValue(":y", year);
    q.bindValue(":m", month);
    if (q.exec() && q.next()) {
        stats["revenu"] = q.value("REVENU");
        stats["depense"] = q.value("DEPENSE");
    }
    return stats;
}

QVariantMap DatabaseManager::getMonthlyEvolution(int year)
{
    QVariantMap evo;
    if (!db.isOpen())
        return evo;

    QSqlQuery q(db);
    q.prepare(R"(
        SELECT EXTRACT(MONTH FROM DATE_TRANSACTION) AS MOIS,
               NVL(SUM(CASE WHEN "TYPE"='Revenu' THEN MONTANT ELSE 0 END),0) AS REVENU,
               NVL(SUM(CASE WHEN "TYPE"='Dépense' THEN MONTANT ELSE 0 END),0) AS DEPENSE
        FROM TRANSACTIONS
        WHERE EXTRACT(YEAR FROM DATE_TRANSACTION)=:y
        GROUP BY EXTRACT(MONTH FROM DATE_TRANSACTION)
        ORDER BY MOIS
    )");
    q.bindValue(":y", year);
    if (q.exec()) {
        while (q.next()) {
            QString mois = q.value("MOIS").toString();
            evo[mois + "_revenu"] = q.value("REVENU");
            evo[mois + "_depense"] = q.value("DEPENSE");
        }
    }
    return evo;
}

double DatabaseManager::getTotalRevenue(int year, int month)
{
    if (!db.isOpen())
        return 0.0;

    QSqlQuery q(db);
    q.prepare(R"(
        SELECT NVL(SUM(MONTANT),0)
        FROM TRANSACTIONS
        WHERE "TYPE"='Revenu'
          AND EXTRACT(YEAR FROM DATE_TRANSACTION)=:y
          AND EXTRACT(MONTH FROM DATE_TRANSACTION)=:m
    )");
    q.bindValue(":y", year);
    q.bindValue(":m", month);
    if (q.exec() && q.next())
        return q.value(0).toDouble();
    return 0.0;
}

double DatabaseManager::getTotalExpenses(int year, int month)
{
    if (!db.isOpen())
        return 0.0;

    QSqlQuery q(db);
    q.prepare(R"(
        SELECT NVL(SUM(MONTANT),0)
        FROM TRANSACTIONS
        WHERE "TYPE"='Dépense'
          AND EXTRACT(YEAR FROM DATE_TRANSACTION)=:y
          AND EXTRACT(MONTH FROM DATE_TRANSACTION)=:m
    )");
    q.bindValue(":y", year);
    q.bindValue(":m", month);
    if (q.exec() && q.next())
        return q.value(0).toDouble();
    return 0.0;
}

//
// ==================== MODULE INCIDENTS ====================
//
bool DatabaseManager::addIncident(const QVariantMap &data)
{
    if (!db.isOpen())
        return false;

    QSqlQuery q(db);
    q.prepare(R"(
        INSERT INTO INCIDENTS (TYPE_INCIDENT, LOCALISATION, DATE_HEURE, NIVEAU, STATUT, CIN_RESIDENT)
        VALUES (:type, :localisation, TO_TIMESTAMP(:date_heure,'YYYY-MM-DD HH24:MI:SS'), :niveau, :statut, :cin)
    )");
    q.bindValue(":type", data.value("type_incident").toString().trimmed());
    q.bindValue(":localisation", data.value("localisation").toString().trimmed());
    q.bindValue(":date_heure", data.value("date_heure").toString().trimmed());
    q.bindValue(":niveau", data.value("niveau").toInt());
    q.bindValue(":statut", data.value("statut").toString().trimmed());
    q.bindValue(":cin", data.value("cin_resident").toString().trimmed());

    if (!q.exec()) {
        qWarning() << "❌ addIncident:" << q.lastError().text();
        QMessageBox::critical(nullptr, "Erreur Oracle",
                              "Impossible d'ajouter l'incident.\n\n" + q.lastError().text());
        return false;
    }
    db.commit();
    return true;
}

bool DatabaseManager::updateIncident(int id, const QVariantMap &data)
{
    if (!db.isOpen())
        return false;

    QSqlQuery q(db);
    q.prepare(R"(
        UPDATE INCIDENTS SET
            TYPE_INCIDENT=:type,
            LOCALISATION=:localisation,
            DATE_HEURE=TO_TIMESTAMP(:date_heure,'YYYY-MM-DD HH24:MI:SS'),
            NIVEAU=:niveau,
            STATUT=:statut,
            CIN_RESIDENT=:cin
        WHERE ID_INCIDENT=:id
    )");
    q.bindValue(":type", data.value("type_incident").toString().trimmed());
    q.bindValue(":localisation", data.value("localisation").toString().trimmed());
    q.bindValue(":date_heure", data.value("date_heure").toString().trimmed());
    q.bindValue(":niveau", data.value("niveau").toInt());
    q.bindValue(":statut", data.value("statut").toString().trimmed());
    q.bindValue(":cin", data.value("cin_resident").toString().trimmed());
    q.bindValue(":id", id);

    if (!q.exec()) {
        qWarning() << "❌ updateIncident:" << q.lastError().text();
        QMessageBox::critical(nullptr, "Erreur Oracle",
                              "Impossible de modifier l'incident.\n\n" + q.lastError().text());
        return false;
    }
    db.commit();
    return true;
}

bool DatabaseManager::deleteIncident(int id)
{
    if (!db.isOpen())
        return false;

    QSqlQuery q(db);
    q.prepare("DELETE FROM INCIDENTS WHERE ID_INCIDENT=:id");
    q.bindValue(":id", id);
    if (!q.exec()) {
        qWarning() << "❌ deleteIncident:" << q.lastError().text();
        QMessageBox::critical(nullptr, "Erreur Oracle",
                              "Impossible de supprimer l'incident.\n\n" + q.lastError().text());
        return false;
    }
    db.commit();
    return true;
}

QList<QVariantMap> DatabaseManager::getAllIncidents()
{
    QList<QVariantMap> list;
    if (!db.isOpen())
        return list;

    QSqlQuery q(db);
    if (!q.exec(R"(
            SELECT ID_INCIDENT, TYPE_INCIDENT, LOCALISATION,
                   TO_CHAR(DATE_HEURE,'YYYY-MM-DD HH24:MI:SS') AS DATE_HEURE,
                   NIVEAU, STATUT, CIN_RESIDENT
            FROM INCIDENTS
            ORDER BY DATE_HEURE DESC
        )")) {
        qWarning() << "❌ getAllIncidents:" << q.lastError().text();
        return list;
    }

    while (q.next()) {
        QVariantMap m;
        m["id"] = q.value("ID_INCIDENT").toInt();
        m["type_incident"] = q.value("TYPE_INCIDENT");
        m["localisation"] = q.value("LOCALISATION");
        m["date_heure"] = q.value("DATE_HEURE");
        m["niveau"] = q.value("NIVEAU");
        m["statut"] = q.value("STATUT");
        m["cin_resident"] = q.value("CIN_RESIDENT");
        list << m;
    }
    return list;
}

QVariantMap DatabaseManager::getIncident(int id)
{
    QVariantMap map;
    if (!db.isOpen())
        return map;

    QSqlQuery q(db);
    q.prepare(R"(
        SELECT ID_INCIDENT, TYPE_INCIDENT, LOCALISATION,
               TO_CHAR(DATE_HEURE,'YYYY-MM-DD HH24:MI:SS') AS DATE_HEURE,
               NIVEAU, STATUT, CIN_RESIDENT
        FROM INCIDENTS
        WHERE ID_INCIDENT=:id
    )");
    q.bindValue(":id", id);
    if (q.exec() && q.next()) {
        map["id"] = q.value("ID_INCIDENT").toInt();
        map["type_incident"] = q.value("TYPE_INCIDENT");
        map["localisation"] = q.value("LOCALISATION");
        map["date_heure"] = q.value("DATE_HEURE");
        map["niveau"] = q.value("NIVEAU");
        map["statut"] = q.value("STATUT");
        map["cin_resident"] = q.value("CIN_RESIDENT");
    }
    return map;
}

QList<QVariantMap> DatabaseManager::searchIncidents(const QString &term)
{
    QList<QVariantMap> list;
    if (!db.isOpen())
        return list;

    QSqlQuery q(db);
    q.prepare(R"(
        SELECT ID_INCIDENT, TYPE_INCIDENT, LOCALISATION,
               TO_CHAR(DATE_HEURE,'YYYY-MM-DD HH24:MI:SS') AS DATE_HEURE,
               NIVEAU, STATUT, CIN_RESIDENT
        FROM INCIDENTS
        WHERE LOWER(TYPE_INCIDENT) LIKE LOWER(:term)
           OR LOWER(LOCALISATION) LIKE LOWER(:term)
           OR LOWER(STATUT) LIKE LOWER(:term)
           OR LOWER(CIN_RESIDENT) LIKE LOWER(:term)
    )");
    q.bindValue(":term", "%" + term + "%");
    if (q.exec()) {
        while (q.next()) {
            QVariantMap m;
            m["id"] = q.value("ID_INCIDENT").toInt();
            m["type_incident"] = q.value("TYPE_INCIDENT");
            m["localisation"] = q.value("LOCALISATION");
            m["date_heure"] = q.value("DATE_HEURE");
            m["niveau"] = q.value("NIVEAU");
            m["statut"] = q.value("STATUT");
            m["cin_resident"] = q.value("CIN_RESIDENT");
            list << m;
        }
    }
    return list;
}

QVariantMap DatabaseManager::getIncidentsStatistics()
{
    QVariantMap stats;
    if (!db.isOpen())
        return stats;

    QSqlQuery q(db);
    if (q.exec("SELECT COUNT(*) FROM INCIDENTS") && q.next())
        stats["total"] = q.value(0).toInt();
    if (q.exec("SELECT COUNT(*) FROM INCIDENTS WHERE NIVEAU >= 3") && q.next())
        stats["critiques"] = q.value(0).toInt();
    if (q.exec("SELECT COUNT(*) FROM INCIDENTS WHERE LOWER(STATUT) = 'signalé'") && q.next())
        stats["signales"] = q.value(0).toInt();
    if (q.exec("SELECT COUNT(*) FROM INCIDENTS WHERE LOWER(STATUT) = 'résolu' OR LOWER(STATUT) = 'resolu'") && q.next())
        stats["resolus"] = q.value(0).toInt();
    return stats;
}

//
// ==================== MODULE VÉHICULES (Parking) ====================
//


bool DatabaseManager::addLocal(const QVariantMap &data)
{
    if (!db.isOpen())
        return false;

    QSqlQuery q(db);
    q.prepare(R"(
        INSERT INTO LOCAUX (ID_L, NOM, ADRESSE, NUM_LOCAUX, SUPERFICIE, "TYPE", STATUT)
        VALUES (:id_l, :nom, :adresse, :numero, :superficie, :type, :statut)
    )");
    q.bindValue(":id_l", data.value("id_l").toString().trimmed());
    q.bindValue(":nom", data.value("nom").toString().trimmed());
    q.bindValue(":adresse", data.value("adresse").toString().trimmed());
    q.bindValue(":numero", data.value("num_locaux").toString().trimmed());
    q.bindValue(":superficie", data.value("superficie").toInt());
    q.bindValue(":type", data.value("type").toString().trimmed());
    q.bindValue(":statut", data.value("statut").toString().trimmed());

    if (!q.exec()) {
        qWarning() << "❌ addLocal:" << q.lastError().text();
        QMessageBox::critical(nullptr, "Erreur Oracle",
                              "Impossible d'ajouter le local.\n\n" + q.lastError().text());
        return false;
    }
    db.commit();
    return true;
}

bool DatabaseManager::updateLocal(const QString &id, const QVariantMap &data)
{
    if (!db.isOpen())
        return false;

    QSqlQuery q(db);
    q.prepare(R"(
        UPDATE LOCAUX SET
            NOM=:nom,
            ADRESSE=:adresse,
            NUM_LOCAUX=:numero,
            SUPERFICIE=:superficie,
            "TYPE"=:type,
            STATUT=:statut
        WHERE ID_L=:id_l
    )");
    q.bindValue(":nom", data.value("nom").toString().trimmed());
    q.bindValue(":adresse", data.value("adresse").toString().trimmed());
    q.bindValue(":numero", data.value("num_locaux").toString().trimmed());
    q.bindValue(":superficie", data.value("superficie").toInt());
    q.bindValue(":type", data.value("type").toString().trimmed());
    q.bindValue(":statut", data.value("statut").toString().trimmed());
    q.bindValue(":id_l", id);

    if (!q.exec()) {
        qWarning() << "❌ updateLocal:" << q.lastError().text();
        QMessageBox::critical(nullptr, "Erreur Oracle",
                              "Impossible de modifier le local.\n\n" + q.lastError().text());
        return false;
    }
    db.commit();
    return true;
}

bool DatabaseManager::deleteLocal(const QString &id)
{
    if (!db.isOpen())
        return false;

    QSqlQuery q(db);
    q.prepare("DELETE FROM LOCAUX WHERE ID_L=:id_l");
    q.bindValue(":id_l", id);
    if (!q.exec()) {
        qWarning() << "❌ deleteLocal:" << q.lastError().text();
        QMessageBox::critical(nullptr, "Erreur Oracle",
                              "Impossible de supprimer le local.\n\n" + q.lastError().text());
        return false;
    }
    db.commit();
    return true;
}

QList<QVariantMap> DatabaseManager::getAllLocaux()
{
    QList<QVariantMap> list;
    if (!db.isOpen())
        return list;

    QSqlQuery q(db);
    if (!q.exec(R"(
            SELECT ID_L, NOM, ADRESSE, NUM_LOCAUX, SUPERFICIE, "TYPE", STATUT
            FROM LOCAUX
            ORDER BY NOM
        )")) {
        qWarning() << "❌ getAllLocaux:" << q.lastError().text();
        return list;
    }

    while (q.next()) {
        QVariantMap m;
        m["id_l"] = q.value("ID_L");
        m["nom"] = q.value("NOM");
        m["adresse"] = q.value("ADRESSE");
        m["num_locaux"] = q.value("NUM_LOCAUX");
        m["superficie"] = q.value("SUPERFICIE");
        m["type"] = q.value("TYPE");
        m["statut"] = q.value("STATUT");
        list << m;
    }
    return list;
}

QVariantMap DatabaseManager::getLocal(const QString &id)
{
    QVariantMap map;
    if (!db.isOpen())
        return map;

    QSqlQuery q(db);
    q.prepare(R"(
        SELECT ID_L, NOM, ADRESSE, NUM_LOCAUX, SUPERFICIE, "TYPE", STATUT
        FROM LOCAUX
        WHERE ID_L=:id_l
    )");
    q.bindValue(":id_l", id);
    if (q.exec() && q.next()) {
        map["id_l"] = q.value("ID_L");
        map["nom"] = q.value("NOM");
        map["adresse"] = q.value("ADRESSE");
        map["num_locaux"] = q.value("NUM_LOCAUX");
        map["superficie"] = q.value("SUPERFICIE");
        map["type"] = q.value("TYPE");
        map["statut"] = q.value("STATUT");
    }
    return map;
}

QList<QVariantMap> DatabaseManager::searchLocaux(const QString &term)
{
    QList<QVariantMap> list;
    if (!db.isOpen())
        return list;

    QSqlQuery q(db);
    q.prepare(R"(
        SELECT ID_L, NOM, ADRESSE, NUM_LOCAUX, SUPERFICIE, "TYPE", STATUT
        FROM LOCAUX
        WHERE LOWER(ID_L) LIKE LOWER(:term)
           OR LOWER(NOM) LIKE LOWER(:term)
           OR LOWER("TYPE") LIKE LOWER(:term)
           OR LOWER(ADRESSE) LIKE LOWER(:term)
    )");
    q.bindValue(":term", "%" + term + "%");
    if (q.exec()) {
        while (q.next()) {
            QVariantMap m;
            m["id_l"] = q.value("ID_L");
            m["nom"] = q.value("NOM");
            m["adresse"] = q.value("ADRESSE");
            m["num_locaux"] = q.value("NUM_LOCAUX");
            m["superficie"] = q.value("SUPERFICIE");
            m["type"] = q.value("TYPE");
            m["statut"] = q.value("STATUT");
            list << m;
        }
    }
    return list;
}

//
// ==================== MODULE TRANSPORT ====================
//
bool DatabaseManager::addTransportVehicule(const QVariantMap &data)
{
    if (!db.isOpen()) {
        qDebug() << "❌ Base de données non connectée";
        QMessageBox::critical(nullptr, "Erreur", "Connexion à la base de données fermée.");
        return false;
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO TRANSPORT_VEHICULES (id_vehicule, type, capacite, zone, horaire, statut, date_ajout, temps_utilise, code_unique) "
                  "VALUES (:id_vehicule, :type, :capacite, :zone, :horaire, :statut, TO_DATE(:date_ajout, 'YYYY-MM-DD'), :temps_utilise, :code_unique)");

    QString id = data.value("id_vehicule").toString().toUpper();
    QString type = data.value("type").toString();
    int capacite = data.value("capacite").toInt();
    QString zone = data.value("zone").toString();
    QString horaire = data.value("horaire").toString();
    QString statut = data.value("statut").toString();
    QString date_ajout = data.value("date_ajout").toString();
    double temps_utilise = data.value("temps_utilise").toDouble();
    QString code_unique = data.value("code_unique").toString().trimmed();

    query.bindValue(":id_vehicule", id);
    query.bindValue(":type", type);
    query.bindValue(":capacite", capacite);
    query.bindValue(":zone", zone);
    query.bindValue(":horaire", horaire);
    query.bindValue(":statut", statut);
    query.bindValue(":date_ajout", date_ajout);
    query.bindValue(":temps_utilise", temps_utilise);

    // Gérer code_unique : NULL si vide, sinon vérifier qu'il existe dans TRANSACTIONS (finance)
    if (code_unique.isEmpty()) {
        query.bindValue(":code_unique", QVariant());
    } else {
        // Vérifier que la transaction existe dans TRANSACTIONS
        QSqlQuery checkQuery(db);
        checkQuery.prepare("SELECT COUNT(*) FROM TRANSACTIONS WHERE CODE_UNIQUE = :code");
        checkQuery.bindValue(":code", code_unique);
        if (checkQuery.exec() && checkQuery.next() && checkQuery.value(0).toInt() == 0) {
            QMessageBox::warning(nullptr, "Erreur de validation",
                                 QString("Le code transaction '%1' n'existe pas dans la base de données.\n"
                                         "Veuillez d'abord créer la transaction ou laisser ce champ vide.").arg(code_unique));
            return false;
        }
        query.bindValue(":code_unique", code_unique);
    }

    qDebug() << "🔍 Exécution INSERT Transport avec valeurs:";
    qDebug() << " ID:" << id;
    qDebug() << " Type:" << type;
    qDebug() << " Capacité:" << capacite;
    qDebug() << " Zone:" << zone;
    qDebug() << " Horaire:" << horaire;
    qDebug() << " Statut:" << statut;
    qDebug() << " Date:" << date_ajout;
    qDebug() << " Temps utilisé:" << temps_utilise;
    qDebug() << " Code unique:" << (code_unique.isEmpty() ? "NULL" : code_unique);

    if (!query.exec()) {
        QString errorMsg = query.lastError().text();
        qDebug() << "❌ Erreur SQL lors de l'insertion:" << errorMsg;
        qDebug() << "❌ Requête:" << query.lastQuery();

        // Vérifier si c'est une erreur de contrainte de clé étrangère
        QString userMessage;
        if (errorMsg.contains("FK_TRANSPORT_LOCAUX") || errorMsg.contains("ORA-02291")) {
            if (code_unique.isEmpty()) {
                userMessage = "Erreur de contrainte de clé étrangère.\n"
                              "Le code transaction doit exister dans la table TRANSACTIONS (finance).\n"
                              "Veuillez créer la transaction d'abord ou laisser le champ vide.";
            } else {
                userMessage = QString("Le code transaction '%1' n'existe pas dans la table TRANSACTIONS (finance).\n"
                                      "Veuillez créer la transaction d'abord ou laisser le champ vide.").arg(code_unique);
            }
        } else {
            userMessage = QString("Impossible d'ajouter le véhicule de transport.\n\nDétails: %1").arg(errorMsg);
        }

        QMessageBox::critical(nullptr, "Erreur Oracle", userMessage);
        return false;
    }

    qDebug() << "✅ Véhicule de transport ajouté avec succès dans la base de données";
    db.commit();

    emit vehiculeAdded();
    emit dataChanged();

    return true;
}

bool DatabaseManager::updateTransportVehicule(const QString &id, const QVariantMap &data)
{
    if (!db.isOpen()) {
        QMessageBox::critical(nullptr, "Erreur", "Connexion à la base de données fermée.");
        return false;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE TRANSPORT_VEHICULES SET type = :type, capacite = :capacite, zone = :zone, horaire = :horaire, "
                  "statut = :statut, date_ajout = TO_DATE(:date_ajout, 'YYYY-MM-DD'), temps_utilise = :temps_utilise, "
                  "code_unique = :code_unique WHERE id_vehicule = :id");

    query.bindValue(":type", data.value("type"));
    query.bindValue(":capacite", data.value("capacite").toInt());
    query.bindValue(":zone", data.value("zone"));
    query.bindValue(":horaire", data.value("horaire"));
    query.bindValue(":statut", data.value("statut"));
    query.bindValue(":date_ajout", data.value("date_ajout"));
    query.bindValue(":temps_utilise", data.value("temps_utilise").toDouble());

    QString code_unique = data.value("code_unique").toString().trimmed();
    if (code_unique.isEmpty()) {
        query.bindValue(":code_unique", QVariant());
    } else {
        // Vérifier que la transaction existe dans TRANSACTIONS
        QSqlQuery checkQuery(db);
        checkQuery.prepare("SELECT COUNT(*) FROM TRANSACTIONS WHERE CODE_UNIQUE = :code");
        checkQuery.bindValue(":code", code_unique);
        if (checkQuery.exec() && checkQuery.next() && checkQuery.value(0).toInt() == 0) {
            QMessageBox::warning(nullptr, "Erreur de validation",
                                 QString("Le code transaction '%1' n'existe pas dans la base de données.\n"
                                         "Veuillez d'abord créer la transaction ou laisser ce champ vide.").arg(code_unique));
            return false;
        }
        query.bindValue(":code_unique", code_unique);
    }

    query.bindValue(":id", id.toUpper());

    if (!query.exec()) {
        QString errorMsg = query.lastError().text();
        qDebug() << "❌ Erreur mise à jour transport:" << errorMsg;

        // Vérifier si c'est une erreur de contrainte de clé étrangère
        QString userMessage;
        if (errorMsg.contains("FK_TRANSPORT_LOCAUX") || errorMsg.contains("ORA-02291")) {
            if (code_unique.isEmpty()) {
                userMessage = "Erreur de contrainte de clé étrangère.\n"
                              "Le code transaction doit exister dans la table TRANSACTIONS (finance).\n"
                              "Veuillez créer la transaction d'abord ou laisser le champ vide.";
            } else {
                userMessage = QString("Le code transaction '%1' n'existe pas dans la table TRANSACTIONS (finance).\n"
                                      "Veuillez créer la transaction d'abord ou laisser le champ vide.").arg(code_unique);
            }
        } else {
            userMessage = QString("Impossible de modifier le véhicule de transport.\n\nDétails: %1").arg(errorMsg);
        }

        QMessageBox::critical(nullptr, "Erreur Oracle", userMessage);
        return false;
    }

    db.commit();
    emit vehiculeUpdated();
    emit dataChanged();

    return true;
}

bool DatabaseManager::deleteTransportVehicule(const QString &id)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM TRANSPORT_VEHICULES WHERE id_vehicule = :id");
    query.bindValue(":id", id.toUpper());

    if (!query.exec()) {
        qDebug() << "❌ Erreur suppression transport:" << query.lastError().text();
        return false;
    }

    db.commit();
    emit vehiculeDeleted();
    emit dataChanged();

    return true;
}

QList<QVariantMap> DatabaseManager::getAllTransportVehicules()
{
    QList<QVariantMap> list;

    if (!db.isOpen()) {
        qDebug() << "❌❌❌ BASE DÉCONNECTÉE dans getAllTransportVehicules()";
        return list;
    }

    qDebug() << "🔍 Exécution requête SELECT * FROM TRANSPORT_VEHICULES";

    QSqlQuery query(db);
    query.setForwardOnly(true);

    if (!query.exec("SELECT id_vehicule, type, capacite, zone, horaire, statut, "
                    "TO_CHAR(date_ajout, 'YYYY-MM-DD') as date_ajout, "
                    "temps_utilise, code_unique FROM TRANSPORT_VEHICULES ORDER BY id_vehicule")) {
        qDebug() << "❌ ERREUR SQL:" << query.lastError().text();
        qDebug() << "❌ Requête:" << query.lastQuery();
        return list;
    }

    int count = 0;
    while (query.next()) {
        QVariantMap map;
        map["id_vehicule"] = query.value("id_vehicule");
        map["type"] = query.value("type");
        map["capacite"] = query.value("capacite");
        map["zone"] = query.value("zone");
        map["horaire"] = query.value("horaire");
        map["statut"] = query.value("statut");
        map["date_ajout"] = query.value("date_ajout");
        map["temps_utilise"] = query.value("temps_utilise");
        map["code_unique"] = query.value("code_unique");

        list.append(map);
        count++;

        qDebug() << " 📖 Véhicule transport lu:" << map["id_vehicule"].toString()
                 << "- Type:" << map["type"].toString()
                 << "- Zone:" << map["zone"].toString();
    }

    qDebug() << "✅" << count << "véhicules de transport lus depuis la base";
    return list;
}

QVariantMap DatabaseManager::getTransportVehicule(const QString &id)
{
    QVariantMap map;
    QSqlQuery query(db);
    query.prepare("SELECT id_vehicule, type, capacite, zone, horaire, statut, "
                  "TO_CHAR(date_ajout, 'YYYY-MM-DD') as date_ajout, "
                  "temps_utilise, code_unique FROM TRANSPORT_VEHICULES WHERE id_vehicule = :id");
    query.bindValue(":id", id.toUpper());

    if (query.exec() && query.next()) {
        map["id_vehicule"] = query.value("id_vehicule");
        map["type"] = query.value("type");
        map["capacite"] = query.value("capacite");
        map["zone"] = query.value("zone");
        map["horaire"] = query.value("horaire");
        map["statut"] = query.value("statut");
        map["date_ajout"] = query.value("date_ajout");
        map["temps_utilise"] = query.value("temps_utilise");
        map["code_unique"] = query.value("code_unique");
    }

    return map;
}
QList<QVariantMap> DatabaseManager::getCodesUniquesFinance()
{
    QList<QVariantMap> codes;

    if (!database().isOpen()) {
        qDebug() << "❌ Base non connectée pour récupérer les codes transaction";
        return codes;
    }

    QSqlQuery query(database());

    // Récupère depuis la table 'transaction'
    QString requete = "SELECT DISTINCT code_unique FROM transaction WHERE code_unique IS NOT NULL AND code_unique != '' ORDER BY code_unique";

    if (!query.exec(requete)) {
        qDebug() << "❌ Erreur lors de la récupération des codes transaction:" << query.lastError().text();
        qDebug() << "📋 Requête:" << requete;
        return codes;
    }

    while (query.next()) {
        QVariantMap code;
        code["code_unique"] = query.value("code_unique").toString();
        codes.append(code);
    }

    qDebug() << "📋" << codes.size() << "codes uniques récupérés de la table transaction";
    return codes;
}
QList<QVariantMap> DatabaseManager::searchTransportVehicules(const QString &term)
{
    QList<QVariantMap> list;
    QSqlQuery query(db);
    query.prepare("SELECT id_vehicule, type, capacite, zone, horaire, statut, "
                  "TO_CHAR(date_ajout, 'YYYY-MM-DD') as date_ajout, "
                  "temps_utilise, code_unique FROM TRANSPORT_VEHICULES WHERE id_vehicule LIKE :term OR type LIKE :term OR zone LIKE :term");
    query.bindValue(":term", "%" + term + "%");

    if (query.exec()) {
        while (query.next()) {
            QVariantMap map;
            map["id_vehicule"] = query.value("id_vehicule");
            map["type"] = query.value("type");
            map["capacite"] = query.value("capacite");
            map["zone"] = query.value("zone");
            map["horaire"] = query.value("horaire");
            map["statut"] = query.value("statut");
            map["date_ajout"] = query.value("date_ajout");
            map["temps_utilise"] = query.value("temps_utilise");
            map["code_unique"] = query.value("code_unique");
            list.append(map);
        }
    }

    return list;
}

QVariantMap DatabaseManager::getTransportStatistics()
{
    QVariantMap stats;
    QSqlQuery query(db);

    if (query.exec("SELECT COUNT(*) FROM TRANSPORT_VEHICULES") && query.next()) {
        stats["total"] = query.value(0).toInt();
    }

    if (query.exec("SELECT type, COUNT(*) FROM TRANSPORT_VEHICULES GROUP BY type")) {
        while (query.next()) {
            stats["count_" + query.value(0).toString()] = query.value(1).toInt();
        }
    }

    if (query.exec("SELECT AVG(temps_utilise) FROM TRANSPORT_VEHICULES") && query.next()) {
        stats["avg_temps"] = query.value(0).toDouble();
    }

    return stats;
}
