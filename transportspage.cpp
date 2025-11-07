#include "transportspage.h"
#include "ui_transportspage.h"
#include "databasemanager.h"
#include "vehiculedialog.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QHeaderView>
#include <QDebug>
#include <QTableWidgetItem>
#include <algorithm>

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

    // Set table headers - AJOUT DE LA COLONNE TEMPS UTILISÉ
    QStringList headers;
    headers << "ID" << "Type" << "Capacité" << "Zone" << "Horaire" << "Statut" << "Date" << "Temps utilisé";
    ui->vehiculesTable->setColumnCount(headers.size());
    ui->vehiculesTable->setHorizontalHeaderLabels(headers);

    // Set column widths
    ui->vehiculesTable->setColumnWidth(0, 100);  // ID
    ui->vehiculesTable->setColumnWidth(1, 120);  // Type
    ui->vehiculesTable->setColumnWidth(2, 100);  // Capacité
    ui->vehiculesTable->setColumnWidth(3, 150);  // Zone
    ui->vehiculesTable->setColumnWidth(4, 120);  // Horaire
    ui->vehiculesTable->setColumnWidth(5, 120);  // Statut
    ui->vehiculesTable->setColumnWidth(6, 120);  // Date
    ui->vehiculesTable->setColumnWidth(7, 120);  // Temps utilisé (NOUVELLE COLONNE)

    // Configure le comboBox de tri - COMBOBOX COMME À L'AVANCE
    ui->sortComboBox->clear();
    ui->sortComboBox->addItem("Trier par...");
    ui->sortComboBox->addItem("Type");
    ui->sortComboBox->addItem("Statut");
    ui->sortComboBox->addItem("Kilométrage");
}

void TransportsPage::loadVehicules()
{
    ui->vehiculesTable->setRowCount(0);
    QString searchTerm = ui->searchLineEdit->text().trimmed();
    QList<QVariantMap> vehicules;

    if (searchTerm.isEmpty()) {
        vehicules = DatabaseManager::instance().getAllVehicules();
    } else {
        vehicules = DatabaseManager::instance().searchVehicules(searchTerm);
    }

    // Appliquer le tri selon l'option sélectionnée
    int sortIndex = ui->sortComboBox->currentIndex();
    if (sortIndex == 1) { // Tri par type
        sortVehiculesByType();
        return;
    } else if (sortIndex == 2) { // Tri par statut
        sortVehiculesByStatus();
        return;
    } else if (sortIndex == 3) { // Tri par kilométrage - NE RIEN FAIRE
        // Le tableau reste tel quel, pas de tri appliqué
        qDebug() << "[TransportsPage] Tri par kilométrage sélectionné - aucun tri appliqué";
    }

    // Tri par défaut (par ID) si aucune option de tri spécifique
    std::sort(vehicules.begin(), vehicules.end(),
              [](const QVariantMap &a, const QVariantMap &b) {
                  return a["id"].toString() < b["id"].toString();
              });

    qDebug() << "[TransportsPage] Loading" << vehicules.size() << "vehicules";

    for (const QVariantMap &v : vehicules) {
        int row = ui->vehiculesTable->rowCount();
        ui->vehiculesTable->insertRow(row);

        // Ajout des données dans chaque colonne
        ui->vehiculesTable->setItem(row, 0, new QTableWidgetItem(v["id"].toString()));
        ui->vehiculesTable->setItem(row, 1, new QTableWidgetItem(v["type"].toString()));
        ui->vehiculesTable->setItem(row, 2, new QTableWidgetItem(QString::number(v["capacite"].toInt())));
        ui->vehiculesTable->setItem(row, 3, new QTableWidgetItem(v["zone"].toString()));
        ui->vehiculesTable->setItem(row, 4, new QTableWidgetItem(v["horaire"].toString()));
        ui->vehiculesTable->setItem(row, 5, new QTableWidgetItem(v["statut"].toString()));
        ui->vehiculesTable->setItem(row, 6, new QTableWidgetItem(v["date"].toString()));

        // NOUVELLE COLONNE : Temps utilisé
        double tempsUtilise = v["temps_utilise"].toDouble();
        QTableWidgetItem *tempsItem = new QTableWidgetItem(QString("%1 h").arg(tempsUtilise, 0, 'f', 1));
        ui->vehiculesTable->setItem(row, 7, tempsItem);

        // Style the status cell based on status
        QTableWidgetItem *statusItem = ui->vehiculesTable->item(row, 5);
        QString statut = v["statut"].toString();
        if (statut == "Disponible") {
            statusItem->setBackground(QColor(232, 248, 245)); // Light green
            statusItem->setForeground(QColor(39, 174, 96));
        } else if (statut == "Réservé") {
            statusItem->setBackground(QColor(254, 245, 231)); // Light orange
            statusItem->setForeground(QColor(243, 156, 18));
        } else if (statut == "Maintenance") {
            statusItem->setBackground(QColor(250, 219, 216)); // Light red
            statusItem->setForeground(QColor(231, 76, 60));
        }

        // Style pour la colonne temps utilisé (optionnel)
        if (tempsUtilise > 50) {
            tempsItem->setBackground(QColor(255, 255, 200)); // Jaune clair pour temps élevé
            tempsItem->setForeground(QColor(230, 126, 34));
        }
    }
}

void TransportsPage::sortVehiculesByType()
{
    QList<QVariantMap> vehicules;

    // Récupérer les véhicules selon la recherche
    QString searchTerm = ui->searchLineEdit->text().trimmed();
    if (searchTerm.isEmpty()) {
        vehicules = DatabaseManager::instance().getAllVehicules();
    } else {
        vehicules = DatabaseManager::instance().searchVehicules(searchTerm);
    }

    // Trier par ordre spécifique: Taxi, Voiture, Bus
    QList<QVariantMap> taxis;
    QList<QVariantMap> voitures;
    QList<QVariantMap> bus;
    QList<QVariantMap> autres;

    // Séparer les véhicules par type
    for (const QVariantMap &v : vehicules) {
        QString type = v["type"].toString().toLower();
        if (type.contains("taxi")) {
            taxis.append(v);
        } else if (type.contains("voiture")) {
            voitures.append(v);
        } else if (type.contains("bus")) {
            bus.append(v);
        } else {
            autres.append(v);
        }
    }

    // Trier chaque catégorie par ID
    auto sortById = [](const QVariantMap &a, const QVariantMap &b) {
        return a["id"].toString() < b["id"].toString();
    };

    std::sort(taxis.begin(), taxis.end(), sortById);
    std::sort(voitures.begin(), voitures.end(), sortById);
    std::sort(bus.begin(), bus.end(), sortById);
    std::sort(autres.begin(), autres.end(), sortById);

    // Combiner dans l'ordre souhaité
    QList<QVariantMap> vehiculesTries;
    vehiculesTries.append(taxis);
    vehiculesTries.append(voitures);
    vehiculesTries.append(bus);
    vehiculesTries.append(autres);

    // Mettre à jour le tableau avec les véhicules triés
    ui->vehiculesTable->setRowCount(0);
    for (const QVariantMap &v : vehiculesTries) {
        int row = ui->vehiculesTable->rowCount();
        ui->vehiculesTable->insertRow(row);

        ui->vehiculesTable->setItem(row, 0, new QTableWidgetItem(v["id"].toString()));
        ui->vehiculesTable->setItem(row, 1, new QTableWidgetItem(v["type"].toString()));
        ui->vehiculesTable->setItem(row, 2, new QTableWidgetItem(QString::number(v["capacite"].toInt())));
        ui->vehiculesTable->setItem(row, 3, new QTableWidgetItem(v["zone"].toString()));
        ui->vehiculesTable->setItem(row, 4, new QTableWidgetItem(v["horaire"].toString()));
        ui->vehiculesTable->setItem(row, 5, new QTableWidgetItem(v["statut"].toString()));
        ui->vehiculesTable->setItem(row, 6, new QTableWidgetItem(v["date"].toString()));

        double tempsUtilise = v["temps_utilise"].toDouble();
        QTableWidgetItem *tempsItem = new QTableWidgetItem(QString("%1 h").arg(tempsUtilise, 0, 'f', 1));
        ui->vehiculesTable->setItem(row, 7, tempsItem);

        // Appliquer les styles
        QTableWidgetItem *statusItem = ui->vehiculesTable->item(row, 5);
        QString statut = v["statut"].toString();
        if (statut == "Disponible") {
            statusItem->setBackground(QColor(232, 248, 245));
            statusItem->setForeground(QColor(39, 174, 96));
        } else if (statut == "Réservé") {
            statusItem->setBackground(QColor(254, 245, 231));
            statusItem->setForeground(QColor(243, 156, 18));
        } else if (statut == "Maintenance") {
            statusItem->setBackground(QColor(250, 219, 216));
            statusItem->setForeground(QColor(231, 76, 60));
        }

        if (tempsUtilise > 50) {
            tempsItem->setBackground(QColor(255, 255, 200));
            tempsItem->setForeground(QColor(230, 126, 34));
        }
    }

    qDebug() << "[TransportsPage] Tri par type appliqué:";
    qDebug() << "Taxis:" << taxis.size();
    qDebug() << "Voitures:" << voitures.size();
    qDebug() << "Bus:" << bus.size();
    qDebug() << "Autres:" << autres.size();
}

void TransportsPage::sortVehiculesByStatus()
{
    QList<QVariantMap> vehicules;

    // Récupérer les véhicules selon la recherche
    QString searchTerm = ui->searchLineEdit->text().trimmed();
    if (searchTerm.isEmpty()) {
        vehicules = DatabaseManager::instance().getAllVehicules();
    } else {
        vehicules = DatabaseManager::instance().searchVehicules(searchTerm);
    }

    // Trier par ordre spécifique: Disponible, Réservé, Maintenance
    QList<QVariantMap> disponibles;
    QList<QVariantMap> reserves;
    QList<QVariantMap> maintenance;
    QList<QVariantMap> autresStatuts;

    // Séparer les véhicules par statut
    for (const QVariantMap &v : vehicules) {
        QString statut = v["statut"].toString();
        if (statut == "Disponible") {
            disponibles.append(v);
        } else if (statut == "Réservé") {
            reserves.append(v);
        } else if (statut == "Maintenance") {
            maintenance.append(v);
        } else {
            autresStatuts.append(v);
        }
    }

    // Trier chaque catégorie par ID
    auto sortById = [](const QVariantMap &a, const QVariantMap &b) {
        return a["id"].toString() < b["id"].toString();
    };

    std::sort(disponibles.begin(), disponibles.end(), sortById);
    std::sort(reserves.begin(), reserves.end(), sortById);
    std::sort(maintenance.begin(), maintenance.end(), sortById);
    std::sort(autresStatuts.begin(), autresStatuts.end(), sortById);

    // Combiner dans l'ordre souhaité
    QList<QVariantMap> vehiculesTries;
    vehiculesTries.append(disponibles);
    vehiculesTries.append(reserves);
    vehiculesTries.append(maintenance);
    vehiculesTries.append(autresStatuts);

    // Mettre à jour le tableau avec les véhicules triés
    ui->vehiculesTable->setRowCount(0);
    for (const QVariantMap &v : vehiculesTries) {
        int row = ui->vehiculesTable->rowCount();
        ui->vehiculesTable->insertRow(row);

        ui->vehiculesTable->setItem(row, 0, new QTableWidgetItem(v["id"].toString()));
        ui->vehiculesTable->setItem(row, 1, new QTableWidgetItem(v["type"].toString()));
        ui->vehiculesTable->setItem(row, 2, new QTableWidgetItem(QString::number(v["capacite"].toInt())));
        ui->vehiculesTable->setItem(row, 3, new QTableWidgetItem(v["zone"].toString()));
        ui->vehiculesTable->setItem(row, 4, new QTableWidgetItem(v["horaire"].toString()));
        ui->vehiculesTable->setItem(row, 5, new QTableWidgetItem(v["statut"].toString()));
        ui->vehiculesTable->setItem(row, 6, new QTableWidgetItem(v["date"].toString()));

        double tempsUtilise = v["temps_utilise"].toDouble();
        QTableWidgetItem *tempsItem = new QTableWidgetItem(QString("%1 h").arg(tempsUtilise, 0, 'f', 1));
        ui->vehiculesTable->setItem(row, 7, tempsItem);

        // Appliquer les styles
        QTableWidgetItem *statusItem = ui->vehiculesTable->item(row, 5);
        QString statut = v["statut"].toString();
        if (statut == "Disponible") {
            statusItem->setBackground(QColor(232, 248, 245));
            statusItem->setForeground(QColor(39, 174, 96));
        } else if (statut == "Réservé") {
            statusItem->setBackground(QColor(254, 245, 231));
            statusItem->setForeground(QColor(243, 156, 18));
        } else if (statut == "Maintenance") {
            statusItem->setBackground(QColor(250, 219, 216));
            statusItem->setForeground(QColor(231, 76, 60));
        }

        if (tempsUtilise > 50) {
            tempsItem->setBackground(QColor(255, 255, 200));
            tempsItem->setForeground(QColor(230, 126, 34));
        }
    }

    qDebug() << "[TransportsPage] Tri par statut appliqué:";
    qDebug() << "Disponibles:" << disponibles.size();
    qDebug() << "Réservés:" << reserves.size();
    qDebug() << "Maintenance:" << maintenance.size();
    qDebug() << "Autres statuts:" << autresStatuts.size();
}

void TransportsPage::refreshStatistics()
{
    QList<QVariantMap> vehicules = DatabaseManager::instance().getAllVehicules();
    if (vehicules.isEmpty()) {
        ui->tauxUtilisationLabel->setText("Temps total : 0 h");
        ui->kmTotalLabel->setText("Véhicule le plus utilisé : —");
        ui->tempsMoyenLabel->setText("Temps moyen : 0 h");
        return;
    }

    // Calculate statistics
    double totalTempsUtilise = 0;
    QString mostUsedVehicle;
    double maxTemps = 0;

    for (const QVariantMap &v : vehicules) {
        double temps = v["temps_utilise"].toDouble();
        totalTempsUtilise += temps;
        if (temps > maxTemps) {
            maxTemps = temps;
            mostUsedVehicle = v["id"].toString();
        }
    }

    double avgTemps = vehicules.size() > 0 ? totalTempsUtilise / vehicules.size() : 0;

    ui->tauxUtilisationLabel->setText(QString("Temps total d'utilisation : %1 h").arg(totalTempsUtilise, 0, 'f', 1));
    ui->kmTotalLabel->setText(QString("Véhicule le plus utilisé : %1 (%2 h)")
                                  .arg(mostUsedVehicle.isEmpty() ? "—" : mostUsedVehicle)
                                  .arg(maxTemps, 0, 'f', 1));
    ui->tempsMoyenLabel->setText(QString("Temps moyen par véhicule : %1 h").arg(avgTemps, 0, 'f', 1));
}

void TransportsPage::refreshFleetInfo()
{
    QList<QVariantMap> vehicules = DatabaseManager::instance().getAllVehicules();
    int total = vehicules.size();
    int disponibles = 0;
    int reserves = 0;
    int maintenance = 0;

    for (const QVariantMap &v : vehicules) {
        QString statut = v["statut"].toString();
        if (statut == "Disponible") disponibles++;
        else if (statut == "Réservé") reserves++;
        else if (statut == "Maintenance") maintenance++;
    }

    ui->totalVehiculesLabel->setText(QString("🚗 Total véhicules: %1").arg(total));
    ui->disponiblesLabel->setText(QString("✅ Disponibles: %1").arg(disponibles));
    ui->reservesLabel->setText(QString("🔒 Réservés: %1").arg(reserves));
    ui->maintenanceLabel->setText(QString("🔧 Maintenance: %1").arg(maintenance));
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
        qDebug() << "[TransportsPage] Adding vehicule with data:" << data;
        if (!DatabaseManager::instance().addVehicule(data)) {
            QMessageBox::critical(this, "Erreur", "Erreur lors de l'ajout du véhicule.\nVérifiez que l'ID n'existe pas déjà.");
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
    loadVehicules();
}

void TransportsPage::on_sortComboBox_currentIndexChanged(int index)
{
    // Implémentation du tri - selon l'option sélectionnée
    loadVehicules();
}

void TransportsPage::on_modifyButton_clicked()
{
    int row = ui->vehiculesTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Transports", "Veuillez sélectionner un véhicule à modifier");
        return;
    }

    QString id = ui->vehiculesTable->item(row, 0)->text();
    QVariantMap vehiculeData = DatabaseManager::instance().getVehicule(id);

    if (vehiculeData.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Impossible de charger les données du véhicule");
        return;
    }

    VehiculeDialog dialog(vehiculeData, this);
    if (dialog.exec() == QDialog::Accepted) {
        QVariantMap data = dialog.getVehiculeData();
        if (!DatabaseManager::instance().updateVehicule(id, data)) {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la modification du véhicule.");
        } else {
            QMessageBox::information(this, "Succès", "Véhicule modifié avec succès!");
            loadVehicules();
            refreshStatistics();
            refreshFleetInfo();
        }
    }
}

void TransportsPage::on_deleteButton_clicked()
{
    int row = ui->vehiculesTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Transports", "Veuillez sélectionner un véhicule à supprimer");
        return;
    }

    QString id = ui->vehiculesTable->item(row, 0)->text();
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Confirmation",
        QString("Êtes-vous sûr de vouloir supprimer le véhicule %1?").arg(id),
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        if (!DatabaseManager::instance().deleteVehicule(id)) {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression du véhicule.");
        } else {
            QMessageBox::information(this, "Succès", "Véhicule supprimé avec succès!");
            loadVehicules();
            refreshStatistics();
            refreshFleetInfo();
        }
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
