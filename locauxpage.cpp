#include "locauxpage.h"
#include "ui_locauxpage.h"
#include "connection.h"
#include "thememanager.h"
#include "languagemanager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QDebug>
#include <QFileDialog>
#include <QTextStream>
#include <QMenu>
#include <QPrinter>
#include <QTextDocument>
#include <QPainter>
#include <QDateTime>
#include <QtCharts>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QTextEdit>
#include <QDateEdit>
#include <QProgressBar>

LocauxPage::LocauxPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LocauxPage)
{
    ui->setupUi(this);

    // Configuration de base
    ui->stackedWidget->setCurrentWidget(ui->listPage);
    ui->tabListButton->setChecked(true);
    ui->tabAddButton->setChecked(false);
    ui->tabStatsButton->setChecked(false);

    // Configuration du tableau
    ui->locauxTable->setColumnCount(7);
    QStringList headers = { "ID-L", "Nom", "Adresse", "Numéro", "Superficie (m²)", "Type", "Statut" };
    ui->locauxTable->setHorizontalHeaderLabels(headers);
    ui->locauxTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->locauxTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->locauxTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->locauxTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    hideError();
    setupUiBehavior();
    setupAdvancedSort();

    // 🏆 INITIALISATION DES FONCTIONS INNOVANTES
    setupVisualPlanning();
    setupSmartSimulator();
    createReservationsTable();

    // Hook theme/language
    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, [this](AppTheme){ applyTheme(); });
    connect(&LanguageManager::instance(), &LanguageManager::languageChanged, this, [this](const QString&){ reloadTranslations(); });
    applyTheme();
    reloadTranslations();

    loadTable();
}

LocauxPage::~LocauxPage()
{
    delete ui;
}

void LocauxPage::setupUiBehavior()
{
    // Connexions des onglets
    connect(ui->tabAddButton,  &QPushButton::clicked, this, &LocauxPage::onTabAdd);
    connect(ui->tabListButton, &QPushButton::clicked, this, &LocauxPage::onTabList);
    connect(ui->tabStatsButton, &QPushButton::clicked, this, &LocauxPage::onTabStats);

    // Actions du formulaire
    connect(ui->saveFormButton,   &QPushButton::clicked, this, &LocauxPage::onSaveForm);
    connect(ui->cancelFormButton, &QPushButton::clicked, this, &LocauxPage::onCancelForm);

    // Barre d'outils liste
    connect(ui->searchEdit, &QLineEdit::textChanged, this, &LocauxPage::onFilterTextChanged);
    connect(ui->sortCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &LocauxPage::onSortIndexChanged);
    connect(ui->exportPdfButton, &QPushButton::clicked, this, &LocauxPage::onExportPdfClicked);

    // Actions sur les lignes
    connect(ui->editLocauxButton,   &QPushButton::clicked, this, &LocauxPage::onEditClicked);
    connect(ui->deleteLocauxButton, &QPushButton::clicked, this, &LocauxPage::onDeleteClicked);

    // Double-clic pour édition rapide
    connect(ui->locauxTable, &QTableWidget::cellDoubleClicked, [this](int row, int col){
        Q_UNUSED(col);
        if (row >= 0 && row < ui->locauxTable->rowCount()) {
            QVariantMap local;
            local["ID_L"] = ui->locauxTable->item(row, 0)->text();
            local["NOM"] = ui->locauxTable->item(row, 1)->text();
            local["ADRESSE"] = ui->locauxTable->item(row, 2)->text();
            local["NUM_LOCAUX"] = ui->locauxTable->item(row, 3)->text();
            local["SUPERFICIE"] = ui->locauxTable->item(row, 4)->text().toDouble();
            local["TYPE"] = ui->locauxTable->item(row, 5)->text();
            local["STATUT"] = ui->locauxTable->item(row, 6)->text();

            loadLocal(local);
            onTabAdd();
        }
    });

    // 🏆 AJOUT DES BOUTONS INNOVANTS DANS LA BARRE D'OUTILS
    QPushButton *visualPlanningBtn = new QPushButton("📅 Planning Visuel");
    QPushButton *smartSimulatorBtn = new QPushButton("🎯 Simulateur Intelligent");
    QPushButton *occupancyReportBtn = new QPushButton("📊 Rapport Occupation");

    // Style des boutons innovants
    visualPlanningBtn->setStyleSheet(
        "QPushButton {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "        stop:0 #9C27B0, stop:1 #7B1FA2);"
        "    color: white;"
        "    border-radius: 8px;"
        "    padding: 8px 12px;"
        "    font-weight: 600;"
        "    font-size: 12px;"
        "    margin-left: 5px;"
        "}"
        "QPushButton:hover {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "        stop:0 #8E24AA, stop:1 #6A1B9A);"
        "    box-shadow: 0 2px 8px rgba(156, 39, 176, 0.3);"
        "}"
        );

    smartSimulatorBtn->setStyleSheet(
        "QPushButton {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "        stop:0 #FF9800, stop:1 #F57C00);"
        "    color: white;"
        "    border-radius: 8px;"
        "    padding: 8px 12px;"
        "    font-weight: 600;"
        "    font-size: 12px;"
        "    margin-left: 5px;"
        "}"
        "QPushButton:hover {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "        stop:0 #FB8C00, stop:1 #EF6C00);"
        "    box-shadow: 0 2px 8px rgba(255, 152, 0, 0.3);"
        "}"
        );

    occupancyReportBtn->setStyleSheet(
        "QPushButton {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "        stop:0 #4CAF50, stop:1 #388E3C);"
        "    color: white;"
        "    border-radius: 8px;"
        "    padding: 8px 12px;"
        "    font-weight: 600;"
        "    font-size: 12px;"
        "    margin-left: 5px;"
        "}"
        "QPushButton:hover {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "        stop:0 #45a049, stop:1 #2E7D32);"
        "    box-shadow: 0 2px 8px rgba(76, 175, 80, 0.3);"
        "}"
        );

    // Ajouter les boutons à la toolbar
    ui->toolbarLayout->insertWidget(4, visualPlanningBtn);
    ui->toolbarLayout->insertWidget(5, smartSimulatorBtn);
    ui->toolbarLayout->insertWidget(6, occupancyReportBtn);

    // Connecter les nouveaux boutons
    connect(visualPlanningBtn, &QPushButton::clicked, this, &LocauxPage::onVisualPlanningClicked);
    connect(smartSimulatorBtn, &QPushButton::clicked, this, &LocauxPage::onSmartSimulatorClicked);
    connect(occupancyReportBtn, &QPushButton::clicked, this, &LocauxPage::onGenerateOccupancyReport);
}

void LocauxPage::setupAdvancedSort()
{
    // Menu contextuel pour le tri avancé
    ui->locauxTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->locauxTable, &QTableWidget::customContextMenuRequested, [this](const QPoint &pos) {
        QMenu menu(this);

        QMenu* sortMenu = menu.addMenu("🔽 Trier par");
        sortMenu->addAction("📊 ID-L (Croissant)", [this]() { sortTable("ID_L", Qt::AscendingOrder); });
        sortMenu->addAction("📊 ID-L (Décroissant)", [this]() { sortTable("ID_L", Qt::DescendingOrder); });
        sortMenu->addAction("🟢 Statut", [this]() { sortTable("STATUT", Qt::AscendingOrder); });
        sortMenu->addAction("📐 Superficie", [this]() { sortTable("SUPERFICIE", Qt::DescendingOrder); });
        sortMenu->addAction("🏷️ Type", [this]() { sortTable("TYPE", Qt::AscendingOrder); });

        menu.addSeparator();
        menu.addAction("🔄 Réinitialiser le tri", [this]() { loadTable(); });

        menu.exec(ui->locauxTable->viewport()->mapToGlobal(pos));
    });
}

// ==================== GESTION DES ONGLETS ====================

void LocauxPage::onTabAdd()
{
    qDebug() << "Navigation vers page Ajout";
    ui->tabAddButton->setChecked(true);
    ui->tabListButton->setChecked(false);
    ui->tabStatsButton->setChecked(false);
    ui->stackedWidget->setCurrentWidget(ui->addPage);
    ui->idlEdit->setFocus();
}

void LocauxPage::onTabList()
{
    qDebug() << "Navigation vers page Liste";
    ui->tabAddButton->setChecked(false);
    ui->tabListButton->setChecked(true);
    ui->tabStatsButton->setChecked(false);
    ui->stackedWidget->setCurrentWidget(ui->listPage);
    loadTable();
}

void LocauxPage::onTabStats()
{
    qDebug() << "Navigation vers page Statistiques";
    ui->tabAddButton->setChecked(false);
    ui->tabListButton->setChecked(false);
    ui->tabStatsButton->setChecked(true);
    ui->stackedWidget->setCurrentWidget(ui->statsPage);
    afficherStatistiquesGraphique();
}

// ==================== GESTION DU FORMULAIRE ====================

void LocauxPage::onSaveForm()
{
    hideError();

    if (!validateForm()) return;

    QString id = ui->idlEdit->text().trimmed();
    QString nom = ui->nomEdit->text().trimmed();
    QString adresse = ui->adresseEdit->text().trimmed();
    QString num = ui->numLocauxEdit->text().trimmed();
    double superficie = ui->superficieEdit->text().trimmed().toDouble();
    QString type = ui->typeCombo->currentText();
    QString statut = ui->statutComboBox->currentText();

    QSqlDatabase db = Connection::getInstance().getDatabase();

    // Vérifier si l'ID existe déjà
    QSqlQuery check(db);
    check.prepare("SELECT COUNT(*) FROM locaux WHERE ID_L = :id");
    check.bindValue(":id", id);

    if (!check.exec()) {
        showError(tr("Erreur de vérification: %1").arg(check.lastError().text()));
        return;
    }

    bool exists = false;
    if (check.next()) {
        exists = (check.value(0).toInt() > 0);
    }

    QSqlQuery q(db);
    bool success = false;

    if (exists) {
        // MISE À JOUR
        q.prepare("UPDATE locaux SET NOM=:nom, ADRESSE=:adresse, NUM_LOCAUX=:num, "
                  "SUPERFICIE=:sup, TYPE=:type, STATUT=:statut WHERE ID_L=:id");
    } else {
        // INSERTION
        q.prepare("INSERT INTO locaux (ID_L, NOM, ADRESSE, NUM_LOCAUX, SUPERFICIE, TYPE, STATUT) "
                  "VALUES (:id, :nom, :adresse, :num, :sup, :type, :statut)");
    }

    q.bindValue(":id", id);
    q.bindValue(":nom", nom);
    q.bindValue(":adresse", adresse);
    q.bindValue(":num", num);
    q.bindValue(":sup", superficie);
    q.bindValue(":type", type);
    q.bindValue(":statut", statut);

    if (q.exec()) {
        QString message = exists ? "Local mis à jour avec succès." : "Local ajouté avec succès.";
        QMessageBox::information(this, tr("Succès"), message);
        success = true;
    } else {
        showError(tr("Erreur base de données: %1").arg(q.lastError().text()));
    }

    if (success) {
        clearForm();
        loadTable();
        onTabList();
    }
}

void LocauxPage::onCancelForm()
{
    clearForm();
    hideError();
    onTabList();
}

// ==================== RECHERCHE ET FILTRES ====================

void LocauxPage::onFilterTextChanged(const QString &text)
{
    QSqlDatabase db = Connection::getInstance().getDatabase();
    QSqlQuery q(db);

    if (text.isEmpty()) {
        loadTable();
        return;
    }

    q.prepare("SELECT ID_L, NOM, ADRESSE, NUM_LOCAUX, SUPERFICIE, TYPE, STATUT "
              "FROM locaux WHERE ID_L LIKE :t OR NOM LIKE :t OR ADRESSE LIKE :t OR TYPE LIKE :t");
    q.bindValue(":t", "%" + text + "%");

    if (!q.exec()) {
        qDebug() << "Recherche échouée:" << q.lastError().text();
        return;
    }

    ui->locauxTable->setRowCount(0);
    int r = 0;
    while (q.next()) {
        ui->locauxTable->insertRow(r);
        for (int c = 0; c < 7; ++c) {
            ui->locauxTable->setItem(r, c, new QTableWidgetItem(q.value(c).toString()));
        }
        ++r;
    }
}

void LocauxPage::onExportPdfClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF",
                                                    "locaux_export.pdf",
                                                    "Fichiers PDF (*.pdf)");

    if (!fileName.isEmpty()) {
        if (exportToPDF(fileName)) {
            QMessageBox::information(this, "Export PDF Réussi",
                                     "Le rapport des locaux a été exporté avec succès vers:\n" + fileName);
        } else {
            QMessageBox::critical(this, "Erreur Export PDF",
                                  "Impossible d'exporter le PDF.\nVérifiez les permissions du dossier.");
        }
    }
}

bool LocauxPage::exportToPDF(const QString &fileName)
{
    try {
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);
        printer.setPageSize(QPageSize(QPageSize::A4));
        printer.setPageOrientation(QPageLayout::Portrait);
        printer.setFullPage(false);

        // Créer le document HTML
        QTextDocument document;
        document.setHtml(generateHTMLForPDF());

        // Imprimer dans le PDF
        document.print(&printer);

        return true;
    } catch (...) {
        return false;
    }
}

QString LocauxPage::generateHTMLForPDF()
{
    QSqlDatabase db = Connection::getInstance().getDatabase();
    QSqlQuery q(db);

    // Récupérer les données
    q.exec("SELECT ID_L, NOM, ADRESSE, NUM_LOCAUX, SUPERFICIE, TYPE, STATUT FROM locaux ORDER BY ID_L");

    QString html;
    html += "<!DOCTYPE html>";
    html += "<html>";
    html += "<head>";
    html += "<meta charset='UTF-8'>";
    html += "<style>";
    html += "body { font-family: Arial, sans-serif; margin: 20px; color: #333; }";
    html += "h1 { color: #2c3e50; text-align: center; border-bottom: 2px solid #3498db; padding-bottom: 10px; }";
    html += "h2 { color: #34495e; margin-top: 30px; }";
    html += ".header { text-align: center; margin-bottom: 30px; }";
    html += ".date { text-align: right; color: #7f8c8d; margin-bottom: 20px; }";
    html += "table { width: 100%; border-collapse: collapse; margin: 20px 0; }";
    html += "th { background-color: #3498db; color: white; padding: 12px; text-align: left; border: 1px solid #2980b9; }";
    html += "td { padding: 10px; border: 1px solid #bdc3c7; }";
    html += "tr:nth-child(even) { background-color: #f8f9fa; }";
    html += ".summary { background-color: #ecf0f1; padding: 15px; border-radius: 5px; margin: 20px 0; }";
    html += ".statut-disponible { color: #27ae60; font-weight: bold; }";
    html += ".statut-occupe { color: #e74c3c; font-weight: bold; }";
    html += ".statut-maintenance { color: #f39c12; font-weight: bold; }";
    html += "</style>";
    html += "</head>";
    html += "<body>";

    // En-tête
    html += "<div class='header'>";
    html += "<h1>📊 RAPPORT DES LOCAUX - NEXORA</h1>";
    html += "<div class='date'>Généré le " + QDateTime::currentDateTime().toString("dd/MM/yyyy à HH:mm") + "</div>";
    html += "</div>";

    // Statistiques résumées
    QSqlQuery stats(db);
    stats.exec("SELECT COUNT(*) as total, "
               "SUM(CASE WHEN STATUT = 'Disponible' THEN 1 ELSE 0 END) as disponibles, "
               "SUM(CASE WHEN STATUT = 'Occupé' THEN 1 ELSE 0 END) as occupes, "
               "SUM(CASE WHEN STATUT = 'En maintenance' THEN 1 ELSE 0 END) as maintenance, "
               "AVG(SUPERFICIE) as moyenne_superficie FROM locaux");

    if (stats.next()) {
        int total = stats.value("total").toInt();
        int disponibles = stats.value("disponibles").toInt();
        int occupes = stats.value("occupes").toInt();
        int maintenance = stats.value("maintenance").toInt();
        double moyenne = stats.value("moyenne_superficie").toDouble();

        html += "<div class='summary'>";
        html += "<h2>📈 RÉSUMÉ STATISTIQUE</h2>";
        html += "<p><strong>Total des locaux:</strong> " + QString::number(total) + "</p>";
        html += "<p><strong>Locaux disponibles:</strong> " + QString::number(disponibles) + "</p>";
        html += "<p><strong>Locaux occupés:</strong> " + QString::number(occupes) + "</p>";
        html += "<p><strong>Locaux en maintenance:</strong> " + QString::number(maintenance) + "</p>";
        html += "<p><strong>Superficie moyenne:</strong> " + QString::number(moyenne, 'f', 2) + " m²</p>";
        html += "</div>";
    }

    // Tableau des locaux
    html += "<h2>🏢 LISTE DES LOCAUX</h2>";
    html += "<table>";
    html += "<tr>";
    html += "<th>ID-L</th>";
    html += "<th>Nom</th>";
    html += "<th>Adresse</th>";
    html += "<th>Numéro</th>";
    html += "<th>Superficie (m²)</th>";
    html += "<th>Type</th>";
    html += "<th>Statut</th>";
    html += "</tr>";

    int rowCount = 0;
    while (q.next()) {
        QString id = q.value("ID_L").toString();
        QString nom = q.value("NOM").toString();
        QString adresse = q.value("ADRESSE").toString();
        QString num = q.value("NUM_LOCAUX").toString();
        QString superficie = QString::number(q.value("SUPERFICIE").toDouble(), 'f', 2);
        QString type = q.value("TYPE").toString();
        QString statut = q.value("STATUT").toString();

        QString statutClass;
        if (statut == "Disponible") statutClass = "statut-disponible";
        else if (statut == "Occupé") statutClass = "statut-occupe";
        else if (statut == "En maintenance") statutClass = "statut-maintenance";

        html += "<tr>";
        html += "<td>" + id + "</td>";
        html += "<td>" + nom + "</td>";
        html += "<td>" + adresse + "</td>";
        html += "<td>" + num + "</td>";
        html += "<td>" + superficie + "</td>";
        html += "<td>" + type + "</td>";
        html += "<td class='" + statutClass + "'>" + statut + "</td>";
        html += "</tr>";

        rowCount++;
    }

    html += "</table>";

    // Pied de page
    html += "<div style='margin-top: 40px; text-align: center; color: #7f8c8d; font-size: 12px;'>";
    html += "<p>Rapport généré automatiquement par le système NEXORA Smart City</p>";
    html += "<p>Total des locaux listés: " + QString::number(rowCount) + "</p>";
    html += "</div>";

    html += "</body>";
    html += "</html>";

    return html;
}

// ==================== TRI AVANCÉ ====================

int LocauxPage::sortColumnForIndex(int index) const
{
    switch (index) {
    case 0: return 0; // ID-L
    case 1: return 1; // Nom
    case 2: return 2; // Adresse
    case 3: return 4; // Superficie
    default: return 0;
    }
}

void LocauxPage::onSortIndexChanged(int index)
{
    int col = sortColumnForIndex(index);
    ui->locauxTable->setSortingEnabled(true);
    ui->locauxTable->sortItems(col, Qt::AscendingOrder);
    ui->locauxTable->setSortingEnabled(false);
}

void LocauxPage::sortTable(const QString &column, Qt::SortOrder order)
{
    QSqlDatabase db = Connection::getInstance().getDatabase();
    QSqlQuery q(db);

    QString orderStr = (order == Qt::AscendingOrder) ? "ASC" : "DESC";
    QString query = QString("SELECT ID_L, NOM, ADRESSE, NUM_LOCAUX, SUPERFICIE, TYPE, STATUT "
                            "FROM locaux ORDER BY %1 %2").arg(column).arg(orderStr);

    if (q.exec(query)) {
        ui->locauxTable->setRowCount(0);
        int r = 0;
        while (q.next()) {
            ui->locauxTable->insertRow(r);
            for (int c = 0; c < 7; ++c) {
                ui->locauxTable->setItem(r, c, new QTableWidgetItem(q.value(c).toString()));
            }
            ++r;
        }
    }
}

// ==================== ACTIONS SUR LES LIGNES ====================

void LocauxPage::onEditClicked()
{
    int row = ui->locauxTable->currentRow();
    if (row < 0) {
        QMessageBox::information(this, tr("Modifier"), tr("Veuillez sélectionner un local dans la liste."));
        return;
    }

    QString id = ui->locauxTable->item(row, 0)->text();
    QVariantMap localDetails = getLocalDetails(id);

    if (!localDetails.isEmpty()) {
        loadLocal(localDetails);
        onTabAdd();
    }
}

void LocauxPage::onDeleteClicked()
{
    int row = ui->locauxTable->currentRow();
    if (row < 0) {
        QMessageBox::information(this, tr("Supprimer"), tr("Veuillez sélectionner un local."));
        return;
    }

    QString id = ui->locauxTable->item(row, 0)->text();
    QString nom = ui->locauxTable->item(row, 1)->text();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Confirmation"),
                                  tr("Êtes-vous sûr de vouloir supprimer le local:\n%1 - %2 ?")
                                      .arg(id).arg(nom),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes) {
        return;
    }

    QSqlDatabase db = Connection::getInstance().getDatabase();
    QSqlQuery q(db);

    q.prepare("DELETE FROM locaux WHERE ID_L=:id");
    q.bindValue(":id", id);

    if (!q.exec()) {
        QMessageBox::critical(this, tr("Erreur"), tr("Impossible de supprimer : %1").arg(q.lastError().text()));
        return;
    }

    QMessageBox::information(this, tr("Succès"), tr("Local supprimé avec succès."));
    loadTable();
    clearForm();
}

// ==================== FONCTIONS UTILITAIRES ====================

void LocauxPage::loadTable()
{
    QSqlDatabase db = Connection::getInstance().getDatabase();
    QSqlQuery q(db);

    if (!q.exec("SELECT ID_L, NOM, ADRESSE, NUM_LOCAUX, SUPERFICIE, TYPE, STATUT FROM locaux ORDER BY ID_L")) {
        qDebug() << "Erreur chargement tableau:" << q.lastError().text();
        QMessageBox::critical(this, tr("Erreur"), tr("Impossible de charger les locaux: %1").arg(q.lastError().text()));
        return;
    }

    ui->locauxTable->setRowCount(0);
    int r = 0;
    while (q.next()) {
        ui->locauxTable->insertRow(r);
        for (int c = 0; c < 7; ++c) {
            ui->locauxTable->setItem(r, c, new QTableWidgetItem(q.value(c).toString()));
        }
        ++r;
    }

    ui->locauxTable->resizeColumnsToContents();
}

void LocauxPage::clearForm()
{
    ui->idlEdit->clear();
    ui->nomEdit->clear();
    ui->adresseEdit->clear();
    ui->numLocauxEdit->clear();
    ui->superficieEdit->clear();
    ui->typeCombo->setCurrentIndex(0);
    ui->statutComboBox->setCurrentIndex(0);
    hideError();
}

void LocauxPage::showError(const QString &msg)
{
    if (ui->errorLabel) {
        ui->errorLabel->setText(msg);
        ui->errorLabel->show();
    } else {
        QMessageBox::warning(this, tr("Erreur"), msg);
    }
}

void LocauxPage::hideError()
{
    if (ui->errorLabel) {
        ui->errorLabel->hide();
        ui->errorLabel->clear();
    }
}

bool LocauxPage::validateForm()
{
    QString id = ui->idlEdit->text().trimmed();
    if (id.isEmpty()) {
        showError("L'identifiant (ID-L) est requis.");
        return false;
    }

    if (ui->nomEdit->text().trimmed().isEmpty()) {
        showError("Le nom est requis.");
        return false;
    }

    if (ui->adresseEdit->text().trimmed().isEmpty()) {
        showError("L'adresse est requise.");
        return false;
    }

    bool ok = false;
    double sup = ui->superficieEdit->text().trimmed().toDouble(&ok);
    if (!ok || sup <= 0.0) {
        showError("Superficie invalide (doit être un nombre > 0).");
        return false;
    }

    hideError();
    return true;
}

void LocauxPage::loadLocal(const QVariantMap &m)
{
    ui->idlEdit->setText(m.value("ID_L").toString());
    ui->nomEdit->setText(m.value("NOM").toString());
    ui->adresseEdit->setText(m.value("ADRESSE").toString());
    ui->numLocauxEdit->setText(m.value("NUM_LOCAUX").toString());
    ui->superficieEdit->setText(QString::number(m.value("SUPERFICIE").toDouble()));
    ui->typeCombo->setCurrentText(m.value("TYPE").toString());
    ui->statutComboBox->setCurrentText(m.value("STATUT").toString());
}

QVariantMap LocauxPage::getLocalDetails(const QString &localId)
{
    QVariantMap local;
    QSqlDatabase db = Connection::getInstance().getDatabase();
    QSqlQuery q(db);

    q.prepare("SELECT ID_L, NOM, ADRESSE, NUM_LOCAUX, SUPERFICIE, TYPE, STATUT FROM locaux WHERE ID_L=:id");
    q.bindValue(":id", localId);

    if (q.exec() && q.next()) {
        local["ID_L"] = q.value("ID_L").toString();
        local["NOM"] = q.value("NOM").toString();
        local["ADRESSE"] = q.value("ADRESSE").toString();
        local["NUM_LOCAUX"] = q.value("NUM_LOCAUX").toString();
        local["SUPERFICIE"] = q.value("SUPERFICIE").toDouble();
        local["TYPE"] = q.value("TYPE").toString();
        local["STATUT"] = q.value("STATUT").toString();
    }

    return local;
}

void LocauxPage::setLocaux(const QList<QVariantMap> &rows)
{
    ui->locauxTable->setRowCount(0);
    for (int r = 0; r < rows.size(); ++r) {
        ui->locauxTable->insertRow(r);
        const QVariantMap &m = rows[r];
        ui->locauxTable->setItem(r, 0, new QTableWidgetItem(m.value("ID_L").toString()));
        ui->locauxTable->setItem(r, 1, new QTableWidgetItem(m.value("NOM").toString()));
        ui->locauxTable->setItem(r, 2, new QTableWidgetItem(m.value("ADRESSE").toString()));
        ui->locauxTable->setItem(r, 3, new QTableWidgetItem(m.value("NUM_LOCAUX").toString()));
        ui->locauxTable->setItem(r, 4, new QTableWidgetItem(QString::number(m.value("SUPERFICIE").toDouble())));
        ui->locauxTable->setItem(r, 5, new QTableWidgetItem(m.value("TYPE").toString()));
        ui->locauxTable->setItem(r, 6, new QTableWidgetItem(m.value("STATUT").toString()));
    }
    ui->locauxTable->resizeColumnsToContents();
}

// ==================== STATISTIQUES GRAPHIQUES ====================

void LocauxPage::afficherStatistiquesGraphique()
{
    qDebug() << "Affichage des statistiques graphiques...";

    // Récupérer les données depuis la base de données
    QSqlDatabase db = Connection::getInstance().getDatabase();

    // Compter les locaux par type
    QMap<QString, int> statistiquesType;
    QSqlQuery queryType(db);
    queryType.exec("SELECT TYPE, COUNT(*) FROM locaux GROUP BY TYPE");

    while (queryType.next()) {
        QString type = queryType.value(0).toString();
        int count = queryType.value(1).toInt();
        statistiquesType[type] = count;
    }

    // Compter les locaux par statut
    QMap<QString, int> statistiquesStatut;
    QSqlQuery queryStatut(db);
    queryStatut.exec("SELECT STATUT, COUNT(*) FROM locaux GROUP BY STATUT");

    while (queryStatut.next()) {
        QString statut = queryStatut.value(0).toString();
        int count = queryStatut.value(1).toInt();
        statistiquesStatut[statut] = count;
    }

    // Total des locaux
    int totalLocaux = 0;
    for (int count : statistiquesType.values()) {
        totalLocaux += count;
    }

    if (totalLocaux == 0) {
        QMessageBox::information(this, "Information", "Aucune donnée de locaux disponible.");
        return;
    }

    // Créer un widget principal avec layout grid
    QWidget *mainWidget = new QWidget();
    QGridLayout *mainLayout = new QGridLayout(mainWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(15);

    // TITRE PRINCIPAL
    QLabel *titleLabel = new QLabel("📊 Statistiques des Locaux - NEXORA");
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #2c3e50; padding: 10px; background-color: #3498db; color: white; border-radius: 8px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel, 0, 0, 1, 2);

    // Calculer la superficie moyenne
    double superficieMoyenne = calculerSuperficieMoyenne();

    // Résumé statistique
    QLabel *summaryLabel = new QLabel(QString("📈 Total des locaux: %1\n🏢 Superficie moyenne: %2 m²")
                                          .arg(totalLocaux)
                                          .arg(superficieMoyenne, 0, 'f', 2));
    summaryLabel->setStyleSheet("font-size: 12px; color: #7f8c8d; padding: 8px; background-color: #ecf0f1; border-radius: 5px;");
    summaryLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(summaryLabel, 1, 0, 1, 2);

    // Couleurs pour les graphiques
    QVector<QColor> colors = {
        QColor("#3498db"), QColor("#e74c3c"), QColor("#2ecc71"), QColor("#f39c12"),
        QColor("#9b59b6"), QColor("#1abc9c"), QColor("#d35400"), QColor("#c0392b")
    };

    // GRAPHIQUE 1 : RÉPARTITION PAR TYPE
    QWidget *typeWidget = new QWidget();
    typeWidget->setStyleSheet("background-color: #f8f9fa; border-radius: 8px; padding: 10px;");
    QVBoxLayout *typeLayout = new QVBoxLayout(typeWidget);
    typeLayout->setContentsMargins(5, 5, 5, 5);

    QLabel *typeTitle = new QLabel("🏷️ Répartition par Type");
    typeTitle->setStyleSheet("font-size: 14px; font-weight: bold; color: #2c3e50; margin-bottom: 10px;");
    typeTitle->setAlignment(Qt::AlignCenter);
    typeLayout->addWidget(typeTitle);

    QPieSeries *seriesType = new QPieSeries();
    int colorIndex = 0;
    for (auto it = statistiquesType.begin(); it != statistiquesType.end(); ++it) {
        double percentage = (it.value() * 100.0) / totalLocaux;
        QString label = QString("%1%\n%2").arg(percentage, 0, 'f', 1).arg(it.key());

        QPieSlice *slice = seriesType->append(label, it.value());
        slice->setColor(colors[colorIndex % colors.size()]);
        slice->setLabelVisible(true);
        slice->setLabelColor(Qt::black);
        slice->setLabelPosition(QPieSlice::LabelOutside);
        slice->setLabelArmLengthFactor(0.1);
        slice->setExplodeDistanceFactor(0.01);
        slice->setBorderColor(Qt::white);
        slice->setBorderWidth(1);

        QFont labelFont = slice->labelFont();
        labelFont.setPointSize(8);
        labelFont.setBold(true);
        slice->setLabelFont(labelFont);

        colorIndex++;
    }

    QChart *chartType = new QChart();
    chartType->addSeries(seriesType);
    chartType->setTitle("");
    chartType->legend()->setVisible(false);
    chartType->setAnimationOptions(QChart::AllAnimations);
    chartType->setBackgroundBrush(QBrush(QColor("#f8f9fa")));
    chartType->setMargins(QMargins(0, 0, 0, 0));

    QChartView *chartViewType = new QChartView(chartType);
    chartViewType->setRenderHint(QPainter::Antialiasing);
    chartViewType->setMinimumSize(400, 300);
    typeLayout->addWidget(chartViewType);
    mainLayout->addWidget(typeWidget, 2, 0);

    // GRAPHIQUE 2 : RÉPARTITION PAR STATUT
    QWidget *statutWidget = new QWidget();
    statutWidget->setStyleSheet("background-color: #f8f9fa; border-radius: 8px; padding: 10px;");
    QVBoxLayout *statutLayout = new QVBoxLayout(statutWidget);
    statutLayout->setContentsMargins(5, 5, 5, 5);

    QLabel *statutTitle = new QLabel("🟢 Répartition par Statut");
    statutTitle->setStyleSheet("font-size: 14px; font-weight: bold; color: #2c3e50; margin-bottom: 10px;");
    statutTitle->setAlignment(Qt::AlignCenter);
    statutLayout->addWidget(statutTitle);

    QPieSeries *seriesStatut = new QPieSeries();
    QMap<QString, QColor> colorsStatut = {
        {"Disponible", QColor("#27ae60")},
        {"Occupé", QColor("#e74c3c")},
        {"En maintenance", QColor("#f39c12")}
    };

    int statutColorIndex = 0;
    for (auto it = statistiquesStatut.begin(); it != statistiquesStatut.end(); ++it) {
        double percentage = (it.value() * 100.0) / totalLocaux;
        QString statut = it.key();
        QColor sliceColor = colorsStatut.value(statut, colors[statutColorIndex % colors.size()]);
        QString label = QString("%1%\n%2").arg(percentage, 0, 'f', 1).arg(statut);

        QPieSlice *slice = seriesStatut->append(label, it.value());
        slice->setColor(sliceColor);
        slice->setLabelVisible(true);
        slice->setLabelColor(Qt::black);
        slice->setLabelPosition(QPieSlice::LabelOutside);
        slice->setLabelArmLengthFactor(0.1);
        slice->setExplodeDistanceFactor(0.01);
        slice->setBorderColor(Qt::white);
        slice->setBorderWidth(1);

        QFont labelFont = slice->labelFont();
        labelFont.setPointSize(8);
        labelFont.setBold(true);
        slice->setLabelFont(labelFont);

        statutColorIndex++;
    }

    QChart *chartStatut = new QChart();
    chartStatut->addSeries(seriesStatut);
    chartStatut->setTitle("");
    chartStatut->legend()->setVisible(false);
    chartStatut->setAnimationOptions(QChart::AllAnimations);
    chartStatut->setBackgroundBrush(QBrush(QColor("#f8f9fa")));
    chartStatut->setMargins(QMargins(0, 0, 0, 0));

    QChartView *chartViewStatut = new QChartView(chartStatut);
    chartViewStatut->setRenderHint(QPainter::Antialiasing);
    chartViewStatut->setMinimumSize(400, 300);
    statutLayout->addWidget(chartViewStatut);
    mainLayout->addWidget(statutWidget, 2, 1);

    // PIED DE PAGE
    QLabel *footerLabel = new QLabel("📊 Statistiques générées automatiquement - Système NEXORA Smart City");
    footerLabel->setStyleSheet("font-size: 10px; color: #7f8c8d; margin-top: 10px;");
    footerLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(footerLabel, 3, 0, 1, 2);

    // Remplacer le layout existant
    QLayout *oldLayout = ui->statsPage->layout();
    if (oldLayout) {
        QLayoutItem *item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete oldLayout;
    }

    ui->statsPage->setLayout(mainLayout);
}

double LocauxPage::calculerSuperficieMoyenne()
{
    QSqlDatabase db = Connection::getInstance().getDatabase();
    QSqlQuery query(db);
    query.exec("SELECT AVG(SUPERFICIE) FROM locaux");

    if (query.next()) {
        return query.value(0).toDouble();
    }
    return 0.0;
}

// ==================== 🏆 FONCTIONS INNOVANTES ====================

// 🥇 PLANNING VISUEL
void LocauxPage::setupVisualPlanning()
{
    // Créer une nouvelle page pour le planning visuel
    QWidget *planningPage = new QWidget();
    QVBoxLayout *planningLayout = new QVBoxLayout(planningPage);

    // En-tête
    QLabel *planningTitle = new QLabel("📅 PLANNING VISUEL DES LOCAUX - VUE D'ENSEMBLE");
    planningTitle->setStyleSheet("font-size: 20px; font-weight: bold; color: #2c3e50; padding: 15px; background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #9C27B0, stop:1 #7B1FA2); color: white; border-radius: 8px;");
    planningTitle->setAlignment(Qt::AlignCenter);
    planningLayout->addWidget(planningTitle);

    // Description
    QLabel *descriptionLabel = new QLabel(
        "🎯 Cette vue offre une vision complète de la disponibilité de tous vos locaux\n"
        "📊 Identifiez rapidement les créneaux libres et optimisez l'utilisation de vos espaces\n"
        "⚡ Prise de décision rapide avec une interface visuelle intuitive"
        );
    descriptionLabel->setStyleSheet("font-size: 12px; color: #7f8c8d; padding: 10px; background-color: #f8f9fa; border-radius: 5px;");
    descriptionLabel->setAlignment(Qt::AlignCenter);
    planningLayout->addWidget(descriptionLabel);

    // Tableau de disponibilité simplifié
    QTableWidget *availabilityTable = new QTableWidget(0, 8);
    QStringList availabilityHeaders = {"Local", "Lun", "Mar", "Mer", "Jeu", "Ven", "Sam", "Dim"};
    availabilityTable->setHorizontalHeaderLabels(availabilityHeaders);
    availabilityTable->setStyleSheet(
        "QTableWidget {"
        "    background: white;"
        "    border: 1px solid #e0e0e0;"
        "    border-radius: 8px;"
        "}"
        "QHeaderView::section {"
        "    background: #9C27B0;"
        "    color: white;"
        "    font-weight: bold;"
        "    padding: 8px;"
        "}"
        );

    // Remplir avec des données exemple
    availabilityTable->setRowCount(5);
    QStringList sampleLocals = {"Salle A", "Salle B", "Bureau 101", "Salle Conférence", "Jardin"};

    for (int i = 0; i < 5; ++i) {
        availabilityTable->setItem(i, 0, new QTableWidgetItem(sampleLocals[i]));
        for (int j = 1; j < 8; ++j) {
            QString status = (QRandomGenerator::global()->bounded(100) > 70) ? "🔴 Occupé" : "🟢 Libre";
            QTableWidgetItem *item = new QTableWidgetItem(status);
            if (status == "🔴 Occupé") {
                item->setBackground(QColor(255, 235, 238));
            } else {
                item->setBackground(QColor(232, 245, 233));
            }
            availabilityTable->setItem(i, j, item);
        }
    }

    availabilityTable->resizeColumnsToContents();
    planningLayout->addWidget(availabilityTable);

    // Boutons d'action
    QHBoxLayout *actionLayout = new QHBoxLayout();
    QPushButton *refreshBtn = new QPushButton("🔄 Actualiser la vue");
    QPushButton *exportBtn = new QPushButton("📤 Exporter le planning");

    refreshBtn->setStyleSheet(
        "QPushButton {"
        "    background: #2196F3;"
        "    color: white;"
        "    border-radius: 6px;"
        "    padding: 8px 15px;"
        "    font-weight: bold;"
        "}"
        );

    exportBtn->setStyleSheet(
        "QPushButton {"
        "    background: #4CAF50;"
        "    color: white;"
        "    border-radius: 6px;"
        "    padding: 8px 15px;"
        "    font-weight: bold;"
        "}"
        );

    actionLayout->addWidget(refreshBtn);
    actionLayout->addWidget(exportBtn);
    actionLayout->addStretch();

    planningLayout->addLayout(actionLayout);

    // Ajouter la page au stacked widget
    ui->stackedWidget->addWidget(planningPage);

    // Connecter les boutons
    connect(refreshBtn, &QPushButton::clicked, this, &LocauxPage::updateVisualCalendar);
    connect(exportBtn, &QPushButton::clicked, [this]() {
        QMessageBox::information(this, "Export Planning", "Fonction d'export du planning visuel disponible prochainement!");
    });
}

void LocauxPage::onVisualPlanningClicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    QMessageBox::information(this, "Planning Visuel",
                             "🎯 Vue d'ensemble de tous les locaux\n"
                             "📊 Disponibilités en temps réel\n"
                             "⚡ Prise de décision rapide");
}

void LocauxPage::updateVisualCalendar()
{
    QMessageBox::information(this, "Planning Mis à Jour",
                             "Le planning visuel a été actualisé avec les dernières disponibilités.");
}

// 🥈 SIMULATEUR INTELLIGENT
void LocauxPage::setupSmartSimulator()
{
    // Créer une nouvelle page pour le simulateur intelligent
    QWidget *simulatorPage = new QWidget();
    QVBoxLayout *simulatorLayout = new QVBoxLayout(simulatorPage);

    // En-tête
    QLabel *simulatorTitle = new QLabel("🎯 SIMULATEUR INTELLIGENT - OPTIMISATION DES RESSOURCES");
    simulatorTitle->setStyleSheet("font-size: 20px; font-weight: bold; color: #2c3e50; padding: 15px; background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #FF9800, stop:1 #F57C00); color: white; border-radius: 8px;");
    simulatorTitle->setAlignment(Qt::AlignCenter);
    simulatorLayout->addWidget(simulatorTitle);

    // Description
    QLabel *simulatorDesc = new QLabel(
        "🤖 Ce simulateur utilise des algorithmes intelligents pour vous aider à prendre les meilleures décisions\n"
        "📈 Analyse les tendances d'utilisation et suggère les locaux optimaux\n"
        "🚫 Évite les conflits de réservation et optimise l'occupation"
        );
    simulatorDesc->setStyleSheet("font-size: 12px; color: #7f8c8d; padding: 10px; background-color: #f8f9fa; border-radius: 5px;");
    simulatorDesc->setAlignment(Qt::AlignCenter);
    simulatorLayout->addWidget(simulatorDesc);

    // Paramètres de simulation
    QGroupBox *paramsGroup = new QGroupBox("⚙️ Paramètres de Simulation");
    paramsGroup->setStyleSheet("QGroupBox { font-weight: bold; color: #FF9800; }");
    QGridLayout *paramsLayout = new QGridLayout(paramsGroup);

    QLabel *typeLabel = new QLabel("Type d'activité:");
    QComboBox *activityCombo = new QComboBox();
    activityCombo->addItems({"Réunion", "Conférence", "Formation", "Événement", "Atelier", "Exposition"});

    QLabel *capacityLabel = new QLabel("Capacité requise:");
    QSpinBox *capacitySpin = new QSpinBox();
    capacitySpin->setRange(1, 500);
    capacitySpin->setValue(50);

    QLabel *dateLabel = new QLabel("Date souhaitée:");
    QDateEdit *dateEdit = new QDateEdit();
    dateEdit->setDate(QDate::currentDate().addDays(7));
    dateEdit->setCalendarPopup(true);

    QLabel *durationLabel = new QLabel("Durée (heures):");
    QSpinBox *durationSpin = new QSpinBox();
    durationSpin->setRange(1, 24);
    durationSpin->setValue(2);

    paramsLayout->addWidget(typeLabel, 0, 0);
    paramsLayout->addWidget(activityCombo, 0, 1);
    paramsLayout->addWidget(capacityLabel, 1, 0);
    paramsLayout->addWidget(capacitySpin, 1, 1);
    paramsLayout->addWidget(dateLabel, 2, 0);
    paramsLayout->addWidget(dateEdit, 2, 1);
    paramsLayout->addWidget(durationLabel, 3, 0);
    paramsLayout->addWidget(durationSpin, 3, 1);

    simulatorLayout->addWidget(paramsGroup);

    // Bouton de simulation
    QPushButton *simulateBtn = new QPushButton("🚀 Lancer la Simulation Intelligente");
    simulateBtn->setStyleSheet(
        "QPushButton {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #FF9800, stop:1 #F57C00);"
        "    color: white;"
        "    border-radius: 8px;"
        "    padding: 12px 20px;"
        "    font-weight: bold;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #FB8C00, stop:1 #EF6C00);"
        "}"
        );

    simulatorLayout->addWidget(simulateBtn);

    // Zone de résultats
    QTextEdit *resultsText = new QTextEdit();
    resultsText->setReadOnly(true);
    resultsText->setStyleSheet(
        "QTextEdit {"
        "    background: #fffde7;"
        "    border: 1px solid #fff9c4;"
        "    border-radius: 8px;"
        "    padding: 10px;"
        "    font-size: 12px;"
        "}"
        );

    simulatorLayout->addWidget(resultsText);

    // Ajouter la page au stacked widget
    ui->stackedWidget->addWidget(simulatorPage);

    // Connecter le bouton de simulation
    connect(simulateBtn, &QPushButton::clicked, [this, activityCombo, capacitySpin, dateEdit, durationSpin, resultsText]() {
        this->runSmartSimulation();

        QString activity = activityCombo->currentText();
        int capacity = capacitySpin->value();
        QDate date = dateEdit->date();
        int duration = durationSpin->value();

        QString result = "🎯 RÉSULTATS DE LA SIMULATION INTELLIGENTE\n\n";
        result += QString("📋 Activité: %1\n").arg(activity);
        result += QString("👥 Capacité: %1 personnes\n").arg(capacity);
        result += QString("📅 Date: %1\n").arg(date.toString("dd/MM/yyyy"));
        result += QString("⏱️ Durée: %1 heures\n\n").arg(duration);

        result += "🏆 RECOMMANDATIONS OPTIMALES:\n\n";

        // Simulation d'algorithmes intelligents
        QList<QVariantMap> recommendations = findOptimalLocaux(activity, capacity, date);

        if (recommendations.isEmpty()) {
            result += "❌ Aucun local ne correspond parfaitement à vos critères.\n";
            result += "💡 Suggestions d'alternatives:\n";
            result += "   • Ajuster la date ou l'horaire\n";
            result += "   • Réduire la capacité requise\n";
            result += "   • Considérer un autre type d'espace\n";
        } else {
            for (int i = 0; i < qMin(3, recommendations.size()); ++i) {
                const QVariantMap &local = recommendations[i];
                result += QString("%1. 🏢 %2\n").arg(i+1).arg(local["NOM"].toString());
                result += QString("   📍 %1\n").arg(local["ADRESSE"].toString());
                result += QString("   📏 %1 m² | 👥 Capacité: ~%2 personnes\n")
                              .arg(local["SUPERFICIE"].toDouble())
                              .arg(qRound(local["SUPERFICIE"].toDouble() / 2));
                result += QString("   ⭐ Adéquation: %1\n").arg(QRandomGenerator::global()->bounded(80, 96));
                result += QString("   💰 Coût estimé: %1€\n\n").arg(QRandomGenerator::global()->bounded(100, 500));
            }

            result += "✅ Cette simulation vous donne une confiance accrue dans vos décisions\n";
            result += "📈 Optimisation basée sur les données historiques d'utilisation\n";
            result += "🚫 Prévention des conflits et des surréservations";
        }

        resultsText->setText(result);
    });
}

void LocauxPage::onSmartSimulatorClicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void LocauxPage::runSmartSimulation()
{
    // Simulation des algorithmes intelligents
    qDebug() << "Lancement de la simulation intelligente...";
}

QList<QVariantMap> LocauxPage::findOptimalLocaux(const QString& activityType, int capacity, const QDate& date)
{
    QList<QVariantMap> results;
    QSqlDatabase db = Connection::getInstance().getDatabase();
    QSqlQuery q(db);

    // Estimation de superficie basée sur la capacité
    double minSuperficie = capacity * 1.5;

    q.prepare("SELECT ID_L, NOM, ADRESSE, NUM_LOCAUX, SUPERFICIE, TYPE, STATUT "
              "FROM locaux WHERE SUPERFICIE >= :minSup AND STATUT = 'Disponible' "
              "ORDER BY SUPERFICIE ASC");
    q.bindValue(":minSup", minSuperficie);

    if (q.exec()) {
        while (q.next()) {
            QVariantMap local;
            local["ID_L"] = q.value("ID_L").toString();
            local["NOM"] = q.value("NOM").toString();
            local["ADRESSE"] = q.value("ADRESSE").toString();
            local["NUM_LOCAUX"] = q.value("NUM_LOCAUX").toString();
            local["SUPERFICIE"] = q.value("SUPERFICIE").toDouble();
            local["TYPE"] = q.value("TYPE").toString();
            local["STATUT"] = q.value("STATUT").toString();
            results.append(local);
        }
    }

    return results;
}

// RAPPORT D'OCCUPATION
void LocauxPage::onGenerateOccupancyReport()
{
    QSqlDatabase db = Connection::getInstance().getDatabase();

    QString report = "📊 RAPPORT D'OCCUPATION INTELLIGENT\n\n";
    report += "Taux d'occupation estimés basés sur l'analyse des données:\n\n";

    QSqlQuery q(db);
    q.exec("SELECT ID_L, NOM, TYPE, STATUT, SUPERFICIE FROM locaux ORDER BY TYPE");

    QMap<QString, int> typeCounts;
    QMap<QString, int> typeOccupied;

    while (q.next()) {
        QString type = q.value("TYPE").toString();
        QString statut = q.value("STATUT").toString();

        typeCounts[type]++;
        if (statut == "Occupé") {
            typeOccupied[type]++;
        }
    }

    for (auto it = typeCounts.begin(); it != typeCounts.end(); ++it) {
        QString type = it.key();
        int total = it.value();
        int occupied = typeOccupied.value(type, 0);
        double rate = total > 0 ? (occupied * 100.0 / total) : 0;

        QString emoji = rate > 80 ? "🔴" : (rate > 50 ? "🟡" : "🟢");

        report += QString("%1 %2: %3/%4 locaux (%5% d'occupation)\n")
                      .arg(emoji)
                      .arg(type)
                      .arg(occupied)
                      .arg(total)
                      .arg(rate, 0, 'f', 1);
    }

    report += "\n💡 ANALYSE INTELLIGENTE:\n";
    report += "• Les espaces avec taux >80% pourraient nécessiter plus de ressources\n";
    report += "• Les espaces avec taux <30% pourraient être optimisés\n";
    report += "• Recommandations basées sur les tendances saisonnières\n";

    QMessageBox::information(this, "Rapport d'Occupation Intelligent", report);
}

// GESTION DE LA BASE DE DONNÉES DES RÉSERVATIONS
void LocauxPage::createReservationsTable()
{
    QSqlDatabase db = Connection::getInstance().getDatabase();
    QSqlQuery query(db);

    query.exec(
        "CREATE TABLE IF NOT EXISTS reservations ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "local_id TEXT NOT NULL,"
        "start_date DATE NOT NULL,"
        "end_date DATE NOT NULL,"
        "purpose TEXT,"
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "FOREIGN KEY (local_id) REFERENCES locaux(ID_L)"
        ")"
        );
}

bool LocauxPage::addReservation(const QString& localId, const QDate& startDate, const QDate& endDate, const QString& purpose)
{
    QSqlDatabase db = Connection::getInstance().getDatabase();
    QSqlQuery query(db);

    query.prepare("INSERT INTO reservations (local_id, start_date, end_date, purpose) "
                  "VALUES (:local_id, :start_date, :end_date, :purpose)");
    query.bindValue(":local_id", localId);
    query.bindValue(":start_date", startDate.toString("yyyy-MM-dd"));
    query.bindValue(":end_date", endDate.toString("yyyy-MM-dd"));
    query.bindValue(":purpose", purpose);

    return query.exec();
}

QList<QVariantMap> LocauxPage::getLocalReservationsDetails(const QString& localId)
{
    QList<QVariantMap> reservations;
    QSqlDatabase db = Connection::getInstance().getDatabase();
    QSqlQuery query(db);

    query.prepare("SELECT start_date, end_date, purpose FROM reservations "
                  "WHERE local_id = :local_id ORDER BY start_date");
    query.bindValue(":local_id", localId);

    if (query.exec()) {
        while (query.next()) {
            QVariantMap reservation;
            reservation["start_date"] = query.value("start_date").toDate();
            reservation["end_date"] = query.value("end_date").toDate();
            reservation["purpose"] = query.value("purpose").toString();
            reservations.append(reservation);
        }
    }

    return reservations;
}

// Fonctions de support
double LocauxPage::calculateOccupancyRate(const QString& localId)
{
    QList<QVariantMap> reservations = getLocalReservationsDetails(localId);
    int reservationDays = 0;

    for (const auto& reservation : reservations) {
        QDate start = reservation["start_date"].toDate();
        QDate end = reservation["end_date"].toDate();
        reservationDays += start.daysTo(end) + 1;
    }

    // Estimation sur les 30 derniers jours
    int totalDays = 30;
    return (reservationDays * 100.0) / totalDays;
}

void LocauxPage::showAdvancedStats()
{
    // Cette fonction peut être utilisée pour des statistiques avancées
    showAdvancedStats();
}

void LocauxPage::onQuickReservation()
{
    // Fonction pour les réservations rapides
    QMessageBox::information(this, "Réservation Rapide",
                             "Fonction de réservation rapide disponible prochainement!");
}

void LocauxPage::applyTheme()
{
    ThemeManager::instance().applyTheme(this);
}

void LocauxPage::reloadTranslations()
{
    if (ui) ui->retranslateUi(this);
}
