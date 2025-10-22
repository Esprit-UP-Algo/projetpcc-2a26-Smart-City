#include "incidentdialog.h"
#include "ui_incidentdialog.h"
#include <QMessageBox>
#include <QtSql>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

IncidentDialog::IncidentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IncidentDialog)
{
    ui->setupUi(this);
    // Initialiser avec la date/heure actuelle
    ui->dateHeureEdit->setDateTime(QDateTime::currentDateTime());
}

IncidentDialog::IncidentDialog(const QVariantMap &incidentData, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IncidentDialog)
{
    ui->setupUi(this);
    
    // Remplir les champs avec les données existantes
    ui->typeCombo->setCurrentText(incidentData.value("categorie").toString());
    ui->localisationEdit->setText(incidentData.value("description").toString());
    ui->statutCombo->setCurrentText(incidentData.value("etat").toString());
    
    // Parser la date
    QString dateStr = incidentData.value("date").toString();
    if (!dateStr.isEmpty()) {
        QDateTime dateTime = QDateTime::fromString(dateStr, "yyyy-MM-dd HH:mm:ss");
        if (dateTime.isValid()) {
            ui->dateHeureEdit->setDateTime(dateTime);
        }
    }
    
    // Gravité si disponible
    if (incidentData.contains("gravite")) {
        ui->graviteSpinBox->setValue(incidentData.value("gravite").toInt());
    }
}

IncidentDialog::~IncidentDialog()
{
    delete ui;
}

QVariantMap IncidentDialog::getIncidentData() {
    QVariantMap data;
    data["categorie"] = ui->typeCombo->currentText();
    data["description"] = ui->localisationEdit->text(); // Le champ localisation contient la description
    data["etat"] = ui->statutCombo->currentText();
    data["date"] = ui->dateHeureEdit->dateTime().toString("yyyy-MM-dd HH:mm:ss");
    data["gravite"] = ui->graviteSpinBox->value();
    return data;
}

void IncidentDialog::on_saveButton_clicked()
{
    if (validateForm()) {
        // Essayer d'abord Oracle
        if (connecterOracle()) {
            // Créer la table si elle n'existe pas
            creerTableIncident();
            
            // Insérer l'incident
            if (insererIncidentOracle()) {
                QMessageBox::information(this, "Succès", "Incident ajouté avec succès dans Oracle !");
                accept();
                return;
            }
        }
        
        // Fallback vers SQLite si Oracle échoue
        QMessageBox::StandardButton reply = QMessageBox::question(
            this, 
            "Oracle non disponible", 
            "Oracle n'est pas accessible. Voulez-vous sauvegarder dans SQLite à la place ?",
            QMessageBox::Yes | QMessageBox::No
        );
        
        if (reply == QMessageBox::Yes) {
            if (insererIncidentSQLite()) {
                QMessageBox::information(this, "Succès", "Incident ajouté avec succès dans SQLite !");
                accept();
            } else {
                QMessageBox::critical(this, "Erreur", "Erreur lors de l'ajout de l'incident dans SQLite.");
            }
        }
    }
}

bool IncidentDialog::validateForm()
{
    if (ui->localisationEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation", "Le champ Localisation est obligatoire.");
        ui->localisationEdit->setFocus();
        return false;
    }
    
    if (ui->dateHeureEdit->dateTime() > QDateTime::currentDateTime()) {
        QMessageBox::warning(this, "Validation", "La date et l'heure ne peuvent pas être dans le futur.");
        ui->dateHeureEdit->setFocus();
        return false;
    }
    
    return true;
}

// ========== MÉTHODES CRUD ORACLE ==========

bool IncidentDialog::connecterOracle()
{
    // Vérifier si une connexion existe déjà
    QSqlDatabase db = QSqlDatabase::database("OracleConnection", false);
    if (db.isValid() && db.isOpen()) {
        qDebug() << "Connexion Oracle déjà ouverte !";
        return true;
    }
    
    // Créer une nouvelle connexion
    db = QSqlDatabase::addDatabase("QODBC", "OracleConnection");
    db.setDatabaseName("XE_ODBC");   // Nom de la source ODBC
    db.setUserName("SYSTEM");       // Utilisateur Oracle
    db.setPassword("wajd123");     // Mot de passe

    qDebug() << "Tentative de connexion à Oracle...";
    qDebug() << "Base de données:" << db.databaseName();
    qDebug() << "Utilisateur:" << db.userName();
    
    if(!db.open()) {
        QString errorMsg = QString("Erreur de connexion Oracle:\n%1\n\nVérifiez que:\n- Oracle XE est démarré\n- La source ODBC 'XE_ODBC' existe\n- Les identifiants sont corrects").arg(db.lastError().text());
        qDebug() << errorMsg;
        QMessageBox::critical(nullptr, "Erreur de connexion Oracle", errorMsg);
        return false;
    }
    qDebug() << "Connexion Oracle réussie !";
    return true;
}

void IncidentDialog::creerTableIncident()
{
    QSqlQuery query;
    if(!query.exec("CREATE TABLE Incident ("
                    "ID_Incident NUMBER PRIMARY KEY, "
                    "Type VARCHAR2(50) NOT NULL, "
                    "Localisation VARCHAR2(100) NOT NULL, "
                    "Date_Incident DATE NOT NULL, "
                    "Description VARCHAR2(255), "
                    "Statut VARCHAR2(20))")) {
        qDebug() << "Erreur création table Oracle:" << query.lastError().text();
    } else {
        qDebug() << "Table Incident Oracle prête! !";
    }
}

bool IncidentDialog::insererIncidentOracle()
{
    QSqlQuery query;
    query.prepare("INSERT INTO Incident (ID_Incident, Type, Localisation, Date_Incident, Description, Statut) "
                  "VALUES (:id, :type, :loc, :date, :desc, :statut)");
    
    // Générer un ID unique
    int id = QDateTime::currentMSecsSinceEpoch() % 100000;
    
    query.bindValue(":id", id);
    query.bindValue(":type", ui->typeCombo->currentText());
    query.bindValue(":loc", ui->localisationEdit->text());
    query.bindValue(":date", ui->dateHeureEdit->dateTime());
    query.bindValue(":desc", ui->localisationEdit->text()); // Description = localisation
    query.bindValue(":statut", ui->statutCombo->currentText());

    if(!query.exec()) {
        qDebug() << "Erreur insertion Oracle:" << query.lastError().text();
        return false;
    } else {
        qDebug() << "Incident ajouté avec succès dans Oracle !";
        return true;
    }
}

bool IncidentDialog::modifierIncidentOracle(int id)
{
    QSqlQuery query;
    query.prepare("UPDATE Incident SET Type=:type, Localisation=:loc, Date_Incident=:date, "
                  "Description=:desc, Statut=:statut WHERE ID_Incident=:id");
    
    query.bindValue(":id", id);
    query.bindValue(":type", ui->typeCombo->currentText());
    query.bindValue(":loc", ui->localisationEdit->text());
    query.bindValue(":date", ui->dateHeureEdit->dateTime());
    query.bindValue(":desc", ui->localisationEdit->text());
    query.bindValue(":statut", ui->statutCombo->currentText());

    if(!query.exec()) {
        qDebug() << "Erreur modification Oracle:" << query.lastError().text();
        return false;
    } else {
        qDebug() << "Incident modifié avec succès dans Oracle !";
        return true;
    }
}

bool IncidentDialog::supprimerIncidentOracle(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM Incident WHERE ID_Incident=:id");
    query.bindValue(":id", id);

    if(!query.exec()) {
        qDebug() << "Erreur suppression Oracle:" << query.lastError().text();
        return false;
    } else {
        qDebug() << "Incident supprimé avec succès d'Oracle !";
        return true;
    }
}

QList<QVariantMap> IncidentDialog::listerIncidentsOracle()
{
    QList<QVariantMap> incidents;
    QSqlQuery query("SELECT * FROM Incident ORDER BY ID_Incident DESC");
    
    if(!query.exec()) {
        qDebug() << "Erreur lecture Oracle:" << query.lastError().text();
        return incidents;
    }

    while(query.next()) {
        QVariantMap incident;
        incident["id"] = query.value("ID_Incident").toInt();
        incident["categorie"] = query.value("Type").toString();
        incident["description"] = query.value("Description").toString();
        incident["etat"] = query.value("Statut").toString();
        incident["date"] = query.value("Date_Incident").toDateTime().toString("yyyy-MM-dd HH:mm:ss");
        incidents << incident;
    }
    
    return incidents;
}

// ========== MÉTHODES FALLBACK SQLITE ==========

bool IncidentDialog::connecterSQLite()
{
    QSqlDatabase db = QSqlDatabase::database("SQLiteConnection", false);
    if (db.isValid() && db.isOpen()) {
        return true;
    }
    
    db = QSqlDatabase::addDatabase("QSQLITE", "SQLiteConnection");
    db.setDatabaseName("incidents_fallback.db");
    
    if (!db.open()) {
        qDebug() << "Erreur connexion SQLite:" << db.lastError().text();
        return false;
    }
    
    // Créer la table SQLite
    QSqlQuery query(db);
    query.exec("CREATE TABLE IF NOT EXISTS incidents ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "categorie TEXT, "
               "description TEXT, "
               "etat TEXT, "
               "date TEXT)");
    
    return true;
}

bool IncidentDialog::insererIncidentSQLite()
{
    if (!connecterSQLite()) {
        return false;
    }
    
    QSqlDatabase db = QSqlDatabase::database("SQLiteConnection");
    QSqlQuery query(db);
    query.prepare("INSERT INTO incidents (categorie, description, etat, date) VALUES (?, ?, ?, ?)");
    query.addBindValue(ui->typeCombo->currentText());
    query.addBindValue(ui->localisationEdit->text());
    query.addBindValue(ui->statutCombo->currentText());
    query.addBindValue(ui->dateHeureEdit->dateTime().toString("yyyy-MM-dd HH:mm:ss"));
    
    if (!query.exec()) {
        qDebug() << "Erreur insertion SQLite:" << query.lastError().text();
        return false;
    }
    
    return true;
}

QList<QVariantMap> IncidentDialog::listerIncidentsSQLite()
{
    QList<QVariantMap> incidents;
    
    if (!connecterSQLite()) {
        return incidents;
    }
    
    QSqlDatabase db = QSqlDatabase::database("SQLiteConnection");
    QSqlQuery query("SELECT * FROM incidents ORDER BY id DESC", db);
    
    while(query.next()) {
        QVariantMap incident;
        incident["id"] = query.value("id").toInt();
        incident["categorie"] = query.value("categorie").toString();
        incident["description"] = query.value("description").toString();
        incident["etat"] = query.value("etat").toString();
        incident["date"] = query.value("date").toString();
        incidents << incident;
    }
    
    return incidents;
}


