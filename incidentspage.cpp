#include "incidentspage.h"
#include "ui_incidentspage.h"
#include "databasemanager.h"
#include "incidentdialog.h"
#include <QMessageBox>
#include <QHeaderView>

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

    ui->incidentsTable->setColumnWidth(0, 80);
    ui->incidentsTable->setColumnWidth(1, 150);
    ui->incidentsTable->setColumnWidth(2, 150);
    ui->incidentsTable->setColumnWidth(3, 180);
    ui->incidentsTable->setColumnWidth(4, 100);
}

void IncidentsPage::loadIncidents(const QString &term)
{
    ui->incidentsTable->setRowCount(0);
    
    // Utiliser IncidentDialog pour récupérer les incidents
    IncidentDialog dialog;
    QList<QVariantMap> incidents;
    
    // Essayer d'abord Oracle, puis SQLite en fallback
    incidents = dialog.listerIncidentsOracle();
    if (incidents.isEmpty()) {
        incidents = dialog.listerIncidentsSQLite();
    }

    for (const QVariantMap &inc : incidents) {
        // Filtrage par terme de recherche
        if (!term.isEmpty()) {
            bool matchesSearch = inc["categorie"].toString().contains(term, Qt::CaseInsensitive) ||
                                inc["description"].toString().contains(term, Qt::CaseInsensitive) ||
                                inc["etat"].toString().contains(term, Qt::CaseInsensitive);
            if (!matchesSearch) {
                continue;
            }
        }

        int row = ui->incidentsTable->rowCount();
        ui->incidentsTable->insertRow(row);
        ui->incidentsTable->setItem(row, 0, new QTableWidgetItem(QString::number(inc["id"].toInt())));
        ui->incidentsTable->setItem(row, 1, new QTableWidgetItem(inc["categorie"].toString()));
        ui->incidentsTable->setItem(row, 2, new QTableWidgetItem(inc["description"].toString()));
        ui->incidentsTable->setItem(row, 3, new QTableWidgetItem(inc["date"].toString()));
        ui->incidentsTable->setItem(row, 4, new QTableWidgetItem(inc["etat"].toString()));
    }
}

void IncidentsPage::refreshStatistics()
{
    // Utiliser IncidentDialog pour récupérer les incidents
    IncidentDialog dialog;
    QList<QVariantMap> incidents;
    
    // Essayer d'abord Oracle, puis SQLite en fallback
    incidents = dialog.listerIncidentsOracle();
    if (incidents.isEmpty()) {
        incidents = dialog.listerIncidentsSQLite();
    }
    
    int total = incidents.size();
    int traites = 0;
    int enAttente = 0;
    int enCours = 0;
    
    for (const QVariantMap &incident : incidents) {
        QString etat = incident["etat"].toString();
        if (etat == "Traité") {
            traites++;
        } else if (etat == "En Attente") {
            enAttente++;
        } else if (etat == "En Cours") {
            enCours++;
        }
    }
    
    ui->totalLabel->setText(QString("Total: %1").arg(total));
    ui->traitesLabel->setText(QString("✅ Traités: %1").arg(traites));
    ui->enAttenteLabel->setText(QString("⏳ En Attente: %1").arg(enAttente));
    ui->enCoursLabel->setText(QString("🔄 En Cours: %1").arg(enCours));
}

void IncidentsPage::refreshDashboard() {}
void IncidentsPage::setupChart() {}

void IncidentsPage::on_addIncidentButton_clicked()
{
    IncidentDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        // L'incident a déjà été ajouté à Oracle dans le dialog
        QMessageBox::information(this, "Succès", "Incident déclaré avec succès dans Oracle!");
        loadIncidents();
        refreshStatistics();
    }
}

void IncidentsPage::on_searchButton_clicked()
{
    QString searchText = ui->searchLineEdit->text();
    loadIncidents(searchText);
}

void IncidentsPage::on_sortComboBox_currentIndexChanged(int index) 
{ 
    QString filter;
    switch(index) {
        case 1: filter = "En Attente"; break;
        case 2: filter = "En Cours"; break;
        case 3: filter = "Traité"; break;
        default: filter = ""; break;
    }
    loadIncidents(filter);
}

void IncidentsPage::on_modifyButton_clicked()
{
    int row = ui->incidentsTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Incidents", "Veuillez sélectionner un incident à modifier");
        return;
    }

    int id = ui->incidentsTable->item(row, 0)->text().toInt();
    
    // Récupérer les données de l'incident depuis Oracle
    IncidentDialog dialog;
    QList<QVariantMap> incidents = dialog.listerIncidentsOracle();
    QVariantMap incidentData;
    
    for (const QVariantMap &inc : incidents) {
        if (inc["id"].toInt() == id) {
            incidentData = inc;
            break;
        }
    }
    
    if (incidentData.isEmpty()) {
        QMessageBox::critical(this, "Erreur", "Impossible de récupérer les données de l'incident.");
        return;
    }

    IncidentDialog editDialog(incidentData, this);
    if (editDialog.exec() == QDialog::Accepted) {
        if (editDialog.modifierIncidentOracle(id)) {
            QMessageBox::information(this, "Succès", "Incident modifié avec succès dans Oracle!");
            loadIncidents();
            refreshStatistics();
        } else {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la modification de l'incident dans Oracle.");
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

    int id = ui->incidentsTable->item(row, 0)->text().toInt();

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Confirmation",
        "Êtes-vous sûr de vouloir supprimer cet incident?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        IncidentDialog dialog;
        if (dialog.supprimerIncidentOracle(id)) {
            QMessageBox::information(this, "Incidents", "Incident supprimé d'Oracle avec succès!");
            loadIncidents();
            refreshStatistics();
        } else {
            QMessageBox::critical(this, "Erreur", "Impossible de supprimer l'incident d'Oracle.");
        }
    }
}

void IncidentsPage::on_exportPDFButton_clicked()
{
    QMessageBox::information(this, "Export", "Export PDF des incidents - À implémenter");
}
