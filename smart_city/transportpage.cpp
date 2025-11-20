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
#include <QTextDocument>
#include <QDateTime>
#include <QMap>
#include <limits>
#include <QChart>
#include <QChartView>
#include <QPieSeries>
#include <QBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialog>
#include <QRegularExpressionValidator>
#include <QDir>

TransportPage::TransportPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TransportPage)
{
    ui->setupUi(this);

    if (!DatabaseManager::instance().database().isOpen()) {
        QMessageBox::critical(this, "Erreur", "Base de données non connectée !");
    } else {
        qDebug() << "✅ Base connectée dans TransportPage";
        // Créer la table historique silencieusement
        DatabaseManager::instance().createHistoriqueTable(); // Pas de vérification du retour
    }

    this->setAttribute(Qt::WA_StyledBackground, true);

    ui->tabAddButton->setChecked(false);
    ui->tabListButton->setChecked(true);
    ui->tabStatsButton->setChecked(false);
    ui->stackedWidget->setCurrentWidget(ui->listPage);

    setupTableHeaders();
    setupUiBehavior();
    setupValidation();

    // Connecter le signal dataChanged du DatabaseManager
    connect(&DatabaseManager::instance(), &DatabaseManager::dataChanged,
            this, &TransportPage::onDataChanged);

    // Charger les données initiales
    QTimer::singleShot(100, this, &TransportPage::loadAllVehicules);

    qDebug() << "✅ TransportPage initialisé avec historique SQL";
}

TransportPage::~TransportPage()
{
    delete ui;
}

// === NOUVELLES MÉTHODES POUR HISTORIQUE SQL ===

void TransportPage::sauvegarderHistoriqueSQL(const QVariantMap &vehicule, const QString &action, const QString &notes)
{
    // Vérifier que nous avons au moins l'ID du véhicule
    QString idVehicule = vehicule.value("id_vehicule").toString();
    if (idVehicule.isEmpty()) {
        qDebug() << "❌ Impossible de sauvegarder l'historique: ID véhicule manquant";
        return;
    }

    QVariantMap historiqueEntry;

    // Copier toutes les données du véhicule
    historiqueEntry["action"] = action;
    historiqueEntry["id_vehicule"] = idVehicule;
    historiqueEntry["type"] = vehicule.value("type");
    historiqueEntry["capacite"] = vehicule.value("capacite");
    historiqueEntry["zone"] = vehicule.value("zone");
    historiqueEntry["horaire"] = vehicule.value("horaire");
    historiqueEntry["statut"] = vehicule.value("statut");
    historiqueEntry["date_ajout"] = vehicule.value("date_ajout");
    historiqueEntry["temps_utilise"] = vehicule.value("temps_utilise");
    historiqueEntry["code_unique"] = vehicule.value("code_unique");
    historiqueEntry["notes"] = notes;

    qDebug() << "💾 Sauvegarde historique - Action:" << action
             << "Véhicule:" << idVehicule
             << "Notes:" << notes;

    if (DatabaseManager::instance().addHistoriqueVehicule(historiqueEntry)) {
        qDebug() << "✅ Historique SQL sauvegardé avec succès";
    } else {
        qDebug() << "❌ Erreur sauvegarde historique SQL";
        QMessageBox::warning(this, "Historique",
                             "Impossible de sauvegarder l'historique pour le véhicule: " + idVehicule);
    }
}

QList<QVariantMap> TransportPage::getHistoriqueCompletSQL()
{
    QList<QVariantMap> historique = DatabaseManager::instance().getAllHistoriqueVehicules();
    qDebug() << "📖 Historique SQL chargé:" << historique.size() << "entrées";
    return historique;
}

// === SLOTS ===

void TransportPage::onHistoriqueClicked()
{
    afficherHistoriqueComplet();
}

void TransportPage::onDataChanged()
{
    qDebug() << "📢 Signal dataChanged reçu - Rechargement automatique";
    loadAllVehicules();
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
}
QString TransportPage::detecterModifications(const QVariantMap &ancien, const QVariantMap &nouveau)
{
    QStringList modifications;

    QStringList champs = {"type", "capacite", "zone", "horaire", "statut", "date_ajout", "temps_utilise", "code_unique"};

    for (const QString &champ : champs) {
        QVariant ancienneValeur = ancien.value(champ);
        QVariant nouvelleValeur = nouveau.value(champ);

        if (ancienneValeur != nouvelleValeur) {
            QString mod = QString("%1: '%2' → '%3'")
                              .arg(champ)
                              .arg(ancienneValeur.toString())
                              .arg(nouvelleValeur.toString());
            modifications << mod;
        }
    }

    if (modifications.isEmpty()) {
        return "Aucune modification détectée";
    }

    return "Modifications: " + modifications.join(" | ");
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
    QVariantMap ancienVehicule = DatabaseManager::instance().getTransportVehicule(id);
    bool isUpdate = !ancienVehicule.isEmpty();

    bool success = false;
    if (isUpdate) {
        qDebug() << "🔄 Mise à jour du véhicule" << id;
        success = DatabaseManager::instance().updateTransportVehicule(id, data);

        if (success) {
            // Sauvegarder les modifications détaillées dans l'historique
            QString modifications = detecterModifications(ancienVehicule, data);
            sauvegarderHistoriqueSQL(data, "MODIFICATION", modifications);
        }
    } else {
        qDebug() << "➕ Ajout du véhicule" << id;
        success = DatabaseManager::instance().addTransportVehicule(data);

        if (success) {
            sauvegarderHistoriqueSQL(data, "AJOUT", "Nouveau véhicule ajouté au parc");
        }
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
    QString file = QFileDialog::getSaveFileName(this, "Exporter PDF", "vehicules_transport.pdf", "PDF (*.pdf)");
    if (file.isEmpty()) return;

    QPdfWriter writer(file);
    writer.setPageSize(QPageSize::A4);
    writer.setPageOrientation(QPageLayout::Portrait);
    writer.setPageMargins(QMarginsF(15, 15, 15, 15));
    writer.setResolution(150);

    QPainter painter(&writer);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    int pageWidth = writer.width();
    int pageHeight = writer.height();

    // Couleurs modernes
    QColor primaryColor(41, 128, 185);
    QColor secondaryColor(52, 152, 219);
    QColor accentColor(46, 204, 113);
    QColor warningColor(241, 196, 15);
    QColor lightGray(248, 249, 250);
    QColor darkGray(52, 58, 64);

    // En-tête avec fond coloré
    painter.fillRect(0, 0, pageWidth, 200, primaryColor);

    // Logo/Titre
    QFont titleFont("Arial", 24, QFont::Bold);
    painter.setFont(titleFont);
    painter.setPen(Qt::white);
    painter.drawText(QRect(0, 50, pageWidth, 60), Qt::AlignCenter, "NEXORA TRANSPORT");

    QFont subtitleFont("Arial", 14);
    painter.setFont(subtitleFont);
    painter.setPen(QColor(200, 200, 200));
    painter.drawText(QRect(0, 110, pageWidth, 40), Qt::AlignCenter, "Gestion des Véhicules - Rapport Détaillé");

    // Date
    QString dateStr = QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm");
    painter.drawText(QRect(0, 150, pageWidth, 30), Qt::AlignCenter, dateStr);

    int yPos = 220;

    // Statistiques résumées
    QTableWidget *t = ui->vehiculesTable;
    int totalVehicules = 0;
    int disponibles = 0;
    int reserves = 0;
    int maintenance = 0;

    for (int r = 0; r < t->rowCount(); ++r) {
        if (t->isRowHidden(r)) continue;
        totalVehicules++;
        QString statut = t->item(r, 5) ? t->item(r, 5)->text().toLower() : "";
        if (statut.contains("disponible")) disponibles++;
        else if (statut.contains("réservé") || statut.contains("reserve")) reserves++;
        else if (statut.contains("maintenance")) maintenance++;
    }

    // Box de statistiques
    int boxWidth = (pageWidth - 40) / 3;
    int boxHeight = 80;

    // Box Total
    painter.fillRect(20, yPos, boxWidth, boxHeight, secondaryColor);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 12, QFont::Bold));
    painter.drawText(QRect(20, yPos, boxWidth, 30), Qt::AlignCenter, "TOTAL");
    painter.setFont(QFont("Arial", 18, QFont::Bold));
    painter.drawText(QRect(20, yPos + 30, boxWidth, 50), Qt::AlignCenter, QString::number(totalVehicules));

    // Box Disponibles
    painter.fillRect(30 + boxWidth, yPos, boxWidth, boxHeight, accentColor);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 12, QFont::Bold));
    painter.drawText(QRect(30 + boxWidth, yPos, boxWidth, 30), Qt::AlignCenter, "DISPONIBLES");
    painter.setFont(QFont("Arial", 18, QFont::Bold));
    painter.drawText(QRect(30 + boxWidth, yPos + 30, boxWidth, 50), Qt::AlignCenter, QString::number(disponibles));

    // Box Réservés
    painter.fillRect(40 + boxWidth * 2, yPos, boxWidth, boxHeight, warningColor);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 12, QFont::Bold));
    painter.drawText(QRect(40 + boxWidth * 2, yPos, boxWidth, 30), Qt::AlignCenter, "RÉSERVÉS");
    painter.setFont(QFont("Arial", 18, QFont::Bold));
    painter.drawText(QRect(40 + boxWidth * 2, yPos + 30, boxWidth, 50), Qt::AlignCenter, QString::number(reserves));

    yPos += boxHeight + 30;

    // Tableau principal
    if (totalVehicules > 0) {
        QStringList headers = {
            "ID", "Type", "Capacité", "Zone", "Horaire",
            "Statut", "Date", "Temps(h)", "Km", "Code"
        };

        QList<int> columnWidths = {
            pageWidth * 8 / 100,
            pageWidth * 10 / 100,
            pageWidth * 8 / 100,
            pageWidth * 15 / 100,
            pageWidth * 12 / 100,
            pageWidth * 12 / 100,
            pageWidth * 10 / 100,
            pageWidth * 8 / 100,
            pageWidth * 8 / 100,
            pageWidth * 9 / 100
        };

        // En-tête du tableau
        painter.fillRect(0, yPos, pageWidth, 40, darkGray);
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 10, QFont::Bold));

        int xPos = 0;
        for (int i = 0; i < headers.size(); ++i) {
            painter.drawText(QRect(xPos, yPos, columnWidths[i], 40),
                             Qt::AlignCenter, headers[i]);
            painter.setPen(QPen(Qt::white, 1));
            painter.drawLine(xPos + columnWidths[i], yPos, xPos + columnWidths[i], yPos + 40);
            xPos += columnWidths[i];
        }

        yPos += 40;

        // Données du tableau
        painter.setFont(QFont("Arial", 9));
        int rowHeight = 35;
        int rowsPerPage = (pageHeight - yPos - 50) / rowHeight;
        int currentRow = 0;

        for (int r = 0; r < t->rowCount(); ++r) {
            if (t->isRowHidden(r)) continue;

            if (currentRow >= rowsPerPage) {
                writer.newPage();
                yPos = 50;
                currentRow = 0;

                painter.fillRect(0, yPos, pageWidth, 40, darkGray);
                painter.setPen(Qt::white);
                painter.setFont(QFont("Arial", 10, QFont::Bold));

                xPos = 0;
                for (int i = 0; i < headers.size(); ++i) {
                    painter.drawText(QRect(xPos, yPos, columnWidths[i], 40),
                                     Qt::AlignCenter, headers[i]);
                    painter.drawLine(xPos + columnWidths[i], yPos, xPos + columnWidths[i], yPos + 40);
                    xPos += columnWidths[i];
                }
                yPos += 40;
            }

            if (currentRow % 2 == 0) {
                painter.fillRect(0, yPos, pageWidth, rowHeight, lightGray);
            } else {
                painter.fillRect(0, yPos, pageWidth, rowHeight, Qt::white);
            }

            QString statut = t->item(r, 5) ? t->item(r, 5)->text() : "";
            QColor statutColor = Qt::black;
            if (statut.toLower().contains("disponible")) statutColor = accentColor;
            else if (statut.toLower().contains("réservé") || statut.toLower().contains("reserve")) statutColor = warningColor;
            else if (statut.toLower().contains("maintenance")) statutColor = QColor(231, 76, 60);

            xPos = 0;
            painter.setPen(Qt::black);

            for (int col = 0; col < t->columnCount(); ++col) {
                QString text = t->item(r, col) ? t->item(r, col)->text() : "";

                if (col == 5) {
                    painter.setPen(statutColor);
                    painter.setFont(QFont("Arial", 9, QFont::Bold));
                } else {
                    painter.setPen(Qt::black);
                    painter.setFont(QFont("Arial", 9));
                }

                Qt::Alignment alignment = Qt::AlignLeft | Qt::AlignVCenter;
                if (col == 2 || col == 7 || col == 8) alignment = Qt::AlignRight | Qt::AlignVCenter;
                else if (col == 0 || col == 9) alignment = Qt::AlignCenter;

                QRect textRect(xPos + 5, yPos, columnWidths[col] - 10, rowHeight);
                painter.drawText(textRect, alignment, text);

                painter.setPen(QPen(QColor(200, 200, 200), 1));
                painter.drawLine(xPos + columnWidths[col], yPos, xPos + columnWidths[col], yPos + rowHeight);

                xPos += columnWidths[col];
            }

            painter.setPen(QPen(QColor(200, 200, 200), 1));
            painter.drawLine(0, yPos + rowHeight, pageWidth, yPos + rowHeight);

            yPos += rowHeight;
            currentRow++;
        }
    } else {
        painter.setPen(darkGray);
        painter.setFont(QFont("Arial", 14, QFont::Bold));
        painter.drawText(QRect(0, yPos, pageWidth, 100), Qt::AlignCenter, "Aucun véhicule à afficher");
    }

    // Pied de page
    int footerY = pageHeight - 40;
    painter.setPen(QColor(150, 150, 150));
    painter.setFont(QFont("Arial", 8));
    painter.drawLine(0, footerY, pageWidth, footerY);
    painter.drawText(QRect(0, footerY + 10, pageWidth / 2, 30),
                     Qt::AlignLeft | Qt::AlignVCenter,
                     "NEXORA Smart City Management System");
    painter.drawText(QRect(0, footerY + 10, pageWidth, 30),
                     Qt::AlignRight | Qt::AlignVCenter,
                     QString("Page 1 • Généré le %1").arg(
                         QDateTime::currentDateTime().toString("dd/MM/yyyy à HH:mm")));

    painter.end();

    QMessageBox::information(this, "Export PDF",
                             QString("📊 PDF exporté avec succès !\n\n"
                                     "• Total: %1 véhicules\n"
                                     "• Disponibles: %2\n"
                                     "• Réservés: %3\n"
                                     "• Maintenance: %4\n\n"
                                     "📁 Fichier: %5")
                                 .arg(totalVehicules)
                                 .arg(disponibles)
                                 .arg(reserves)
                                 .arg(maintenance)
                                 .arg(file));
}

void TransportPage::onEditClicked()
{
    int row = ui->vehiculesTable->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Modifier", "Veuillez sélectionner un véhicule.");
        return;
    }

    QString id = ui->vehiculesTable->item(row, 0)->text();
    QVariantMap v = DatabaseManager::instance().getTransportVehicule(id);

    if (v.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Véhicule introuvable !");
        return;
    }

    loadVehicule(v);
    onTabAdd();
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

    // Récupérer les données complètes du véhicule avant suppression pour l'historique
    QVariantMap vehicule = DatabaseManager::instance().getTransportVehicule(id);

    if (vehicule.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Véhicule introuvable !");
        return;
    }

    if (DatabaseManager::instance().deleteTransportVehicule(id)) {
        // SAUVEGARDER DANS L'HISTORIQUE SQL avec toutes les données
        sauvegarderHistoriqueSQL(vehicule, "SUPPRESSION", "Véhicule supprimé définitivement du parc");

        QMessageBox::information(this, "Succès", "Véhicule supprimé avec succès !");
        loadAllVehicules();
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

void TransportPage::onAfficherStatistiques()
{
    afficherGraphiqueKilometrage();
}

void TransportPage::onHoraireTextChanged()
{
    hideError();

    QString horaire = ui->horaireEdit->text().trimmed();

    // Validation du format HH:MM-HH:MM en temps réel
    if (!horaire.isEmpty()) {
        if (!validateHoraire(horaire)) {
            showError("Format d'horaire invalide. Utilisez HH:MM-HH:MM (ex: 08:00-18:00)");
            return;
        }
    }
}

// === MÉTHODES PRIVÉES ===

void TransportPage::setupTableHeaders()
{
    QTableWidget *t = ui->vehiculesTable;
    t->setColumnCount(10);
    QStringList headers{
        "ID Véhicule", "Type", "Capacité", "Zone", "Horaire",
        "Statut", "Date Ajout", "Temps Utilisé (h)", "Kilométrage (km)", "Code Unique"
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
    connect(ui->afficherStatsButton, &QPushButton::clicked, this, &TransportPage::onAfficherStatistiques);
    connect(ui->historiqueButton, &QPushButton::clicked, this, &TransportPage::onHistoriqueClicked);
}

void TransportPage::setupValidation()
{
    ui->errorLabel->hide();
    ui->idEdit->setMaxLength(10);
    ui->idEdit->setPlaceholderText("VH001");
    ui->capaciteEdit->setPlaceholderText("Ex: 4, 20, 5");

    // Validation de l'horaire
    QRegularExpressionValidator *horaireValidator = new QRegularExpressionValidator(
        QRegularExpression("^([0-1]?[0-9]|2[0-3]):[0-5][0-9]-([0-1]?[0-9]|2[0-3]):[0-5][0-9]$"), this);
    ui->horaireEdit->setValidator(horaireValidator);
    ui->horaireEdit->setPlaceholderText("HH:MM-HH:MM (ex: 08:00-18:00)");

    connect(ui->idEdit, &QLineEdit::textChanged, this, &TransportPage::onIdTextChanged);
    connect(ui->capaciteEdit, &QLineEdit::textChanged, this, &TransportPage::onCapaciteTextChanged);
    connect(ui->horaireEdit, &QLineEdit::textChanged, this, &TransportPage::onHoraireTextChanged);

    ui->dateAjoutEdit->setDate(QDate::currentDate());
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

    QList<QVariantMap> rows = DatabaseManager::instance().getAllTransportVehicules();
    qDebug() << "📊 Trouvé" << rows.size() << "véhicules dans la base";

    if (rows.isEmpty()) {
        qDebug() << "⚠ Aucun véhicule trouvé";
        t->blockSignals(blocked);
        return;
    }

    t->setRowCount(rows.size());

    for (int r = 0; r < rows.size(); ++r) {
        const auto &m = rows[r];

        QString type = m.value("type").toString();
        double tempsUtilise = m.value("temps_utilise").toDouble();
        double kilometrage = calculerKilometrage(type, tempsUtilise);

        t->setItem(r, 0, new QTableWidgetItem(m.value("id_vehicule").toString()));
        t->setItem(r, 1, new QTableWidgetItem(type));
        t->setItem(r, 2, new QTableWidgetItem(m.value("capacite").toString()));
        t->setItem(r, 3, new QTableWidgetItem(m.value("zone").toString()));
        t->setItem(r, 4, new QTableWidgetItem(m.value("horaire").toString()));
        t->setItem(r, 5, new QTableWidgetItem(m.value("statut").toString()));
        t->setItem(r, 6, new QTableWidgetItem(m.value("date_ajout").toString()));
        t->setItem(r, 7, new QTableWidgetItem(m.value("temps_utilise").toString()));
        t->setItem(r, 8, new QTableWidgetItem(QString::number(kilometrage, 'f', 1))); // Kilométrage
        t->setItem(r, 9, new QTableWidgetItem(m.value("code_unique").toString()));

        qDebug() << "  ➜ Ligne" << r << ":" << m.value("id_vehicule").toString()
                 << "- Km:" << kilometrage;
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

bool TransportPage::validateForm()
{
    if (!validateId(ui->idEdit->text())) {
        showError("L'ID doit commencer par 'VH'");
        return false;
    }

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

bool TransportPage::validateHoraire(const QString &horaire)
{
    QRegularExpression regex("^([0-1]?[0-9]|2[0-3]):[0-5][0-9]-([0-1]?[0-9]|2[0-3]):[0-5][0-9]$");
    QRegularExpressionMatch match = regex.match(horaire);

    if (!match.hasMatch()) {
        return false;
    }

    // Validation supplémentaire pour s'assurer que c'est une heure logique
    QStringList parties = horaire.split('-');
    if (parties.size() != 2) {
        return false;
    }

    QString debut = parties[0];
    QString fin = parties[1];

    QStringList debutParts = debut.split(':');
    QStringList finParts = fin.split(':');

    if (debutParts.size() != 2 || finParts.size() != 2) {
        return false;
    }

    int heuresDebut = debutParts[0].toInt();
    int minutesDebut = debutParts[1].toInt();
    int heuresFin = finParts[0].toInt();
    int minutesFin = finParts[1].toInt();

    // Validation des heures et minutes
    bool debutValide = (heuresDebut >= 0 && heuresDebut <= 23) && (minutesDebut >= 0 && minutesDebut <= 59);
    bool finValide = (heuresFin >= 0 && heuresFin <= 23) && (minutesFin >= 0 && minutesFin <= 59);

    return debutValide && finValide;
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

double TransportPage::calculerKilometrage(const QString& type, double tempsUtilise) const
{
    QMap<QString, double> vitessesMoyennes = {
        {"taxi", 25.0},    // Vitesse réduite en ville, nombreux arrêts
        {"bus", 20.0},     // Vitesse très réduite, arrêts fréquents
        {"voiture", 35.0}  // Vitesse normale en milieu urbain/périurbain
    };

    QString typeClean = type.trimmed().toLower();
    double vitesse = vitessesMoyennes.value(typeClean, 30.0);

    return tempsUtilise * vitesse;
}

void TransportPage::calculerPourcentagesKilometrage()
{
    QList<QVariantMap> vehicules = DatabaseManager::instance().getAllTransportVehicules();

    if (vehicules.isEmpty()) {
        ui->pourcentageKilometrage->setText("Aucun véhicule disponible");
        return;
    }

    double totalKilometrage = 0;
    QMap<QString, double> kmParType;

    for (const auto &vehicule : vehicules) {
        QString type = vehicule.value("type").toString().toLower();
        double tempsUtilise = vehicule.value("temps_utilise").toDouble();
        double km = calculerKilometrage(type, tempsUtilise);

        if (!type.isEmpty()) {
            kmParType[type] += km;
            totalKilometrage += km;
        }
    }

    if (totalKilometrage == 0) {
        ui->pourcentageKilometrage->setText("Aucun kilométrage enregistré");
        return;
    }

    QString pourcentages = "📊 RÉPARTITION DU KILOMÉTRAGE :\n\n";

    QList<QPair<QString, double>> typesTries;
    for (auto it = kmParType.begin(); it != kmParType.end(); ++it) {
        typesTries.append(qMakePair(it.key(), it.value()));
    }

    std::sort(typesTries.begin(), typesTries.end(),
              [](const QPair<QString, double> &a, const QPair<QString, double> &b) {
                  return a.second > b.second;
              });

    for (const auto &pair : typesTries) {
        QString type = pair.first;
        double kmType = pair.second;
        double pourcentage = (kmType / totalKilometrage) * 100;

        QString icone = "🚗";
        if (type == "bus") icone = "🚌";
        else if (type == "taxi") icone = "🚕";

        pourcentages += QString("%1 %2 : %3%\n")
                            .arg(icone)
                            .arg(type.toUpper())
                            .arg(QString::number(pourcentage, 'f', 1));
    }

    pourcentages += QString("\n🌍 Total : %1 km")
                        .arg(QString::number(totalKilometrage, 'f', 1));

    ui->pourcentageKilometrage->setText(pourcentages.trimmed());
}

void TransportPage::calculateAndDisplayStatistics()
{
    calculerPourcentagesKilometrage();

    QList<QVariantMap> vehicules = DatabaseManager::instance().getAllTransportVehicules();

    if (vehicules.isEmpty()) {
        ui->vehiculePlusUtilise->setText("Aucun véhicule");
        ui->vehiculeMoinsUtilise->setText("Aucun véhicule");
        return;
    }

    double maxKm = 0;
    double minKm = std::numeric_limits<double>::max();
    QString vehiculePlusUtilise;
    QString vehiculeMoinsUtilise;

    for (const auto &v : vehicules) {
        QString type = v.value("type").toString();
        double tempsUtilise = v.value("temps_utilise").toDouble();
        double km = calculerKilometrage(type, tempsUtilise);
        QString id = v.value("id_vehicule").toString();

        if (km > maxKm) {
            maxKm = km;
            vehiculePlusUtilise = QString("%1 (%2 km)").arg(id).arg(QString::number(km, 'f', 1));
        }

        if (km < minKm) {
            minKm = km;
            vehiculeMoinsUtilise = QString("%1 (%2 km)").arg(id).arg(QString::number(km, 'f', 1));
        }
    }

    ui->vehiculePlusUtilise->setText(vehiculePlusUtilise);
    ui->vehiculeMoinsUtilise->setText(vehiculeMoinsUtilise);
}

void TransportPage::afficherGraphiqueKilometrage()
{
    QList<QVariantMap> vehicules = DatabaseManager::instance().getAllTransportVehicules();

    if (vehicules.isEmpty()) {
        QMessageBox::information(this, "Statistiques", "Aucun véhicule disponible pour afficher les statistiques.");
        return;
    }

    QMap<QString, double> kmTotalParType;
    double kilometrageTotalGlobal = 0;

    for (const auto &vehicule : vehicules) {
        QString type = vehicule.value("type").toString().toLower();
        double tempsUtilise = vehicule.value("temps_utilise").toDouble();
        double km = calculerKilometrage(type, tempsUtilise);

        kmTotalParType[type] += km;
        kilometrageTotalGlobal += km;
    }

    if (kilometrageTotalGlobal == 0) {
        QMessageBox::information(this, "Statistiques", "Aucun kilométrage enregistré.");
        return;
    }

    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("📊 Statistiques de Kilométrage par Type");
    dialog->setMinimumSize(800, 600);
    dialog->setStyleSheet(R"(
        QDialog {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #F8FAFB, stop:1 #E2E8F0);
            font-family: "Segoe UI";
        }
        QLabel {
            font-size: 14px;
            font-weight: 600;
            color: #2D3748;
            padding: 8px;
        }
        QTableWidget {
            border: 1px solid rgba(135,206,235,0.3);
            border-radius: 8px;
            background: white;
        }
        QHeaderView::section {
            background: #87CEEB;
            color: white;
            font-weight: 600;
            padding: 8px;
        }
    )");

    QVBoxLayout *layout = new QVBoxLayout(dialog);

    QLabel *titleLabel = new QLabel("📊 RÉPARTITION DU KILOMÉTRAGE PAR TYPE DE VÉHICULE");
    titleLabel->setStyleSheet("font-size: 18px; font-weight: 700; color: #1F2D5C; padding: 15px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    QPieSeries *series = new QPieSeries();

    QMap<QString, QColor> couleurs = {
        {"taxi", QColor(255, 193, 7)},
        {"bus", QColor(33, 150, 243)},
        {"voiture", QColor(76, 175, 80)}
    };

    QList<QPair<QString, double>> typesTries;
    for (auto it = kmTotalParType.begin(); it != kmTotalParType.end(); ++it) {
        typesTries.append(qMakePair(it.key(), it.value()));
    }

    std::sort(typesTries.begin(), typesTries.end(),
              [](const QPair<QString, double> &a, const QPair<QString, double> &b) {
                  return a.second > b.second;
              });

    for (const auto &pair : typesTries) {
        QString type = pair.first;
        double km = pair.second;
        double pourcentage = (km / kilometrageTotalGlobal) * 100;

        QString label = type.toUpper() + QString(" (%1%)").arg(QString::number(pourcentage, 'f', 1));
        QPieSlice *slice = series->append(label, km);

        QString icone;
        if (type == "taxi") {
            icone = "🚕";
            slice->setColor(couleurs["taxi"]);
        } else if (type == "bus") {
            icone = "🚌";
            slice->setColor(couleurs["bus"]);
        } else if (type == "voiture") {
            icone = "🚗";
            slice->setColor(couleurs["voiture"]);
        } else {
            icone = "🚙";
            slice->setColor(QColor(158, 158, 158));
        }

        slice->setLabel(icone + " " + label);
        slice->setLabelVisible(true);
        slice->setExploded(true);
        slice->setExplodeDistanceFactor(0.05);
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition du Kilométrage");
    chart->setTitleFont(QFont("Segoe UI", 14, QFont::Bold));
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setAnimationOptions(QChart::AllAnimations);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(600, 400);
    layout->addWidget(chartView);

    QLabel *tableTitle = new QLabel("📋 DÉTAIL PAR TYPE");
    tableTitle->setStyleSheet("font-size: 16px; font-weight: 700; color: #1F2D5C; padding: 10px;");
    layout->addWidget(tableTitle);

    QTableWidget *table = new QTableWidget();
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"Type", "Kilométrage", "Pourcentage", "Icône"});
    table->setRowCount(typesTries.size());

    int row = 0;
    for (const auto &pair : typesTries) {
        QString type = pair.first;
        double km = pair.second;
        double pourcentage = (km / kilometrageTotalGlobal) * 100;

        QString icone;
        if (type == "taxi") icone = "🚕";
        else if (type == "bus") icone = "🚌";
        else if (type == "voiture") icone = "🚗";
        else icone = "🚙";

        table->setItem(row, 0, new QTableWidgetItem(type.toUpper()));
        table->setItem(row, 1, new QTableWidgetItem(QString::number(km, 'f', 1) + " km"));
        table->setItem(row, 2, new QTableWidgetItem(QString::number(pourcentage, 'f', 1) + " %"));
        table->setItem(row, 3, new QTableWidgetItem(icone));

        for (int col = 0; col < table->columnCount(); ++col) {
            QTableWidgetItem *item = table->item(row, col);
            if (item) {
                if (type == "taxi") {
                    item->setBackground(QColor(255, 245, 157));
                } else if (type == "bus") {
                    item->setBackground(QColor(197, 202, 233));
                } else if (type == "voiture") {
                    item->setBackground(QColor(200, 230, 201));
                }
            }
        }
        row++;
    }

    table->resizeColumnsToContents();
    layout->addWidget(table);

    QLabel *summaryLabel = new QLabel(
        QString("🌍 KILOMÉTRAGE TOTAL: %1 km\n"
                "📊 NOMBRE DE VÉHICULES: %2")
            .arg(QString::number(kilometrageTotalGlobal, 'f', 1))
            .arg(vehicules.size())
        );
    summaryLabel->setStyleSheet("font-size: 14px; font-weight: 600; color: #2D3748; background: #E3F2FD; padding: 12px; border-radius: 8px;");
    summaryLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(summaryLabel);

    dialog->exec();
    delete dialog;
}

void TransportPage::afficherHistoriqueComplet()
{
    // UTILISEZ LA NOUVELLE MÉTHODE SQL
    QList<QVariantMap> historique = getHistoriqueCompletSQL();

    if (historique.isEmpty()) {
        QMessageBox::information(this, "Historique",
                                 "Aucun historique disponible dans la base de données.\n\n"
                                 "L'historique enregistre automatiquement :\n"
                                 "• Les ajouts de véhicules\n"
                                 "• Les modifications de véhicules\n"
                                 "• Les suppressions de véhicules");
        return;
    }

    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("📜 Historique Complet des Véhicules (Base de Données SQL)");
    dialog->setMinimumSize(1400, 800);
    dialog->setStyleSheet(R"(
        QDialog {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #F8FAFB, stop:1 #E2E8F0);
            font-family: "Segoe UI";
        }
        QLabel {
            font-size: 14px;
            font-weight: 600;
            color: #2D3748;
            padding: 8px;
        }
        QTableWidget {
            border: 2px solid #87CEEB;
            border-radius: 10px;
            background: white;
            gridline-color: #E2E8F0;
        }
        QHeaderView::section {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #4A90E2, stop:1 #357ABD);
            color: white;
            font-weight: bold;
            padding: 12px 8px;
            border: none;
            font-size: 11px;
        }
        QTableWidget::item {
            padding: 8px;
            border-bottom: 1px solid #E2E8F0;
        }
    )");

    QVBoxLayout *layout = new QVBoxLayout(dialog);

    // En-tête
    QLabel *titleLabel = new QLabel("📜 HISTORIQUE COMPLET DES VÉHICULES (BASE DE DONNÉES SQL)");
    titleLabel->setStyleSheet("font-size: 22px; font-weight: 700; color: #1F2D5C; padding: 20px; background: rgba(135,206,235,0.2); border-radius: 12px; margin: 5px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    // Statistiques
    int ajouts = 0, modifications = 0, suppressions = 0;
    QSet<QString> vehiculesUniques;

    for (const auto &v : historique) {
        QString action = v.value("action").toString();
        QString idVehicule = v.value("id_vehicule").toString();

        if (action == "AJOUT") ajouts++;
        else if (action == "MODIFICATION") modifications++;
        else if (action == "SUPPRESSION") suppressions++;

        vehiculesUniques.insert(idVehicule);
    }

    QLabel *statsLabel = new QLabel(
        QString("📊 STATISTIQUES :  "
                "🏷️ Véhicules uniques: %1  |  "
                "➕ Ajouts: %2  |  "
                "✏️ Modifications: %3  |  "
                "🗑️ Suppressions: %4  |  "
                "📋 Total opérations: %5")
            .arg(vehiculesUniques.size())
            .arg(ajouts).arg(modifications).arg(suppressions).arg(historique.size())
        );
    statsLabel->setStyleSheet("font-size: 13px; font-weight: 600; color: #2D3748; background: #E3F2FD; padding: 12px; border-radius: 8px; margin: 5px;");
    statsLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(statsLabel);

    // Tableau
    QTableWidget *table = new QTableWidget();
    table->setColumnCount(13);
    table->setHorizontalHeaderLabels({
        "ID", "Action", "Date/Heure", "ID Véhicule", "Type", "Capacité", "Zone",
        "Horaire", "Statut", "Date Ajout", "Temps Utilisé (h)", "Code Unique", "Notes"
    });

    table->setRowCount(historique.size());

    for (int i = 0; i < historique.size(); ++i) {
        const auto &v = historique[i];
        QString action = v.value("action").toString();
        QString timestamp = v.value("timestamp").toString();

        table->setItem(i, 0, new QTableWidgetItem(v.value("id").toString()));
        table->setItem(i, 1, new QTableWidgetItem(action));
        table->setItem(i, 2, new QTableWidgetItem(timestamp));
        table->setItem(i, 3, new QTableWidgetItem(v.value("id_vehicule").toString()));
        table->setItem(i, 4, new QTableWidgetItem(v.value("type").toString()));
        table->setItem(i, 5, new QTableWidgetItem(v.value("capacite").toString()));
        table->setItem(i, 6, new QTableWidgetItem(v.value("zone").toString()));
        table->setItem(i, 7, new QTableWidgetItem(v.value("horaire").toString()));
        table->setItem(i, 8, new QTableWidgetItem(v.value("statut").toString()));
        table->setItem(i, 9, new QTableWidgetItem(v.value("date_ajout").toString()));
        table->setItem(i, 10, new QTableWidgetItem(v.value("temps_utilise").toString()));
        table->setItem(i, 11, new QTableWidgetItem(v.value("code_unique").toString()));
        table->setItem(i, 12, new QTableWidgetItem(v.value("notes").toString()));

        // Colorer la ligne selon l'action
        QColor rowColor;
        QString icone;

        if (action == "AJOUT") {
            rowColor = QColor(220, 237, 200); // Vert clair
            icone = "🟢";
        } else if (action == "MODIFICATION") {
            rowColor = QColor(255, 243, 205); // Jaune clair
            icone = "🟡";
        } else if (action == "SUPPRESSION") {
            rowColor = QColor(248, 215, 218); // Rouge clair
            icone = "🔴";
        }

        // Ajouter l'icône à la colonne action
        table->item(i, 1)->setText(icone + " " + action);

        for (int col = 0; col < table->columnCount(); ++col) {
            QTableWidgetItem *item = table->item(i, col);
            if (item) {
                item->setBackground(rowColor);
                item->setToolTip(QString("Action: %1\nVéhicule: %2\nDate: %3\nNotes: %4")
                                     .arg(action)
                                     .arg(v.value("id_vehicule").toString())
                                     .arg(timestamp)
                                     .arg(v.value("notes").toString()));

                if (col == 1) {
                    QFont font = item->font();
                    font.setBold(true);
                    item->setFont(font);
                    item->setTextAlignment(Qt::AlignCenter);
                }
            }
        }
    }

    table->resizeColumnsToContents();
    table->setAlternatingRowColors(false);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    layout->addWidget(table);

    // Bouton Fermer
    QPushButton *closeButton = new QPushButton("✖️ Fermer");
    closeButton->setStyleSheet(R"(
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #F59E0B, stop:1 #D97706);
            color: white;
            border-radius: 8px;
            padding: 10px 20px;
            font-weight: 600;
            border: none;
            font-size: 12px;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #D97706, stop:1 #B45309);
        }
    )");
    connect(closeButton, &QPushButton::clicked, dialog, &QDialog::close);
    layout->addWidget(closeButton, 0, Qt::AlignRight);

    dialog->exec();
    delete dialog;
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
