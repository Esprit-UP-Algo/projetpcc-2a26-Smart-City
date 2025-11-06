#include "incidentspage.h"
#include "ui_incidentspage.h"
#include "databasemanager.h"
#include "incidentdialog.h"
#include <QMessageBox>
#include <QHeaderView>
#include <QDebug>
#include <QTableWidgetItem>

IncidentsPage::IncidentsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IncidentsPage)
{
    ui->setupUi(this);
    setupTable();
    loadIncidents();
    refreshStatistics();
    refreshDashboard();
}

IncidentsPage::~IncidentsPage()
{
    delete ui;
}

void IncidentsPage::setupTable()
{
    ui->incidentsTable->horizontalHeader()->setStretchLastSection(true);
    ui->incidentsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->incidentsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->incidentsTable->setAlternatingRowColors(true);

    // Set column widths
    ui->incidentsTable->setColumnWidth(0, 80);   // ID
    ui->incidentsTable->setColumnWidth(1, 150);  // Type
    ui->incidentsTable->setColumnWidth(2, 200);  // Localisation
    ui->incidentsTable->setColumnWidth(3, 150);  // Date & Heure
    ui->incidentsTable->setColumnWidth(4, 100);  // Gravité
}

void IncidentsPage::loadIncidents()
{
    ui->incidentsTable->setRowCount(0);

    QList<QVariantMap> incidents = DatabaseManager::instance().getAllIncidents();

    qDebug() << "[IncidentsPage] Loading" << incidents.size() << "incidents";

    for (const QVariantMap &incident : incidents) {
        int row = ui->incidentsTable->rowCount();
        ui->incidentsTable->insertRow(row);

        // ID
        QTableWidgetItem *idItem = new QTableWidgetItem(incident["id"].toString());
        idItem->setTextAlignment(Qt::AlignCenter);
        ui->incidentsTable->setItem(row, 0, idItem);

        // Type
        QTableWidgetItem *typeItem = new QTableWidgetItem(incident["type"].toString());
        ui->incidentsTable->setItem(row, 1, typeItem);

        // Localisation
        QTableWidgetItem *locItem = new QTableWidgetItem(incident["localisation"].toString());
        ui->incidentsTable->setItem(row, 2, locItem);

        // Date & Heure
        QTableWidgetItem *dateItem = new QTableWidgetItem(incident["date_heure"].toString());
        ui->incidentsTable->setItem(row, 3, dateItem);

        // Gravité
        int gravite = incident["niveau_gravite"].toInt();
        QString graviteText = QString::number(gravite);
        QTableWidgetItem *graviteItem = new QTableWidgetItem(graviteText);
        graviteItem->setTextAlignment(Qt::AlignCenter);

        // Color code by severity
        if (gravite >= 4) {
            graviteItem->setBackground(QColor(231, 76, 60)); // Red
            graviteItem->setForeground(QColor(255, 255, 255));
        } else if (gravite == 3) {
            graviteItem->setBackground(QColor(243, 156, 18)); // Orange
            graviteItem->setForeground(QColor(255, 255, 255));
        } else {
            graviteItem->setBackground(QColor(46, 204, 113)); // Green
            graviteItem->setForeground(QColor(255, 255, 255));
        }
        ui->incidentsTable->setItem(row, 4, graviteItem);

        // Statut
        QString statut = incident["statut"].toString();
        QTableWidgetItem *statutItem = new QTableWidgetItem(statut);
        statutItem->setTextAlignment(Qt::AlignCenter);

        // Color code by status
        if (statut == "Traité") {
            statutItem->setBackground(QColor(39, 174, 96)); // Green
            statutItem->setForeground(QColor(255, 255, 255));
        } else if (statut == "En Cours") {
            statutItem->setBackground(QColor(243, 156, 18)); // Orange
            statutItem->setForeground(QColor(255, 255, 255));
        } else {
            statutItem->setBackground(QColor(231, 76, 60)); // Red
            statutItem->setForeground(QColor(255, 255, 255));
        }
        ui->incidentsTable->setItem(row, 5, statutItem);
    }

    qDebug() << "[IncidentsPage] Table loaded with" << ui->incidentsTable->rowCount() << "rows";
}

void IncidentsPage::refreshStatistics()
{
    QVariantMap stats = DatabaseManager::instance().getIncidentStatistics();

    int total = stats.value("total", 0).toInt();
    int traites = stats.value("traites", 0).toInt();
    int enAttente = stats.value("en_attente", 0).toInt();
    int enCours = stats.value("en_cours", 0).toInt();

    ui->totalLabel->setText(QString("Total: %1").arg(total));
    ui->traitesLabel->setText(QString("✅ Traités: %1").arg(traites));
    ui->enAttenteLabel->setText(QString("⏳ En Attente: %1").arg(enAttente));
    ui->enCoursLabel->setText(QString("🔄 En Cours: %1").arg(enCours));

    qDebug() << "[IncidentsPage] Statistics updated - Total:" << total
             << "Traités:" << traites << "En Attente:" << enAttente << "En Cours:" << enCours;
}

void IncidentsPage::refreshDashboard()
{
    // Update dashboard metrics if needed
}

void IncidentsPage::setupChart()
{
    // Chart setup placeholder
}

void IncidentsPage::on_addIncidentButton_clicked()
{
    qDebug() << "[IncidentsPage] Add incident button clicked";

    IncidentDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QVariantMap data = dialog.getIncidentData();

        qDebug() << "[IncidentsPage] Saving incident data:" << data;

        if (!DatabaseManager::instance().addIncident(data)) {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la déclaration d'incident.");
            qWarning() << "[IncidentsPage] Failed to add incident";
        } else {
            QMessageBox::information(this, "Succès", "Incident déclaré avec succès!");
            qDebug() << "[IncidentsPage] Incident added successfully";

            // Refresh the display
            loadIncidents();
            refreshStatistics();
            refreshDashboard();
        }
    } else {
        qDebug() << "[IncidentsPage] Dialog cancelled";
    }
}

void IncidentsPage::on_searchButton_clicked()
{
    QString searchText = ui->searchLineEdit->text().trimmed();

    if (searchText.isEmpty()) {
        loadIncidents();
        return;
    }

    qDebug() << "[IncidentsPage] Searching for:" << searchText;

    ui->incidentsTable->setRowCount(0);

    QList<QVariantMap> incidents = DatabaseManager::instance().searchIncidents(searchText);

    for (const QVariantMap &incident : incidents) {
        int row = ui->incidentsTable->rowCount();
        ui->incidentsTable->insertRow(row);

        // ID
        QTableWidgetItem *idItem = new QTableWidgetItem(incident["id"].toString());
        idItem->setTextAlignment(Qt::AlignCenter);
        ui->incidentsTable->setItem(row, 0, idItem);

        // Type
        ui->incidentsTable->setItem(row, 1, new QTableWidgetItem(incident["type"].toString()));

        // Localisation
        ui->incidentsTable->setItem(row, 2, new QTableWidgetItem(incident["localisation"].toString()));

        // Date & Heure
        ui->incidentsTable->setItem(row, 3, new QTableWidgetItem(incident["date_heure"].toString()));

        // Gravité
        int gravite = incident["niveau_gravite"].toInt();
        QTableWidgetItem *graviteItem = new QTableWidgetItem(QString::number(gravite));
        graviteItem->setTextAlignment(Qt::AlignCenter);

        if (gravite >= 4) {
            graviteItem->setBackground(QColor(231, 76, 60));
            graviteItem->setForeground(QColor(255, 255, 255));
        } else if (gravite == 3) {
            graviteItem->setBackground(QColor(243, 156, 18));
            graviteItem->setForeground(QColor(255, 255, 255));
        } else {
            graviteItem->setBackground(QColor(46, 204, 113));
            graviteItem->setForeground(QColor(255, 255, 255));
        }
        ui->incidentsTable->setItem(row, 4, graviteItem);

        // Statut
        QString statut = incident["statut"].toString();
        QTableWidgetItem *statutItem = new QTableWidgetItem(statut);
        statutItem->setTextAlignment(Qt::AlignCenter);

        if (statut == "Traité") {
            statutItem->setBackground(QColor(39, 174, 96));
            statutItem->setForeground(QColor(255, 255, 255));
        } else if (statut == "En Cours") {
            statutItem->setBackground(QColor(243, 156, 18));
            statutItem->setForeground(QColor(255, 255, 255));
        } else {
            statutItem->setBackground(QColor(231, 76, 60));
            statutItem->setForeground(QColor(255, 255, 255));
        }
        ui->incidentsTable->setItem(row, 5, statutItem);
    }
}

void IncidentsPage::on_sortComboBox_currentIndexChanged(int /* index */)
{
    loadIncidents();
}

void IncidentsPage::on_modifyButton_clicked()
{
    int row = ui->incidentsTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Incidents", "Veuillez sélectionner un incident à modifier");
        return;
    }

    // Get incident ID from the first column
    int id = ui->incidentsTable->item(row, 0)->text().toInt();

    qDebug() << "[IncidentsPage] Modifying incident ID:" << id;

    // Fetch incident data from database
    QVariantMap incidentData = DatabaseManager::instance().getIncident(id);

    if (incidentData.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Impossible de charger les données de l'incident");
        return;
    }

    IncidentDialog dialog(incidentData, this);
    if (dialog.exec() == QDialog::Accepted) {
        QVariantMap data = dialog.getIncidentData();

        if (DatabaseManager::instance().updateIncident(id, data)) {
            QMessageBox::information(this, "Succès", "Incident modifié avec succès!");
            loadIncidents();
            refreshStatistics();
        } else {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la modification de l'incident");
        }
    }
}

void IncidentsPage::on_deleteButton_clicked()
{
    int row = ui->incidentsTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Incidents", "Veuillez sélectionner un incident à supprimer");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Confirmation",
        "Êtes-vous sûr de vouloir supprimer cet incident?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        int id = ui->incidentsTable->item(row, 0)->text().toInt();

        qDebug() << "[IncidentsPage] Deleting incident ID:" << id;

        if (DatabaseManager::instance().deleteIncident(id)) {
            QMessageBox::information(this, "Succès", "Incident supprimé avec succès!");
            loadIncidents();
            refreshStatistics();
        } else {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression de l'incident");
        }
    }
}

void IncidentsPage::on_exportPDFButton_clicked()
{
    QMessageBox::information(this, "Export", "Export PDF des incidents - À implémenter");
}
