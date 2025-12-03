#include "incidentspage.h"
#include "ui_incidentspage.h"
#include "connection.h"
#include "databasemanager.h"
#include "MapDialog.h"
#include "thememanager.h"
#include "languagemanager.h"

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
        // Update button states
        ui->tabAddButton->setChecked(true);
        ui->tabListButton->setChecked(false);
        ui->tabStatsButton->setChecked(false);
    });

    connect(ui->tabListButton, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(1);
        refreshList();
        // Update button states
        ui->tabAddButton->setChecked(false);
        ui->tabListButton->setChecked(true);
        ui->tabStatsButton->setChecked(false);
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
        
        // Update button states
        ui->tabAddButton->setChecked(false);
        ui->tabListButton->setChecked(false);
        ui->tabStatsButton->setChecked(true);

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
    // Hook theme/language
    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, [this](AppTheme){ applyTheme(); });
    connect(&LanguageManager::instance(), &LanguageManager::languageChanged, this, [this](const QString&){ reloadTranslations(); });
    applyTheme();
    reloadTranslations();
    // Dans IncidentsPage::IncidentsPage(), ajoutez :
    connect(ui->showMapButton, &QPushButton::clicked, this, [this]() {
        MapDialog *dialog = new MapDialog(this);

        // Ajouter les bâtiments
        dialog->addBuilding("Batiment 1", QRect(396, 703, 363, 330));
        dialog->addBuilding("Batiment 2", QRect(992, 641, 370, 290));
        dialog->addBuilding("Batiment 3", QRect(1375, 422, 303, 319));
        dialog->addBuilding("Parking 1", QRect(10, 151, 373, 316));
        dialog->addBuilding("Batiment 4", QRect(615, 235, 255, 261));

        // AJOUTEZ CE DEBUG POUR VERIFIER
        qDebug() << "=== CHARGEMENT INCIDENTS DANS LA CARTE ===";
        qDebug() << "Nombre d'incidents dans le tableau:" << ui->incidentTable->rowCount();

        // Ajouter les incidents existants AVEC LEUR TYPE
        for (int row = 0; row < ui->incidentTable->rowCount(); ++row) {
            QString localisation = ui->incidentTable->item(row, 2)->text();
            QString type = ui->incidentTable->item(row, 1)->text();

            qDebug() << "Incident" << row << ":" << localisation << "-" << type;
            dialog->addIncident(localisation, type);
        }

        dialog->exec();
        delete dialog;
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

void IncidentsPage::applyTheme()
{
    ThemeManager::instance().applyTheme(this);
}

void IncidentsPage::reloadTranslations()
{
    if (ui) ui->retranslateUi(this);
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
    Q_UNUSED(okIdR); // Variable used for validation but not needed after check
    if (cinResidentText.length() != 8) {
        ui->errorLabel->setText("CIN résident doit contenir 8 caractères !");
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
            // -------------------- ENVOI EMAIL --------------------
            QString recipientEmail = getServiceEmail(type);
            qDebug() << "TEST getServiceEmail:";
            qDebug() << "Type:" << type;
            qDebug() << "Email retourné:" << recipientEmail;

            // Test manuel des différents types
            qDebug() << "Bruit excessif ->" << getServiceEmail("Bruit excessif");
            qDebug() << "Panne électrique ->" << getServiceEmail("Panne électrique");
            qDebug() << "=== DÉBOGAGE EMAIL ===";
            qDebug() << "Type incident:" << type;
            qDebug() << "Email destinataire:" << recipientEmail;
            qDebug() << "======================";


            QString body = "Un nouvel incident a été ajouté à NEXORA :\n\n"
                           "Type : " + type + "\n"
                                    "Localisation : " + localisation + "\n"
                                            "Niveau : " + QString::number(niveau) + "\n"
                                                       "Statut : " + statutText + "\n"
                                          "CIN Résident : " + cinResidentText + "\n"
                                               "Date & Heure : " + dateHeure.toString("yyyy-MM-dd HH:mm:ss") + "\n\n"
                                                                         "Veuillez traiter cet incident dans les plus brefs délais.";

            bool emailSent = sendEmail(recipientEmail, "Nouvel incident - " + type, body);

            if (emailSent) {
                QMessageBox::information(this, "Email envoyé",
                                         "Un email a été envoyé au service concerné : " + recipientEmail);
            } else {
                QMessageBox::warning(this, "Erreur Email",
                                     "Incident ajouté mais l'envoi du mail a échoué.\n"
                                     "Service concerné: " + recipientEmail);
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
    // Vérifier qu'un incident est sélectionné
    QModelIndexList selectedIndexes = ui->incidentTable->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, "Export PDF", "Veuillez sélectionner un incident à exporter.");
        return;
    }

    // Ligne sélectionnée
    int selectedRow = selectedIndexes.first().row();

    QString fileName = QFileDialog::getSaveFileName(
        this, "Exporter Rapport PDF", "Rapport_Incident_", "*.pdf"
        );
    if (fileName.isEmpty()) return;

    // Lecture des données dans le tableau
    QString id          = ui->incidentTable->item(selectedRow, 0)->text();
    QString type        = ui->incidentTable->item(selectedRow, 1)->text();
    QString localisation= ui->incidentTable->item(selectedRow, 2)->text();
    QString dateHeure   = ui->incidentTable->item(selectedRow, 3)->text();
    QString niveau      = ui->incidentTable->item(selectedRow, 4)->text();
    QString statut      = ui->incidentTable->item(selectedRow, 5)->text();
    QString cinResident = ui->incidentTable->item(selectedRow, 6)->text();

    // Récup nom résident + parties techniques
    QString nomResident     = getNomResidentFromCIN(cinResident);
    QString retourTechnique = genererRetourTechnique(statut, type, niveau);
    QString decisionFinale  = genererDecisionFinale(statut, type, niveau);

    // HTML amélioré — version claire et propre Nexora
    QString html = QString(
                       "<html><head>"
                       "<style>"
                       "body { font-family: Arial, sans-serif; margin: 25px; }"

                       ".header { text-align:center; padding-bottom:10px;"
                       " border-bottom: 3px solid #2E86C1; }"

                       ".title { font-size:26pt; font-weight:bold; color:#2E86C1; }"
                       ".ref   { font-size:12pt; color:#555; margin-top:4px; }"

                       "table { width:100%; border-collapse:collapse; margin-top:20px; }"
                       "th { background:#2E86C1; color:white; padding:10px; font-size:12pt; }"
                       "td { border:1px solid #999; padding:10px; font-size:11pt; }"
                       "tr:nth-child(even) { background:#F8F9F9; }"

                       ".section { margin-top:25px; padding:15px; background:#F4F6F7;"
                       " border-left:5px solid #2E86C1; }"
                       ".section h3 { margin:0 0 10px 0; color:#2E86C1; }"
                       "</style>"
                       "</head><body>"

                       "<div class='header'>"
                       "<div class='title'>RAPPORT D'INCIDENT — NEXORA</div>"
                       "<div class='ref'>Référence : INC-%1 — Généré le %2</div>"
                       "</div>"

                       "<table>"
                       "<tr><th>Champ</th><th>Valeur</th></tr>"
                       "<tr><td><b>ID Incident</b></td><td>%1</td></tr>"
                       "<tr><td><b>Type</b></td><td>%3</td></tr>"
                       "<tr><td><b>Localisation</b></td><td>%4</td></tr>"
                       "<tr><td><b>Date & Heure</b></td><td>%5</td></tr>"
                       "<tr><td><b>Niveau d'urgence</b></td><td>%6</td></tr>"
                       "<tr><td><b>Statut</b></td><td>%7</td></tr>"
                       "<tr><td><b>Résident</b></td><td>%8 (CIN: %9)</td></tr>"
                       "</table>"

                       "<div class='section'>"
                       "<h3>Analyse Technique</h3>"
                       "%10"
                       "</div>"

                       "<div class='section'>"
                       "<h3>Décision Finale</h3>"
                       "%11"
                       "</div>"

                       "</body></html>"
                       )
                       .arg(id)
                       .arg(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm"))
                       .arg(type)
                       .arg(localisation)
                       .arg(dateHeure)
                       .arg(niveau)
                       .arg(statut)
                       .arg(nomResident)
                       .arg(cinResident)
                       .arg(retourTechnique)
                       .arg(decisionFinale);

    // ===============================
    // 🚀 PDF A4 SIZE — FULL PAGE FIX
    // ===============================
    QPdfWriter writer(fileName);

    writer.setResolution(96);  // FULL PDF FIX
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setPageOrientation(QPageLayout::Portrait);
    writer.setPageMargins(QMarginsF(15, 15, 15, 15)); // mm

    QTextDocument doc;
    doc.setHtml(html);

    // Match page size
    QSizeF pageSize(writer.width(), writer.height());
    doc.setPageSize(pageSize);

    QPainter painter(&writer);

    // 🟦 Fill full page — HUGE FIX
    double scale = writer.width() / doc.idealWidth();
    painter.scale(scale, scale);

    doc.drawContents(&painter);
    painter.end();

    QMessageBox::information(this, "Export PDF", "Rapport d'incident exporté avec succès !");
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
    series->append("Signalé", 5);
    series->append("En cours", 3);
    series->append("Résolu", 8);
    series->append("Archivé", 2);

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

    if (!ui || !ui->chartView) {
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
QString IncidentsPage::removeAccents(const QString &input)
{
    QString normalized = input.normalized(QString::NormalizationForm_D);
    QString output;

    for (int i = 0; i < normalized.length(); ++i) {
        if (normalized.at(i).category() != QChar::Mark_NonSpacing) {
            output.append(normalized.at(i));
        }
    }

    return output;
}

bool IncidentsPage::sendEmail(const QString &to, const QString &subject, const QString &body)
{
    // Déterminer quel compte utiliser selon le destinataire
    QString gmailUser, gmailPassword;

    if (to.contains("wajd.mrabet")) {
        gmailUser = "wajd.mrabet24@gmail.com";
        gmailPassword = "vsbu szuy bqfr aktm";
        qDebug() << "Utilisation du compte: WAJD";
    }
    else if (to.contains("saidanenada") || to.contains("nada") || to.contains("saida")) {
        gmailUser = "saidanenada47@gmail.com";
        gmailPassword = "qrnd blos ojqs lnsm";  // ← NOUVEAU MOT DE PASSE
        qDebug() << "Utilisation du compte: NADA (mdp POST-authenticator)";
    }
    else {
        gmailUser = "wajd.mrabet24@gmail.com";
        gmailPassword = "vsbu szuy bqfr aktm";
        qDebug() << "Utilisation du compte: WAJD (fallback)";
    }

    qDebug() << "=== TEST NOUVEAU MOT DE PASSE NADA ===";
    qDebug() << "Compte Gmail:" << gmailUser;
    qDebug() << "Nouveau mot de passe:" << gmailPassword;

    // ... le reste de votre code sendEmail reste identique ...
    QString program = "curl";
    QStringList arguments;
    arguments << "--url" << "smtp://smtp.gmail.com:587"
              << "--ssl-reqd"
              << "--mail-from" << gmailUser
              << "--mail-rcpt" << to
              << "--user" << gmailUser + ":" + gmailPassword
              << "-v"
              << "-T" << "-";

    QProcess process;
    process.start(program, arguments);

    if (!process.waitForStarted(5000)) {
        qDebug() << "❌ ERREUR: Impossible de démarrer curl";
        return false;
    }

    QString emailData = "From: " + gmailUser + "\r\n"
                                               "To: " + to + "\r\n"
                               "Subject: " + subject + "\r\n"
                                    "\r\n" + body + "\r\n";

    process.write(emailData.toUtf8());
    process.closeWriteChannel();

    if (!process.waitForFinished(30000)) {
        qDebug() << "❌ ERREUR: Timeout";
        process.kill();
        return false;
    }

    QString output = process.readAllStandardOutput();
    QString error  = process.readAllStandardError();
    int exitCode = process.exitCode();

    qDebug() << "=== RÉSULTAT AVEC NOUVEAU MDP ===";
    qDebug() << "Code de sortie:" << exitCode;
    qDebug() << "Erreur:" << error;

    if (exitCode == 0) {
        qDebug() << "✅ SUCCÈS - Nouveau mot de passe fonctionne !";
    } else {
        qDebug() << "❌ ÉCHEC - Vérifiez la validation 2 étapes";
    }

    return (exitCode == 0) && !error.contains("Failed") && !error.contains("Error");
}

QString IncidentsPage::getServiceEmail(const QString &incidentType)
{
    // Normalisation complète du texte
    QString typeLower = removeAccents(incidentType).toLower();

    if (typeLower.contains("electri") || typeLower.contains("panne electrique")) {
        return "wajd.mrabet24@gmail.com";  // Électrique
    }
    else if (typeLower.contains("eau") || typeLower.contains("fuite")
             || typeLower.contains("fuite deau")) {
        return "wajd.mrabet24@gmail.com";  // Eau
    }
    else if (typeLower.contains("bruit") || typeLower.contains("excessif")) {
        return "saidanenada47@gmail.com";  // Bruit
    }
    else if (typeLower.contains("ascenseur") || typeLower.contains("elevateur")
             || typeLower.contains("probleme ascenseur")) {
        return "saidanenada47@gmail.com";  // Ascenseur
    }
    else if (typeLower.contains("autre") || typeLower.contains("divers")) {
        return "wajd.mrabet24@gmail.com";  // Catégorie "Autre"
    }

    // Fallback
    return "wajd.mrabet24@gmail.com";
}

QString IncidentsPage::getNomResidentFromCIN(const QString& cin)
{
    QSqlQuery query;
    query.prepare("SELECT NOM, PRENOM FROM RESIDENTS WHERE CIN = ?");
    query.addBindValue(cin);

    if (query.exec() && query.next()) {
        QString nom = query.value(0).toString();
        QString prenom = query.value(1).toString();
        return nom + " " + prenom;
    }

    return "Résident non trouvé";
}

QString IncidentsPage::genererRetourTechnique(const QString& statut, const QString& type, const QString& niveau)
{
    QString typeLower = type.toLower();
    QString statutLower = statut.toLower();
    int niveauInt = niveau.toInt();

    if (statutLower == "signalé" || statutLower == "en attente") {
        QString priorite;
        if (niveauInt >= 4) {
            priorite = "URGENCE - Intervention immédiate requise";
        } else if (niveauInt >= 3) {
            priorite = "Élevée - Traitement prioritaire";
        } else {
            priorite = "Normale";
        }

        QString delai = niveauInt >= 4 ? "24 heures" : "48 heures";

        return QString("<strong>DIAGNOSTIC INITIAL</strong><br><br>"
                       "• Statut : Incident nouvellement signalé<br>"
                       "• Priorité : %1<br>"
                       "• Action : Attribution au service technique compétent<br>"
                       "• Délai estimation : Premier diagnostic sous %2<br>"
                       "• Équipement : Préparation du matériel d'intervention").arg(priorite, delai);
    }
    else if (statutLower == "en cours") {
        if (typeLower.contains("électri") || typeLower.contains("panne") || typeLower.contains("courant")) {
            QString probleme = typeLower.contains("panne") ? "Panne générale secteur" : "Défaut circuit spécifique";
            QString difficulte = niveauInt >= 4 ? "Défaut majeur nécessitant expertise spécialisée" : "Problème standard en cours de résolution";
            int delaiJours = 2 + niveauInt;

            return QString("<strong>INTERVENTION ÉLECTRIQUE EN COURS</strong><br><br>"
                           "• Problème identifié : %1<br>"
                           "• Localisation défaut : Tableau électrique principal<br>"
                           "• Équipement nécessaire : Multimètre, disjoncteurs de remplacement<br>"
                           "• Difficulté technique : %2<br>"
                           "• Sécurité : Alimentation coupée, zone sécurisée<br>"
                           "• Délai estimé : %3 jours").arg(probleme, difficulte, QString::number(delaiJours));
        }
        else if (typeLower.contains("eau") || typeLower.contains("fuite") || typeLower.contains("inondation")) {
            QString nature = typeLower.contains("fuite") ? "Fuite active" : "Débit anormal";
            QString action = niveauInt >= 4 ? "Coupure d'urgence générale" : "Réduction débit secteur";
            QString statutAppro = niveauInt >= 3 ? "Commande express en cours" : "Stock disponible";
            QString impact = niveauInt >= 4 ? "Service interrompu" : "Service réduit";

            return QString("<strong>INTERVENTION PLOMBERIE</strong><br><br>"
                           "• Nature problème : %1<br>"
                           "• Localisation précise : Conduite principale<br>"
                           "• Action immédiate : %2<br>"
                           "• Matériel requis : Joints, vannes de remplacement<br>"
                           "• Statut approvisionnement : %3<br>"
                           "• Impact résidents : %4").arg(nature, action, statutAppro, impact);
        }
        else if (typeLower.contains("bruit") || typeLower.contains("sonore") || typeLower.contains("nuisance")) {
            QString source = niveauInt >= 4 ? "Équipement technique défectueux" : "Conflit entre résidents";
            QString recommandation = niveauInt >= 4 ? "Intervention maintenance urgente" : "Médiation et suivi";
            QString conformite = niveauInt >= 3 ? "Dépassement des normes acoustiques" : "Dans les limites acceptables";
            int niveauDb = 60 + niveauInt * 5;

            return QString("<strong>ENQUÊTE NUISANCE SONORE</strong><br><br>"
                           "• Source identifiée : %1<br>"
                           "• Mesures : Niveau sonore mesuré à %2 dB<br>"
                           "• Actions entreprises : Contact avec les parties, analyse horaires<br>"
                           "• Recommandation : %3<br>"
                           "• Conformité : %4").arg(source, QString::number(niveauDb), recommandation, conformite);
        }
        else if (typeLower.contains("ascenseur") || typeLower.contains("élévateur")) {
            QString naturePanne = niveauInt >= 4 ? "Blocage cabine" : "Défaut fonctionnement";
            QString pieces = niveauInt >= 4 ? "Commande urgente déclenchée" : "Disponibles en stock";
            QString delai = niveauInt >= 4 ? "Intervention sous 4 heures" : "Résolution sous 48h";

            return QString("<strong>MAINTENANCE ASCENSEUR</strong><br><br>"
                           "• Nature panne : %1<br>"
                           "• Sécurité : Ascenseur hors service, signalisation en place<br>"
                           "• Technicien : Spécialiste ascenseur dépêché sur site<br>"
                           "• Pièces nécessaires : %2<br>"
                           "• Délai : %3").arg(naturePanne, pieces, delai);
        }
        else {
            QString complexite = niveauInt >= 4 ? "Nécessite expertise externe" : "Résolution interne standard";
            QString impact = niveauInt >= 3 ? "Service perturbé" : "Service normal maintenu";

            return QString("<strong>INTERVENTION TECHNIQUE GÉNÉRALE</strong><br><br>"
                           "• Équipe : Technicien spécialisé sur site<br>"
                           "• Analyse : Diagnostic approfondi en cours<br>"
                           "• Complexité : %1<br>"
                           "• Communication : Prochain point dans 24 heures<br>"
                           "• Impact : %2").arg(complexite, impact);
        }
    }
    else if (statutLower.contains("résolu") || statutLower.contains("resolu")) {
        QString methode;
        if (typeLower.contains("électri")) {
            methode = "Remplacement circuit défectueux";
        } else if (typeLower.contains("eau")) {
            methode = "Changement joints et test pression";
        } else if (typeLower.contains("bruit")) {
            methode = "Médiation et ajustements techniques";
        } else {
            methode = "Intervention corrective standard";
        }

        int duree = 1 + niveauInt;
        QString tests = niveauInt >= 3 ? "Tests complets réussis" : "Contrôle standard OK";

        return QString("<strong>BILAN FINAL D'INTERVENTION</strong><br><br>"
                       "• Résolution : Problème complètement résolu<br>"
                       "• Méthode : %1<br>"
                       "• Durée totale : %2 jours<br>"
                       "• Tests validation : %3<br>"
                       "• Garantie : 90 jours sur les réparations effectuées").arg(methode, QString::number(duree), tests);
    }
    else {
        QString dateMiseAJour = QDateTime::currentDateTime().toString("dd/MM/yyyy à HH:mm");

        return QString("<strong>STATUT : %1</strong><br><br>"
                       "• Évaluation technique en cours d'analyse<br>"
                       "• Contact équipe technique pour informations détaillées<br>"
                       "• Dernière mise à jour : %2").arg(statut.toUpper(), dateMiseAJour);
    }
}

QString IncidentsPage::genererDecisionFinale(const QString& statut, const QString& type, const QString& niveau)
{
    QString statutLower = statut.toLower();
    int niveauInt = niveau.toInt();

    if (statutLower == "signalé" || statutLower == "en attente") {
        QString service;
        if (type.toLower().contains("électri")) {
            service = "Électricité";
        } else if (type.toLower().contains("eau")) {
            service = "Plomberie";
        } else if (type.toLower().contains("bruit")) {
            service = "Médiation";
        } else if (type.toLower().contains("ascenseur")) {
            service = "Maintenance ascenseurs";
        } else {
            service = "Technique général";
        }

        QString delai;
        if (niveauInt >= 4) {
            delai = "INTERVENTION URGENTE - Sous 24h";
        } else if (niveauInt >= 3) {
            delai = "Sous 48 heures";
        } else {
            delai = "Sous 72 heures";
        }

        QString ressources = "Équipe standard";
        if (niveauInt >= 4) {
            ressources += " + renfort urgence";
        }
        QString budget = QString::number(100 + niveauInt * 50) + " €";

        return QString("<strong>DÉCISION : PLANIFICATION INTERVENTION</strong><br><br>"
                       "• Service attribué : %1<br>"
                       "• Délai engagement : %2<br>"
                       "• Ressources : %3<br>"
                       "• Budget prévisionnel : %4").arg(service, delai, ressources, budget);
    }
    else if (statutLower == "en cours") {
        QString priorite;
        if (niveauInt >= 4) {
            priorite = "MAXIMUM - Mobilisation totale";
        } else if (niveauInt >= 3) {
            priorite = "Élevée - Ressources supplémentaires";
        } else {
            priorite = "Maintenue";
        }

        QString escalation = niveauInt >= 4 ? "Direction informée" : "Niveau technique standard";

        return QString("<strong>DÉCISION : ACCÉLÉRATION TRAVAUX</strong><br><br>"
                       "• Priorité : %1<br>"
                       "• Approvisionnement : Validation commandes urgentes<br>"
                       "• Coordination : Réunions quotidiennes de suivi<br>"
                       "• Communication : Mise à jour résident toutes les 24h<br>"
                       "• Escalade : %2").arg(priorite, escalation);
    }
    else if (statutLower.contains("résolu") || statutLower.contains("resolu")) {
        QString prevention = niveauInt >= 3 ? "Inspection préventive programmée" : "Recommandations standards";

        return QString("<strong>DÉCISION : CLÔTURE INCIDENT</strong><br><br>"
                       "• Validation : Réparation conforme aux standards qualité<br>"
                       "• Documentation : Dossier technique archivé<br>"
                       "• Prévention : %1<br>"
                       "• Feedback : Contact résident pour évaluation satisfaction<br>"
                       "• Clôture : Processus administratif complet").arg(prevention);
    }
    else {
        QString surveillance = niveauInt >= 3 ? "Rapports quotidiens obligatoires" : "Suivi standard";

        return QString("<strong>DÉCISION : SUIVI RENFORCÉ</strong><br><br>"
                       "• Prochaine étape : Réunion technique de coordination<br>"
                       "• Délai décision : Sous 48 heures maximum<br>"
                       "• Surveillance : %1<br>"
                       "• Communication : Point situation dans 24 heures").arg(surveillance);
    }
}
