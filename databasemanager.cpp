#include "databasemanager.h"
#include "connection.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
#include <QDebug>
#include <QMessageBox>
#include <QDate>

/*--------------------------------------------------
 *  CONSTRUCTEUR / ACCÈS BDD
 *-------------------------------------------------*/

DatabaseManager::DatabaseManager(QObject *parent)
    : QObject(parent)
{
    // Récupère l'unique connexion Oracle (singleton)
    if (!Connection::getInstance().isOpen()) {
        Connection::getInstance().createConnection();
    }

    db = Connection::getInstance().getDatabase();

    if (db.isOpen()) {
        qInfo() << "✅ DatabaseManager initialisé : connexion Oracle active.";
    } else {
        qWarning() << "❌ DatabaseManager : impossible d'ouvrir Oracle.";
        QMessageBox::critical(nullptr, "Erreur Oracle",
                              "Connexion Oracle non établie.\n"
                              "Vérifie le DSN OracleXE dans ODBC.");
    }
}

QSqlDatabase DatabaseManager::database() const
{
    return db;
}

/*==================================================
 *  MODULE RESIDENTS
 *=================================================*/
QList<int> DatabaseManager::getMonthlySatisfactionScores()
{
    QList<int> scores;
    scores.reserve(12);

    // Récupérer tous les résidents
    QList<QVariantMap> residents = getAllResidents();
    if (residents.isEmpty()) {
        for (int i = 0; i < 12; ++i)
            scores << 80;
        return scores;
    }

    // Pour chaque mois M-11 ... M
    for (int m = 11; m >= 0; m--) {

        int totalScore = 0;

        for (const QVariantMap &r : residents) {

            int score = 80;

            QString cin = r.value("cin").toString();
            QString statut = r.value("statut").toString().toLower();
            QDate dateEntree = QDate::fromString(r.value("date_entree").toString(), "yyyy-MM-dd");

            // ID du résident
            int residentId = getResidentIdFromCin(cin).toInt();

            // Nombre d'incidents dans CE mois
            QSqlQuery q(db);
            q.prepare(R"(
                SELECT COUNT(*)
                FROM INCIDENTS
                WHERE ID_RESIDENT = :id
                AND DATE_HEURE >= ADD_MONTHS(TRUNC(SYSDATE), :offset)
                AND DATE_HEURE <  ADD_MONTHS(TRUNC(SYSDATE), :offset + 1)
            )");
            q.bindValue(":id", residentId);
            q.bindValue(":offset", -m);

            int incidents = 0;
            if (q.exec() && q.next())
                incidents = q.value(0).toInt();

            // 📌 Règles du score :
            if (statut == "propriétaire")
                score += 5;

            score -= incidents * 5;

            // Bonus si aucun incident dans les 6 derniers mois
            if (getIncidentsCount(residentId, 6) == 0)
                score += 8;

            // Fidélité
            if (dateEntree.isValid()) {
                int years = dateEntree.daysTo(QDate::currentDate()) / 365;
                if (years >= 3)
                    score += 6;
            }

            // Clamp
            if (score < 0) score = 0;
            if (score > 100) score = 100;

            totalScore += score;
        }

        // Score moyen du mois
        int avg = qRound(double(totalScore) / residents.size());
        scores << avg;
    }

    return scores;
}

bool DatabaseManager::addResident(const QVariantMap &data)
{
    if (!db.isOpen()) {
        QMessageBox::warning(nullptr, "Erreur", "Connexion Oracle fermée.");
        return false;
    }

    QSqlQuery q(db);
    q.prepare(R"(
        INSERT INTO RESIDENTS
        (CIN, NOM, PRENOM, SEXE, TELEPHONE, EMAIL,
         DATE_ENTREE, APPARTEMENT, ETAGE, STATUT)
        VALUES (:cin, :nom, :prenom, :sexe, :telephone, :email,
                NVL(TO_DATE(:date_entree, 'YYYY-MM-DD'), SYSDATE),
                :appartement, :etage, :statut)
    )");

    QString sexe = data.value("sexe").toString().trimmed().toLower();
    if (sexe == "homme")
        sexe = "Homme";
    else if (sexe == "femme")
        sexe = "Femme";
    else
        sexe = "Homme";

    q.bindValue(":cin", data.value("cin"));
    q.bindValue(":nom", data.value("nom"));
    q.bindValue(":prenom", data.value("prenom"));
    q.bindValue(":sexe", sexe);
    q.bindValue(":telephone", data.value("telephone"));
    q.bindValue(":email", data.value("email"));
    q.bindValue(":date_entree", data.value("date_entree"));
    q.bindValue(":appartement", data.value("appartement"));
    q.bindValue(":etage", data.value("etage"));
    q.bindValue(":statut", data.value("statut"));

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
            SEXE=:sexe,
            TELEPHONE=:telephone,
            EMAIL=:email,
            DATE_ENTREE=TO_DATE(:date_entree, 'YYYY-MM-DD'),
            APPARTEMENT=:appartement,
            ETAGE=:etage,
            STATUT=:statut
        WHERE CIN=:cin
    )");

    QString sexe = data.value("sexe").toString().trimmed().toLower();
    if (sexe == "homme")
        sexe = "Homme";
    else if (sexe == "femme")
        sexe = "Femme";
    else
        sexe = "Homme";

    q.bindValue(":nom", data.value("nom"));
    q.bindValue(":prenom", data.value("prenom"));
    q.bindValue(":sexe", sexe);
    q.bindValue(":telephone", data.value("telephone"));
    q.bindValue(":email", data.value("email"));
    q.bindValue(":date_entree", data.value("date_entree").toString());
    q.bindValue(":appartement", data.value("appartement"));
    q.bindValue(":etage", data.value("etage"));
    q.bindValue(":statut", data.value("statut"));
    q.bindValue(":cin", cin);

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
        SELECT
            ID,
            CIN, NOM, PRENOM, SEXE, TELEPHONE, EMAIL,
            TO_CHAR(DATE_ENTREE,'YYYY-MM-DD') AS DATE_ENTREE,
            APPARTEMENT, ETAGE, STATUT,
            INCIDENT_COUNT_12M,
            CONTRACT_RENEWALS,
            SATISFACTION_SCORE,
            TO_CHAR(LAST_ACTIVITY_DATE,'YYYY-MM-DD') AS LAST_ACTIVITY_DATE
        FROM RESIDENTS
        ORDER BY DATE_ENTREE DESC
    )");

    while (q.next()) {
        QVariantMap m;
        m["id"]                  = q.value("ID");
        m["cin"]                 = q.value("CIN");
        m["nom"]                 = q.value("NOM");
        m["prenom"]              = q.value("PRENOM");
        m["sexe"]                = q.value("SEXE");
        m["telephone"]           = q.value("TELEPHONE");
        m["email"]               = q.value("EMAIL");
        m["date_entree"]         = q.value("DATE_ENTREE");
        m["appartement"]         = q.value("APPARTEMENT");
        m["etage"]               = q.value("ETAGE");
        m["statut"]              = q.value("STATUT");

        // New SQL columns
        m["incident_count_12m"]  = q.value("INCIDENT_COUNT_12M");
        m["contract_renewals"]   = q.value("CONTRACT_RENEWALS");
        m["satisfaction_score"]  = q.value("SATISFACTION_SCORE");
        m["last_activity_date"]  = q.value("LAST_ACTIVITY_DATE");

        list << m;
    }
    return list;
}


QString DatabaseManager::getResidentIdFromCin(const QString &cin)
{
    if (!db.isOpen())
        return QString();

    QSqlQuery q(db);
    q.prepare("SELECT ID FROM RESIDENTS WHERE CIN = :cin");
    q.bindValue(":cin", cin);

    if (q.exec() && q.next())
        return q.value(0).toString();

    return QString();
}

QVariantMap DatabaseManager::getResident(const QString &cin)
{
    QVariantMap m;
    
    if (!db.isOpen()) {
        qWarning() << "❌ Database connection is not open";
        return m;
    }
    
    // Quick connection test
    QSqlQuery testQuery(db);
    testQuery.prepare("SELECT 1 FROM DUAL");
    if (!testQuery.exec()) {
        qWarning() << "❌ Database connection test failed:" << testQuery.lastError().text();
        return m;
    }
    
    QSqlQuery q(db);

    q.prepare(R"(
        SELECT
            ID,
            CIN, NOM, PRENOM, SEXE, TELEPHONE, EMAIL,
            TO_CHAR(DATE_ENTREE,'YYYY-MM-DD') AS DATE_ENTREE,
            APPARTEMENT, ETAGE, STATUT,
            INCIDENT_COUNT_12M,
            CONTRACT_RENEWALS,
            SATISFACTION_SCORE,
            TO_CHAR(LAST_ACTIVITY_DATE,'YYYY-MM-DD') AS LAST_ACTIVITY_DATE
        FROM RESIDENTS
        WHERE CIN = :cin
    )");

    q.bindValue(":cin", cin);

    if (!q.exec()) {
        qWarning() << "❌ SQL Error in getResident:" << q.lastError().text();
        return m;
    }
    
    if (q.next()) {
        m["id"]                  = q.value("ID");
        m["cin"]                 = q.value("CIN");
        m["nom"]                 = q.value("NOM");
        m["prenom"]              = q.value("PRENOM");
        m["sexe"]                = q.value("SEXE");
        m["telephone"]           = q.value("TELEPHONE");
        m["email"]               = q.value("EMAIL");
        m["date_entree"]         = q.value("DATE_ENTREE");
        m["appartement"]         = q.value("APPARTEMENT");
        m["etage"]               = q.value("ETAGE");
        m["statut"]              = q.value("STATUT");

        // New SQL columns
        m["incident_count_12m"]  = q.value("INCIDENT_COUNT_12M");
        m["contract_renewals"]   = q.value("CONTRACT_RENEWALS");
        m["satisfaction_score"]  = q.value("SATISFACTION_SCORE");
        m["last_activity_date"]  = q.value("LAST_ACTIVITY_DATE");
    }

    return m;
}

QList<QVariantMap> DatabaseManager::searchResidents(const QString &term)
{
    QList<QVariantMap> list;
    QSqlQuery q(db);
    q.prepare(R"(
        SELECT CIN, NOM, PRENOM, APPARTEMENT
        FROM RESIDENTS
        WHERE LOWER(NOM) LIKE LOWER(:t)
           OR LOWER(PRENOM) LIKE LOWER(:t)
           OR LOWER(CIN) LIKE LOWER(:t)
    )");
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
    QSqlQuery q(db);
    q.exec("SELECT COUNT(*) AS TOTAL FROM RESIDENTS");
    if (q.next())
        stats["total"] = q.value("TOTAL").toInt();
    return stats;
}

/*==================================================
 *  MODULE TRANSACTIONS (Finance)
 *=================================================*/

static QString normalizeDateVariant(const QVariant &value)
{
    if (value.canConvert<QDate>())
        return value.toDate().toString("yyyy-MM-dd");

    QString str = value.toString().trimmed();
    if (str.isEmpty())
        return QDate::currentDate().toString("yyyy-MM-dd");

    QDate parsed = QDate::fromString(str, "yyyy-MM-dd");
    if (!parsed.isValid())
        parsed = QDate::fromString(str, "dd/MM/yyyy");

    return parsed.isValid()
               ? parsed.toString("yyyy-MM-dd")
               : QDate::currentDate().toString("yyyy-MM-dd");
}

bool DatabaseManager::addTransaction(const QVariantMap &data)
{
    if (!db.isOpen()) {
        QMessageBox::warning(nullptr, "Erreur", "Connexion Oracle fermée.");
        return false;
    }

    QString code        = data.value("code_unique").toString().trimmed();
    QString type        = data.value("type").toString().trimmed();
    QString categorie   = data.value("categorie").toString().trimmed();
    QString description = data.value("description").toString().trimmed();
    double  montant     = data.value("montant").toDouble();
    QString date        = normalizeDateVariant(data.value("date_transaction"));
    QString cin         = data.value("cin_resident").toString().trimmed();

    qDebug() << "[addTransaction] code_unique:" << code;
    qDebug() << "[addTransaction] montant (double):" << montant << "Type:" << data.value("montant").typeName();
    qDebug() << "[addTransaction] type:" << type;
    qDebug() << "[addTransaction] categorie:" << categorie;
    qDebug() << "[addTransaction] date:" << date;
    qDebug() << "[addTransaction] cin:" << cin;

    if (code.isEmpty() || type.isEmpty() || date.isEmpty()) {
        QMessageBox::warning(nullptr, "Validation",
                             "Code, type et date de transaction sont obligatoires.");
        return false;
    }

    // CIN -> ID_RESIDENT (ou NULL)
    QVariant idResident;
    if (!cin.isEmpty()) {
        QString id = getResidentIdFromCin(cin);
        if (id.isEmpty()) {
            QMessageBox::warning(nullptr, "Erreur",
                                 QString("Le CIN %1 n'existe pas dans RESIDENTS.").arg(cin));
            return false;
        }
        idResident = id;
    } else {
        idResident = QVariant(QVariant::String);  // Explicit NULL for Oracle
    }

    QSqlQuery q(db);
    q.prepare(R"(
        INSERT INTO TRANSACTIONS
        (CODE_UNIQUE, MONTANT, "TYPE", CATEGORIE, DESCRIPTION,
         DATE_TRANSACTION, ID_RESIDENT)
        VALUES (:code, :montant, :type, :categorie, :description,
                TO_DATE(:date, 'YYYY-MM-DD'), :idResident)
    )");

    q.bindValue(":code",        code);
    q.bindValue(":montant",     montant);
    q.bindValue(":type",        type);
    q.bindValue(":categorie",   categorie);
    q.bindValue(":description", description);
    q.bindValue(":date",        date);
    q.bindValue(":idResident",  idResident);

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
        SELECT  t.CODE_UNIQUE,
                t.MONTANT,
                t."TYPE",
                t.CATEGORIE,
                t.DESCRIPTION,
                TO_CHAR(t.DATE_TRANSACTION,'YYYY-MM-DD') AS DATE_TRANSACTION,
                r.CIN AS CIN_RESIDENT
        FROM TRANSACTIONS t
        LEFT JOIN RESIDENTS r ON t.ID_RESIDENT = r.ID
        ORDER BY t.DATE_TRANSACTION DESC
    )")) {
        qWarning() << "❌ getAllTransactions:" << q.lastError().text();
        return list;
    }

    while (q.next()) {
        QVariantMap m;
        m["code_unique"]      = q.value("CODE_UNIQUE");
        m["montant"]          = q.value("MONTANT");
        m["type"]             = q.value("TYPE");
        m["categorie"]        = q.value("CATEGORIE");
        m["description"]      = q.value("DESCRIPTION");
        m["date_transaction"] = q.value("DATE_TRANSACTION");
        m["cin_resident"]     = q.value("CIN_RESIDENT");
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
        SELECT  t.CODE_UNIQUE,
                t.MONTANT,
                t."TYPE",
                t.CATEGORIE,
                t.DESCRIPTION,
                TO_CHAR(t.DATE_TRANSACTION,'YYYY-MM-DD') AS DATE_TRANSACTION,
                r.CIN AS CIN_RESIDENT
        FROM TRANSACTIONS t
        LEFT JOIN RESIDENTS r ON t.ID_RESIDENT = r.ID
        WHERE t.CODE_UNIQUE = :code
    )");
    q.bindValue(":code", code);

    if (q.exec() && q.next()) {
        m["code_unique"]      = q.value("CODE_UNIQUE");
        m["montant"]          = q.value("MONTANT");
        m["type"]             = q.value("TYPE");
        m["categorie"]        = q.value("CATEGORIE");
        m["description"]      = q.value("DESCRIPTION");
        m["date_transaction"] = q.value("DATE_TRANSACTION");
        m["cin_resident"]     = q.value("CIN_RESIDENT");
    }
    return m;
}

bool DatabaseManager::updateTransaction(const QString &code, const QVariantMap &data)
{
    if (!db.isOpen())
        return false;

    QString type        = data.value("type").toString().trimmed();
    QString categorie   = data.value("categorie").toString().trimmed();
    QString description = data.value("description").toString().trimmed();
    double  montant     = data.value("montant").toDouble();
    QString date        = normalizeDateVariant(
        data.value("date_transaction", data.value("date")));
    QString cin         = data.value("cin_resident").toString().trimmed();

    QVariant idResident;
    if (!cin.isEmpty()) {
        QString id = getResidentIdFromCin(cin);
        if (id.isEmpty()) {
            QMessageBox::warning(nullptr, "Erreur",
                                 QString("Le CIN %1 n'existe pas dans RESIDENTS.").arg(cin));
            return false;
        }
        idResident = id;
    }

    QSqlQuery q(db);
    q.prepare(R"(
        UPDATE TRANSACTIONS SET
            MONTANT        = :montant,
            "TYPE"         = :type,
            CATEGORIE      = :categorie,
            DESCRIPTION    = :description,
            DATE_TRANSACTION = TO_DATE(:date,'YYYY-MM-DD'),
            ID_RESIDENT    = :idResident
        WHERE CODE_UNIQUE = :code
    )");

    q.bindValue(":montant",    montant);
    q.bindValue(":type",       type);
    q.bindValue(":categorie",  categorie);
    q.bindValue(":description",description);
    q.bindValue(":date",       date);
    q.bindValue(":idResident", idResident);
    q.bindValue(":code",       code);

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
        SELECT  t.CODE_UNIQUE,
                t.MONTANT,
                t."TYPE",
                t.CATEGORIE,
                t.DESCRIPTION,
                TO_CHAR(t.DATE_TRANSACTION,'YYYY-MM-DD') AS DATE_TRANSACTION,
                r.CIN AS CIN_RESIDENT
        FROM TRANSACTIONS t
        LEFT JOIN RESIDENTS r ON t.ID_RESIDENT = r.ID
        WHERE  LOWER(t.CODE_UNIQUE)  LIKE LOWER(:term)
            OR LOWER(t."TYPE")       LIKE LOWER(:term)
            OR LOWER(t.CATEGORIE)    LIKE LOWER(:term)
            OR LOWER(t.DESCRIPTION)  LIKE LOWER(:term)
            OR LOWER(r.CIN)          LIKE LOWER(:term)
    )");
    q.bindValue(":term", "%" + term + "%");

    if (q.exec()) {
        while (q.next()) {
            QVariantMap m;
            m["code_unique"]      = q.value("CODE_UNIQUE");
            m["montant"]          = q.value("MONTANT");
            m["type"]             = q.value("TYPE");
            m["categorie"]        = q.value("CATEGORIE");
            m["description"]      = q.value("DESCRIPTION");
            m["date_transaction"] = q.value("DATE_TRANSACTION");
            m["cin_resident"]     = q.value("CIN_RESIDENT");
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
        SELECT NVL(SUM(CASE WHEN "TYPE"='Revenu'  THEN MONTANT ELSE 0 END),0) AS REVENU,
               NVL(SUM(CASE WHEN "TYPE"='Dépense' THEN MONTANT ELSE 0 END),0) AS DEPENSE
        FROM TRANSACTIONS
        WHERE EXTRACT(YEAR FROM DATE_TRANSACTION)=:y
          AND EXTRACT(MONTH FROM DATE_TRANSACTION)=:m
    )");
    q.bindValue(":y", year);
    q.bindValue(":m", month);
    if (q.exec() && q.next()) {
        stats["revenu"]  = q.value("REVENU");
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
               NVL(SUM(CASE WHEN "TYPE"='Revenu'  THEN MONTANT ELSE 0 END),0) AS REVENU,
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
            evo[mois + "_revenu"]  = q.value("REVENU");
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

/*==================================================
 *  MODULE INCIDENTS  (avec ID_RESIDENT)
 *=================================================*/
bool DatabaseManager::addIncident(const QVariantMap &data)
{
    if (!db.isOpen())
        return false;

    // ============================
    // Conversion du CIN en ID_RESIDENT
    // ============================
    QString rawCin;
    int idResident = -1;  // -1 = NULL

    QVariant rawValue = data.value("cin_resident");

    if (rawValue.type() == QVariant::ByteArray) {
        rawCin = QString::fromUtf8(rawValue.toByteArray()).trimmed();
    } else {
        rawCin = rawValue.toString().trimmed();
    }

    if (!rawCin.isEmpty()) {
        QString rid = getResidentIdFromCin(rawCin);
        if (!rid.isEmpty()) {
            idResident = rid.toInt();
        }
    }

    // ============================
    // INSERT
    // ============================
    QSqlQuery q(db);
    q.prepare(R"(
        INSERT INTO INCIDENTS
            (TYPE_INCIDENT, LOCALISATION, DATE_HEURE,
             NIVEAU, STATUT, ID_RESIDENT)
        VALUES (
            :type, :localisation,
            TO_TIMESTAMP(:date_heure, 'YYYY-MM-DD HH24:MI:SS'),
            :niveau, :statut, :idResident
        )
    )");

    q.bindValue(":type",         data.value("type_incident").toString());
    q.bindValue(":localisation", data.value("localisation").toString());
    q.bindValue(":date_heure",   data.value("date_heure").toString());
    q.bindValue(":niveau",       data.value("niveau").toInt());
    q.bindValue(":statut",       data.value("statut").toString());
    
    // Passer NULL si idResident est -1, sinon passer l'ID
    if (idResident == -1) {
        q.bindValue(":idResident", QVariant(QVariant::Int));  // NULL explicite
    } else {
        q.bindValue(":idResident", idResident);
    }

    if (!q.exec()) {
        qWarning() << "❌ addIncident:" << q.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::updateIncident(int id, const QVariantMap &data)
{
    if (!db.isOpen())
        return false;

    QString cin = data.value("cin_resident").toString().trimmed();
    QVariant idResident;
    if (!cin.isEmpty()) {
        QString rid = getResidentIdFromCin(cin);
        if (rid.isEmpty()) {
            QMessageBox::warning(nullptr, "Erreur",
                                 QString("Le CIN %1 n'existe pas dans RESIDENTS.").arg(cin));
            return false;
        }
        idResident = rid;
    }

    QSqlQuery q(db);
    q.prepare(R"(
        UPDATE INCIDENTS SET
            TYPE_INCIDENT = :type,
            LOCALISATION  = :localisation,
            DATE_HEURE    = TO_TIMESTAMP(:date_heure,'YYYY-MM-DD HH24:MI:SS'),
            NIVEAU        = :niveau,
            STATUT        = :statut,
            ID_RESIDENT   = :idResident
        WHERE ID_INCIDENT = :id
    )");
    q.bindValue(":type",         data.value("type_incident").toString().trimmed());
    q.bindValue(":localisation", data.value("localisation").toString().trimmed());
    q.bindValue(":date_heure",   data.value("date_heure").toString().trimmed());
    q.bindValue(":niveau",       data.value("niveau").toInt());
    q.bindValue(":statut",       data.value("statut").toString().trimmed());
    q.bindValue(":idResident",   idResident);
    q.bindValue(":id",           id);

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

    q.prepare(R"(
    SELECT
        I.ID_INCIDENT AS ID,
        I.TYPE_INCIDENT,
        I.LOCALISATION,
        TO_CHAR(I.DATE_HEURE, 'YYYY-MM-DD HH24:MI:SS') AS DATE_HEURE,
        I.NIVEAU,
        I.STATUT,
        R.CIN AS CIN_RESIDENT
    FROM INCIDENTS I
    LEFT JOIN RESIDENTS R ON I.ID_RESIDENT = R.ID
    ORDER BY I.ID_INCIDENT DESC
)");


    if (!q.exec()) {
        qWarning() << "❌ getAllIncidents:" << q.lastError().text();
        return list;
    }

    while (q.next()) {
        QVariantMap m;
        m["id"] = q.value("ID");   // <-- le nom EXACT du select


        m["type_incident"] = q.value("TYPE_INCIDENT");
        m["localisation"]  = q.value("LOCALISATION");
        m["date_heure"]    = q.value("DATE_HEURE");
        m["niveau"]        = q.value("NIVEAU");
        m["statut"]        = q.value("STATUT");
        m["cin_resident"]  = q.value("CIN_RESIDENT");  // ⭐ CIN ICI

        list.append(m);
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
        SELECT  i.ID_INCIDENT,
                i.TYPE_INCIDENT,
                i.LOCALISATION,
                TO_CHAR(i.DATE_HEURE,'YYYY-MM-DD HH24:MI:SS') AS DATE_HEURE,
                i.NIVEAU,
                i.STATUT,
                r.CIN AS CIN_RESIDENT
        FROM INCIDENTS i
        LEFT JOIN RESIDENTS r ON i.ID_RESIDENT = r.ID
        WHERE i.ID_INCIDENT = :id
    )");
    q.bindValue(":id", id);

    if (q.exec() && q.next()) {
        map["id"]           = q.value("ID_INCIDENT").toInt();
        map["type_incident"]= q.value("TYPE_INCIDENT");
        map["localisation"] = q.value("LOCALISATION");
        map["date_heure"]   = q.value("DATE_HEURE");
        map["niveau"]       = q.value("NIVEAU");
        map["statut"]       = q.value("STATUT");
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
        SELECT  i.ID_INCIDENT,
                i.TYPE_INCIDENT,
                i.LOCALISATION,
                TO_CHAR(i.DATE_HEURE,'YYYY-MM-DD HH24:MI:SS') AS DATE_HEURE,
                i.NIVEAU,
                i.STATUT,
                r.CIN AS CIN_RESIDENT
        FROM INCIDENTS i
        LEFT JOIN RESIDENTS r ON i.ID_RESIDENT = r.ID
        WHERE  LOWER(i.TYPE_INCIDENT) LIKE LOWER(:term)
            OR LOWER(i.LOCALISATION)  LIKE LOWER(:term)
            OR LOWER(i.STATUT)        LIKE LOWER(:term)
            OR LOWER(r.CIN)           LIKE LOWER(:term)
    )");
    q.bindValue(":term", "%" + term + "%");

    if (q.exec()) {
        while (q.next()) {
            QVariantMap m;
            m["id"]           = q.value("ID_INCIDENT").toInt();
            m["type_incident"]= q.value("TYPE_INCIDENT");
            m["localisation"] = q.value("LOCALISATION");
            m["date_heure"]   = q.value("DATE_HEURE");
            m["niveau"]       = q.value("NIVEAU");
            m["statut"]       = q.value("STATUT");
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
    if (q.exec("SELECT COUNT(*) FROM INCIDENTS WHERE LOWER(STATUT) IN ('résolu','resolu')") && q.next())
        stats["resolus"] = q.value(0).toInt();
    return stats;
}
int DatabaseManager::getIncidentsCount(int residentId, int months)
{
    if (!db.isOpen())
        return 0;

    QSqlQuery q(db);
    q.prepare(R"(
        SELECT COUNT(*)
        FROM INCIDENTS
        WHERE ID_RESIDENT = :id
        AND DATE_HEURE >= ADD_MONTHS(SYSDATE, :months * -1)
    )");
    q.bindValue(":id", residentId);
    q.bindValue(":months", months);

    if (q.exec() && q.next())
        return q.value(0).toInt();

    return 0;
}

/*==================================================
 *  MODULE VÉHICULES (PARKING SIMPLIFIÉ)
 *=================================================*/

bool DatabaseManager::addVehicule(const QVariantMap &data)
{
    QSqlQuery q(db);
    q.prepare("INSERT INTO VEHICULES (MATRICULE, TYPE, PROPRIETAIRE) "
              "VALUES (:matricule, :type, :proprietaire)");
    q.bindValue(":matricule", data.value("matricule"));
    q.bindValue(":type", data.value("type"));
    q.bindValue(":proprietaire", data.value("proprietaire"));
    if (!q.exec()) {
        qWarning() << "❌ addVehicule:" << q.lastError().text();
        return false;
    }
    db.commit();
    return true;
}

/*==================================================
 *  MODULE LOCAUX
 *=================================================*/

bool DatabaseManager::addLocal(const QVariantMap &data)
{
    if (!db.isOpen())
        return false;

    QSqlQuery q(db);
    q.prepare(R"(
        INSERT INTO LOCAUX (ID_L, NOM, ADRESSE, NUM_LOCAUX,
                            SUPERFICIE, "TYPE", STATUT)
        VALUES (:id_l, :nom, :adresse, :numero,
                :superficie, :type, :statut)
    )");
    q.bindValue(":id_l",       data.value("id_l").toString().trimmed());
    q.bindValue(":nom",        data.value("nom").toString().trimmed());
    q.bindValue(":adresse",    data.value("adresse").toString().trimmed());
    q.bindValue(":numero",     data.value("num_locaux").toString().trimmed());
    q.bindValue(":superficie", data.value("superficie").toInt());
    q.bindValue(":type",       data.value("type").toString().trimmed());
    q.bindValue(":statut",     data.value("statut").toString().trimmed());

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
            NOM        = :nom,
            ADRESSE    = :adresse,
            NUM_LOCAUX = :numero,
            SUPERFICIE = :superficie,
            "TYPE"     = :type,
            STATUT     = :statut
        WHERE ID_L = :id_l
    )");
    q.bindValue(":nom",        data.value("nom").toString().trimmed());
    q.bindValue(":adresse",    data.value("adresse").toString().trimmed());
    q.bindValue(":numero",     data.value("num_locaux").toString().trimmed());
    q.bindValue(":superficie", data.value("superficie").toInt());
    q.bindValue(":type",       data.value("type").toString().trimmed());
    q.bindValue(":statut",     data.value("statut").toString().trimmed());
    q.bindValue(":id_l",       id);

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
        SELECT ID_L, NOM, ADRESSE, NUM_LOCAUX,
               SUPERFICIE, "TYPE", STATUT
        FROM LOCAUX
        ORDER BY NOM
    )")) {
        qWarning() << "❌ getAllLocaux:" << q.lastError().text();
        return list;
    }

    while (q.next()) {
        QVariantMap m;
        m["id_l"]       = q.value("ID_L");
        m["nom"]        = q.value("NOM");
        m["adresse"]    = q.value("ADRESSE");
        m["num_locaux"] = q.value("NUM_LOCAUX");
        m["superficie"] = q.value("SUPERFICIE");
        m["type"]       = q.value("TYPE");
        m["statut"]     = q.value("STATUT");
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
        SELECT ID_L, NOM, ADRESSE, NUM_LOCAUX,
               SUPERFICIE, "TYPE", STATUT
        FROM LOCAUX
        WHERE ID_L = :id_l
    )");
    q.bindValue(":id_l", id);
    if (q.exec() && q.next()) {
        map["id_l"]       = q.value("ID_L");
        map["nom"]        = q.value("NOM");
        map["adresse"]    = q.value("ADRESSE");
        map["num_locaux"] = q.value("NUM_LOCAUX");
        map["superficie"] = q.value("SUPERFICIE");
        map["type"]       = q.value("TYPE");
        map["statut"]     = q.value("STATUT");
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
        SELECT ID_L, NOM, ADRESSE, NUM_LOCAUX,
               SUPERFICIE, "TYPE", STATUT
        FROM LOCAUX
        WHERE LOWER(ID_L)   LIKE LOWER(:term)
           OR LOWER(NOM)    LIKE LOWER(:term)
           OR LOWER("TYPE") LIKE LOWER(:term)
           OR LOWER(ADRESSE)LIKE LOWER(:term)
    )");
    q.bindValue(":term", "%" + term + "%");

    if (q.exec()) {
        while (q.next()) {
            QVariantMap m;
            m["id_l"]       = q.value("ID_L");
            m["nom"]        = q.value("NOM");
            m["adresse"]    = q.value("ADRESSE");
            m["num_locaux"] = q.value("NUM_LOCAUX");
            m["superficie"] = q.value("SUPERFICIE");
            m["type"]       = q.value("TYPE");
            m["statut"]     = q.value("STATUT");
            list << m;
        }
    }
    return list;
}

/*==================================================
 *  MODULE TRANSPORT_VEHICULES
 *=================================================*/

bool DatabaseManager::addTransportVehicule(const QVariantMap &data)
{
    if (!db.isOpen()) {
        QMessageBox::critical(nullptr, "Erreur",
                              "Connexion à la base de données fermée.");
        return false;
    }

    QSqlQuery query(db);
    query.prepare(R"(
        INSERT INTO TRANSPORT_VEHICULES
            (id_vehicule, type, capacite, zone, horaire,
             statut, date_ajout, temps_utilise, code_unique)
        VALUES (:id_vehicule, :type, :capacite, :zone, :horaire,
                :statut, TO_DATE(:date_ajout, 'YYYY-MM-DD'),
                :temps_utilise, :code_unique)
    )");

    QString id           = data.value("id_vehicule").toString().toUpper();
    QString type         = data.value("type").toString();
    int     capacite     = data.value("capacite").toInt();
    QString zone         = data.value("zone").toString();
    QString horaire      = data.value("horaire").toString();
    QString statut       = data.value("statut").toString();
    QString date_ajout   = data.value("date_ajout").toString();
    double  tempsUtilise = data.value("temps_utilise").toDouble();
    QString code_unique  = data.value("code_unique").toString().trimmed();

    query.bindValue(":id_vehicule",   id);
    query.bindValue(":type",          type);
    query.bindValue(":capacite",      capacite);
    query.bindValue(":zone",          zone);
    query.bindValue(":horaire",       horaire);
    query.bindValue(":statut",        statut);
    query.bindValue(":date_ajout",    date_ajout);
    query.bindValue(":temps_utilise", tempsUtilise);

    if (code_unique.isEmpty()) {
        query.bindValue(":code_unique", QVariant());
    } else {
        QSqlQuery checkQuery(db);
        checkQuery.prepare("SELECT COUNT(*) FROM TRANSACTIONS WHERE CODE_UNIQUE = :code");
        checkQuery.bindValue(":code", code_unique);
        if (checkQuery.exec() && checkQuery.next()
            && checkQuery.value(0).toInt() == 0) {
            QMessageBox::warning(nullptr, "Erreur de validation",
                                 QString("Le code transaction '%1' n'existe pas dans la base.\n"
                                         "Crée d'abord la transaction ou laisse ce champ vide.")
                                     .arg(code_unique));
            return false;
        }
        query.bindValue(":code_unique", code_unique);
    }

    if (!query.exec()) {
        QString errorMsg = query.lastError().text();
        qDebug() << "❌ Erreur SQL insertion transport:" << errorMsg;

        QString userMessage;
        if (errorMsg.contains("FK_TRANSPORT_TRANSACTION")
            || errorMsg.contains("ORA-02291")) {
            userMessage =
                "Erreur de contrainte de clé étrangère.\n"
                "Le code transaction doit exister dans la table TRANSACTIONS.";
        } else {
            userMessage =
                QString("Impossible d'ajouter le véhicule de transport.\n\nDétails: %1")
                    .arg(errorMsg);
        }

        QMessageBox::critical(nullptr, "Erreur Oracle", userMessage);
        return false;
    }

    db.commit();
    emit vehiculeAdded();
    emit dataChanged();

    return true;
}

bool DatabaseManager::updateTransportVehicule(const QString &id,
                                              const QVariantMap &data)
{
    if (!db.isOpen()) {
        QMessageBox::critical(nullptr, "Erreur",
                              "Connexion à la base de données fermée.");
        return false;
    }

    QSqlQuery query(db);
    query.prepare(R"(
        UPDATE TRANSPORT_VEHICULES SET
            type          = :type,
            capacite      = :capacite,
            zone          = :zone,
            horaire       = :horaire,
            statut        = :statut,
            date_ajout    = TO_DATE(:date_ajout, 'YYYY-MM-DD'),
            temps_utilise = :temps_utilise,
            code_unique   = :code_unique
        WHERE id_vehicule = :id
    )");

    query.bindValue(":type",          data.value("type"));
    query.bindValue(":capacite",      data.value("capacite").toInt());
    query.bindValue(":zone",          data.value("zone"));
    query.bindValue(":horaire",       data.value("horaire"));
    query.bindValue(":statut",        data.value("statut"));
    query.bindValue(":date_ajout",    data.value("date_ajout"));
    query.bindValue(":temps_utilise", data.value("temps_utilise").toDouble());

    QString code_unique = data.value("code_unique").toString().trimmed();
    if (code_unique.isEmpty()) {
        query.bindValue(":code_unique", QVariant());
    } else {
        QSqlQuery checkQuery(db);
        checkQuery.prepare("SELECT COUNT(*) FROM TRANSACTIONS WHERE CODE_UNIQUE = :code");
        checkQuery.bindValue(":code", code_unique);
        if (checkQuery.exec() && checkQuery.next()
            && checkQuery.value(0).toInt() == 0) {
            QMessageBox::warning(nullptr, "Erreur de validation",
                                 QString("Le code transaction '%1' n'existe pas.\n"
                                         "Crée d'abord la transaction ou laisse ce champ vide.")
                                     .arg(code_unique));
            return false;
        }
        query.bindValue(":code_unique", code_unique);
    }

    query.bindValue(":id", id.toUpper());

    if (!query.exec()) {
        QString errorMsg = query.lastError().text();
        qDebug() << "❌ Erreur mise à jour transport:" << errorMsg;

        QString userMessage;
        if (errorMsg.contains("FK_TRANSPORT_TRANSACTION")
            || errorMsg.contains("ORA-02291")) {
            userMessage =
                "Erreur de contrainte de clé étrangère.\n"
                "Le code transaction doit exister dans la table TRANSACTIONS.";
        } else {
            userMessage =
                QString("Impossible de modifier le véhicule de transport.\n\nDétails: %1")
                    .arg(errorMsg);
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
        qDebug() << "❌ Base déconnectée dans getAllTransportVehicules()";
        return list;
    }

    QSqlQuery query(db);
    query.setForwardOnly(true);

    if (!query.exec(R"(
        SELECT id_vehicule, type, capacite, zone, horaire, statut,
               TO_CHAR(date_ajout, 'YYYY-MM-DD') AS date_ajout,
               temps_utilise, code_unique
        FROM TRANSPORT_VEHICULES
        ORDER BY id_vehicule
    )")) {
        qDebug() << "❌ ERREUR SQL:" << query.lastError().text();
        return list;
    }

    while (query.next()) {
        QVariantMap map;
        map["id_vehicule"] = query.value("id_vehicule");
        map["type"]        = query.value("type");
        map["capacite"]    = query.value("capacite");
        map["zone"]        = query.value("zone");
        map["horaire"]     = query.value("horaire");
        map["statut"]      = query.value("statut");
        map["date_ajout"]  = query.value("date_ajout");
        map["temps_utilise"] = query.value("temps_utilise");
        map["code_unique"]   = query.value("code_unique");
        list.append(map);
    }

    return list;
}

QVariantMap DatabaseManager::getTransportVehicule(const QString &id)
{
    QVariantMap map;
    QSqlQuery query(db);
    query.prepare(R"(
        SELECT id_vehicule, type, capacite, zone, horaire, statut,
               TO_CHAR(date_ajout, 'YYYY-MM-DD') AS date_ajout,
               temps_utilise, code_unique
        FROM TRANSPORT_VEHICULES
        WHERE id_vehicule = :id
    )");
    query.bindValue(":id", id.toUpper());

    if (query.exec() && query.next()) {
        map["id_vehicule"]  = query.value("id_vehicule");
        map["type"]         = query.value("type");
        map["capacite"]     = query.value("capacite");
        map["zone"]         = query.value("zone");
        map["horaire"]      = query.value("horaire");
        map["statut"]       = query.value("statut");
        map["date_ajout"]   = query.value("date_ajout");
        map["temps_utilise"]= query.value("temps_utilise");
        map["code_unique"]  = query.value("code_unique");
    }

    return map;
}

QList<QVariantMap> DatabaseManager::searchTransportVehicules(const QString &term)
{
    QList<QVariantMap> list;
    QSqlQuery query(db);
    query.prepare(R"(
        SELECT id_vehicule, type, capacite, zone, horaire, statut,
               TO_CHAR(date_ajout, 'YYYY-MM-DD') AS date_ajout,
               temps_utilise, code_unique
        FROM TRANSPORT_VEHICULES
        WHERE id_vehicule LIKE :term
           OR type        LIKE :term
           OR zone        LIKE :term
    )");
    query.bindValue(":term", "%" + term + "%");

    if (query.exec()) {
        while (query.next()) {
            QVariantMap map;
            map["id_vehicule"]  = query.value("id_vehicule");
            map["type"]         = query.value("type");
            map["capacite"]     = query.value("capacite");
            map["zone"]         = query.value("zone");
            map["horaire"]      = query.value("horaire");
            map["statut"]       = query.value("statut");
            map["date_ajout"]   = query.value("date_ajout");
            map["temps_utilise"]= query.value("temps_utilise");
            map["code_unique"]  = query.value("code_unique");
            list.append(map);
        }
    }

    return list;
}

QVariantMap DatabaseManager::getTransportStatistics()
{
    QVariantMap stats;
    QSqlQuery query(db);

    if (query.exec("SELECT COUNT(*) FROM TRANSPORT_VEHICULES") && query.next())
        stats["total"] = query.value(0).toInt();

    if (query.exec("SELECT type, COUNT(*) FROM TRANSPORT_VEHICULES GROUP BY type")) {
        while (query.next()) {
            stats["count_" + query.value(0).toString()] = query.value(1).toInt();
        }
    }

    if (query.exec("SELECT AVG(temps_utilise) FROM TRANSPORT_VEHICULES") && query.next())
        stats["avg_temps"] = query.value(0).toDouble();

    return stats;
}

/*==================================================
 *  BULK OPERATIONS (Clear)
 *=================================================*/

bool DatabaseManager::clearAllResidents()
{
    if (!db.isOpen()) {
        qWarning() << "❌ Database not open for clearAllResidents";
        return false;
    }

    db.transaction();
    QSqlQuery q(db);
    if (!q.exec("DELETE FROM RESIDENTS")) {
        qWarning() << "❌ clearAllResidents:" << q.lastError().text();
        db.rollback();
        return false;
    }

    db.commit();
    emit dataChanged();
    return true;
}

bool DatabaseManager::clearAllTransactions()
{
    if (!db.isOpen()) {
        qWarning() << "❌ Database not open for clearAllTransactions";
        return false;
    }

    db.transaction();
    QSqlQuery q(db);
    if (!q.exec("DELETE FROM TRANSACTIONS")) {
        qWarning() << "❌ clearAllTransactions:" << q.lastError().text();
        db.rollback();
        return false;
    }

    db.commit();
    emit dataChanged();
    return true;
}

bool DatabaseManager::clearAllIncidents()
{
    if (!db.isOpen()) {
        qWarning() << "❌ Database not open for clearAllIncidents";
        return false;
    }

    db.transaction();
    QSqlQuery q(db);
    if (!q.exec("DELETE FROM INCIDENTS")) {
        qWarning() << "❌ clearAllIncidents:" << q.lastError().text();
        db.rollback();
        return false;
    }

    db.commit();
    emit dataChanged();
    return true;
}

bool DatabaseManager::clearAllLocaux()
{
    if (!db.isOpen()) {
        qWarning() << "❌ Database not open for clearAllLocaux";
        return false;
    }

    db.transaction();
    QSqlQuery q(db);
    if (!q.exec("DELETE FROM LOCAUX")) {
        qWarning() << "❌ clearAllLocaux:" << q.lastError().text();
        db.rollback();
        return false;
    }

    db.commit();
    emit dataChanged();
    return true;
}

bool DatabaseManager::clearAllTransportVehicules()
{
    if (!db.isOpen()) {
        qWarning() << "❌ Database not open for clearAllTransportVehicules";
        return false;
    }

    db.transaction();
    QSqlQuery q(db);
    if (!q.exec("DELETE FROM TRANSPORT_VEHICULES")) {
        qWarning() << "❌ clearAllTransportVehicules:" << q.lastError().text();
        db.rollback();
        return false;
    }

    db.commit();
    emit dataChanged();
    return true;
}

bool DatabaseManager::clearAllData()
{
    if (!db.isOpen()) {
        qWarning() << "❌ Database not open for clearAllData";
        return false;
    }

    db.transaction();
    QSqlQuery q(db);

    QStringList queries = {
        "DELETE FROM TRANSPORT_VEHICULES",
        "DELETE FROM POSSEDER",
        "DELETE FROM INCIDENTS",
        "DELETE FROM TRANSACTIONS",
        "DELETE FROM LOCAUX",
        "DELETE FROM RESIDENTS"
    };

    for (const QString &query : queries) {
        if (!q.exec(query)) {
            qWarning() << "❌ clearAllData:" << q.lastError().text()
                << "for query:" << query;
            db.rollback();
            return false;
        }
    }

    db.commit();
    emit dataChanged();
    return true;
}







QVariantMap DatabaseManager::getIncidentStatusCounts()
{
    QVariantMap stats;

    if (!db.isOpen())
        return stats;

    QSqlQuery q(db);
    q.prepare(R"(
        SELECT LOWER(STATUT), COUNT(*)
        FROM INCIDENTS
        GROUP BY LOWER(STATUT)
    )");

    if (!q.exec()) {
        qWarning() << "❌ getIncidentStatusCounts:" << q.lastError().text();
        return stats;
    }

    while (q.next()) {
        QString statut = q.value(0).toString().trimmed();
        int count = q.value(1).toInt();
        stats[statut] = count;
    }

    return stats;
}


QString DatabaseManager::getResidentIdFromRFID(const QString &uid)
{
    QSqlQuery q(db);
    q.prepare("SELECT ID FROM RESIDENTS WHERE RFID_UID = :uid");
    q.bindValue(":uid", uid);

    if (q.exec() && q.next())
        return q.value(0).toString();

    return "";
}

QVariantMap DatabaseManager::getResidentByRFID(const QString &uid)
{
    QVariantMap resident;
    QSqlQuery q(db);
    
    q.prepare("SELECT ID, NOM, PRENOM, CIN, EMAIL, TELEPHONE, STATUT, DATE_ENTREE FROM RESIDENTS WHERE RFID_UID = :uid");
    q.bindValue(":uid", uid);
    
    if (q.exec() && q.next()) {
        QSqlRecord record = q.record();
        for (int i = 0; i < record.count(); ++i) {
            resident[record.fieldName(i)] = q.value(i);
        }
    }
    
    return resident;
}

/*==================================================
 *  ARDUINO TRANSPORT (BORNE) - NOUVELLES MÉTHODES
 *=================================================*/

QVariantMap DatabaseManager::getVehiculeByCode(const QString &code)
{
    QVariantMap vehicule;
    
    qDebug() << "[DatabaseManager] 🔍 Recherche véhicule avec ID_VEHICULE:" << code;
    
    if (!db.isOpen()) {
        qWarning() << "❌ Database not open for getVehiculeByCode";
        return vehicule;
    }
    
    QSqlQuery q(db);
    q.prepare("SELECT ID_VEHICULE, CODE_UNIQUE, TYPE, CAPACITE, ZONE, HORAIRE, STATUT, "
              "DATE_AJOUT, TEMPS_UTILISE "
              "FROM TRANSPORT_VEHICULES WHERE ID_VEHICULE = :code");
    q.bindValue(":code", code);
    
    qDebug() << "[DatabaseManager] 📊 Requête SQL:" << q.executedQuery();
    qDebug() << "[DatabaseManager] 🔑 ID_VEHICULE recherché:" << code;
    
    if (q.exec()) {
        if (q.next()) {
            QSqlRecord record = q.record();
            for (int i = 0; i < record.count(); ++i) {
                vehicule[record.fieldName(i)] = q.value(i);
            }
            qDebug() << "[DatabaseManager] ✅ Véhicule trouvé:" << code;
            qDebug() << "[DatabaseManager] 📦 Données:" << vehicule;
        } else {
            qWarning() << "[DatabaseManager] ⚠️ Aucun résultat pour ID_VEHICULE:" << code;
            
            // Afficher tous les ID existants pour debug
            QSqlQuery debugQuery(db);
            debugQuery.exec("SELECT ID_VEHICULE FROM TRANSPORT_VEHICULES");
            QStringList existingCodes;
            while (debugQuery.next()) {
                existingCodes << debugQuery.value(0).toString();
            }
            qDebug() << "[DatabaseManager] 📋 ID_VEHICULE existants dans TRANSPORT_VEHICULES:" << existingCodes;
        }
    } else {
        qWarning() << "[DatabaseManager] ❌ Erreur SQL:" << q.lastError().text();
    }
    
    return vehicule;
}

double DatabaseManager::getMontantTotalVehicule(const QString &code)
{
    double total = 0.0;
    
    if (!db.isOpen()) {
        qWarning() << "❌ Database not open for getMontantTotalVehicule";
        return total;
    }
    
    // Récupérer le montant de LA transaction du véhicule (il n'y en a qu'une seule maintenant)
    QSqlQuery q(db);
    q.prepare("SELECT MONTANT FROM TRANSACTIONS "
              "WHERE CATEGORIE = 'Transport' AND DESCRIPTION LIKE :pattern");
    q.bindValue(":pattern", QString("%Véhicule: %1%").arg(code));
    
    if (q.exec() && q.next()) {
        total = q.value("MONTANT").toDouble();
        qDebug() << "[DatabaseManager] 💰 Montant total pour véhicule" << code << ":" << total << "DT";
    } else {
        qDebug() << "[DatabaseManager] ℹ️ Aucune transaction pour véhicule" << code << "(montant = 0)";
    }
    
    return total;
}

QVariantMap DatabaseManager::getTransactionByVehicule(const QString &vehiculeID)
{
    QVariantMap transaction;
    
    if (!db.isOpen()) {
        qWarning() << "❌ Database not open for getTransactionByVehicule";
        return transaction;
    }
    
    // Chercher la transaction de ce véhicule (CATEGORIE='Transport' et DESCRIPTION contient le vehiculeID)
    QSqlQuery q(db);
    q.prepare("SELECT CODE_UNIQUE, MONTANT, TYPE, CATEGORIE, DESCRIPTION, DATE_TRANSACTION, ID_RESIDENT "
              "FROM TRANSACTIONS "
              "WHERE CATEGORIE = 'Transport' AND DESCRIPTION LIKE :pattern");
    q.bindValue(":pattern", QString("%Véhicule: %1%").arg(vehiculeID));
    
    if (q.exec() && q.next()) {
        transaction["CODE_UNIQUE"] = q.value("CODE_UNIQUE").toString();
        transaction["MONTANT"] = q.value("MONTANT").toDouble();
        transaction["TYPE"] = q.value("TYPE").toString();
        transaction["CATEGORIE"] = q.value("CATEGORIE").toString();
        transaction["DESCRIPTION"] = q.value("DESCRIPTION").toString();
        transaction["DATE_TRANSACTION"] = q.value("DATE_TRANSACTION").toString();
        transaction["ID_RESIDENT"] = q.value("ID_RESIDENT").toString();
        
        qDebug() << "[DatabaseManager] 🔍 Transaction existante trouvée pour véhicule" << vehiculeID 
                 << "- Code:" << transaction["CODE_UNIQUE"].toString() 
                 << "- Montant:" << transaction["MONTANT"].toDouble();
    } else {
        qDebug() << "[DatabaseManager] ℹ️ Aucune transaction existante pour véhicule" << vehiculeID;
    }
    
    return transaction;
}

bool DatabaseManager::updateTransactionMontant(const QString &code, double nouveauMontant)
{
    if (!db.isOpen()) {
        qWarning() << "❌ Database not open for updateTransactionMontant";
        return false;
    }
    
    QSqlQuery q(db);
    q.prepare("UPDATE TRANSACTIONS SET MONTANT = :montant, DATE_TRANSACTION = SYSDATE "
              "WHERE CODE_UNIQUE = :code");
    q.bindValue(":montant", nouveauMontant);
    q.bindValue(":code", code);
    
    if (q.exec()) {
        qDebug() << "[DatabaseManager] ✅ Montant mis à jour:" << code << "→" << nouveauMontant << "DT";
        emit dataChanged();
        return true;
    } else {
        qWarning() << "[DatabaseManager] ❌ Erreur mise à jour montant:" << q.lastError().text();
        return false;
    }
}

