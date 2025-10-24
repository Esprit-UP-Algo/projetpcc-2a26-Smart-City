#include "transportspage.h"
#include "ui_transportspage.h"
#include "databasemanager.h"
#include "vehiculedialog.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QHeaderView>
#include <QDebug>

TransportsPage::TransportsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TransportsPage)
{
    ui->setupUi(this);

    // Update statistics card title for mileage tracking
    ui->statsCardTitle->setText("📊 Répartition du kilométrage (%)");

    setupTable();
    loadVehicules();
    refreshStatistics();
    refreshFleetInfo();
}

TransportsPage::~TransportsPage()
{
    delete ui;
}

void TransportsPage::setupTable()
{
    ui->vehiculesTable->horizontalHeader()->setStretchLastSection(true);
    ui->vehiculesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->vehiculesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->vehiculesTable->setAlternatingRowColors(true);

    // Set table headers programmatically
    QStringList headers;
    headers << "ID" << "Type" << "Capacité" << "Zone" << "Statut" << "Date";
    ui->vehiculesTable->setHorizontalHeaderLabels(headers);

    // Set column widths
    ui->vehiculesTable->setColumnWidth(0, 100);  // ID
    ui->vehiculesTable->setColumnWidth(1, 120);  // Type
    ui->vehiculesTable->setColumnWidth(2, 100);  // Capacité
    ui->vehiculesTable->setColumnWidth(3, 150);  // Zone
    ui->vehiculesTable->setColumnWidth(4, 120);  // Statut
    ui->vehiculesTable->setColumnWidth(5, 120);  // Date
}

void TransportsPage::loadVehicules()
{
    ui->vehiculesTable->setRowCount(0);
    // Table structure is ready for future database integration
}

void TransportsPage::refreshStatistics()
{
    ui->tauxUtilisationLabel->setText("Kilométrage total : 0 km");
    ui->kmTotalLabel->setText("Véhicule le plus utilisé : —");
    ui->tempsMoyenLabel->setText("Répartition moyenne : — %");
}

void TransportsPage::refreshFleetInfo()
{
    ui->totalVehiculesLabel->setText("🚗 Total véhicules: 0");
    ui->disponiblesLabel->setText("✅ Disponibles: 0");
    ui->reservesLabel->setText("🔒 Réservés: 0");
    ui->maintenanceLabel->setText("🔧 Maintenance: 0");
}

void TransportsPage::setupChart()
{
    // TODO: Setup pie chart for fleet status distribution
}

void TransportsPage::on_addVehiculeButton_clicked()
{

    VehiculeDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QVariantMap data = dialog.getVehiculeData();

        if (!DatabaseManager::instance().addVehicule(data)) {
            QMessageBox::critical(this, "Erreur", "Erreur lors de l'ajout du véhicule.");
        } else {
            QMessageBox::information(this, "Succès", "Véhicule ajouté avec succès!");
            loadVehicules();
            refreshStatistics();
            refreshFleetInfo();
        }
    }
}

void TransportsPage::on_searchButton_clicked()
{
    QString searchText = ui->searchLineEdit->text();
    loadVehicules();
}

void TransportsPage::on_sortComboBox_currentIndexChanged(int /* index */)
{
    loadVehicules();
}

void TransportsPage::on_modifyButton_clicked()
{
    int row = ui->vehiculesTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Transports", "Veuillez sélectionner un véhicule à modifier");
        return;
    }
    QMessageBox::information(this, "Transports", "Modification de véhicule - À implémenter");
}

void TransportsPage::on_deleteButton_clicked()
{
    int row = ui->vehiculesTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Transports", "Veuillez sélectionner un véhicule à supprimer");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Confirmation",
        "Êtes-vous sûr de vouloir supprimer ce véhicule?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        // TODO: Delete from database
        QMessageBox::information(this, "Transports", "Véhicule supprimé");
    }
}

void TransportsPage::on_exportPDFButton_clicked()
{
    QMessageBox::information(this, "Export", "Export PDF de la flotte - À implémenter");
}

void TransportsPage::on_historiqueButton_clicked()
{
    QMessageBox::information(this, "Historique des Véhicules",
                             "Affichage de l'historique détaillé des véhicules et de leurs trajets.\n\n"
                             "Cette fonctionnalité permet de consulter :\n"
                             "• Historique complet des véhicules\n"
                             "• Analyse des trajets par date\n"
                             "• Statistiques d'utilisation\n"
                             "• Rapports de maintenance\n\n"
                             "Fonctionnalité à implémenter complètement.");
}
