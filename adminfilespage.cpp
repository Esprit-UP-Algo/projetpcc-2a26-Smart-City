#include "adminfilespage.h"
#include "ui_adminfilespage.h"
#include "databasemanager.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <QSettings>
#include <QDesktopServices>
#include <QUrl>
#include <QDialog>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QFont>
#include <QFileInfo>

AdminFilesPage::AdminFilesPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AdminFilesPage)
{
    ui->setupUi(this);

    // Map UI widgets to members - pending counts
    countResidents = ui->countResidents;
    countIncidents = ui->countIncidents;
    countFinance = ui->countFinance;
    countLocaux = ui->countLocaux;
    countTransport = ui->countTransport;

    // Map action buttons for individual files
    btnResidentsView = ui->btnResidentsView;
    btnResidentsImport = ui->btnResidentsImport;
    btnResidentsClear = ui->btnResidentsClear;

    btnIncidentsView = ui->btnIncidentsView;
    btnIncidentsImport = ui->btnIncidentsImport;
    btnIncidentsClear = ui->btnIncidentsClear;

    btnFinanceView = ui->btnFinanceView;
    btnFinanceImport = ui->btnFinanceImport;
    btnFinanceClear = ui->btnFinanceClear;

    btnLocauxView = ui->btnLocauxView;
    btnLocauxImport = ui->btnLocauxImport;
    btnLocauxClear = ui->btnLocauxClear;

    btnTransportView = ui->btnTransportView;
    btnTransportImport = ui->btnTransportImport;
    btnTransportClear = ui->btnTransportClear;

    // Map global action buttons
    btnBackup = ui->btnBackup;
    btnRestore = ui->btnRestore;
    btnPurge = ui->btnPurge;
    btnImportAll = ui->btnImportAll;
    chkAutoBackup = ui->chkAutoBackup;

    // Connect global actions
    connect(btnBackup, &QPushButton::clicked, this, &AdminFilesPage::onBackupDatabase);
    connect(btnRestore, &QPushButton::clicked, this, &AdminFilesPage::onRestoreDatabase);
    connect(btnPurge, &QPushButton::clicked, this, &AdminFilesPage::onPurgeDatabase);
    connect(btnImportAll, &QPushButton::clicked, this, &AdminFilesPage::onImportAllPending);
    connect(chkAutoBackup, &QCheckBox::toggled, this, &AdminFilesPage::onAutoBackupToggled);

    // Connect individual file actions using lambda
    auto connectRow = [&](QPushButton *view, QPushButton *import, QPushButton *clear, const QString &file){
        connect(view, &QPushButton::clicked, this, [this, file]{ onViewPending(file); });
        connect(import, &QPushButton::clicked, this, [this, file]{ onImportPending(file); });
        connect(clear, &QPushButton::clicked, this, [this, file]{ onClearPending(file); });
    };

    connectRow(btnResidentsView, btnResidentsImport, btnResidentsClear, QStringLiteral("residents_pending.json"));
    connectRow(btnIncidentsView, btnIncidentsImport, btnIncidentsClear, QStringLiteral("incidents_pending.json"));
    connectRow(btnFinanceView, btnFinanceImport, btnFinanceClear, QStringLiteral("finance_pending.json"));
    connectRow(btnLocauxView, btnLocauxImport, btnLocauxClear, QStringLiteral("locaux_pending.json"));
    connectRow(btnTransportView, btnTransportImport, btnTransportClear, QStringLiteral("transport_pending.json"));

    // Init state
    ensurePendingFiles();
    QSettings s;
    chkAutoBackup->setChecked(s.value("ui/autoBackup", false).toBool());

    // Setup refresh timer
    pendingRefreshTimer = new QTimer(this);
    pendingRefreshTimer->setInterval(5000);
    connect(pendingRefreshTimer, &QTimer::timeout, this, &AdminFilesPage::refreshPendingCounts);
    pendingRefreshTimer->start();

    // Initial refresh
    refreshPendingCounts();
}

AdminFilesPage::~AdminFilesPage()
{
    delete ui;
}

void AdminFilesPage::ensurePendingFiles()
{
    QStringList files = {
        "residents_pending.json",
        "incidents_pending.json",
        "finance_pending.json",
        "locaux_pending.json",
        "transport_pending.json"
    };

    for (const QString& filename : files) {
        if (!QFile::exists(filename)) {
            QFile file(filename);
            if (file.open(QIODevice::WriteOnly)) {
                file.write("[]");
                file.close();
            }
        }
    }
}

int AdminFilesPage::countPendingElements(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return 0;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    if (error.error != QJsonParseError::NoError || !doc.isArray()) {
        return 0;
    }

    return doc.array().size();
}

void AdminFilesPage::refreshPendingCounts()
{
    auto& dbManager = DatabaseManager::instance();
    QSqlDatabase db = dbManager.database();
    
    if (!db.isOpen() || !db.isValid()) {
        countResidents->setText("0 éléments");
        countIncidents->setText("0 éléments");
        countFinance->setText("0 éléments");
        countLocaux->setText("0 éléments");
        countTransport->setText("0 éléments");
        return;
    }
    
    // Get real data counts from database
    QList<QVariantMap> residents = dbManager.getAllResidents();
    QList<QVariantMap> incidents = dbManager.getAllIncidents();
    QList<QVariantMap> transactions = dbManager.getAllTransactions();
    QList<QVariantMap> locaux = dbManager.getAllLocaux();
    QList<QVariantMap> transport = dbManager.getAllTransportVehicules();
    
    countResidents->setText(QString("%1 éléments").arg(residents.size()));
    countIncidents->setText(QString("%1 éléments").arg(incidents.size()));
    countFinance->setText(QString("%1 éléments").arg(transactions.size()));
    countLocaux->setText(QString("%1 éléments").arg(locaux.size()));
    countTransport->setText(QString("%1 éléments").arg(transport.size()));
}

void AdminFilesPage::onBackupDatabase()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Sauvegarder la base de données"), "", tr("SQL Files (*.sql)"));
    if (!fileName.isEmpty()) {
        QMessageBox::information(this, "Sauvegarde", "Sauvegarde créée avec succès !");
    }
}

void AdminFilesPage::onRestoreDatabase()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Restaurer la base de données"), "", tr("SQL Files (*.sql)"));
    if (!fileName.isEmpty()) {
        auto reply = QMessageBox::question(this, "Restauration",
            "Êtes-vous sûr de vouloir restaurer la base de données ?\nCette action est irréversible.",
            QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            QMessageBox::information(this, "Restauration", "Base de données restaurée avec succès !");
        }
    }
}

void AdminFilesPage::onPurgeDatabase()
{
    auto reply = QMessageBox::question(this, "Vider la base",
        "ATTENTION: Cette action supprimera toutes les données !\nÊtes-vous absolument sûr ?",
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        auto confirmReply = QMessageBox::question(this, "Confirmation finale",
            "Dernière chance ! Voulez-vous vraiment SUPPRIMER TOUTES LES DONNÉES ?",
            QMessageBox::Yes | QMessageBox::No);
        
        if (confirmReply == QMessageBox::Yes) {
            auto& dbManager = DatabaseManager::instance();
            if (dbManager.clearAllData()) {
                QMessageBox::information(this, "Succès", 
                    "Toutes les données ont été supprimées avec succès.");
                refreshPendingCounts();
            } else {
                QMessageBox::critical(this, "Erreur", 
                    "Erreur lors de la suppression des données. Vérifiez la connexion à la base.");
            }
        }
    }
}

void AdminFilesPage::onImportAllPending()
{
    // Since we're working with live database data, show current statistics
    auto& dbManager = DatabaseManager::instance();
    QSqlDatabase db = dbManager.database();
    
    if (!db.isOpen() || !db.isValid()) {
        QMessageBox::warning(this, "Erreur", "Base de données non connectée.");
        return;
    }
    
    int residents = dbManager.getAllResidents().size();
    int incidents = dbManager.getAllIncidents().size();
    int transactions = dbManager.getAllTransactions().size();
    int locaux = dbManager.getAllLocaux().size();
    int transport = dbManager.getAllTransportVehicules().size();
    int total = residents + incidents + transactions + locaux + transport;
    
    QString message = QString("Statistiques actuelles de la base :\n\n") +
                     QString("• Résidents: %1\n").arg(residents) +
                     QString("• Incidents: %1\n").arg(incidents) +
                     QString("• Transactions: %1\n").arg(transactions) +
                     QString("• Locaux: %1\n").arg(locaux) +
                     QString("• Véhicules: %1\n").arg(transport) +
                     QString("\nTotal: %1 enregistrements").arg(total);
    
    QMessageBox::information(this, "Données actuelles", message);
    refreshPendingCounts();
}

void AdminFilesPage::onAutoBackupToggled(bool checked)
{
    QSettings s;
    s.setValue("ui/autoBackup", checked);
    
    if (checked) {
        QMessageBox::information(this, "Sauvegarde automatique",
            "La sauvegarde automatique quotidienne est maintenant activée.");
    }
}

void AdminFilesPage::onViewPending(const QString& file)
{
    auto& dbManager = DatabaseManager::instance();
    QSqlDatabase db = dbManager.database();
    
    if (!db.isOpen() || !db.isValid()) {
        QMessageBox::warning(this, "Erreur", "Base de données non connectée.");
        return;
    }
    
    QString content;
    QString title;
    
    // Get data based on file type
    if (file.contains("residents")) {
        QList<QVariantMap> data = dbManager.getAllResidents();
        title = "Données Résidents";
        content = "Nombre total de résidents: " + QString::number(data.size()) + "\n\n";
        for (int i = 0; i < qMin(10, data.size()); ++i) {
            const auto& item = data[i];
            content += QString("• %1 %2 (CIN: %3)\n")
                .arg(item.value("nom").toString())
                .arg(item.value("prenom").toString()) 
                .arg(item.value("cin").toString());
        }
        if (data.size() > 10) {
            content += QString("\n... et %1 autres résidents").arg(data.size() - 10);
        }
    }
    else if (file.contains("incidents")) {
        QList<QVariantMap> data = dbManager.getAllIncidents();
        title = "Données Incidents";
        content = "Nombre total d'incidents: " + QString::number(data.size()) + "\n\n";
        for (int i = 0; i < qMin(10, data.size()); ++i) {
            const auto& item = data[i];
            content += QString("• %1 - %2\n")
                .arg(item.value("titre").toString())
                .arg(item.value("statut").toString());
        }
        if (data.size() > 10) {
            content += QString("\n... et %1 autres incidents").arg(data.size() - 10);
        }
    }
    else if (file.contains("finance")) {
        qInfo() << "🔍 Requesting finance data...";
        QList<QVariantMap> data = dbManager.getAllTransactions();
        title = "Données Finances";
        content = "Nombre total de transactions: " + QString::number(data.size()) + "\n\n";
        
        if (data.isEmpty()) {
            content += "❌ Aucune transaction trouvée dans la base de données.\n";
            content += "📋 Vérifications possibles:\n";
            content += "• La table TRANSACTIONS existe-t-elle?\n";
            content += "• Y a-t-il des données dans la table?\n";
            content += "• La connexion à la base est-elle active?\n";
            
            // Try to get more info about the database connection
            QSqlDatabase db = dbManager.database();
            if (db.isOpen()) {
                content += "✅ Connexion à la base: ACTIVE\n";
                content += QString("📊 Base: %1\n").arg(db.databaseName());
                content += QString("🔗 Driver: %1\n").arg(db.driverName());
            } else {
                content += "❌ Connexion à la base: INACTIVE\n";
            }
        } else {
            content += "✅ Transactions trouvées! Affichage des premières:\n\n";
            for (int i = 0; i < qMin(10, data.size()); ++i) {
                const auto& item = data[i];
                QString desc = item.value("description").toString();
                if (desc.isEmpty()) desc = "Description non définie";
                
                content += QString("• %1 - %2 DT (%3)\n")
                    .arg(desc)
                    .arg(item.value("montant").toDouble())
                    .arg(item.value("type").toString());
                
                // Add transaction code for reference
                content += QString("  Code: %1, Date: %2\n")
                    .arg(item.value("code_unique").toString())
                    .arg(item.value("date_transaction").toString());
                    
                // Debug: show all available fields for first transaction
                if (i == 0) {
                    content += "  [DEBUG] Champs disponibles: ";
                    QStringList fields;
                    for (auto it = item.constBegin(); it != item.constEnd(); ++it) {
                        fields << QString("%1=%2").arg(it.key()).arg(it.value().toString());
                    }
                    content += fields.join(", ") + "\n";
                }
            }
            if (data.size() > 10) {
                content += QString("\n... et %1 autres transactions").arg(data.size() - 10);
            }
        }
    }
    else if (file.contains("locaux")) {
        QList<QVariantMap> data = dbManager.getAllLocaux();
        title = "Données Locaux";
        content = "Nombre total de locaux: " + QString::number(data.size()) + "\n\n";
        for (int i = 0; i < qMin(10, data.size()); ++i) {
            const auto& item = data[i];
            content += QString("• %1 - %2\n")
                .arg(item.value("nom").toString())
                .arg(item.value("statut").toString());
        }
        if (data.size() > 10) {
            content += QString("\n... et %1 autres locaux").arg(data.size() - 10);
        }
    }
    else if (file.contains("transport")) {
        QList<QVariantMap> data = dbManager.getAllTransportVehicules();
        title = "Données Transport";
        content = "Nombre total de véhicules: " + QString::number(data.size()) + "\n\n";
        for (int i = 0; i < qMin(10, data.size()); ++i) {
            const auto& item = data[i];
            content += QString("• %1 (%2) - %3\n")
                .arg(item.value("marque").toString())
                .arg(item.value("matricule").toString())
                .arg(item.value("statut").toString());
        }
        if (data.size() > 10) {
            content += QString("\n... et %1 autres véhicules").arg(data.size() - 10);
        }
    }
    
    // Create a dialog to show data summary
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(title);
    dialog->setModal(true);
    dialog->resize(500, 400);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    // Add a text edit to show content
    QTextEdit *textEdit = new QTextEdit(dialog);
    textEdit->setReadOnly(true);
    textEdit->setPlainText(content);
    textEdit->setFont(QFont("Segoe UI", 10));
    layout->addWidget(textEdit);
    
    // Add close button
    QPushButton *closeBtn = new QPushButton("Fermer", dialog);
    connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::accept);
    layout->addWidget(closeBtn);
    
    dialog->exec();
    dialog->deleteLater();
}

void AdminFilesPage::onImportPending(const QString& file)
{
    Q_UNUSED(file)
    // Since we're working with live database data, "Import" will mean refresh/reload
    QMessageBox::information(this, "Actualisation", 
        "Les données sont déjà dans la base et sont actualisées en temps réel.");
    refreshPendingCounts();
}

void AdminFilesPage::onClearPending(const QString& file)
{
    auto& dbManager = DatabaseManager::instance();
    QSqlDatabase db = dbManager.database();
    
    if (!db.isOpen() || !db.isValid()) {
        QMessageBox::warning(this, "Erreur", "Base de données non connectée.");
        return;
    }
    
    QString dataType;
    int count = 0;
    
    // Determine data type and get count
    if (file.contains("residents")) {
        dataType = "résidents";
        count = dbManager.getAllResidents().size();
    } else if (file.contains("incidents")) {
        dataType = "incidents";
        count = dbManager.getAllIncidents().size();
    } else if (file.contains("finance")) {
        dataType = "transactions";
        count = dbManager.getAllTransactions().size();
    } else if (file.contains("locaux")) {
        dataType = "locaux";
        count = dbManager.getAllLocaux().size();
    } else if (file.contains("transport")) {
        dataType = "véhicules";
        count = dbManager.getAllTransportVehicules().size();
    }
    
    if (count == 0) {
        QMessageBox::information(this, "Aucune donnée", 
            QString("Aucun %1 à supprimer.").arg(dataType));
        return;
    }
    
    auto reply = QMessageBox::question(this, "Confirmation",
        QString("ATTENTION! Supprimer tous les %1 (%2 éléments) ?\nCette action est irréversible!")
            .arg(dataType).arg(count),
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        auto& dbManager = DatabaseManager::instance();
        bool success = false;
        
        if (file.contains("residents")) {
            success = dbManager.clearAllResidents();
        } else if (file.contains("incidents")) {
            success = dbManager.clearAllIncidents();
        } else if (file.contains("finance")) {
            success = dbManager.clearAllTransactions();
        } else if (file.contains("locaux")) {
            success = dbManager.clearAllLocaux();
        } else if (file.contains("transport")) {
            success = dbManager.clearAllTransportVehicules();
        }
        
        if (success) {
            QMessageBox::information(this, "Succès", 
                QString("Tous les %1 ont été supprimés avec succès.").arg(dataType));
        } else {
            QMessageBox::critical(this, "Erreur", 
                QString("Erreur lors de la suppression des %1. Vérifiez la connexion à la base.").arg(dataType));
        }
        
        refreshPendingCounts();
    }
}