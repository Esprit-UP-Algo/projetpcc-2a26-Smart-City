#include "residentspage.h"
#include "ui_residentspage.h"
#include "residentdialog.h"
#include "databasemanager.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QPdfWriter>
#include <QPainter>
#include <QStandardPaths>
#include <QDate>
#include <QDebug>

ResidentsPage::ResidentsPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ResidentsPage)
{
    ui->setupUi(this);
    setupConnections();
    setupTable();
    setupInitialData();
    loadResidents();
    updateStatistics();
}

ResidentsPage::~ResidentsPage()
{
    delete ui;
}

// -------------------- Setup --------------------
void ResidentsPage::setupConnections()
{
    connect(ui->addResidentButton, &QPushButton::clicked, this, &ResidentsPage::onAddResidentClicked);
    connect(ui->editResidentButton, &QPushButton::clicked, this, &ResidentsPage::onEditResidentClicked);
    connect(ui->deleteResidentButton, &QPushButton::clicked, this, &ResidentsPage::onDeleteResidentClicked);
    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, &ResidentsPage::onSearchTextChanged);
    connect(ui->sortComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ResidentsPage::onSortChanged);
    connect(ui->exportPdfButton, &QPushButton::clicked, this, &ResidentsPage::onExportPdfClicked);
    connect(ui->residentsTable, &QTableWidget::itemSelectionChanged, this, &ResidentsPage::onTableSelectionChanged);
}

void ResidentsPage::setupTable()
{
    ui->residentsTable->horizontalHeader()->setStretchLastSection(true);
    ui->residentsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->residentsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->residentsTable->setAlternatingRowColors(true);

    QStringList headers = {
        "CIN", "Nom", "Prénom", "Sexe", "Téléphone",
        "Email", "Date d'entrée", "Appartement", "Étage", "Statut"
    };
    ui->residentsTable->setColumnCount(headers.size());
    ui->residentsTable->setHorizontalHeaderLabels(headers);
}

void ResidentsPage::setupInitialData()
{
    ui->editResidentButton->setEnabled(false);
    ui->deleteResidentButton->setEnabled(false);
}

// -------------------- CRUD --------------------
void ResidentsPage::onAddResidentClicked()
{
    ResidentDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        QVariantMap residentData = dialog.getResidentData();
        DatabaseManager &db = DatabaseManager::instance();

        if (db.addResident(residentData))
        {
            addResidentToTable(residentData);
            updateStatistics();
            QMessageBox::information(this, "Succès",
                                     QString("Résident %1 %2 ajouté avec succès.")
                                         .arg(residentData["prenom"].toString(), residentData["nom"].toString()));
        }
        else
        {
            QMessageBox::warning(this, "Erreur",
                                 "Impossible d'ajouter le résident à la base de données.\n"
                                 "Vérifiez la connexion Oracle et les champs obligatoires.");
        }
    }
}

void ResidentsPage::onEditResidentClicked()
{
    int currentRow = ui->residentsTable->currentRow();
    if (currentRow < 0) return;

    QVariantMap currentData = getResidentDataFromRow(currentRow);
    QString originalCin = currentData["cin"].toString();
    ResidentDialog dialog(this, currentData);

    if (dialog.exec() == QDialog::Accepted)
    {
        QVariantMap updatedData = dialog.getResidentData();
        DatabaseManager &db = DatabaseManager::instance();

        if (db.updateResident(originalCin, updatedData))
        {
            updateTableRow(currentRow, updatedData);
            updateStatistics();
            QMessageBox::information(this, "Succès", "Résident modifié avec succès !");
        }
        else
        {
            QMessageBox::warning(this, "Erreur",
                                 "Impossible de modifier le résident dans la base de données.");
        }
    }
}

void ResidentsPage::onDeleteResidentClicked()
{
    int currentRow = ui->residentsTable->currentRow();
    if (currentRow < 0) return;

    QString cin = ui->residentsTable->item(currentRow, 0)->text();
    QString nom = ui->residentsTable->item(currentRow, 1)->text();
    QString prenom = ui->residentsTable->item(currentRow, 2)->text();

    if (QMessageBox::question(this, "Confirmation",
                              QString("Supprimer le résident %1 %2 ?").arg(prenom, nom))
        == QMessageBox::Yes)
    {
        DatabaseManager &db = DatabaseManager::instance();
        if (db.deleteResident(cin))
        {
            ui->residentsTable->removeRow(currentRow);
            updateStatistics();
            QMessageBox::information(this, "Succès", "Résident supprimé avec succès !");
        }
        else
        {
            QMessageBox::warning(this, "Erreur",
                                 "Impossible de supprimer le résident dans la base de données.");
        }
    }
}

void ResidentsPage::loadResidents()
{
    ui->residentsTable->setRowCount(0);
    DatabaseManager &db = DatabaseManager::instance();
    QList<QVariantMap> residents = db.getAllResidents();

    for (const QVariantMap &r : residents)
        addResidentToTable(r);

    updateStatistics();
}

// -------------------- Recherche / Tri --------------------
void ResidentsPage::onSearchTextChanged(const QString &text)
{
    QString search = text.toLower();
    for (int row = 0; row < ui->residentsTable->rowCount(); ++row)
    {
        bool visible = false;
        for (int col = 0; col < ui->residentsTable->columnCount(); ++col)
        {
            QTableWidgetItem *item = ui->residentsTable->item(row, col);
            if (item && item->text().toLower().contains(search))
            {
                visible = true;
                break;
            }
        }
        ui->residentsTable->setRowHidden(row, !visible);
    }
}

void ResidentsPage::onSortChanged(int index)
{
    ui->residentsTable->sortItems(index, Qt::AscendingOrder);
}

// -------------------- PDF Export --------------------
void ResidentsPage::onExportPdfClicked()
{
    int currentRow = ui->residentsTable->currentRow();
    if (currentRow < 0)
    {
        QMessageBox::information(this, "Export PDF", "Veuillez sélectionner un résident avant d’exporter.");
        return;
    }

    QString nom = ui->residentsTable->item(currentRow, 1)->text();
    QString prenom = ui->residentsTable->item(currentRow, 2)->text();
    QString path = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
                   + QString("/Fiche_%1_%2.pdf").arg(prenom, nom);

    QPdfWriter pdf(path);
    QPainter painter(&pdf);
    painter.setFont(QFont("Arial", 12));
    painter.drawText(100, 200, QString("Fiche du résident : %1 %2").arg(prenom, nom));
    painter.drawText(100, 250, "Export généré via NEXORA Smart City");
    painter.end();

    QMessageBox::information(this, "PDF Exporté",
                             QString("Fiche PDF générée avec succès :\n%1").arg(path));
}

// -------------------- Table helpers --------------------
void ResidentsPage::onTableSelectionChanged()
{
    bool hasSelection = ui->residentsTable->currentRow() >= 0;
    ui->editResidentButton->setEnabled(hasSelection);
    ui->deleteResidentButton->setEnabled(hasSelection);
}

void ResidentsPage::addResidentToTable(const QVariantMap &r)
{
    int row = ui->residentsTable->rowCount();
    ui->residentsTable->insertRow(row);

    ui->residentsTable->setItem(row, 0, new QTableWidgetItem(r["cin"].toString()));
    ui->residentsTable->setItem(row, 1, new QTableWidgetItem(r["nom"].toString()));
    ui->residentsTable->setItem(row, 2, new QTableWidgetItem(r["prenom"].toString()));
    ui->residentsTable->setItem(row, 3, new QTableWidgetItem(r["sexe"].toString()));
    ui->residentsTable->setItem(row, 4, new QTableWidgetItem(r["telephone"].toString()));
    ui->residentsTable->setItem(row, 5, new QTableWidgetItem(r["email"].toString()));
    ui->residentsTable->setItem(row, 6, new QTableWidgetItem(r["date_entree"].toString()));
    ui->residentsTable->setItem(row, 7, new QTableWidgetItem(r["appartement"].toString()));
    ui->residentsTable->setItem(row, 8, new QTableWidgetItem(r["etage"].toString()));
    ui->residentsTable->setItem(row, 9, new QTableWidgetItem(r["statut"].toString()));
}

QVariantMap ResidentsPage::getResidentDataFromRow(int row)
{
    QVariantMap d;
    for (int i = 0; i < 10; ++i)
        if (ui->residentsTable->item(row, i))
            d[ui->residentsTable->horizontalHeaderItem(i)->text().toLower()] = ui->residentsTable->item(row, i)->text();
    return d;
}

void ResidentsPage::updateTableRow(int row, const QVariantMap &r)
{
    for (int i = 0; i < ui->residentsTable->columnCount(); ++i)
    {
        QString key = ui->residentsTable->horizontalHeaderItem(i)->text().toLower();
        if (r.contains(key))
            ui->residentsTable->item(row, i)->setText(r[key].toString());
    }
}

// -------------------- Statistiques --------------------
void ResidentsPage::updateStatistics()
{
    int totalResidents = ui->residentsTable->rowCount();
    ui->totalResidentsValue->setText(QString::number(totalResidents));
    ui->newResidentsValue->setText(QString::number(calculateNewResidentsThisMonth()));
    double satisfaction = calculateSatisfactionScore();
    ui->satisfactionValue->setText(QString("%1%").arg(qRound(satisfaction)));
}

int ResidentsPage::calculateNewResidentsThisMonth()
{
    QDate current = QDate::currentDate();
    int count = 0;
    for (int r = 0; r < ui->residentsTable->rowCount(); ++r)
    {
        QDate date = QDate::fromString(ui->residentsTable->item(r, 6)->text(), "yyyy-MM-dd");
        if (date.month() == current.month() && date.year() == current.year())
            count++;
    }
    return count;
}

double ResidentsPage::calculateSatisfactionScore()
{
    int total = ui->residentsTable->rowCount();
    if (total == 0) return 75.0;
    return qMin(100.0, 75.0 + total * 0.5);
}
