#include "transportpage.h"
#include "ui_transportpage.h"
#include "databasemanager.h"
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QDate>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QDebug>
#include <QTimer>
#include <QApplication>
#include <QSqlError>

TransportPage::TransportPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TransportPage)
{
    ui->setupUi(this);

    if (!DatabaseManager::instance().database().isOpen()) {
        QMessageBox::critical(this, "Erreur", "Base de données non connectée !");
    } else {
        qDebug() << "✅ Base connectée dans TransportPage";
    }

    this->setAttribute(Qt::WA_StyledBackground, true);

    ui->tabAddButton->setChecked(true);
    ui->tabListButton->setChecked(false);
    ui->tabStatsButton->setChecked(false);
    ui->stackedWidget->setCurrentWidget(ui->addPage);

    setupTableHeaders();
    setupUiBehavior();
    setupValidation();

    // Connecter le signal dataChanged du DatabaseManager
    connect(&DatabaseManager::instance(), &DatabaseManager::dataChanged,
            this, &TransportPage::onDataChanged);

    // Charger les données initiales
    QTimer::singleShot(100, this, &TransportPage::loadAllVehicules);

    qDebug() << "✅ TransportPage initialisé";
}

TransportPage::~TransportPage()
{
    delete ui;
}

void TransportPage::onDataChanged()
{
    qDebug() << "📢 Signal dataChanged reçu - Rechargement automatique";
    loadAllVehicules();
}

void TransportPage::loadAllVehicules()
{
    qDebug() << "=== CHARGEMENT VÉHICULES ===";

    QTableWidget *t = ui->vehiculesTable;
    const bool blocked = t->signalsBlocked();
    t->blockSignals(true);
    t->setSortingEnabled(false);
    t->clearContents();
    t->setRowCount(0);

    if (!DatabaseManager::instance().database().isOpen()) {
        qDebug() << "❌ Base non connectée";
        t->blockSignals(blocked);
        return;
    }

    // ✅ CORRECTION ICI - Utiliser la bonne méthode
    QList<QVariantMap> rows = DatabaseManager::instance().getAllTransportVehicules();
    qDebug() << "📊 Trouvé" << rows.size() << "véhicules dans la base";

    if (rows.isEmpty()) {
        qDebug() << "⚠️ Aucun véhicule trouvé";
        t->blockSignals(blocked);
        return;
    }

    t->setRowCount(rows.size());

    for (int r = 0; r < rows.size(); ++r) {
        const auto &m = rows[r];

        t->setItem(r, 0, new QTableWidgetItem(m.value("id_vehicule").toString()));
        t->setItem(r, 1, new QTableWidgetItem(m.value("type").toString()));
        t->setItem(r, 2, new QTableWidgetItem(m.value("capacite").toString()));
        t->setItem(r, 3, new QTableWidgetItem(m.value("zone").toString()));
        t->setItem(r, 4, new QTableWidgetItem(m.value("horaire").toString()));
        t->setItem(r, 5, new QTableWidgetItem(m.value("statut").toString()));
        t->setItem(r, 6, new QTableWidgetItem(m.value("date_ajout").toString()));
        t->setItem(r, 7, new QTableWidgetItem(m.value("temps_utilise").toString()));
        t->setItem(r, 8, new QTableWidgetItem(m.value("code_unique").toString()));

        qDebug() << "  ➜ Ligne" << r << ":" << m.value("id_vehicule").toString();
    }

    t->blockSignals(blocked);
    t->resizeColumnsToContents();
    t->setSortingEnabled(true);
    t->scrollToTop();
    t->clearSelection();
    t->viewport()->update();

    qDebug() << "✅ Tableau mis à jour avec" << t->rowCount() << "lignes";
    qDebug() << "=== FIN CHARGEMENT ===";
}

void TransportPage::setupTableHeaders()
{
    QTableWidget *t = ui->vehiculesTable;
    t->setColumnCount(9);
    QStringList headers{
        "ID Véhicule", "Type", "Capacité", "Zone", "Horaire",
        "Statut", "Date Ajout", "Temps Utilisé (h)", "Code Unique"
    };
    t->setHorizontalHeaderLabels(headers);
    t->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    t->setSelectionBehavior(QAbstractItemView::SelectRows);
    t->setSelectionMode(QAbstractItemView::SingleSelection);
    t->setEditTriggers(QAbstractItemView::NoEditTriggers);
    t->setAlternatingRowColors(true);
}

void TransportPage::setupUiBehavior()
{
    connect(ui->tabAddButton, &QPushButton::clicked, this, &TransportPage::onTabAdd);
    connect(ui->tabListButton, &QPushButton::clicked, this, &TransportPage::onTabList);
    connect(ui->tabStatsButton, &QPushButton::clicked, this, &TransportPage::onTabStats);
    connect(ui->saveFormButton, &QPushButton::clicked, this, &TransportPage::onSaveForm);
    connect(ui->cancelFormButton, &QPushButton::clicked, this, &TransportPage::onCancelForm);
    connect(ui->searchEdit, &QLineEdit::textChanged, this, &TransportPage::onFilterTextChanged);
    connect(ui->sortCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &TransportPage::onSortIndexChanged);
    connect(ui->exportPdfButton, &QPushButton::clicked, this, &TransportPage::onExportPdf);
    connect(ui->editVehiculeButton, &QPushButton::clicked, this, &TransportPage::onEditClicked);
    connect(ui->deleteVehiculeButton, &QPushButton::clicked, this, &TransportPage::onDeleteClicked);
}

void TransportPage::setupValidation()
{
    ui->errorLabel->hide();
    ui->idEdit->setMaxLength(10);
    ui->idEdit->setPlaceholderText("VH001");
    ui->capaciteEdit->setPlaceholderText("Ex: 4, 20, 5");
    connect(ui->idEdit, &QLineEdit::textChanged, this, &TransportPage::onIdTextChanged);
    connect(ui->capaciteEdit, &QLineEdit::textChanged, this, &TransportPage::onCapaciteTextChanged);
    ui->dateAjoutEdit->setDate(QDate::currentDate());
}

void TransportPage::onTabAdd()
{
    ui->tabAddButton->setChecked(true);
    ui->tabListButton->setChecked(false);
    ui->tabStatsButton->setChecked(false);
    ui->stackedWidget->setCurrentWidget(ui->addPage);
}

void TransportPage::onTabList()
{
    ui->tabAddButton->setChecked(false);
    ui->tabListButton->setChecked(true);
    ui->tabStatsButton->setChecked(false);
    ui->stackedWidget->setCurrentWidget(ui->listPage);

    // Recharger les données à chaque fois qu'on ouvre l'onglet liste
    loadAllVehicules();
}

void TransportPage::onTabStats()
{
    ui->tabAddButton->setChecked(false);
    ui->tabListButton->setChecked(false);
    ui->tabStatsButton->setChecked(true);
    ui->stackedWidget->setCurrentWidget(ui->statsPage);
    calculateAndDisplayStatistics();
    populateHistoriqueTable();
}

void TransportPage::onSaveForm()
{
    qDebug() << "=== DÉBUT SAUVEGARDE ===";

    if (!validateForm()) {
        qDebug() << "❌ Validation échouée";
        return;
    }

    QString id = ui->idEdit->text().trimmed().toUpper();

    QVariantMap data;
    data["id_vehicule"] = id;
    data["type"] = ui->typeCombo->currentText();
    bool ok = false;
    int capacite = ui->capaciteEdit->text().trimmed().toInt(&ok);
    data["capacite"] = ok ? capacite : 0;
    data["zone"] = ui->zoneEdit->text().trimmed();
    data["horaire"] = ui->horaireEdit->text().trimmed();
    data["statut"] = ui->statutCombo->currentText();
    data["date_ajout"] = ui->dateAjoutEdit->date().toString("yyyy-MM-dd");
    double tempsUtilise = ui->tempsUtiliseEdit->text().trimmed().toDouble(&ok);
    data["temps_utilise"] = ok ? tempsUtilise : 0.0;
    data["code_unique"] = ui->codeUniqueEdit->text().trimmed();

    // Vérifier si c'est une mise à jour ou un ajout
    bool isUpdate = !DatabaseManager::instance().getTransportVehicule(id).isEmpty();

    bool success = false;
    if (isUpdate) {
        qDebug() << "🔄 Mise à jour du véhicule" << id;
        success = DatabaseManager::instance().updateTransportVehicule(id, data);
    } else {
        qDebug() << "➕ Ajout du véhicule" << id;
        success = DatabaseManager::instance().addTransportVehicule(data);  // ✅ CORRECTION ICI
    }

    if (success) {
        QMessageBox::information(this, "Succès",
                                 isUpdate ? "Véhicule modifié avec succès !"
                                          : "Véhicule ajouté avec succès !");
        clearForm();
        loadAllVehicules();
        onTabList();
    } else {
        QString errorMsg = DatabaseManager::instance().database().lastError().text();
        if (errorMsg.isEmpty()) {
            errorMsg = "Vérifiez que tous les champs sont correctement remplis et que le code transaction existe (si fourni).";
        }
        QMessageBox::critical(this, "Erreur", 
                             QString("Échec de l'enregistrement !\n\n%1").arg(errorMsg));
    }

    qDebug() << "=== FIN SAUVEGARDE ===";
}

void TransportPage::onCancelForm()
{
    clearForm();
    onTabList();
}

void TransportPage::clearForm()
{
    ui->idEdit->clear();
    ui->capaciteEdit->clear();
    ui->zoneEdit->clear();
    ui->horaireEdit->clear();
    ui->tempsUtiliseEdit->clear();
    ui->codeUniqueEdit->clear();
    ui->typeCombo->setCurrentIndex(0);
    ui->statutCombo->setCurrentIndex(0);
    ui->dateAjoutEdit->setDate(QDate::currentDate());
    hideError();
}

void TransportPage::onFilterTextChanged(const QString &text)
{
    const QString s = text.trimmed().toLower();
    QTableWidget *t = ui->vehiculesTable;

    for (int r = 0; r < t->rowCount(); ++r) {
        bool match = false;
        for (int c = 0; c < t->columnCount(); ++c) {
            if (t->item(r, c) && t->item(r, c)->text().toLower().contains(s)) {
                match = true;
                break;
            }
        }
        t->setRowHidden(r, !match);
    }
}

void TransportPage::onSortIndexChanged(int index)
{
    int col = (index == 0) ? 6 : 1;
    ui->vehiculesTable->sortItems(col, Qt::AscendingOrder);
}

void TransportPage::onExportPdf()
{
    QString file = QFileDialog::getSaveFileName(this, "Exporter PDF", "vehicules.pdf", "PDF (*.pdf)");
    if (file.isEmpty()) return;

    QPdfWriter writer(file);
    writer.setPageSize(QPageSize::A4);
    QPainter painter(&writer);

    painter.drawText(100, 100, "Liste des Véhicules - NEXORA");

    int y = 200;
    QTableWidget *t = ui->vehiculesTable;
    for (int r = 0; r < t->rowCount(); ++r) {
        if (t->isRowHidden(r)) continue;

        QString line = QString("%1 | %2 | %3 | %4")
                           .arg(t->item(r,0)->text())
                           .arg(t->item(r,1)->text())
                           .arg(t->item(r,2)->text())
                           .arg(t->item(r,5)->text());
        painter.drawText(100, y, line);
        y += 50;
    }

    painter.end();
    QMessageBox::information(this, "PDF", "Exporté avec succès !");
}

void TransportPage::onEditClicked()
{
    int row = ui->vehiculesTable->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Modifier", "Veuillez sélectionner un véhicule.");
        return;
    }

    QString id = ui->vehiculesTable->item(row, 0)->text();
    QVariantMap v = DatabaseManager::instance().getTransportVehicule(id);  // ✅ CORRECTION ICI

    if (v.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Véhicule introuvable !");
        return;
    }

    loadVehicule(v);
    onTabAdd();
}
void TransportPage::loadVehicule(const QVariantMap &v)
{
    ui->idEdit->setText(v.value("id_vehicule").toString());
    ui->typeCombo->setCurrentText(v.value("type").toString());
    ui->capaciteEdit->setText(v.value("capacite").toString());
    ui->zoneEdit->setText(v.value("zone").toString());
    ui->horaireEdit->setText(v.value("horaire").toString());
    ui->statutCombo->setCurrentText(v.value("statut").toString());
    ui->dateAjoutEdit->setDate(QDate::fromString(v.value("date_ajout").toString(), "yyyy-MM-dd"));
    ui->tempsUtiliseEdit->setText(v.value("temps_utilise").toString());
    ui->codeUniqueEdit->setText(v.value("code_unique").toString());
}

void TransportPage::onDeleteClicked()
{
    int row = ui->vehiculesTable->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Supprimer", "Veuillez sélectionner un véhicule.");
        return;
    }

    QString id = ui->vehiculesTable->item(row, 0)->text();

    if (QMessageBox::question(this, "Confirmer",
                              QString("Voulez-vous vraiment supprimer le véhicule %1 ?").arg(id),
                              QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
        return;
    }

    if (DatabaseManager::instance().deleteTransportVehicule(id)) {  // ✅ CORRECTION ICI
        QMessageBox::information(this, "Succès", "Véhicule supprimé avec succès !");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression !");
    }
}

void TransportPage::onIdTextChanged()
{
    hideError();
}

void TransportPage::onCapaciteTextChanged()
{
    hideError();
}

bool TransportPage::validateForm()
{
    if (!validateId(ui->idEdit->text())) {
        showError("L'ID doit commencer par 'VH'");
        return false;
    }

    // code_unique est optionnel (peut être NULL)
    // La validation d'existence sera faite dans DatabaseManager

    if (ui->typeCombo->currentText().isEmpty()) {
        showError("Veuillez choisir un type");
        return false;
    }

    if (!validateCapacite(ui->capaciteEdit->text())) {
        showError("Capacité invalide (doit être un nombre positif)");
        return false;
    }

    if (ui->zoneEdit->text().trimmed().isEmpty()) {
        showError("La zone est obligatoire");
        return false;
    }

    return true;
}

bool TransportPage::validateId(const QString &id)
{
    QString trimmed = id.trimmed().toUpper();
    return trimmed.startsWith("VH") && trimmed.length() >= 3;
}

bool TransportPage::validateCapacite(const QString &c)
{
    bool ok;
    int val = c.toInt(&ok);
    return ok && val > 0;
}

void TransportPage::showError(const QString &msg)
{
    ui->errorLabel->setText(msg);
    ui->errorLabel->setStyleSheet("color: red; font-weight: bold;");
    ui->errorLabel->show();
}

void TransportPage::hideError()
{
    ui->errorLabel->hide();
}

void TransportPage::calculateAndDisplayStatistics()
{
    auto s = DatabaseManager::instance().getTransportStatistics();
    int total = s.value("total", 0).toInt();

    ui->vehiculePlusUtilise->setText(total ? QString("Total: %1").arg(total) : "Aucun");
    ui->vehiculeMoinsUtilise->setText(total ? QString("Moyenne: %1 h").arg(s.value("avg_temps", 0).toDouble(), 0, 'f', 1) : "-");
    ui->pourcentageKilometrage->setText(QString("Bus: %1").arg(s.value("count_Bus", 0).toInt()));
}

void TransportPage::populateHistoriqueTable()
{
    QTableWidget *t = ui->historiqueTable;
    t->setRowCount(0);

    // ✅ CORRECTION ICI
    auto list = DatabaseManager::instance().getAllTransportVehicules();
    t->setRowCount(list.size());

    for (int i = 0; i < list.size(); ++i) {
        auto &v = list[i];
        t->setItem(i, 0, new QTableWidgetItem(v.value("date_ajout").toString()));
        t->setItem(i, 1, new QTableWidgetItem(v.value("type").toString()));
        t->setItem(i, 2, new QTableWidgetItem(v.value("id_vehicule").toString()));
        t->setItem(i, 3, new QTableWidgetItem(v.value("temps_utilise").toString()));
        t->setItem(i, 4, new QTableWidgetItem("-"));
    }

    t->resizeColumnsToContents();
}

void TransportPage::setVehicules(const QList<QVariantMap> &rows)
{
    Q_UNUSED(rows);
    loadAllVehicules();
}

void TransportPage::showEmptyTable()
{
    ui->vehiculesTable->setRowCount(0);
}

int TransportPage::sortColumnForIndex(int index) const
{
    return (index == 0) ? 6 : 1;
}
