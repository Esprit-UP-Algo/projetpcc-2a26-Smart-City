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
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QFont>
#include <QFileInfo>
#include <QDateTime>
#include <QDebug>
#include <QTableWidget>
#include <QHeaderView>
#include <QTextStream>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QDesktopServices>

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QLegend>



AdminFilesPage::AdminFilesPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AdminFilesPage)
{
    ui->setupUi(this);

    // KPI labels
    countResidents  = ui->countResidents;
    countIncidents  = ui->countIncidents;
    countFinance    = ui->countFinance;
    countLocaux     = ui->countLocaux;
    countTransport  = ui->countTransport;

    // Row buttons
    btnResidentsView    = ui->btnResidentsView;
    btnResidentsImport  = ui->btnResidentsImport;
    btnResidentsClear   = ui->btnResidentsClear;

    btnIncidentsView    = ui->btnIncidentsView;
    btnIncidentsImport  = ui->btnIncidentsImport;
    btnIncidentsClear   = ui->btnIncidentsClear;

    btnFinanceView      = ui->btnFinanceView;
    btnFinanceImport    = ui->btnFinanceImport;
    btnFinanceClear     = ui->btnFinanceClear;

    btnLocauxView       = ui->btnLocauxView;
    btnLocauxImport     = ui->btnLocauxImport;
    btnLocauxClear      = ui->btnLocauxClear;

    btnTransportView    = ui->btnTransportView;
    btnTransportImport  = ui->btnTransportImport;
    btnTransportClear   = ui->btnTransportClear;

    // Global actions
    btnBackup        = ui->btnBackup;
    btnRestore       = ui->btnRestore;
    btnPurge         = ui->btnPurge;
    btnImportAll     = ui->btnImportAll;
    chkAutoBackup    = ui->chkAutoBackup;

    connect(btnBackup,    &QPushButton::clicked, this, &AdminFilesPage::onBackupDatabase);
    connect(btnRestore,   &QPushButton::clicked, this, &AdminFilesPage::onRestoreDatabase);
    connect(btnPurge,     &QPushButton::clicked, this, &AdminFilesPage::onPurgeDatabase);
    connect(btnImportAll, &QPushButton::clicked, this, &AdminFilesPage::onImportAllPending);
    connect(chkAutoBackup,&QCheckBox::toggled,   this, &AdminFilesPage::onAutoBackupToggled);

    // Module row actions (clé "file" = type logique)
    auto connectRow = [&](QPushButton *view, QPushButton *import, QPushButton *clear, const QString &fileKey){
        connect(view,   &QPushButton::clicked, this, [this, fileKey]{ onViewPending(fileKey); });
        connect(import, &QPushButton::clicked, this, [this, fileKey]{ onImportPending(fileKey); });
        connect(clear,  &QPushButton::clicked, this, [this, fileKey]{ onClearPending(fileKey); });
    };

    connectRow(btnResidentsView,   btnResidentsImport,   btnResidentsClear,   QStringLiteral("residents"));
    connectRow(btnIncidentsView,   btnIncidentsImport,   btnIncidentsClear,   QStringLiteral("incidents"));
    connectRow(btnFinanceView,     btnFinanceImport,     btnFinanceClear,     QStringLiteral("finance"));
    connectRow(btnLocauxView,      btnLocauxImport,      btnLocauxClear,      QStringLiteral("locaux"));
    connectRow(btnTransportView,   btnTransportImport,   btnTransportClear,   QStringLiteral("transport"));

    // Auto-backup setting
    QSettings s;
    chkAutoBackup->setChecked(s.value("ui/autoBackup", false).toBool());

    // Refresh timer = live KPIs
    pendingRefreshTimer = new QTimer(this);
    pendingRefreshTimer->setInterval(5000); // 5s
    connect(pendingRefreshTimer, &QTimer::timeout, this, &AdminFilesPage::refreshPendingCounts);
    pendingRefreshTimer->start();

    refreshPendingCounts();
}

AdminFilesPage::~AdminFilesPage()
{
    delete ui;
}

/* ============================================================
 *  LIVE STATISTICS (KPI COUNTERS)
 * ============================================================*/

void AdminFilesPage::refreshPendingCounts()
{
    auto& dbManager = DatabaseManager::instance();
    QSqlDatabase db = dbManager.database();

    if (!db.isOpen() || !db.isValid()) {
        countResidents ->setText("0 enregistrements");
        countIncidents ->setText("0 enregistrements");
        countFinance   ->setText("0 enregistrements");
        countLocaux    ->setText("0 enregistrements");
        countTransport ->setText("0 enregistrements");
        return;
    }

    // Live data
    QList<QVariantMap> residents    = dbManager.getAllResidents();
    QList<QVariantMap> incidents    = dbManager.getAllIncidents();
    QList<QVariantMap> transactions = dbManager.getAllTransactions();
    QList<QVariantMap> locaux       = dbManager.getAllLocaux();
    QList<QVariantMap> transport    = dbManager.getAllTransportVehicules();

    const int rCount = residents.size();
    const int iCount = incidents.size();
    const int fCount = transactions.size();
    const int lCount = locaux.size();
    const int tCount = transport.size();

    countResidents ->setText(QString("%1 enregistrements").arg(rCount));
    countIncidents ->setText(QString("%1 enregistrements").arg(iCount));
    countFinance   ->setText(QString("%1 enregistrements").arg(fCount));
    countLocaux    ->setText(QString("%1 enregistrements").arg(lCount));
    countTransport ->setText(QString("%1 enregistrements").arg(tCount));

    qDebug() << "[AdminFilesPage] Stats:"
             << "Residents" << rCount
             << "Incidents" << iCount
             << "Finance"   << fCount
             << "Locaux"    << lCount
             << "Transport" << tCount;
}

/* ============================================================
 *  BACKUP / RESTORE / PURGE / STATS
 * ============================================================*/

void AdminFilesPage::onBackupDatabase()
{
    // Emplacement du backup
    QString fileName = QFileDialog::getSaveFileName(
        this,
        tr("Sauvegarder la base de données"),
        QDir::homePath() + "/nexora_backup.sql",
        tr("Fichiers SQL (*.sql);;Tous les fichiers (*.*)")
        );

    if (fileName.isEmpty())
        return;

    auto& dbManager = DatabaseManager::instance();
    QSqlDatabase db = dbManager.database();

    if (!db.isOpen() || !db.isValid()) {
        QMessageBox::warning(this, "Erreur", "Base de données non connectée. Sauvegarde impossible.");
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Erreur",
                              "Impossible de créer le fichier de sauvegarde.\nChemin : " + fileName);
        return;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);

    auto exportTable = [&](const QString &tableName, const QList<QVariantMap> &rows){
        if (rows.isEmpty())
            return;

        // Colonnes = clés du premier enregistrement
        QStringList columns = rows.first().keys();
        std::sort(columns.begin(), columns.end(), [](const QString &a, const QString &b){
            return a.toUpper() < b.toUpper();
        });

        out << "-- --------------------------------------------------\n";
        out << "-- TABLE : " << tableName << "\n";
        out << "-- NOMBRE D'ENREGISTREMENTS : " << rows.size() << "\n";
        out << "-- --------------------------------------------------\n\n";

        for (const QVariantMap &row : rows) {
            out << "INSERT INTO " << tableName << " (";
            out << columns.join(", ");
            out << ") VALUES (";

            QStringList values;
            for (const QString &col : columns) {
                QVariant v = row.value(col);
                if (v.isNull()) {
                    values << "NULL";
                } else if (v.typeId() == QMetaType::Int ||
                           v.typeId() == QMetaType::LongLong ||
                           v.typeId() == QMetaType::Double) {
                    values << v.toString();
                } else {
                    // Tout le reste = string, avec échappement des quotes
                    QString s = v.toString();
                    s.replace("'", "''");
                    values << "'" + s + "'";
                }
            }

            out << values.join(", ");
            out << ");\n";
        }

        out << "\n\n";
    };

    // Export logique des 5 modules
    exportTable("RESIDENTS",              dbManager.getAllResidents());
    exportTable("INCIDENTS",              dbManager.getAllIncidents());
    exportTable("TRANSACTIONS",           dbManager.getAllTransactions());
    exportTable("LOCAUX",                 dbManager.getAllLocaux());
    exportTable("TRANSPORT_VEHICULES",    dbManager.getAllTransportVehicules());

    file.close();

    // Sauvegarde de la date pour l'autre page (AdminDatabaseInfoPage)
    QSettings s;
    const QString now = QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm");
    s.setValue("db/lastBackup", now);

    QMessageBox::information(
        this,
        "Sauvegarde SQL créée",
        "Un script SQL de sauvegarde a été généré avec succès.\n\n"
        "📁 Fichier : " + fileName + "\n"
                         "🕒 Date   : " + now + "\n\n"
                    "Ce fichier contient des INSERT pour toutes les tables NEXORA."
        );
}

void AdminFilesPage::onRestoreDatabase()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Restaurer la base de données"),
        QDir::homePath(),
        tr("Fichiers SQL (*.sql);;Tous les fichiers (*.*)")
        );

    if (fileName.isEmpty())
        return;

    auto reply = QMessageBox::question(
        this,
        "Restauration depuis script SQL",
        "Ce script SQL devra être exécuté dans Oracle (SQL*Plus ou autre outil).\n\n"
        "Voulez-vous ouvrir le dossier contenant le script ?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(fileName).absolutePath()));
    }
}

void AdminFilesPage::onPurgeDatabase()
{
    auto reply = QMessageBox::question(
        this,
        "Vider la base",
        "ATTENTION : cette action supprimera toutes les données (résidents, incidents,\n"
        "finances, locaux, véhicules) de la base NEXORA.\n\n"
        "Voulez-vous continuer ?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply != QMessageBox::Yes)
        return;

    auto confirmReply = QMessageBox::question(
        this,
        "Confirmation finale",
        "Dernière chance ! Voulez-vous vraiment SUPPRIMER TOUTES LES DONNÉES ?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (confirmReply != QMessageBox::Yes)
        return;

    auto& dbManager = DatabaseManager::instance();
    if (dbManager.clearAllData()) {
        QMessageBox::information(this, "Succès",
                                 "Toutes les données ont été supprimées avec succès.");
        refreshPendingCounts();
    } else {
        QMessageBox::critical(this, "Erreur",
                              "Erreur lors de la suppression des données. "
                              "Vérifiez la connexion à la base.");
    }
}

/*
 * BOUTON "VOIR LES STATISTIQUES"
 * -> Ouvre un vrai dashboard avec PIE + BAR + COURBE
 */
void AdminFilesPage::onImportAllPending()
{
    auto& dbManager = DatabaseManager::instance();
    QSqlDatabase db = dbManager.database();

    if (!db.isOpen() || !db.isValid()) {
        QMessageBox::warning(this, "Erreur", "Base de données non connectée.");
        return;
    }

    int residents     = dbManager.getAllResidents().size();
    int incidents     = dbManager.getAllIncidents().size();
    int transactions  = dbManager.getAllTransactions().size();
    int locaux        = dbManager.getAllLocaux().size();
    int transport     = dbManager.getAllTransportVehicules().size();
    int total         = residents + incidents + transactions + locaux + transport;

    // ======= NEXORA STATISTICS DIALOG =======
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("📊 Statistiques NEXORA - Administration");
    dialog->resize(1000, 700);
    dialog->setModal(true);
    
    // Modern Nexora styling
    dialog->setStyleSheet(
        "QDialog {"
            "background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, "
                "stop:0 #E8F1FF, stop:1 #F5F9FF);"
            "border: 2px solid #1F2D5C;"
            "border-radius: 15px;"
        "}"
        "QLabel {"
            "background: transparent;"
        "}");

    // Effet de fade-in (premium)
    auto *effect = new QGraphicsOpacityEffect(dialog);
    dialog->setGraphicsEffect(effect);
    auto *anim = new QPropertyAnimation(effect, "opacity", dialog);
    anim->setDuration(250);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->start(QAbstractAnimation::DeleteWhenStopped);

    QVBoxLayout *mainLayout = new QVBoxLayout(dialog);

    QLabel *title = new QLabel("📈 Analytics Dashboard - Vue Analytique NEXORA");
    title->setStyleSheet(
        "QLabel {"
            "font-size: 24px;"
            "font-weight: 800;"
            "color: #1F2D5C;"
            "padding: 15px;"
            "background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, "
                "stop:0 #FFFFFF, stop:1 #F0F4FF);"
            "border: 2px solid #E3ECFF;"
            "border-radius: 12px;"
            "margin-bottom: 10px;"
        "}");
    mainLayout->addWidget(title);

    QLabel *sub = new QLabel(
        QString("🏢 Total Système : %1 enregistrements • Modules: Résidents, Incidents, Finances, Locaux, Transport")
            .arg(total));
    sub->setStyleSheet(
        "QLabel {"
            "font-size: 14px;"
            "color: #34495E;"
            "padding: 8px 15px;"
            "background: #F8FBFF;"
            "border: 1px solid #E8F1FF;"
            "border-radius: 8px;"
            "font-weight: 500;"
        "}");
    sub->setWordWrap(true);
    mainLayout->addWidget(sub);

    // Layout horizontal pour les charts
    QHBoxLayout *chartsLayout = new QHBoxLayout();
    mainLayout->addLayout(chartsLayout);

    // ---------------- PIE CHART (répartition)
    {
        QPieSeries *series = new QPieSeries();
        if (residents    > 0) series->append("Résidents",    residents);
        if (incidents    > 0) series->append("Incidents",    incidents);
        if (transactions > 0) series->append("Finances",     transactions);
        if (locaux       > 0) series->append("Locaux",       locaux);
        if (transport    > 0) series->append("Transport",    transport);

        for (auto slice : series->slices()) {
            slice->setLabel(QString("%1 (%2)").arg(slice->label()).arg(slice->value()));
            slice->setLabelVisible(true);
        }

        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Répartition par module");
        chart->legend()->setAlignment(Qt::AlignBottom);

        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->setMinimumSize(420, 320);
        chartsLayout->addWidget(chartView);
    }

    // ---------------- BAR CHART (barres)
    {
        QBarSet *set = new QBarSet("Enregistrements");
        *set << residents << incidents << transactions << locaux << transport;

        QBarSeries *series = new QBarSeries();
        series->append(set);

        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Nombre d'enregistrements par module");
        chart->setAnimationOptions(QChart::SeriesAnimations);

        QStringList categories;
        categories << "Résidents" << "Incidents" << "Finances" << "Locaux" << "Transport";

        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append(categories);
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        QValueAxis *axisY = new QValueAxis();
        axisY->setLabelFormat("%d");
        axisY->setTitleText("Enregistrements");
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);

        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->setMinimumSize(420, 320);
        chartsLayout->addWidget(chartView);
    }

    // ---------------- LINE CHART (courbe synthétique)
    {
        QLineSeries *series = new QLineSeries();
        series->setName("Distribution globale");

        // x = index du module, y = nb d'enregistrements
        series->append(0, residents);
        series->append(1, incidents);
        series->append(2, transactions);
        series->append(3, locaux);
        series->append(4, transport);

        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Courbe synthétique des enregistrements");
        chart->createDefaultAxes();
        chart->axisX()->setTitleText("Module (0=Résidents, 1=Incidents, 2=Finances, 3=Locaux, 4=Transport)");
        chart->axisY()->setTitleText("Nombre d'enregistrements");
        chart->setAnimationOptions(QChart::SeriesAnimations);

        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->setMinimumHeight(220);

        mainLayout->addWidget(chartView);
    }

    // Bouton fermer
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    QPushButton *closeBtn = new QPushButton("✨ Fermer Dashboard");
    closeBtn->setMinimumWidth(160);
    closeBtn->setMinimumHeight(40);
    closeBtn->setStyleSheet(
        "QPushButton {"
            "background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, "
                "stop:0 #1F2D5C, stop:1 #2A3F6F);"
            "color: white;"
            "border: none;"
            "border-radius: 12px;"
            "padding: 10px 20px;"
            "font-weight: 700;"
            "font-size: 14px;"
        "}"
        "QPushButton:hover {"
            "background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, "
                "stop:0 #2A3F6F, stop:1 #34527A);"
            "transform: translateY(-2px);"
        "}"
        "QPushButton:pressed {"
            "background: #1A2650;"
        "}");
    btnLayout->addWidget(closeBtn);
    mainLayout->addLayout(btnLayout);

    connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::accept);

    dialog->exec();
    dialog->deleteLater();

    // Remet les KPIs à jour
    refreshPendingCounts();
}

/* ============================================================
 *  AUTO-BACKUP OPTION
 * ============================================================*/

void AdminFilesPage::onAutoBackupToggled(bool checked)
{
    QSettings s;
    s.setValue("ui/autoBackup", checked);

    if (checked) {
        QMessageBox::information(
            this,
            "Sauvegarde automatique",
            "La sauvegarde automatique quotidienne est maintenant activée.\n"
            "Dans un vrai déploiement, un service planifié lancerait les sauvegardes."
            );
    }
}

/* ============================================================
 *  MODULE ACTIONS : VOIR / ACTUALISER / VIDER
 * ============================================================*/

void AdminFilesPage::onViewPending(const QString& file)
{
    auto& dbManager = DatabaseManager::instance();
    QSqlDatabase db = dbManager.database();

    if (!db.isOpen() || !db.isValid()) {
        QMessageBox::warning(this, "Erreur", "Base de données non connectée.");
        return;
    }

    QList<QVariantMap> data;
    QString title;

    if (file == "residents") {
        data = dbManager.getAllResidents();
        title = "Résidents";
    } else if (file == "incidents") {
        data = dbManager.getAllIncidents();
        title = "Incidents";
    } else if (file == "finance") {
        data = dbManager.getAllTransactions();
        title = "Transactions financières";
    } else if (file == "locaux") {
        data = dbManager.getAllLocaux();
        title = "Locaux";
    } else if (file == "transport") {
        data = dbManager.getAllTransportVehicules();
        title = "Véhicules de transport";
    }

    // ======= NEXORA DATA PREVIEW DIALOG =======
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("🔍 NEXORA Data Preview - " + title);
    dialog->resize(1000, 650);
    dialog->setModal(true);
    
    // Modern Nexora styling
    dialog->setStyleSheet(
        "QDialog {"
            "background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, "
                "stop:0 #F8FBFF, stop:1 #FFFFFF);"
            "border: 2px solid #1F2D5C;"
            "border-radius: 15px;"
        "}"
        "QLabel {"
            "background: transparent;"
        "}");

    auto *effect = new QGraphicsOpacityEffect(dialog);
    dialog->setGraphicsEffect(effect);
    auto *anim = new QPropertyAnimation(effect, "opacity", dialog);
    anim->setDuration(220);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->start(QAbstractAnimation::DeleteWhenStopped);

    QVBoxLayout *layout = new QVBoxLayout(dialog);

    QLabel *header = new QLabel(QString("📋 Module %1 • %2 enregistrements")
                                    .arg(title)
                                    .arg(data.size()));
    header->setStyleSheet(
        "QLabel {"
            "font-size: 20px;"
            "font-weight: 800;"
            "color: #1F2D5C;"
            "padding: 12px 20px;"
            "background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, "
                "stop:0 #FFFFFF, stop:1 #F0F6FF);"
            "border: 2px solid #E3ECFF;"
            "border-radius: 10px;"
            "margin-bottom: 15px;"
        "}");
    layout->addWidget(header);

    if (data.isEmpty()) {
        QLabel *empty = new QLabel("📭 Aucune donnée disponible pour ce module NEXORA.");
        empty->setStyleSheet(
            "QLabel {"
                "font-size: 16px;"
                "color: #7F8C8D;"
                "padding: 30px;"
                "background: #F8FBFF;"
                "border: 2px dashed #E8F1FF;"
                "border-radius: 12px;"
                "text-align: center;"
                "font-style: italic;"
            "}");
        empty->setAlignment(Qt::AlignCenter);
        layout->addWidget(empty);
    } else {
        // Colonnes = clés du premier enregistrement
        QStringList columns = data.first().keys();
        std::sort(columns.begin(), columns.end(), [](const QString &a, const QString &b){
            return a.toUpper() < b.toUpper();
        });

        QTableWidget *table = new QTableWidget(dialog);
        table->setColumnCount(columns.size());
        table->setRowCount(qMin(data.size(), 100)); // limite à 100 pour l'aperçu
        table->setHorizontalHeaderLabels(columns);
        table->setAlternatingRowColors(true);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->horizontalHeader()->setStretchLastSection(true);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
        table->setStyleSheet(
            "QTableWidget {"
                "font-size: 13px;"
                "background-color: white;"
                "border: 2px solid #E8F1FF;"
                "border-radius: 8px;"
                "gridline-color: #F0F6FF;"
            "}"
            "QHeaderView::section {"
                "background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, "
                    "stop:0 #1F2D5C, stop:1 #2A3F6F);"
                "color: white;"
                "padding: 8px 12px;"
                "border: 1px solid #34527A;"
                "font-weight: 700;"
                "font-size: 12px;"
            "}"
            "QTableWidget::item {"
                "padding: 6px 8px;"
                "border-bottom: 1px solid #F0F6FF;"
            "}"
            "QTableWidget::item:selected {"
                "background-color: #E3ECFF;"
                "color: #1F2D5C;"
            "}"
            "QTableWidget::item:alternate {"
                "background-color: #F8FBFF;"
            "}");

        for (int row = 0; row < table->rowCount(); ++row) {
            const QVariantMap &m = data[row];
            for (int col = 0; col < columns.size(); ++col) {
                const QString &key = columns[col];
                QTableWidgetItem *item = new QTableWidgetItem(m.value(key).toString());
                table->setItem(row, col, item);
            }
        }

        layout->addWidget(table);

        // Bas : export CSV + fermer
        QHBoxLayout *btnLayout = new QHBoxLayout();
        QPushButton *exportBtn = new QPushButton("📁 Exporter CSV");
        QPushButton *closeBtn  = new QPushButton("✨ Fermer");
        
        exportBtn->setMinimumHeight(40);
        closeBtn->setMinimumHeight(40);

        exportBtn->setStyleSheet(
            "QPushButton {"
                "background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, "
                    "stop:0 #27AE60, stop:1 #229954);"
                "color: white;"
                "border: none;"
                "border-radius: 10px;"
                "padding: 8px 16px;"
                "font-weight: 700;"
                "font-size: 13px;"
            "}"
            "QPushButton:hover {"
                "background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, "
                    "stop:0 #2ECC71, stop:1 #27AE60);"
                "transform: translateY(-1px);"
            "}");
        closeBtn->setStyleSheet(
            "QPushButton {"
                "background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, "
                    "stop:0 #1F2D5C, stop:1 #2A3F6F);"
                "color: white;"
                "border: none;"
                "border-radius: 10px;"
                "padding: 8px 16px;"
                "font-weight: 700;"
                "font-size: 13px;"
            "}"
            "QPushButton:hover {"
                "background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, "
                    "stop:0 #2A3F6F, stop:1 #34527A);"
                "transform: translateY(-1px);"
            "}");

        btnLayout->addWidget(exportBtn);
        btnLayout->addStretch();
        btnLayout->addWidget(closeBtn);
        layout->addLayout(btnLayout);

        // Export CSV
        connect(exportBtn, &QPushButton::clicked, dialog, [dialog, title, table]() {
            QString fileName = QFileDialog::getSaveFileName(
                dialog,
                "Exporter " + title + " en CSV",
                QDir::homePath() + "/" + title.toLower().replace(" ", "_") + ".csv",
                "Fichiers CSV (*.csv);;Tous les fichiers (*.*)"
                );
            if (fileName.isEmpty())
                return;

            QFile file(fileName);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QMessageBox::critical(dialog, "Erreur", "Impossible de créer le fichier CSV.");
                return;
            }

            QTextStream out(&file);
            out.setEncoding(QStringConverter::Utf8);

            // Entêtes
            QStringList headers;
            for (int col = 0; col < table->columnCount(); ++col) {
                headers << table->horizontalHeaderItem(col)->text();
            }
            out << headers.join(";") << "\n";

            // Lignes
            for (int row = 0; row < table->rowCount(); ++row) {
                QStringList rowData;
                for (int col = 0; col < table->columnCount(); ++col) {
                    QTableWidgetItem *item = table->item(row, col);
                    rowData << (item ? item->text().replace(";", ",") : "");
                }
                out << rowData.join(";") << "\n";
            }

            file.close();
            QMessageBox::information(dialog, "Export terminé",
                                     "Les données ont été exportées en CSV avec succès.");
        });

        connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::accept);
    }

    dialog->exec();
    dialog->deleteLater();
}

void AdminFilesPage::onImportPending(const QString& file)
{
    Q_UNUSED(file)
    QMessageBox::information(
        this,
        "Actualisation",
        "Les données sont déjà dans la base et les indicateurs sont rafraîchis automatiquement."
        );
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

    if (file == "residents") {
        dataType = "résidents";
        count = dbManager.getAllResidents().size();
    } else if (file == "incidents") {
        dataType = "incidents";
        count = dbManager.getAllIncidents().size();
    } else if (file == "finance") {
        dataType = "transactions";
        count = dbManager.getAllTransactions().size();
    } else if (file == "locaux") {
        dataType = "locaux";
        count = dbManager.getAllLocaux().size();
    } else if (file == "transport") {
        dataType = "véhicules";
        count = dbManager.getAllTransportVehicules().size();
    }

    if (count == 0) {
        QMessageBox::information(this, "Aucune donnée",
                                 QString("Aucun %1 à supprimer.").arg(dataType));
        return;
    }

    auto reply = QMessageBox::question(
        this,
        "Confirmation",
        QString("ATTENTION ! Supprimer tous les %1 (%2 éléments) ?\n"
                "Cette action est irréversible !")
            .arg(dataType).arg(count),
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply != QMessageBox::Yes)
        return;

    bool success = false;

    if (file == "residents") {
        success = dbManager.clearAllResidents();
    } else if (file == "incidents") {
        success = dbManager.clearAllIncidents();
    } else if (file == "finance") {
        success = dbManager.clearAllTransactions();
    } else if (file == "locaux") {
        success = dbManager.clearAllLocaux();
    } else if (file == "transport") {
        success = dbManager.clearAllTransportVehicules();
    }

    if (success) {
        QMessageBox::information(
            this,
            "Succès",
            QString("Tous les %1 ont été supprimés avec succès.").arg(dataType)
            );
    } else {
        QMessageBox::critical(
            this,
            "Erreur",
            QString("Erreur lors de la suppression des %1. Vérifiez la connexion à la base.")
                .arg(dataType)
            );
    }

    refreshPendingCounts();
}
