#include "incidentspage.h"
#include "ui_incidentspage.h"
#include "connection.h"
#include "databasemanager.h"
#include "MapDialog.h"

#include <QMessageBox>
#include <QDateTime>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QPdfWriter>
#include <QPainter>
#include <QTextDocument>
#include <QTextTable>
#include <QTextCursor>
#include <QFileDialog>
#include <QPageSize>
#include <QTimer>

#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QLegend>

#include <QProcess>

IncidentsPage::IncidentsPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::IncidentsPage)
{
    ui->setupUi(this);

    // Configuration du layout
    ui->mainLayout->setSpacing(10);
    ui->contentLayout->setSpacing(10);

    // Configuration du tableau
    ui->incidentTable->horizontalHeader()->setStretchLastSection(true);
    ui->incidentTable->verticalHeader()->setDefaultSectionSize(40);
    ui->incidentTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->incidentTable->setAlternatingRowColors(true);

    // Ajuster les largeurs des colonnes
    ui->incidentTable->setColumnWidth(0, 60);   // ID
    ui->incidentTable->setColumnWidth(1, 150);  // Type
    ui->incidentTable->setColumnWidth(2, 200);  // Localisation
    ui->incidentTable->setColumnWidth(3, 150);  // Date/Heure
    ui->incidentTable->setColumnWidth(4, 80);   // Niveau
    ui->incidentTable->setColumnWidth(5, 100);  // Statut

    // Configuration du formulaire
    ui->formLayout->setColumnStretch(0, 1);  // labels
    ui->formLayout->setColumnStretch(1, 3);  // champs principaux
    ui->formLayout->setColumnStretch(2, 1);  // labels secondaires
    ui->formLayout->setColumnStretch(3, 2);  // champs secondaires

    // Remplir combo CIN des résidents
    ui->CINRComboBox->clear();
    QSqlQuery query("SELECT CIN FROM RESIDENTS ORDER BY CIN");
    while (query.next()) {
        ui->CINRComboBox->addItem(query.value(0).toString());
    }

    // Configuration du chartView
    ui->chartView->setMinimumHeight(350);
    ui->chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // Connexions
    connect(ui->tabAddButton, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->tabListButton, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(1);
        refreshList();
    });
    connect(ui->tabStatsButton, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(2);
        QTimer::singleShot(50, this, &IncidentsPage::refreshStats);
    });

    connect(ui->cancelFormButton, &QPushButton::clicked, this, [=]() {
        clearForm();
        ui->stackedWidget->setCurrentIndex(1);
    });
    connect(ui->saveFormButton, &QPushButton::clicked, this, &IncidentsPage::onSaveIncident);

    connect(ui->searchButton, &QPushButton::clicked, this, &IncidentsPage::onSearch);
    connect(ui->searchEdit, &QLineEdit::returnPressed, this, &IncidentsPage::onSearch);
    connect(ui->sortCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &IncidentsPage::onSortIndexChanged);

    connect(ui->exportPdfButton, &QPushButton::clicked, this, &IncidentsPage::onExportPdf);
    connect(ui->editIncidentButton, &QPushButton::clicked, this, &IncidentsPage::onEditIncident);
    connect(ui->deleteIncidentButton, &QPushButton::clicked, this, &IncidentsPage::onDeleteIncident);

    // Initialisation
    if (Connection::isOpen()) {
        refreshList();
    } else {
        qWarning() << "[IncidentsPage] Connexion non ouverte, initialisation partielle";
    }
    ui->tabListButton->setChecked(true);

    // Carte
    connect(ui->showMapButton, &QPushButton::clicked, this, [=]() {
        MapDialog *dialog = new MapDialog(this);

        // Ajouter les bâtiments
        dialog->addBuilding("Batiment 1", QRect(396, 703, 363, 330));
        dialog->addBuilding("Batiment 2", QRect(992, 641, 370, 290));
        dialog->addBuilding("Batiment 3", QRect(1375, 422, 303, 319));
        dialog->addBuilding("Parking 1", QRect(10, 151, 373, 316));
        dialog->addBuilding("Batiment 4", QRect(615, 235, 255, 261));

        // Ajouter les incidents existants
        for (int row = 0; row < ui->incidentTable->rowCount(); ++row) {
            QString localisation = ui->incidentTable->item(row, 2)->text();
            dialog->addIncident(localisation);
        }

        dialog->exec();
    });

    // Initialisation des stats
    QTimer::singleShot(1000, this, &IncidentsPage::refreshStats);
}
IncidentsPage::~IncidentsPage() {
    delete ui;
}

// -------------------- Enregistrement d'un incident --------------------
void IncidentsPage::onSaveIncident()
{
    ui->errorLabel->clear();

    QString type = ui->typeCombo->currentText().trimmed();
    QString localisation = ui->localisationEdit->text().trimmed();
    QDateTime dateHeure = ui->dateHeureEdit->dateTime();
    int niveau = ui->niveauCombo->currentIndex() + 1;
    QString statutText = ui->statutCombo->currentText().trimmed();
    QString cinResidentText = ui->CINRComboBox->currentText().trimmed();

    if (type.isEmpty() || localisation.isEmpty() || statutText.isEmpty() || cinResidentText.isEmpty()) {
        ui->errorLabel->setText("Veuillez remplir tous les champs obligatoires.");
        return;
    }

    bool okIdR = false;
    int idRes = cinResidentText.toInt(&okIdR);
    if (!okIdR || cinResidentText.length() != 8) {
        ui->errorLabel->setText("ID résident invalide !");
        return;
    }

    QVariantMap data;
    data["type_incident"] = type;
    data["localisation"] = localisation;
    data["date_heure"] = dateHeure.toString("yyyy-MM-dd HH:mm:ss");
    data["niveau"] = niveau;
    data["statut"] = statutText;
    data["cin_resident"] = cinResidentText;

    bool success = false;

    if (currentEditId > 0) {
        // Modification
        success = DatabaseManager::instance().updateIncident(currentEditId, data);
        if (success) {
            QMessageBox::information(this, "Succès", "Incident modifié avec succès !");
        }
    } else {
        // Nouvel incident
        success = DatabaseManager::instance().addIncident(data);
        if (success) {
            QMessageBox::information(this, "Succès", "Incident signalé avec succès !");

            // -------------------- ENVOI EMAIL --------------------
            QString body = "Un nouvel incident a été ajouté à NEXORA :\n\n"
                           "Type : " + type + "\n"
                                    "Localisation : " + localisation + "\n"
                                            "Niveau : " + QString::number(niveau) + "\n"
                                                       "Statut : " + statutText + "\n"
                                          "CIN Résident : " + cinResidentText + "\n"
                                               "Date & Heure : " + dateHeure.toString("yyyy-MM-dd HH:mm:ss") + "\n";

            bool emailSent = sendEmail("wajd.mrabet24@gmail.com", "Nouvel incident ajouté", body);

            if (!emailSent) {
                QMessageBox::warning(this, "Erreur Email", "Incident ajouté mais l'envoi du mail a échoué.");
            }
        }
    }

    if (success) {
        clearForm();
        currentEditId = -1;
        ui->stackedWidget->setCurrentIndex(1);
        refreshList();
        refreshStats();
    } else {
        QSqlError err = Connection::getInstance().getDatabase().lastError();
        QString errorMsg = err.text();
        if (errorMsg.contains("ORA-02291") || errorMsg.contains("parent key not found") ||
            errorMsg.contains("FK_INCIDENTS_RESIDENTS")) {
            ui->errorLabel->setText("Erreur : Le résident avec le CIN " + cinResidentText + " n'existe pas.");
        } else {
            ui->errorLabel->setText("Erreur Oracle : " + errorMsg);
        }
    }
}


// -------------------- Rafraîchir liste --------------------
void IncidentsPage::refreshList()
{
    auto list = DatabaseManager::instance().getAllIncidents();
    ui->incidentTable->setSortingEnabled(false);
    ui->incidentTable->setRowCount(0);

    for (const auto &m : list) {
        int row = ui->incidentTable->rowCount();
        ui->incidentTable->insertRow(row);

        // ID
        QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(m["id"].toInt()));
        idItem->setData(Qt::UserRole, m["id"].toInt());
        ui->incidentTable->setItem(row, 0, idItem);

        // Type
        QTableWidgetItem *typeItem = new QTableWidgetItem(m["type_incident"].toString());
        ui->incidentTable->setItem(row, 1, typeItem);

        // Localisation
        QTableWidgetItem *locItem = new QTableWidgetItem(m["localisation"].toString());
        ui->incidentTable->setItem(row, 2, locItem);

        // Date & Heure
        QDateTime dateHeure = QDateTime::fromString(m["date_heure"].toString(), "yyyy-MM-dd HH:mm:ss");
        if (!dateHeure.isValid()) {
            dateHeure = QDateTime::fromString(m["date_heure"].toString(), "yyyy-MM-dd HH:mm");
        }
        QTableWidgetItem *dateItem = new QTableWidgetItem(dateHeure.toString("yyyy-MM-dd HH:mm:ss"));
        dateItem->setData(Qt::UserRole, dateHeure);
        ui->incidentTable->setItem(row, 3, dateItem);

        // Niveau
        QTableWidgetItem *niveauItem = new QTableWidgetItem(QString::number(m["niveau"].toInt()));
        niveauItem->setData(Qt::UserRole, m["niveau"].toInt());
        ui->incidentTable->setItem(row, 4, niveauItem);

        // Statut
        QTableWidgetItem *statutItem = new QTableWidgetItem(m["statut"].toString());
        ui->incidentTable->setItem(row, 5, statutItem);

        // CIN Résident
        QTableWidgetItem *cinItem = new QTableWidgetItem(m["cin_resident"].toString());
        ui->incidentTable->setItem(row, 6, cinItem);
    }

    ui->incidentTable->setSortingEnabled(true);
}

// -------------------- Recherche --------------------
void IncidentsPage::onSearch()
{
    QString searchText = ui->searchEdit->text().trimmed();

    for (int row = 0; row < ui->incidentTable->rowCount(); ++row) {
        bool match = false;
        for (int col = 0; col < ui->incidentTable->columnCount(); ++col) {
            QTableWidgetItem *item = ui->incidentTable->item(row, col);
            if (item && item->text().contains(searchText, Qt::CaseInsensitive)) {
                match = true;
                break;
            }
        }
        ui->incidentTable->setRowHidden(row, !match);
    }
}

// -------------------- Suppression --------------------
void IncidentsPage::onDeleteIncident()
{
    int row = ui->incidentTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Attention", "Sélectionnez un incident à supprimer.");
        return;
    }
    int id = ui->incidentTable->item(row, 0)->text().toInt();
    if (QMessageBox::question(this, "Confirmer", "Supprimer cet incident ?") == QMessageBox::Yes) {
        if (DatabaseManager::instance().deleteIncident(id)) {
            QMessageBox::information(this, "Succès", "Incident supprimé !");
            refreshList();
            refreshStats();
        } else {
            QSqlError err = Connection::getInstance().getDatabase().lastError();
            QMessageBox::critical(this, "Erreur", "Impossible de supprimer l'incident.\n" + err.text());
        }
    }
}

// -------------------- Modification --------------------
void IncidentsPage::onEditIncident()
{
    int row = ui->incidentTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Attention", "Sélectionnez un incident à modifier.");
        return;
    }

    int id = ui->incidentTable->item(row, 0)->text().toInt();

    ui->typeCombo->setCurrentText(ui->incidentTable->item(row, 1)->text());
    ui->localisationEdit->setText(ui->incidentTable->item(row, 2)->text());

    QDateTime dateHeure = QDateTime::fromString(ui->incidentTable->item(row, 3)->text(), "yyyy-MM-dd HH:mm:ss");
    if (!dateHeure.isValid()) {
        dateHeure = QDateTime::fromString(ui->incidentTable->item(row, 3)->text(), "yyyy-MM-dd HH:mm");
    }
    ui->dateHeureEdit->setDateTime(dateHeure);

    int niveau = ui->incidentTable->item(row, 4)->text().toInt();
    ui->niveauCombo->setCurrentIndex(niveau - 1);

    ui->statutCombo->setCurrentText(ui->incidentTable->item(row, 5)->text());
    ui->CINRComboBox->setCurrentText(ui->incidentTable->item(row, 6)->text());

    currentEditId = id;
    ui->stackedWidget->setCurrentIndex(0);
    ui->tabAddButton->setChecked(true);
}

// -------------------- Formulaire --------------------
void IncidentsPage::clearForm()
{
    ui->typeCombo->setCurrentIndex(0);
    ui->localisationEdit->clear();
    ui->dateHeureEdit->setDateTime(QDateTime::currentDateTime());
    ui->niveauCombo->setCurrentIndex(0);
    ui->statutCombo->setCurrentIndex(0);
    ui->CINRComboBox->setCurrentIndex(0);
    ui->errorLabel->clear();
    currentEditId = -1;
}

// -------------------- Export PDF --------------------
void IncidentsPage::onExportPdf()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter PDF", "", "*.pdf");
    if (fileName.isEmpty()) return;

    QMap<QString, int> incidentsParResident;
    QMap<QString, int> incidentsParLocalisation;

    for (int row = 0; row < ui->incidentTable->rowCount(); ++row) {
        QString cin = ui->incidentTable->item(row, 6)->text(); // CIN_RESIDENT (colonne 6)
        QString localisation = ui->incidentTable->item(row, 2)->text(); // LOCALISATION

        incidentsParResident[cin] += 1;
        incidentsParLocalisation[localisation] += 1;
    }

    QString html = "<html><head><style>"
                   "table { border-collapse: collapse; width: 100%; margin-bottom: 20px; }"
                   "th, td { border: 1px solid black; padding: 5px; text-align: center; }"
                   "th { background-color: #f2f2f2; }"
                   "h2 { text-align: center; }"
                   "</style></head><body>";

    html += "<h2>Incidents par résident</h2>";
    html += "<table><tr><th>CIN Résident</th><th>Nombre d'incidents</th></tr>";
    for (auto it = incidentsParResident.begin(); it != incidentsParResident.end(); ++it) {
        html += "<tr><td>" + it.key() + "</td><td>" + QString::number(it.value()) + "</td></tr>";
    }
    html += "</table>";

    html += "<h2>Incidents par localisation</h2>";
    html += "<table><tr><th>Localisation</th><th>Nombre d'incidents</th></tr>";
    for (auto it = incidentsParLocalisation.begin(); it != incidentsParLocalisation.end(); ++it) {
        html += "<tr><td>" + it.key() + "</td><td>" + QString::number(it.value()) + "</td></tr>";
    }
    html += "</table>";

    html += "</body></html>";

    QPdfWriter writer(fileName);
    writer.setPageSize(QPageSize(QPageSize::A4));
    QPainter painter(&writer);

    QTextDocument doc;
    doc.setHtml(html);
    doc.setPageSize(QSizeF(writer.width(), writer.height()));

    doc.drawContents(&painter);
    painter.end();

    QMessageBox::information(this, "Export PDF", "Export terminé avec succès !");
}

void IncidentsPage::onSortIndexChanged(int index)
{
    int column = 3; // Date par défaut
    Qt::SortOrder order = Qt::DescendingOrder;

    switch(index) {
    case 0: column = 3; order = Qt::DescendingOrder; break; // Trier par date
    case 1: column = 4; order = Qt::DescendingOrder; break; // Trier par niveau
    case 2: column = 5; order = Qt::AscendingOrder; break;  // Trier par statut
    }

    ui->incidentTable->sortItems(column, order);
}

// -------------------- Statistiques --------------------
void IncidentsPage::updateStats()
{
    qDebug() << "=== CRÉATION DU CHART ===";

    // 1. SUPPRIMER L'ANCIEN CHART
    if (ui->chartView->chart()) {
        delete ui->chartView->chart();
    }

    // 2. CRÉER UN NOUVEAU CHART
    QChart *chart = new QChart();

    // 3. CRÉER LES DONNÉES DE TEST
    QPieSeries *series = new QPieSeries();
    series->append("Signalé", 5);
    series->append("En cours", 3);
    series->append("Résolu", 8);
    series->append("Archivé", 2);

    // 4. CONFIGURER LES SLICES
    for (QPieSlice *slice : series->slices()) {
        slice->setLabelVisible(true);
        slice->setLabel(QString("%1: %2").arg(slice->label()).arg(slice->value()));
        slice->setLabelArmLengthFactor(0.1);
    }

    // 5. CONFIGURER LE CHART
    chart->addSeries(series);
    chart->setTitleFont(QFont("Arial", 6, QFont::Bold));
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    chart->setMargins(QMargins(20, 30, 20, 20)); // Haut: 30, autres: 20


    // 6. APPLIQUER LE CHART
    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);

    // 7. METTRE À JOUR LE TEXTE

    // 8. DEBUG
    qDebug() << "Chart créé avec succès";
    qDebug() << "ChartView size:" << ui->chartView->size();
    qDebug() << "Chart visible:" << ui->chartView->isVisible();
}

void IncidentsPage::refreshStats() {
    qDebug() << "=== REFRESH STATS ===";
    updateStats();
}


//////////////////MAILING////////////////////////////
bool IncidentsPage::sendEmail(const QString &to, const QString &subject, const QString &body)
{
    QString gmailUser = "wajd.mrabet24@gmail.com";         // ← TON EMAIL
    QString gmailPassword = "vsbu szuy bqfr aktm";         // ← MOT DE PASSE D’APPLICATION

    QString program = "curl";

    QStringList arguments;
    arguments << "--url" << "smtp://smtp.gmail.com:587"
              << "--ssl"
              << "--mail-from" << gmailUser
              << "--mail-rcpt" << to
              << "--user" << gmailUser + ":" + gmailPassword
              << "-T" << "-";

    QProcess process;
    process.start(program, arguments);
    if (!process.waitForStarted())
        return false;

    QString data = "Subject: " + subject + "\r\n\r\n" + body + "\r\n";
    process.write(data.toUtf8());
    process.closeWriteChannel();

    process.waitForFinished();
    QString error  = process.readAllStandardError();
    qDebug() << "EMAIL ERROR :" << error;

    // Considère comme succès si stderr ne contient pas "Failed" ou "Error"
    if (error.contains("Failed") || error.contains("Error")) {
        return false;
    } else {
        return true;
    }}

