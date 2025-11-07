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
    db.setDatabaseName("OracleXE");   // DSN exact dans ODBC 64 bits
    db.setUserName("NEXORA");
    db.setPassword("nexora124");

    if (db.open()) {
        qInfo() << "✅ Connexion Oracle établie via ODBC (DSN=OracleXE)";
    } else {
        qWarning() << "❌ Échec de connexion Oracle:" << db.lastError().text();
        QMessageBox::critical(nullptr, "Erreur Oracle",
                              "Impossible de se connecter à Oracle.\n\n"
                              "Vérifie le DSN (OracleXE), le login (NEXORA) et le mot de passe.\n\n"
                              "Détails : " + db.lastError().text());
    }
}

QSqlDatabase DatabaseManager::database() const
{
    return db;
}

//
// ==================== MODULE RÉSIDENTS ====================
bool DatabaseManager::addResident(const QVariantMap &data)
{
    if (!db.isOpen()) {
        QMessageBox::warning(nullptr, "Erreur", "Connexion Oracle fermée.");
        return false;
    }

    QSqlQuery q;
    q.prepare(R"(
        INSERT INTO RESIDENTS
        (CIN, NOM, PRENOM, SEXE, TELEPHONE, EMAIL, DATE_ENTREE, APPARTEMENT, ETAGE, STATUT)
        VALUES (:cin, :nom, :prenom, :sexe, :telephone, :email,
                NVL(TO_DATE(:date_entree, 'YYYY-MM-DD'), SYSDATE),
                :appartement, :etage, :statut)
    )");

    QString sexe = data.value("sexe").toString().trimmed();
    if (sexe.compare("homme", Qt::CaseInsensitive) == 0)
        sexe = "Homme";
    else if (sexe.compare("femme", Qt::CaseInsensitive) == 0)
        sexe = "Femme";

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
        return false;
    }
    qInfo() << "✅ Résident ajouté avec succès.";
    return true;
}

bool DatabaseManager::updateResident(const QString &cin, const QVariantMap &data)
{
    QSqlQuery q;
    q.prepare(R"(
        UPDATE RESIDENTS SET
        NOM=:nom, PRENOM=:prenom, SEXE=:sexe, TELEPHONE=:telephone,
        EMAIL=:email, APPARTEMENT=:appartement, ETAGE=:etage, STATUT=:statut
        WHERE CIN=:cin
    )");

    QString sexe = data.value("sexe").toString().trimmed();
    if (sexe.compare("homme", Qt::CaseInsensitive) == 0)
        sexe = "Homme";
    else if (sexe.compare("femme", Qt::CaseInsensitive) == 0)
        sexe = "Femme";

    q.bindValue(":nom", data.value("nom"));
    q.bindValue(":prenom", data.value("prenom"));
    q.bindValue(":sexe", sexe);
    q.bindValue(":telephone", data.value("telephone"));
    q.bindValue(":email", data.value("email"));
    q.bindValue(":appartement", data.value("appartement"));
    q.bindValue(":etage", data.value("etage"));
    q.bindValue(":statut", data.value("statut"));
    q.bindValue(":cin", cin);

    if (!q.exec()) {
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
        QMessageBox::critical(nullptr, "Erreur Oracle",
                              "Impossible de supprimer le résident.\n\n" + q.lastError().text());
        return false;
    }
    return true;
}

QList<QVariantMap> DatabaseManager::getAllResidents()
{
    QList<QVariantMap> list;
    QSqlQuery q(R"(
        SELECT CIN, NOM, PRENOM, SEXE, TELEPHONE, EMAIL,
               TO_CHAR(DATE_ENTREE,'YYYY-MM-DD') AS DATE_ENTREE,
               APPARTEMENT, ETAGE, STATUT
        FROM RESIDENTS ORDER BY ID DESC
    )");
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
    q.prepare(R"(
        SELECT CIN, NOM, PRENOM, SEXE, TELEPHONE, EMAIL,
               TO_CHAR(DATE_ENTREE,'YYYY-MM-DD') AS DATE_ENTREE,
               APPARTEMENT, ETAGE, STATUT
        FROM RESIDENTS WHERE CIN=:cin
    )");
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
    q.prepare(R"(
        SELECT CIN, NOM, PRENOM, APPARTEMENT
        FROM RESIDENTS
        WHERE LOWER(NOM) LIKE LOWER(:t) OR LOWER(PRENOM) LIKE LOWER(:t)
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
    QSqlQuery q("SELECT COUNT(*) FROM RESIDENTS");
    if (q.next())
        stats["total"] = q.value(0).toInt();
    return stats;
}

//
// ==================== MODULE TRANSACTIONS ====================
bool DatabaseManager::addTransaction(const QVariantMap &data)
{
    QSqlQuery q;
    q.prepare(R"(
        INSERT INTO TRANSACTIONS (CODE, DATE, TYPE, MONTANT, DESCRIPTION)
        VALUES (:code, TO_DATE(:date,'YYYY-MM-DD'), :type, :montant, :description)
    )");
    q.bindValue(":code", data.value("code"));
    q.bindValue(":date", data.value("date"));
    q.bindValue(":type", data.value("type"));
    q.bindValue(":montant", data.value("montant"));

    // Store category inside the DESCRIPTION field as a prefix so existing DB schema is compatible.
    QString categorie = data.value("categorie").toString().trimmed();
    QString description = data.value("description").toString().trimmed();
    QString fullDesc = categorie.isEmpty() ? description : (categorie + " - " + description);
    q.bindValue(":description", fullDesc);

    if (!q.exec()) {
        QMessageBox::critical(nullptr, "Erreur Oracle",
                              "Impossible d'ajouter la transaction.\n\n" + q.lastError().text());
        qWarning() << "❌ addTransaction failed:" << q.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::updateTransaction(const QString &code, const QVariantMap &data)
{
    QSqlQuery q;
    q.prepare(R"(
        UPDATE TRANSACTIONS SET
        DATE=TO_DATE(:date,'YYYY-MM-DD'), TYPE=:type,
        MONTANT=:montant, DESCRIPTION=:description
        WHERE CODE=:code
    )");
    q.bindValue(":date", data.value("date"));
    q.bindValue(":type", data.value("type"));
    q.bindValue(":montant", data.value("montant"));

    QString categorie = data.value("categorie").toString().trimmed();
    QString description = data.value("description").toString().trimmed();
    QString fullDesc = categorie.isEmpty() ? description : (categorie + " - " + description);
    q.bindValue(":description", fullDesc);

    q.bindValue(":code", code);

    if (!q.exec()) {
        QMessageBox::critical(nullptr, "Erreur Oracle",
                              "Impossible de modifier la transaction.\n\n" + q.lastError().text());
        qWarning() << "❌ updateTransaction failed:" << q.lastError().text();
        return false;
    }
    return true;
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
    QSqlQuery q("SELECT CODE, TO_CHAR(DATE,'YYYY-MM-DD') AS DATE, TYPE, MONTANT, DESCRIPTION FROM TRANSACTIONS ORDER BY DATE DESC");
    while (q.next()) {
        QVariantMap m;
        m["code"] = q.value("CODE");
        m["date"] = q.value("DATE");
        m["type"] = q.value("TYPE");
        m["montant"] = q.value("MONTANT");
        QString rawDesc = q.value("DESCRIPTION").toString();
        // If we stored category as 'categorie - description', split it back
        int sep = rawDesc.indexOf(" - ");
        if (sep > 0) {
            m["categorie"] = rawDesc.left(sep);
            m["description"] = rawDesc.mid(sep + 3);
        } else {
            m["categorie"] = QString();
            m["description"] = rawDesc;
        }
        list << m;
    }
    return list;
}

QVariantMap DatabaseManager::getTransaction(const QString &code)
{
    QVariantMap m;
    QSqlQuery q;
    q.prepare("SELECT CODE, TO_CHAR(DATE,'YYYY-MM-DD') AS DATE, TYPE, MONTANT, DESCRIPTION FROM TRANSACTIONS WHERE CODE=:code");
    q.bindValue(":code", code);
    if (q.exec() && q.next()) {
        m["code"] = q.value("CODE");
        m["date"] = q.value("DATE");
        m["type"] = q.value("TYPE");
        m["montant"] = q.value("MONTANT");
        QString rawDesc = q.value("DESCRIPTION").toString();
        int sep = rawDesc.indexOf(" - ");
        if (sep > 0) {
            m["categorie"] = rawDesc.left(sep);
            m["description"] = rawDesc.mid(sep + 3);
        } else {
            m["categorie"] = QString();
            m["description"] = rawDesc;
        }
    }
    return m;
}

QList<QVariantMap> DatabaseManager::searchTransactions(const QString &term)
{
    QList<QVariantMap> list;
    QSqlQuery q;
    q.prepare("SELECT CODE, TYPE, MONTANT, DESCRIPTION FROM TRANSACTIONS WHERE LOWER(TYPE) LIKE LOWER(:t) OR LOWER(DESCRIPTION) LIKE LOWER(:t)");
    q.bindValue(":t", "%" + term + "%");
    if (q.exec()) {
        while (q.next()) {
            QVariantMap m;
            m["code"] = q.value("CODE");
            m["type"] = q.value("TYPE");
            m["montant"] = q.value("MONTANT");
            QString rawDesc = q.value("DESCRIPTION").toString();
            int sep = rawDesc.indexOf(" - ");
            if (sep > 0) {
                m["categorie"] = rawDesc.left(sep);
                m["description"] = rawDesc.mid(sep + 3);
            } else {
                m["categorie"] = QString();
                m["description"] = rawDesc;
            }
            list << m;
        }
    }
    return list;
}

QVariantMap DatabaseManager::getFinancialStatistics(int year, int month)
{
    QVariantMap stats;
    QSqlQuery q;
    q.prepare(R"(
        SELECT NVL(SUM(CASE WHEN TYPE='Revenu' THEN MONTANT ELSE 0 END),0) AS REVENU,
               NVL(SUM(CASE WHEN TYPE='Dépense' THEN MONTANT ELSE 0 END),0) AS DEPENSE
        FROM TRANSACTIONS
        WHERE EXTRACT(YEAR FROM DATE)=:y AND EXTRACT(MONTH FROM DATE)=:m
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
    QSqlQuery q;
    q.prepare(R"(
        SELECT EXTRACT(MONTH FROM DATE) AS MOIS,
               NVL(SUM(CASE WHEN TYPE='Revenu' THEN MONTANT ELSE 0 END),0) AS REVENU,
               NVL(SUM(CASE WHEN TYPE='Dépense' THEN MONTANT ELSE 0 END),0) AS DEPENSE
        FROM TRANSACTIONS
        WHERE EXTRACT(YEAR FROM DATE)=:y
        GROUP BY EXTRACT(MONTH FROM DATE)
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
    QSqlQuery q;
    q.prepare("SELECT NVL(SUM(MONTANT),0) FROM TRANSACTIONS WHERE TYPE='Revenu' AND EXTRACT(YEAR FROM DATE)=:y AND EXTRACT(MONTH FROM DATE)=:m");
    q.bindValue(":y", year);
    q.bindValue(":m", month);
    if (q.exec() && q.next())
        return q.value(0).toDouble();
    return 0.0;
}

double DatabaseManager::getTotalExpenses(int year, int month)
{
    QSqlQuery q;
    q.prepare("SELECT NVL(SUM(MONTANT),0) FROM TRANSACTIONS WHERE TYPE='Dépense' AND EXTRACT(YEAR FROM DATE)=:y AND EXTRACT(MONTH FROM DATE)=:m");
    q.bindValue(":y", year);
    q.bindValue(":m", month);
    if (q.exec() && q.next())
        return q.value(0).toDouble();
    return 0.0;
}

//
// ==================== MODULE INCIDENTS ====================
bool DatabaseManager::addIncident(const QVariantMap &data)
{
    QSqlQuery q;
    q.prepare("INSERT INTO INCIDENTS (CATEGORIE, DESCRIPTION, ETAT, DATE) VALUES (:categorie, :description, :etat, TO_DATE(:date,'YYYY-MM-DD'))");
    q.bindValue(":categorie", data.value("categorie"));
    q.bindValue(":description", data.value("description"));
    q.bindValue(":etat", data.value("etat"));
    q.bindValue(":date", data.value("date"));
    return q.exec();
}

//
// ==================== MODULE VÉHICULES ====================
bool DatabaseManager::addVehicule(const QVariantMap &data)
{
    QSqlQuery q;
    q.prepare("INSERT INTO VEHICULES (MATRICULE, TYPE, PROPRIETAIRE) VALUES (:matricule, :type, :proprietaire)");
    q.bindValue(":matricule", data.value("matricule"));
    q.bindValue(":type", data.value("type"));
    q.bindValue(":proprietaire", data.value("proprietaire"));
    return q.exec();
}
