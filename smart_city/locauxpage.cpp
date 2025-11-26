#include "locauxpage.h"
#include "ui_locauxpage.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QHeaderView>

LocauxPage::LocauxPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LocauxPage)
{
    ui->setupUi(this);
    setupTable();
    loadLocaux();
}

LocauxPage::~LocauxPage()
{
    delete ui;
}

void LocauxPage::setupTable()
{
    // Configure table headers
    ui->locauxTableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->locauxTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->locauxTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    
    // Set column widths
    ui->locauxTableWidget->setColumnWidth(0, 80);  // ID
    ui->locauxTableWidget->setColumnWidth(1, 150); // Nom
    ui->locauxTableWidget->setColumnWidth(2, 120); // Type
    ui->locauxTableWidget->setColumnWidth(3, 100); // Capacité
    ui->locauxTableWidget->setColumnWidth(4, 100); // Superficie
    ui->locauxTableWidget->setColumnWidth(5, 200); // Adresse
    ui->locauxTableWidget->setColumnWidth(6, 120); // Statut
    ui->locauxTableWidget->setColumnWidth(7, 80);  // Ouvert
}

void LocauxPage::loadLocaux()
{
    // Clear existing data
    ui->locauxTableWidget->setRowCount(0);
    
    // For now, add some sample data since no database integration is required
    // This would normally load from database
    QStringList sampleData[] = {
        {"L001", "Jardin Central", "Jardin", "50", "1200", "Place Central, Tunis", "Disponible", "Oui"},
        {"L002", "Salle Sport Alpha", "Salle de sport", "100", "500", "Rue Habib Bourguiba", "Occupé", "Oui"},
        {"L003", "Théâtre Municipal", "Salle de spectacle", "300", "800", "Avenue de la République", "En maintenance", "Non"},
        {"L004", "Café des Arts", "Restaurant/Café", "40", "120", "Rue de la Liberté", "Occupé", "Oui"},
        {"L005", "Parking Nord", "Parking", "200", "2000", "Zone Industrielle", "Disponible", "Oui"}
    };
    
    int sampleCount = sizeof(sampleData) / sizeof(sampleData[0]);
    ui->locauxTableWidget->setRowCount(sampleCount);
    
    for (int i = 0; i < sampleCount; ++i) {
        for (int j = 0; j < sampleData[i].size(); ++j) {
            QTableWidgetItem *item = new QTableWidgetItem(sampleData[i][j]);
            
            // Color code status column
            if (j == 6) { // Status column
                if (sampleData[i][j] == "Disponible") {
                    item->setBackground(QColor("#D5F4E6"));
                    item->setForeground(QColor("#27AE60"));
                } else if (sampleData[i][j] == "Occupé") {
                    item->setBackground(QColor("#FCF3CF"));
                    item->setForeground(QColor("#F39C12"));
                } else if (sampleData[i][j] == "En maintenance") {
                    item->setBackground(QColor("#FADBD8"));
                    item->setForeground(QColor("#E74C3C"));
                }
                item->setTextAlignment(Qt::AlignCenter);
            }
            
            // Center align certain columns
            if (j == 0 || j == 3 || j == 4 || j == 7) { // ID, Capacité, Superficie, Ouvert
                item->setTextAlignment(Qt::AlignCenter);
            }
            
            ui->locauxTableWidget->setItem(i, j, item);
        }
    }
}

void LocauxPage::clearForm()
{
    ui->idLineEdit->clear();
    ui->nomLineEdit->clear();
    ui->capaciteSpinBox->setValue(1);
    ui->adresseLineEdit->clear();
    ui->occupantLineEdit->clear();
    ui->superficieSpinBox->setValue(1);
    ui->typeComboBox->setCurrentIndex(0);
    ui->statutComboBox->setCurrentIndex(0);
    ui->ouvertComboBox->setCurrentIndex(0);
}

void LocauxPage::populateForm(int row)
{
    if (row < 0 || row >= ui->locauxTableWidget->rowCount()) {
        return;
    }
    
    ui->idLineEdit->setText(ui->locauxTableWidget->item(row, 0)->text());
    ui->nomLineEdit->setText(ui->locauxTableWidget->item(row, 1)->text());
    
    // Set type combo box
    QString type = ui->locauxTableWidget->item(row, 2)->text();
    int typeIndex = ui->typeComboBox->findText(type);
    if (typeIndex >= 0) {
        ui->typeComboBox->setCurrentIndex(typeIndex);
    }
    
    ui->capaciteSpinBox->setValue(ui->locauxTableWidget->item(row, 3)->text().toInt());
    ui->superficieSpinBox->setValue(ui->locauxTableWidget->item(row, 4)->text().toInt());
    ui->adresseLineEdit->setText(ui->locauxTableWidget->item(row, 5)->text());
    
    // Set status combo box
    QString statut = ui->locauxTableWidget->item(row, 6)->text();
    int statutIndex = ui->statutComboBox->findText(statut);
    if (statutIndex >= 0) {
        ui->statutComboBox->setCurrentIndex(statutIndex);
    }
    
    // Set ouvert combo box
    QString ouvert = ui->locauxTableWidget->item(row, 7)->text();
    int ouvertIndex = ui->ouvertComboBox->findText(ouvert);
    if (ouvertIndex >= 0) {
        ui->ouvertComboBox->setCurrentIndex(ouvertIndex);
    }
}

void LocauxPage::on_ajouterButton_clicked()
{
    // Validate form
    if (ui->idLineEdit->text().isEmpty() || ui->nomLineEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Attention", "Veuillez remplir au moins l'ID et le nom du local.");
        return;
    }
    
    // Check for duplicate ID
    for (int i = 0; i < ui->locauxTableWidget->rowCount(); ++i) {
        if (ui->locauxTableWidget->item(i, 0)->text() == ui->idLineEdit->text()) {
            QMessageBox::warning(this, "Erreur", "Un local avec cet ID existe déjà.");
            return;
        }
    }
    
    // Add new row
    int newRow = ui->locauxTableWidget->rowCount();
    ui->locauxTableWidget->insertRow(newRow);
    
    // Populate new row
    ui->locauxTableWidget->setItem(newRow, 0, new QTableWidgetItem(ui->idLineEdit->text()));
    ui->locauxTableWidget->setItem(newRow, 1, new QTableWidgetItem(ui->nomLineEdit->text()));
    ui->locauxTableWidget->setItem(newRow, 2, new QTableWidgetItem(ui->typeComboBox->currentText()));
    ui->locauxTableWidget->setItem(newRow, 3, new QTableWidgetItem(QString::number(ui->capaciteSpinBox->value())));
    ui->locauxTableWidget->setItem(newRow, 4, new QTableWidgetItem(QString::number(ui->superficieSpinBox->value())));
    ui->locauxTableWidget->setItem(newRow, 5, new QTableWidgetItem(ui->adresseLineEdit->text()));
    ui->locauxTableWidget->setItem(newRow, 6, new QTableWidgetItem(ui->statutComboBox->currentText()));
    ui->locauxTableWidget->setItem(newRow, 7, new QTableWidgetItem(ui->ouvertComboBox->currentText()));
    
    // Apply formatting to new row
    for (int j = 0; j < ui->locauxTableWidget->columnCount(); ++j) {
        QTableWidgetItem *item = ui->locauxTableWidget->item(newRow, j);
        
        // Color code status column
        if (j == 6) {
            if (item->text() == "Disponible") {
                item->setBackground(QColor("#D5F4E6"));
                item->setForeground(QColor("#27AE60"));
            } else if (item->text() == "Occupé") {
                item->setBackground(QColor("#FCF3CF"));
                item->setForeground(QColor("#F39C12"));
            } else if (item->text() == "En maintenance") {
                item->setBackground(QColor("#FADBD8"));
                item->setForeground(QColor("#E74C3C"));
            }
            item->setTextAlignment(Qt::AlignCenter);
        }
        
        // Center align certain columns
        if (j == 0 || j == 3 || j == 4 || j == 7) {
            item->setTextAlignment(Qt::AlignCenter);
        }
    }
    
    clearForm();
    QMessageBox::information(this, "Succès", "Local ajouté avec succès!");
}

void LocauxPage::on_modifierButton_clicked()
{
    int currentRow = ui->locauxTableWidget->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un local à modifier.");
        return;
    }
    
    if (ui->idLineEdit->text().isEmpty() || ui->nomLineEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Attention", "Veuillez remplir au moins l'ID et le nom du local.");
        return;
    }
    
    // Update the selected row
    ui->locauxTableWidget->item(currentRow, 0)->setText(ui->idLineEdit->text());
    ui->locauxTableWidget->item(currentRow, 1)->setText(ui->nomLineEdit->text());
    ui->locauxTableWidget->item(currentRow, 2)->setText(ui->typeComboBox->currentText());
    ui->locauxTableWidget->item(currentRow, 3)->setText(QString::number(ui->capaciteSpinBox->value()));
    ui->locauxTableWidget->item(currentRow, 4)->setText(QString::number(ui->superficieSpinBox->value()));
    ui->locauxTableWidget->item(currentRow, 5)->setText(ui->adresseLineEdit->text());
    ui->locauxTableWidget->item(currentRow, 6)->setText(ui->statutComboBox->currentText());
    ui->locauxTableWidget->item(currentRow, 7)->setText(ui->ouvertComboBox->currentText());
    
    // Reapply formatting
    QTableWidgetItem *statusItem = ui->locauxTableWidget->item(currentRow, 6);
    if (statusItem->text() == "Disponible") {
        statusItem->setBackground(QColor("#D5F4E6"));
        statusItem->setForeground(QColor("#27AE60"));
    } else if (statusItem->text() == "Occupé") {
        statusItem->setBackground(QColor("#FCF3CF"));
        statusItem->setForeground(QColor("#F39C12"));
    } else if (statusItem->text() == "En maintenance") {
        statusItem->setBackground(QColor("#FADBD8"));
        statusItem->setForeground(QColor("#E74C3C"));
    }
    
    clearForm();
    QMessageBox::information(this, "Succès", "Local modifié avec succès!");
}

void LocauxPage::on_supprimerButton_clicked()
{
    int currentRow = ui->locauxTableWidget->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un local à supprimer.");
        return;
    }
    
    QString localName = ui->locauxTableWidget->item(currentRow, 1)->text();
    auto reply = QMessageBox::question(this, "Confirmation",
        QString("Êtes-vous sûr de vouloir supprimer le local '%1'?").arg(localName),
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        ui->locauxTableWidget->removeRow(currentRow);
        clearForm();
        QMessageBox::information(this, "Succès", "Local supprimé avec succès!");
    }
}

void LocauxPage::on_consulterButton_clicked()
{
    int currentRow = ui->locauxTableWidget->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un local à consulter.");
        return;
    }
    
    populateForm(currentRow);
}

void LocauxPage::on_exportButton_clicked()
{
    QMessageBox::information(this, "Export Excel", 
        "Fonctionnalité d'export Excel à venir!\n\n"
        "Cela permettra d'exporter un rapport comparatif sur les coûts des types de locaux.");
}

void LocauxPage::on_searchLineEdit_textChanged(const QString &arg1)
{
    QString searchText = arg1.toLower();
    
    for (int i = 0; i < ui->locauxTableWidget->rowCount(); ++i) {
        bool shouldShow = false;
        
        // Search in ID, Type, and Address columns
        for (int j : {0, 2, 5}) { // ID, Type, Address columns
            if (ui->locauxTableWidget->item(i, j)->text().toLower().contains(searchText)) {
                shouldShow = true;
                break;
            }
        }
        
        ui->locauxTableWidget->setRowHidden(i, !shouldShow);
    }
}

void LocauxPage::on_sortComboBox_currentIndexChanged(int index)
{
    if (index == 0) {
        // Sort by ID (column 0)
        ui->locauxTableWidget->sortItems(0, Qt::AscendingOrder);
    } else if (index == 1) {
        // Sort by Status (column 6)
        ui->locauxTableWidget->sortItems(6, Qt::AscendingOrder);
    }
}