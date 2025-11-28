#include "incidentspage.h"
#include "ui_incidentspage.h"
#include "connection.h"
#include "databasemanager.h"
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>
#include <QSqlQuery>

IncidentsPage::IncidentsPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::IncidentsPage)
{
    ui->setupUi(this);

    // Connexions
    connect(ui->tabAddButton, &QPushButton::clicked, this, [=]() { ui->stackedWidget->setCurrentIndex(0); });
    connect(ui->tabListButton, &QPushButton::clicked, this, [=]() { ui->stackedWidget->setCurrentIndex(1); refreshList(); });
    connect(ui->tabStatsButton, &QPushButton::clicked, this, [=]() { ui->stackedWidget->setCurrentIndex(2); refreshStats(); });

    connect(ui->cancelFormButton, &QPushButton::clicked, this, [=]() { clearForm(); ui->stackedWidget->setCurrentIndex(1); });
    connect(ui->saveFormButton, &QPushButton::clicked, this, &IncidentsPage::onSaveIncident);

    connect(ui->searchButton, &QPushButton::clicked, this, &IncidentsPage::onSearch);
    connect(ui->searchEdit, &QLineEdit::returnPressed, this, &IncidentsPage::onSearch);
    connect(ui->sortCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &IncidentsPage::onSortIndexChanged);

    connect(ui->exportPdfButton, &QPushButton::clicked, this, &IncidentsPage::onExportPdf);

    connect(ui->editIncidentButton, &QPushButton::clicked, this, &IncidentsPage::onEditIncident);
    connect(ui->deleteIncidentButton, &QPushButton::clicked, this, &IncidentsPage::onDeleteIncident);

    // Initialiser seulement si la connexion est ouverte
    if (Connection::isOpen()) {
        refreshList();
        setupChart();
    } else {
        qWarning() << "[IncidentsPage] Connexion non ouverte, initialisation partielle";
    }
    ui->tabListButton->setChecked(true);
}

IncidentsPage::~IncidentsPage() { delete ui; }
void IncidentsPage::onSaveIncident()
{
    ui->errorLabel->setText("");

    // Récupération des valeurs du formulaire
    QString type = ui->typeCombo->currentText().trimmed();
    QString localisation = ui->localisationEdit->text().trimmed();
    QDateTime dateHeure = ui->dateHeureEdit->dateTime();
    int niveau = ui->niveauCombo->currentIndex() + 1;  // 1=Faible, 2=Moyen, 3=Elevé, 4=Critique
    QString statutText = ui->statutCombo->currentText().trimmed(); // "Signalé", "En cours", "Résolu", "Archivé"
    QString cinResidentText = ui->idREdit->text().trimmed();

    // Validation des champs obligatoires
    if (type.isEmpty() || localisation.isEmpty() || statutText.isEmpty() || cinResidentText.isEmpty()) {
        ui->errorLabel->setText("Veuillez remplir tous les champs obligatoires.");
        return;
    }

    // Vérifier ID_R
    bool okIdR = false;
    int idRes = cinResidentText.toInt(&okIdR);
    if (!okIdR || cinResidentText.length() != 8) {
        ui->errorLabel->setText("ID résident invalide !");
        return;
    }

    // Préparer les données pour DatabaseManager
    QVariantMap data;
    data["type_incident"] = type;
    data["localisation"] = localisation;
    data["date_heure"] = dateHeure.toString("yyyy-MM-dd HH:mm:ss");
    data["niveau"] = niveau;
    data["statut"] = statutText;  // On envoie le texte complet
    data["cin_resident"] = cinResidentText;

    // Appel à DatabaseManager (ajout ou modification)
    bool success = false;
    if (currentEditId > 0) {
        // Modification
        success = DatabaseManager::instance().updateIncident(currentEditId, data);
        if (success) {
            QMessageBox::information(this, "Succès", "Incident modifié avec succès !");
        }
    } else {
        // Ajout
        success = DatabaseManager::instance().addIncident(data);
        if (success) {
            QMessageBox::information(this, "Succès", "Incident signalé avec succès !");
        }
    }

    if (success) {
        clearForm();
        currentEditId = -1;
        ui->stackedWidget->setCurrentIndex(1);
        refreshList();
        refreshStats();
    } else {
        QSqlError err = Connection::getInstance().getDatabase().lastError();
        QString errorMsg = err.text();

        // Message d'erreur plus clair pour l'utilisateur
        if (errorMsg.contains("ORA-02291") || errorMsg.contains("parent key not found")) {
            ui->errorLabel->setText("Erreur : Le résident avec le CIN " + cinResidentText + " n'existe pas dans la base de données.");
        } else if (errorMsg.contains("FK_INCIDENTS_RESIDENTS")) {
            ui->errorLabel->setText("Erreur : Le résident avec le CIN " + cinResidentText + " n'existe pas. Veuillez vérifier le CIN.");
        } else {
            ui->errorLabel->setText("Erreur Oracle : " + errorMsg);
        }
    }
}


void IncidentsPage::refreshList()
{
    auto list = DatabaseManager::instance().getAllIncidents();
    ui->incidentTable->setRowCount(0);
    ui->incidentTable->setSortingEnabled(false);

    for (const auto &m : list) {
        int row = ui->incidentTable->rowCount();
        ui->incidentTable->insertRow(row);
        
        // ID (numérique)
        QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(m["id"].toInt()));
        idItem->setData(Qt::UserRole, m["id"].toInt());
        ui->incidentTable->setItem(row, 0, idItem);
        
        ui->incidentTable->setItem(row, 1, new QTableWidgetItem(m["type_incident"].toString()));
        ui->incidentTable->setItem(row, 2, new QTableWidgetItem(m["localisation"].toString()));
        ui->incidentTable->setItem(row, 3, new QTableWidgetItem(m["date_heure"].toString()));
        
        // Niveau (numérique)
        QTableWidgetItem *niveauItem = new QTableWidgetItem(QString::number(m["niveau"].toInt()));
        niveauItem->setData(Qt::UserRole, m["niveau"].toInt());
        ui->incidentTable->setItem(row, 4, niveauItem);
        
        ui->incidentTable->setItem(row, 5, new QTableWidgetItem(m["statut"].toString()));
        ui->incidentTable->setItem(row, 6, new QTableWidgetItem(m["cin_resident"].toString()));
    }
    
    ui->incidentTable->setSortingEnabled(true);
}

void IncidentsPage::onSearch()
{
    QString term = ui->searchEdit->text().trimmed();
    auto list = term.isEmpty() ? DatabaseManager::instance().getAllIncidents()
                               : DatabaseManager::instance().searchIncidents(term);
    ui->incidentTable->setRowCount(0);
    ui->incidentTable->setSortingEnabled(false);
    
    for (const auto &m : list) {
        int row = ui->incidentTable->rowCount();
        ui->incidentTable->insertRow(row);
        
        // ID (numérique)
        QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(m["id"].toInt()));
        idItem->setData(Qt::UserRole, m["id"].toInt());
        ui->incidentTable->setItem(row, 0, idItem);
        
        ui->incidentTable->setItem(row, 1, new QTableWidgetItem(m["type_incident"].toString()));
        ui->incidentTable->setItem(row, 2, new QTableWidgetItem(m["localisation"].toString()));
        ui->incidentTable->setItem(row, 3, new QTableWidgetItem(m["date_heure"].toString()));
        
        // Niveau (numérique)
        QTableWidgetItem *niveauItem = new QTableWidgetItem(QString::number(m["niveau"].toInt()));
        niveauItem->setData(Qt::UserRole, m["niveau"].toInt());
        ui->incidentTable->setItem(row, 4, niveauItem);
        
        ui->incidentTable->setItem(row, 5, new QTableWidgetItem(m["statut"].toString()));
        ui->incidentTable->setItem(row, 6, new QTableWidgetItem(m["cin_resident"].toString()));
    }
    
    ui->incidentTable->setSortingEnabled(true);
}

void IncidentsPage::onDeleteIncident()
{
    int row = ui->incidentTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Attention", "Sélectionnez un incident à supprimer.");
        return;
    }
    int id = ui->incidentTable->item(row, 0)->text().toInt();
    if (QMessageBox::question(this, "Confirmer", "Supprimer cet incident ?") == QMessageBox::Yes) {
        if (DatabaseManager::instance().deleteIncident(id)) {
            QMessageBox::information(this, "Succès", "Incident supprimé avec succès !");
            refreshList();
            refreshStats();
        } else {
            QSqlError err = Connection::getInstance().getDatabase().lastError();
            QMessageBox::critical(this, "Erreur", "Impossible de supprimer l'incident.\n\n" + err.text());
        }
    }
}

void IncidentsPage::onEditIncident()
{
    int row = ui->incidentTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Attention", "Sélectionnez un incident à modifier.");
        return;
    }

    int id = ui->incidentTable->item(row, 0)->text().toInt();

    // Remplir le formulaire avec les données de l'incident sélectionné
    ui->typeCombo->setCurrentText(ui->incidentTable->item(row, 1)->text());
    ui->localisationEdit->setText(ui->incidentTable->item(row, 2)->text());

    QDateTime dateHeure = QDateTime::fromString(ui->incidentTable->item(row, 3)->text(), "yyyy-MM-dd HH:mm:ss");
    if (!dateHeure.isValid()) {
        dateHeure = QDateTime::fromString(ui->incidentTable->item(row, 3)->text(), "yyyy-MM-dd HH:mm");
    }
    ui->dateHeureEdit->setDateTime(dateHeure);

    int niveau = ui->incidentTable->item(row, 4)->text().toInt();
    ui->niveauCombo->setCurrentIndex(niveau - 1);

    ui->statutCombo->setCurrentText(ui->incidentTable->item(row, 5)->text());
    ui->idREdit->setText(ui->incidentTable->item(row, 6)->text());

    // Stocker l'ID pour la modification
    currentEditId = id;

    // Passer à l'onglet formulaire
    ui->stackedWidget->setCurrentIndex(0);
    ui->tabAddButton->setChecked(true);
}

void IncidentsPage::onExportPdf()
{
    QMessageBox::information(this, "PDF", "Export PDF en cours de développement.");
}

void IncidentsPage::refreshStats()
{
    auto stats = DatabaseManager::instance().getIncidentsStatistics();
    int total = stats["total"].toInt();
    int critiques = stats["critiques"].toInt();
    // int signales = stats["signales"].toInt();

    ui->riskScore->setText(QString("Niveau de risque : %1 / 100").arg(critiques * 25));
    ui->riskAnalysis->setText(QString("Attention : %1 incidents critiques sur %2 ce mois-ci.")
                                  .arg(critiques).arg(total));

    setupChart(); // Mise à jour graphique
}

void IncidentsPage::setupChart()
{
    if (!Connection::isOpen()) {
        qWarning() << "[IncidentsPage] Connexion non ouverte, impossible de charger le graphique";
        return;
    }

    auto list = DatabaseManager::instance().getAllIncidents();

    QMap<QString, int> count;
    for (const auto &m : list) {
        count[m["statut"].toString()]++;
    }

    QPieSeries *series = new QPieSeries();
    for (auto it = count.constBegin(); it != count.constEnd(); ++it) {
        series->append(it.key(), it.value());
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des incidents");
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);

    ui->chart->setChart(chart);
    ui->chart->setRenderHint(QPainter::Antialiasing);
}

void IncidentsPage::clearForm()
{
    ui->typeCombo->setCurrentIndex(0);
    ui->localisationEdit->clear();
    ui->dateHeureEdit->setDateTime(QDateTime::currentDateTime());
    ui->niveauCombo->setCurrentIndex(0);
    ui->statutCombo->setCurrentIndex(0);
    ui->idREdit->clear();
    ui->errorLabel->setText("");
    currentEditId = -1;
}

int IncidentsPage::getNextId()
{
    if (!Connection::isOpen()) {
        qWarning() << "[IncidentsPage] Connexion non ouverte pour getNextId";
        return 1;
    }

    QSqlQuery q(Connection::getInstance().getDatabase());
    if (!q.exec("SELECT NVL(MAX(ID_INCIDENT), 0) + 1 FROM INCIDENTS")) {
        qCritical() << "[IncidentsPage] ❌ Échec getNextId:" << q.lastError().text();
        return 1;
    }

    if (q.next()) {
        return q.value(0).toInt();
    }
    return 1;
}

void IncidentsPage::onSortIndexChanged(int index)
{
    QTableWidget *t = ui->incidentTable;
    if (!t || t->rowCount() == 0) {
        return;
    }

    int column = -1;
    switch (index) {
    case 0: // Trier par date
        column = 3;
        break;
    case 1: // Trier par type
        column = 1;
        break;
    case 2: // Trier par niveau
        column = 4;
        break;
    case 3: // Trier par statut
        column = 5;
        break;
    case 4: // Trier par CIN résident
        column = 6;
        break;
    default:
        return;
    }

    if (column >= 0) {
        t->setSortingEnabled(true);
        t->sortItems(column, Qt::AscendingOrder);
    }
}
