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

// Fonction pour générer des retours techniques fictifs - VERSION CORRIGÉE
// Fonction pour générer des retours techniques fictifs - VERSION CORRIGÉE
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

void IncidentsPage::onExportPdf()
{
    // Vérifier qu'un incident est sélectionné
    QModelIndexList selectedIndexes = ui->incidentTable->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, "Export PDF", "Veuillez sélectionner un incident à exporter.");
        return;
    }

    // Récupérer la ligne sélectionnée
    int selectedRow = selectedIndexes.first().row();

    QString fileName = QFileDialog::getSaveFileName(this, "Exporter Rapport PDF", "Rapport_Incident_", "*.pdf");
    if (fileName.isEmpty()) return;

    // Récupérer les données de l'incident sélectionné
    QString id = ui->incidentTable->item(selectedRow, 0)->text();
    QString type = ui->incidentTable->item(selectedRow, 1)->text();
    QString localisation = ui->incidentTable->item(selectedRow, 2)->text();
    QString dateHeure = ui->incidentTable->item(selectedRow, 3)->text();
    QString niveau = ui->incidentTable->item(selectedRow, 4)->text();
    QString statut = ui->incidentTable->item(selectedRow, 5)->text();
    QString cinResident = ui->incidentTable->item(selectedRow, 6)->text();

    // Obtenir le nom du résident
    QString nomResident = getNomResidentFromCIN(cinResident);

    // Générer les contenus techniques
    QString retourTechnique = genererRetourTechnique(statut, type, niveau);
    QString decisionFinale = genererDecisionFinale(statut, type, niveau);

    // Construction du HTML avec fond rouge pour le titre et gris pour le contenu
    QString html = QString(
                       "<html><head><style>"
                       "body { font-family: 'Arial', sans-serif; margin: 0; line-height: 1.5; font-size: 12pt; background-color: #f0f0f0; }"

                       "/* EN-TÊTE AVEC FOND ROUGE */"
                       ".header { text-align: center; margin-bottom: 30px; padding: 40px 0; background-color: #c0392b; color: white; border-bottom: 5px solid #a93226; }"
                       ".main-title { font-size: 32pt; font-weight: bold; color: white; margin-bottom: 15px; text-transform: uppercase; }"
                       ".sub-title { font-size: 16pt; color: #f8f9fa; margin-bottom: 20px; }"
                       ".reference { font-size: 14pt; color: white; background-color: #a93226; padding: 10px 25px; border-radius: 8px; display: inline-block; border: 2px solid #922b21; }"

                       "/* CONTENEUR PRINCIPAL AVEC FOND GRIS */"
                       ".container { background-color: #f8f9fa; margin: 20px; padding: 30px; border-radius: 10px; border: 2px solid #bdc3c7; }"

                       "/* TABLEAU AVEC FOND GRIS CLAIR ET BORDURES VISIBLES */"
                       ".data-table { width: 100%%; border-collapse: collapse; margin: 25px 0; font-size: 12pt; border: 3px solid #34495e; background-color: #e9ecef; }"
                       ".data-table th { background-color: #c0392b; color: white; padding: 16px; text-align: center; border: 2px solid #a93226; font-weight: bold; font-size: 14pt; }"
                       ".data-table td { padding: 14px; border: 2px solid #95a5a6; text-align: center; vertical-align: middle; font-size: 12pt; background-color: white; }"
                       ".data-table tr:nth-child(even) td { background-color: #f8f9fa; }"
                       ".data-table tr:nth-child(odd) td { background-color: #ffffff; }"

                       "/* SECTIONS AVEC FOND GRIS */"
                       ".section { margin: 30px 0; padding: 0; }"
                       ".section-title { background-color: #c0392b; color: white; padding: 16px; font-weight: bold; font-size: 18pt; text-align: center; border-radius: 8px; margin-bottom: 20px; border: 2px solid #a93226; }"

                       "/* CONTENU TECHNIQUE AVEC FOND GRIS */"
                       ".technical-content { background-color: #e9ecef; padding: 25px; border: 2px solid #3498db; border-radius: 8px; font-size: 12pt; margin: 15px 0; line-height: 1.6; }"
                       ".decision-content { background-color: #e9ecef; padding: 25px; border: 2px solid #27ae60; border-radius: 8px; font-size: 12pt; margin: 15px 0; line-height: 1.6; }"

                       "/* BADGES */"
                       ".badge { display: inline-block; padding: 8px 16px; border-radius: 20px; color: white; font-weight: bold; font-size: 11pt; margin: 2px; border: 1px solid rgba(0,0,0,0.2); }"
                       ".badge-urgence-1 { background-color: #2ecc71; border-color: #27ae60; }"
                       ".badge-urgence-2 { background-color: #f39c12; border-color: #d35400; }"
                       ".badge-urgence-3 { background-color: #e67e22; border-color: #d35400; }"
                       ".badge-urgence-4 { background-color: #e74c3c; border-color: #c0392b; }"
                       ".badge-urgence-5 { background-color: #c0392b; border-color: #a93226; }"
                       ".badge-statut-resolu { background-color: #27ae60; border-color: #229954; }"
                       ".badge-statut-cours { background-color: #f39c12; border-color: #e67e22; }"
                       ".badge-statut-signale { background-color: #3498db; border-color: #2980b9; }"
                       ".badge-statut-autre { background-color: #95a5a6; border-color: #7f8c8d; }"

                       "/* SIGNATURE AVEC FOND GRIS */"
                       ".signature-section { margin-top: 40px; padding: 30px; background-color: #e9ecef; border: 2px solid #bdc3c7; border-radius: 8px; }"
                       ".signature-table { width: 100%%; border-collapse: collapse; margin-top: 20px; }"
                       ".signature-table td { padding: 25px; text-align: center; vertical-align: bottom; width: 50%%; }"
                       ".signature-line { border-top: 2px solid #2c3e50; width: 250px; margin: 70px auto 10px auto; }"

                       "/* PIED DE PAGE */"
                       ".footer { text-align: center; margin-top: 30px; padding: 20px; background-color: #2c3e50; color: white; border-radius: 8px; font-size: 11pt; }"

                       "/* TEXTES */"
                       ".label { font-weight: bold; color: #2c3e50; }"
                       ".value { color: #34495e; font-weight: normal; }"
                       ".important { font-weight: bold; color: #c0392b; }"
                       "</style></head><body>"

                       "<!-- EN-TÊTE AVEC FOND ROUGE COMPLET -->"
                       "<div class='header'>"
                       "<div class='main-title'>RAPPORT D'INCIDENT NEXORA</div>"
                       "<div class='sub-title'>Document Officiel de Gestion des Incidents</div>"
                       "<div class='reference'>Référence : INC-%1 | Date de génération : %11</div>"
                       "</div>"

                       "<!-- CONTENEUR PRINCIPAL AVEC FOND GRIS -->"
                       "<div class='container'>"

                       "<!-- TABLEAU AVEC FOND GRIS CLAIR -->"
                       "<div class='section'>"
                       "<div class='section-title'>📊 SYNTHÈSE DE L'INCIDENT</div>"
                       "<table class='data-table'>"
                       "<tr>"
                       "<th style='width: 30%%'>Élément</th>"
                       "<th style='width: 35%%'>Valeur</th>"
                       "<th style='width: 35%%'>Détails</th>"
                       "</tr>"
                       "<tr>"
                       "<td class='label'>🔢 Référence</td>"
                       "<td class='value important'>INC-%1</td>"
                       "<td class='value'>Identifiant unique</td>"
                       "</tr>"
                       "<tr>"
                       "<td class='label'>📅 Date & Heure</td>"
                       "<td class='value'>%4</td>"
                       "<td class='value'>Moment de la déclaration</td>"
                       "</tr>"
                       "<tr>"
                       "<td class='label'>⚡ Type d'incident</td>"
                       "<td class='value'>%2</td>"
                       "<td class='value'>Nature du problème</td>"
                       "</tr>"
                       "<tr>"
                       "<td class='label'>📍 Localisation</td>"
                       "<td class='value'>%3</td>"
                       "<td class='value'>Lieu exact</td>"
                       "</tr>"
                       "<tr>"
                       "<td class='label'>🚨 Niveau d'urgence</td>"
                       "<td class='value'><span class='badge badge-urgence-%5'>NIVEAU %5</span></td>"
                       "<td class='value'>Priorité d'intervention</td>"
                       "</tr>"
                       "<tr>"
                       "<td class='label'>📊 Statut actuel</td>"
                       "<td class='value'><span class='%16'>%6</span></td>"
                       "<td class='value'>État de traitement</td>"
                       "</tr>"
                       "<tr>"
                       "<td class='label'>👤 Résident concerné</td>"
                       "<td class='value' colspan='2'>%7</td>"
                       "</tr>"
                       "<tr>"
                       "<td class='label'>🔑 CIN Résident</td>"
                       "<td class='value' colspan='2'>%8</td>"
                       "</tr>"
                       "</table>"

                       "<!-- SIGNATURE DIRECTEMENT SOUS LE TABLEAU -->"
                       "<div class='signature-section'>"
                       "<div style='text-align: center; font-size: 18pt; font-weight: bold; color: #2c3e50; margin-bottom: 25px;'>VALIDATION OFFICIELLE</div>"
                       "<table class='signature-table'>"
                       "<tr>"
                       "<td>"
                       "<div style='text-align: center;'>"
                       "<div style='font-size: 14pt; font-weight: bold; margin-bottom: 20px; color: #34495e;'>RESPONSABLE TECHNIQUE</div>"
                       "<div class='signature-line'></div>"
                       "<div style='margin-top: 12px; font-size: 12pt; color: #7f8c8d;'>Nom et signature</div>"
                       "<div style='margin-top: 30px; font-size: 11pt; color: #95a5a6;'>Date : _________________________</div>"
                       "</div>"
                       "</td>"
                       "<td>"
                       "<div style='text-align: center;'>"
                       "<div style='font-size: 14pt; font-weight: bold; margin-bottom: 20px; color: #34495e;'>DIRECTEUR NEXORA</div>"
                       "<div class='signature-line'></div>"
                       "<div style='margin-top: 12px; font-size: 12pt; color: #7f8c8d;'>Nom et signature</div>"
                       "<div style='margin-top: 30px; font-size: 11pt; color: #95a5a6;'>Date : _________________________</div>"
                       "</div>"
                       "</td>"
                       "</tr>"
                       "</table>"
                       "</div>"
                       "</div>"

                       "<!-- ANALYSE TECHNIQUE -->"
                       "<div class='section'>"
                       "<div class='section-title'>🔧 DIAGNOSTIC TECHNIQUE DÉTAILLÉ</div>"
                       "<div class='technical-content'>"
                       "<div style='text-align: center; font-weight: bold; color: #2c3e50; margin-bottom: 15px; font-size: 14pt;'>ANALYSE DU SERVICE TECHNIQUE</div>"
                       "%9"
                       "</div>"
                       "</div>"

                       "<!-- DÉCISION FINALE -->"
                       "<div class='section'>"
                       "<div class='section-title'>✅ DÉCISION ET PLAN D'ACTION</div>"
                       "<div class='decision-content'>"
                       "<div style='text-align: center; font-weight: bold; color: #2c3e50; margin-bottom: 15px; font-size: 14pt;'>RECOMMANDATIONS OFFICIELLES</div>"
                       "%10"
                       "</div>"
                       "</div>"

                       "</div>"  // Fermeture du container principal

                       "<!-- PIED DE PAGE -->"
                       "<div class='footer'>"
                       "<div style='font-size: 13pt; margin-bottom: 10px;'>📄 DOCUMENT OFFICIEL NEXORA</div>"
                       "<div>Rapport généré automatiquement le %11 à %12</div>"
                       "<div style='margin-top: 10px; font-size: 10pt; opacity: 0.9;'>Système de Gestion des Incidents - Version 1.0</div>"
                       "</div>"
                       "</body></html>"
                       ).arg(
                           id, type, localisation, dateHeure, niveau, statut.toUpper(), nomResident, cinResident,
                           retourTechnique, decisionFinale,
                           QDate::currentDate().toString("dd/MM/yyyy"), QTime::currentTime().toString("HH:mm"),
                           // Déterminer la classe du badge de statut
                           (statut.toLower().contains("résolu") || statut.toLower().contains("resolu") ? "badge badge-statut-resolu" :
                                statut.toLower().contains("en cours") ? "badge badge-statut-cours" :
                                statut.toLower().contains("signalé") ? "badge badge-statut-signale" : "badge badge-statut-autre")
                           );

    // Génération du PDF avec haute qualité
    QPdfWriter writer(fileName);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setTitle("Rapport Incident NEXORA - INC-" + id);
    writer.setCreator("Système de Gestion NEXORA");

    // Haute résolution pour une meilleure qualité
    writer.setResolution(200);

    QPainter painter(&writer);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    QTextDocument doc;
    doc.setHtml(html);
    doc.setPageSize(QSizeF(writer.width(), writer.height()));
    doc.drawContents(&painter);

    painter.end();

    QMessageBox::information(this, "Export PDF", "Rapport détaillé exporté avec succès !");
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

    QSqlQuery query;
    query.prepare("SELECT statut, COUNT(*) FROM incidents GROUP BY statut");

    if (query.exec()) {
        while (query.next()) {
            QString statut = query.value(0).toString();
            int count = query.value(1).toInt();
            series->append(statut, count);
        }
    } else {
        // Fallback si la requête échoue
        series->append("Signalé", 0);
        series->append("En cours", 0);
        series->append("Résolu", 0);
        series->append("Archivé", 0);
    }

    for (QPieSlice *slice : series->slices()) {
        slice->setLabelVisible(true);
        slice->setLabelPosition(QPieSlice::LabelOutside);
        slice->setLabelBrush(QBrush(Qt::black));
        slice->setLabelFont(QFont("Arial", 10, QFont::Bold));

        double percentage = (slice->value() / series->sum()) * 100;
        QString label = QString("%1\n%2 incidents\n%3%")
                            .arg(slice->label())
                            .arg(slice->value())
                            .arg(QString::number(percentage, 'f', 1));
        slice->setLabel(label);
    }

    series->setLabelsVisible(true);
    statsChart->addSeries(series);
    statsChart->setTitleFont(QFont("Arial", 14, QFont::Bold));
    statsChart->legend()->setVisible(true);
    statsChart->legend()->setAlignment(Qt::AlignBottom);
    statsChart->legend()->setFont(QFont("Arial", 8));
    statsChart->setMargins(QMargins(5, 5, 5, 5));
    statsChart->setAnimationOptions(QChart::AllAnimations);
    ui->chartView->setChart(statsChart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);
    ui->chartView->setMinimumHeight(350);

    qDebug() << "Chart initialisé avec données BD - Total:" << series->sum();
}
void IncidentsPage::updateStats()
{
    if (!ui->chartView) return;

    // Méthode SANS suppression/création - juste mise à jour
    if (!statsChart) {
        initializeStatsChart();
        return;
    }

    // Récupérer le series existant
    if (statsChart->series().isEmpty()) return;

    QPieSeries *oldSeries = qobject_cast<QPieSeries*>(statsChart->series().at(0));
    if (!oldSeries) return;

    // Créer un nouveau series avec données fraîches
    QPieSeries *newSeries = new QPieSeries();

    QSqlQuery query;
    query.prepare("SELECT statut, COUNT(*) FROM incidents GROUP BY statut");

    if (query.exec()) {
        while (query.next()) {
            QString statut = query.value(0).toString();
            int count = query.value(1).toInt();
            if (count > 0) {
                newSeries->append(statut, count);
            }
        }
    }

    // Configurer les labels du nouveau series
    for (QPieSlice *slice : newSeries->slices()) {
        slice->setLabelVisible(true);
        slice->setLabelPosition(QPieSlice::LabelOutside);
        slice->setLabelBrush(QBrush(Qt::black));
        slice->setLabelFont(QFont("Arial", 10, QFont::Bold));

        double percentage = (slice->value() / newSeries->sum()) * 100;
        QString label = QString("%1\n%2 incidents\n%3%")
                            .arg(slice->label())
                            .arg(slice->value())
                            .arg(QString::number(percentage, 'f', 1));
        slice->setLabel(label);
    }

    // Remplacer l'ancien series
    statsChart->removeSeries(oldSeries);
    statsChart->addSeries(newSeries);

    delete oldSeries; // Nettoyer l'ancien

    ui->chartView->update();
    qDebug() << "Chart mis à jour - Total:" << newSeries->sum() << "incidents";
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
