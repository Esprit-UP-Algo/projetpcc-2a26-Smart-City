#include "residentspage.h"
#include "ui_residentspage.h"
#include "residentdialog.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QHeaderView>

ResidentsPage::ResidentsPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ResidentsPage)
{
    ui->setupUi(this);
    setupConnections();
    setupTable();
    setupInitialData();
}

ResidentsPage::~ResidentsPage()
{
    delete ui;
}

void ResidentsPage::setupConnections()
{
    // Connect the main add resident button
    connect(ui->addResidentButton, &QPushButton::clicked, this, &ResidentsPage::onAddResidentClicked);
    
    // Connect table action buttons
    connect(ui->editResidentButton, &QPushButton::clicked, this, &ResidentsPage::onEditResidentClicked);
    connect(ui->deleteResidentButton, &QPushButton::clicked, this, &ResidentsPage::onDeleteResidentClicked);
    
    // Connect search and sort
    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, &ResidentsPage::onSearchTextChanged);
    connect(ui->sortComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ResidentsPage::onSortChanged);
    
    // Connect export button
    connect(ui->exportPdfButton, &QPushButton::clicked, this, &ResidentsPage::onExportPdfClicked);
    
    // Connect table selection
    connect(ui->residentsTable, &QTableWidget::itemSelectionChanged, this, &ResidentsPage::onTableSelectionChanged);
}

void ResidentsPage::setupTable()
{
    // Configure table properties
    ui->residentsTable->horizontalHeader()->setStretchLastSection(true);
    ui->residentsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->residentsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->residentsTable->setAlternatingRowColors(true);
    
    // Set column widths for better display
    ui->residentsTable->setColumnWidth(0, 100); // CIN
    ui->residentsTable->setColumnWidth(1, 120); // Nom
    ui->residentsTable->setColumnWidth(2, 120); // Prénom
    ui->residentsTable->setColumnWidth(3, 80);  // Sexe
    ui->residentsTable->setColumnWidth(4, 120); // Téléphone
    ui->residentsTable->setColumnWidth(5, 150); // Email
    ui->residentsTable->setColumnWidth(6, 100); // Date d'entrée
    ui->residentsTable->setColumnWidth(7, 100); // Appartement
}

void ResidentsPage::setupInitialData()
{
    // Update the satisfaction score with dynamic content
    updateSatisfactionScore();
    
    // Enable/disable buttons based on selection
    ui->editResidentButton->setEnabled(false);
    ui->deleteResidentButton->setEnabled(false);
}

void ResidentsPage::updateSatisfactionScore()
{
    int totalResidents = ui->residentsTable->rowCount();
    int baseScore = 70;
    int score = baseScore + (totalResidents > 0 ? totalResidents * 2 : 0);
    score = qMin(100, score); // Cap at 100
    
    QString scoreText = QString("Score global de satisfaction : %1 / 100 — ").arg(score);
    if (score >= 85) {
        scoreText += "Excellente stabilité, résidents très satisfaits.";
    } else if (score >= 70) {
        scoreText += "Bonne stabilité. Quelques incidents mineurs signalés ce mois-ci.";
    } else {
        scoreText += "Stabilité acceptable, surveillance recommandée.";
    }
    
    ui->globalScoreLabel->setText(scoreText);
}

void ResidentsPage::onAddResidentClicked()
{
    // Open the ResidentDialog
    ResidentDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        // Get the data from the dialog
        QVariantMap residentData = dialog.getResidentData();
        
        // Add the resident to the table (without database)
        addResidentToTable(residentData);
        
        // Update satisfaction score
        updateSatisfactionScore();
        
        QMessageBox::information(this, "Succès", 
            QString("Résident %1 %2 ajouté avec succès!")
            .arg(residentData["prenom"].toString())
            .arg(residentData["nom"].toString()));
    }
}

void ResidentsPage::onEditResidentClicked()
{
    int currentRow = ui->residentsTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un résident à modifier.");
        return;
    }
    
    // Get current resident data from table
    QVariantMap currentData = getResidentDataFromRow(currentRow);
    
    // Open dialog with current data
    ResidentDialog dialog(this, currentData);
    if (dialog.exec() == QDialog::Accepted) {
        QVariantMap updatedData = dialog.getResidentData();
        
        // Update the table row
        updateTableRow(currentRow, updatedData);
        
        QMessageBox::information(this, "Succès", "Résident modifié avec succès!");
    }
}

void ResidentsPage::onDeleteResidentClicked()
{
    int currentRow = ui->residentsTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un résident à supprimer.");
        return;
    }
    
    QString nom = ui->residentsTable->item(currentRow, 1)->text();
    QString prenom = ui->residentsTable->item(currentRow, 2)->text();
    
    auto reply = QMessageBox::question(this, "Confirmation",
        QString("Êtes-vous sûr de vouloir supprimer le résident %1 %2?")
        .arg(prenom).arg(nom),
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        ui->residentsTable->removeRow(currentRow);
        updateSatisfactionScore();
        QMessageBox::information(this, "Succès", "Résident supprimé avec succès!");
    }
}

void ResidentsPage::onSearchTextChanged(const QString &text)
{
    QString searchText = text.toLower();
    
    for (int row = 0; row < ui->residentsTable->rowCount(); ++row) {
        bool shouldShow = false;
        
        // Search in CIN, Nom, Prénom columns (0, 1, 2)
        for (int col = 0; col < 3; ++col) {
            QTableWidgetItem *item = ui->residentsTable->item(row, col);
            if (item && item->text().toLower().contains(searchText)) {
                shouldShow = true;
                break;
            }
        }
        
        ui->residentsTable->setRowHidden(row, !shouldShow);
    }
}

void ResidentsPage::onSortChanged(int index)
{
    switch (index) {
        case 0: // Trier par CIN
            ui->residentsTable->sortItems(0, Qt::AscendingOrder);
            break;
        case 1: // Trier par Nom
            ui->residentsTable->sortItems(1, Qt::AscendingOrder);
            break;
        case 2: // Trier par Appartement
            ui->residentsTable->sortItems(7, Qt::AscendingOrder);
            break;
        case 3: // Trier par Date d'entrée
            ui->residentsTable->sortItems(6, Qt::AscendingOrder);
            break;
    }
}

void ResidentsPage::onExportPdfClicked()
{
    int currentRow = ui->residentsTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::information(this, "Export PDF", 
            "Veuillez sélectionner un résident pour générer sa fiche PDF.\n\n"
            "La fiche contiendra :\n"
            "• Logo NEXORA\n"
            "• Informations complètes du résident\n"
            "• Détails du contrat de location\n"
            "• Signature de l'administrateur");
        return;
    }
    
    QString nom = ui->residentsTable->item(currentRow, 1)->text();
    QString prenom = ui->residentsTable->item(currentRow, 2)->text();
    
    QMessageBox::information(this, "Export PDF", 
        QString("Génération de la fiche PDF pour %1 %2...\n\n"
        "Fonctionnalité en développement :\n"
        "• Logo NEXORA en en-tête\n"
        "• Informations complètes du résident\n"
        "• Détails du contrat et échéances\n"
        "• Signature officielle de l'administrateur")
        .arg(prenom).arg(nom));
}

void ResidentsPage::onTableSelectionChanged()
{
    bool hasSelection = ui->residentsTable->currentRow() >= 0;
    ui->editResidentButton->setEnabled(hasSelection);
    ui->deleteResidentButton->setEnabled(hasSelection);
}

void ResidentsPage::addResidentToTable(const QVariantMap &residentData)
{
    int rowCount = ui->residentsTable->rowCount();
    ui->residentsTable->insertRow(rowCount);
    
    // Fill the table row with resident data
    ui->residentsTable->setItem(rowCount, 0, new QTableWidgetItem(residentData["cin"].toString()));
    ui->residentsTable->setItem(rowCount, 1, new QTableWidgetItem(residentData["nom"].toString()));
    ui->residentsTable->setItem(rowCount, 2, new QTableWidgetItem(residentData["prenom"].toString()));
    ui->residentsTable->setItem(rowCount, 3, new QTableWidgetItem(residentData["sexe"].toString()));
    ui->residentsTable->setItem(rowCount, 4, new QTableWidgetItem(residentData["telephone"].toString()));
    ui->residentsTable->setItem(rowCount, 5, new QTableWidgetItem(residentData["email"].toString()));
    ui->residentsTable->setItem(rowCount, 6, new QTableWidgetItem(residentData["date_entree"].toString()));
    ui->residentsTable->setItem(rowCount, 7, new QTableWidgetItem(residentData["appartement"].toString()));
}

QVariantMap ResidentsPage::getResidentDataFromRow(int row)
{
    QVariantMap data;
    if (row >= 0 && row < ui->residentsTable->rowCount()) {
        data["cin"] = ui->residentsTable->item(row, 0)->text();
        data["nom"] = ui->residentsTable->item(row, 1)->text();
        data["prenom"] = ui->residentsTable->item(row, 2)->text();
        data["sexe"] = ui->residentsTable->item(row, 3)->text();
        data["telephone"] = ui->residentsTable->item(row, 4)->text();
        data["email"] = ui->residentsTable->item(row, 5)->text();
        data["date_entree"] = ui->residentsTable->item(row, 6)->text();
        data["appartement"] = ui->residentsTable->item(row, 7)->text();
    }
    return data;
}

void ResidentsPage::updateTableRow(int row, const QVariantMap &residentData)
{
    if (row >= 0 && row < ui->residentsTable->rowCount()) {
        ui->residentsTable->item(row, 0)->setText(residentData["cin"].toString());
        ui->residentsTable->item(row, 1)->setText(residentData["nom"].toString());
        ui->residentsTable->item(row, 2)->setText(residentData["prenom"].toString());
        ui->residentsTable->item(row, 3)->setText(residentData["sexe"].toString());
        ui->residentsTable->item(row, 4)->setText(residentData["telephone"].toString());
        ui->residentsTable->item(row, 5)->setText(residentData["email"].toString());
        ui->residentsTable->item(row, 6)->setText(residentData["date_entree"].toString());
        ui->residentsTable->item(row, 7)->setText(residentData["appartement"].toString());
    }
}