#include "incidentspage.h"
#include "ui_incidentspage.h"
#include "databasemanager.h"
#include "incidentdialog.h"
#include <QMessageBox>
#include <QHeaderView>
#include <QDebug>

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
    ui->incidentsTable->setColumnWidth(2, 150);  // Localisation
    ui->incidentsTable->setColumnWidth(3, 180);  // Date & Heure
    ui->incidentsTable->setColumnWidth(4, 100);  // Gravité
}

void IncidentsPage::loadIncidents()
{
    ui->incidentsTable->setRowCount(0);
    
    // TODO: Load from database when CRUD methods are implemented
    // For now, just setup the table structure
}

void IncidentsPage::refreshStatistics()
{
    // TODO: Calculate statistics when database methods are ready
    ui->totalLabel->setText("Total: 0");
    ui->traitesLabel->setText("✅ Traités: 0");
    ui->enAttenteLabel->setText("⏳ En Attente: 0");
    ui->enCoursLabel->setText("🔄 En Cours: 0");
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
    IncidentDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QVariantMap data = dialog.getIncidentData();

        if (!DatabaseManager::instance().addIncident(data)) {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la déclaration d'incident.");
        } else {
            QMessageBox::information(this, "Succès", "Incident déclaré avec succès!");
            loadIncidents();
            refreshStatistics();
            refreshDashboard();
        }
    }
}

void IncidentsPage::on_searchButton_clicked()
{
    QString searchText = ui->searchLineEdit->text();
    loadIncidents();
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
    QMessageBox::information(this, "Incidents", "Modification d'incident - À implémenter");
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
        // TODO: Delete from database
        QMessageBox::information(this, "Incidents", "Incident supprimé");
    }
}

void IncidentsPage::on_exportPDFButton_clicked()
{
    QMessageBox::information(this, "Export", "Export PDF des incidents - À implémenter");
}
