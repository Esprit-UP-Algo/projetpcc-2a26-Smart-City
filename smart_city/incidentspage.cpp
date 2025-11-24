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
    , currentEditId(-1)  // ← INITIALISATION IMPORTANTE

{
    ui->setupUi(this);
    qDebug() << "=== TEST AU DÉMARRAGE ===";
    testQtCharts();
    qDebug() << "=== FIN TEST ===";



    qDebug() << "ui valide:" << (ui != nullptr);
    qDebug() << "stackedWidget:" << (ui->stackedWidget != nullptr);
    qDebug() << "incidentTable:" << (ui->incidentTable != nullptr);
    qDebug() << "chartView:" << (ui->chartView != nullptr);
    qDebug() << "tabAddButton:" << (ui->tabAddButton != nullptr);
    qDebug() << "tabListButton:" << (ui->tabListButton != nullptr);
    qDebug() << "tabStatsButton:" << (ui->tabStatsButton != nullptr);

    if (!ui->stackedWidget || !ui->incidentTable || !ui->chartView) {
        qCritical() << "ERREUR: Widgets critiques non initialisés!";
        QMessageBox::critical(nullptr, "Erreur", "Échec de l'initialisation de l'interface");
        return;
    }

    // CONFIGURATION DE BASE UNIQUEMENT
    ui->mainLayout->setSpacing(10);

    // Configuration tableau
    ui->incidentTable->horizontalHeader()->setStretchLastSection(true);
    ui->incidentTable->verticalHeader()->setDefaultSectionSize(40);

    // Remplir combo CIN
    ui->CINRComboBox->clear();
    QSqlQuery query("SELECT CIN FROM RESIDENTS ORDER BY CIN");
    while (query.next()) {
        ui->CINRComboBox->addItem(query.value(0).toString());
    }

    // CONNEXIONS DIRECTES
    connect(ui->tabAddButton, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(0);
    });

    connect(ui->tabListButton, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(1);
        refreshList();
    });

    // Dans le constructeur, remplacez cette connexion :
    // Dans le constructeur, modifiez la connexion des stats :
    // Dans le constructeur :
    connect(ui->tabStatsButton, &QPushButton::clicked, this, [this]() {
        qDebug() << "=== CLIC ONGLET STATS ===";

        if (!ui || !ui->stackedWidget) {
            qCritical() << "UI non disponible";
            return;
        }

        // Changer d'onglet d'abord
        ui->stackedWidget->setCurrentIndex(2);

        // Attendre que l'UI soit complètement mise à jour
        QTimer::singleShot(200, this, [this]() {
            qDebug() << "Exécution différée après changement d'onglet";
            if (this && ui && ui->chartView) {
                updateStats();
            } else {
                qCritical() << "Contexte invalide après délai";
            }
        });
    });

    connect(ui->saveFormButton, &QPushButton::clicked, this, &IncidentsPage::onSaveIncident);
    connect(ui->cancelFormButton, &QPushButton::clicked, this, [this]() {
        clearForm();
        ui->stackedWidget->setCurrentIndex(1);
    });

    // Initialisation
    if (Connection::isOpen()) {
        refreshList();
    }
    ui->tabListButton->setChecked(true);
    // Dans IncidentsPage::IncidentsPage(), ajoutez :
    connect(ui->showMapButton, &QPushButton::clicked, this, [this]() {
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
    // Dans IncidentsPage::IncidentsPage(), ajoutez :
    connect(ui->exportPdfButton, &QPushButton::clicked, this, &IncidentsPage::onExportPdf);
    connect(ui->sortCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &IncidentsPage::onSortIndexChanged);

    // 2. RECHERCHE - IMPORTANT !
    connect(ui->searchButton, &QPushButton::clicked, this, &IncidentsPage::onSearch);
    connect(ui->searchEdit, &QLineEdit::returnPressed, this, &IncidentsPage::onSearch);
    connect(ui->editIncidentButton, &QPushButton::clicked, this, &IncidentsPage::onEditIncident);
    connect(ui->deleteIncidentButton, &QPushButton::clicked, this, &IncidentsPage::onDeleteIncident);
}
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>

void IncidentsPage::testQtCharts()
{
    qDebug() << "=== TEST QTCHARTS ===";

    // Test 1: Création basique
    QChart *testChart = new QChart();
    qDebug() << "Chart créé:" << (testChart != nullptr);

    // Test 2: Création série
    QPieSeries *testSeries = new QPieSeries();
    qDebug() << "Série créée:" << (testSeries != nullptr);

    // Test 3: Assemblage
    testSeries->append("Test", 1);
    testChart->addSeries(testSeries);
    qDebug() << "Série ajoutée au chart";

    // Nettoyage
    delete testChart; // Cela devrait aussi supprimer la série
    qDebug() << "Test QtCharts terminé";
}

// Appelez cette méthode au début de votre application pour tester
IncidentsPage::~IncidentsPage() {
    // Nettoyer le chart avant l'UI
    if (statsChart) {
        delete statsChart;
        statsChart = nullptr;
    }
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
// -------------------- Statistiques --------------------
// -------------------- Statistiques --------------------
void IncidentsPage::initializeStatsChart()
{
    if (statsChart) return;

    statsChart = new QChart();
    QPieSeries *series = new QPieSeries();

    // DONNÉES FIXES POUR TEST - REMPLACEZ PLUS TARD PAR LA VERSION BD
    QPieSlice *slice1 = series->append("Signalé", 5);
    QPieSlice *slice2 = series->append("En cours", 3);
    QPieSlice *slice3 = series->append("Résolu", 8);
    QPieSlice *slice4 = series->append("Archivé", 2);

    // CONFIGURATION DES LABELS - TRÈS IMPORTANT
    for (QPieSlice *slice : series->slices()) {
        slice->setLabelVisible(true);
        slice->setLabelPosition(QPieSlice::LabelOutside);
        slice->setLabelBrush(QBrush(Qt::black));
        slice->setLabelFont(QFont("Arial", 10, QFont::Bold));

        // Calcul du pourcentage
        double percentage = (slice->value() / series->sum()) * 100;
        QString label = QString("%1\n%2 incidents\n%3%")
                            .arg(slice->label())
                            .arg(slice->value())
                            .arg(QString::number(percentage, 'f', 1));
        slice->setLabel(label);
    }

    // FORCER L'AFFICHAGE DES LABELS
    series->setLabelsVisible(true);

    // AJOUT AU CHART
    statsChart->addSeries(series);
///////////////////////////////////////
    statsChart->setTitleFont(QFont("Arial", 14, QFont::Bold));

    // LÉGENDE
    statsChart->legend()->setVisible(true);
    statsChart->legend()->setAlignment(Qt::AlignBottom);
    statsChart->legend()->setFont(QFont("Arial", 8));

    statsChart->setMargins(QMargins(5, 5, 5, 5)); // Marges réduites


    // ANIMATIONS
    statsChart->setAnimationOptions(QChart::AllAnimations);

    // APPLICATION
    ui->chartView->setChart(statsChart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);

    ui->chartView->setMinimumHeight(350); // Réduit la hauteur minimum


    qDebug() << "Chart initialisé avec données de test";
}

void IncidentsPage::updateStats()
{
    qDebug() << "=== UPDATE STATS ===";

    if (!this || !ui || !ui->chartView) {
        qCritical() << "Pointeurs invalides";
        return;
    }

    if (!statsChart) {
        qDebug() << "Initialisation du chart...";
        initializeStatsChart();
    }

    qDebug() << "=== UPDATE STATS TERMINÉ ===";
}
void IncidentsPage::refreshStats() {
    qDebug() << "=== REFRESH STATS AVANT ===";

    // Petit délai pour éviter les conflits
    QTimer::singleShot(100, this, [this]() {
        qDebug() << "=== REFRESH STATS EXÉCUTION ===";
        if (!this || !ui) {
            qCritical() << "RefreshStats: Objet détruit";
            return;
        }
        updateStats();
    });
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

