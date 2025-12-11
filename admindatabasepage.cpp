#include "admindatabasepage.h"
#include "ui_admindatabasepage.h"

#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QStringConverter>
#include <QDateTime>

static QString tableResidents() { return QStringLiteral("RESIDENTS"); }
static QString tableIncidents() { return QStringLiteral("INCIDENTS"); }
static QString tableFinances() { return QStringLiteral("FINANCES"); }
static QString tableLocaux() { return QStringLiteral("LOCAUX"); }
static QString tableTransport() { return QStringLiteral("TRANSPORT_VEHICULES"); }

AdminDatabasePage::AdminDatabasePage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AdminDatabasePage)
{
    ui->setupUi(this);

    // Map UI widgets to members
    lblType = ui->labelType;
    lblDsn = ui->labelDsn;
    lblStatus = ui->labelStatus;
    lblUser = ui->labelUser;
    lblSize = ui->labelSize;
    lblLastBackup = ui->valueBackup;

    lblResidentsCount = ui->countResidents;
    lblIncidentsCount = ui->countIncidents;
    lblFinanceCount = ui->countFinance;
    lblLocauxCount = ui->countLocaux;
    lblTransportCount = ui->countTransport;

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

    btnBackup = ui->btnBackup;
    btnRestore = ui->btnRestore;
    btnPurge = ui->btnPurge;
    btnImportAll = ui->btnImportAll;
    chkAutoBackup = ui->chkAutoBackup;

    // Connections
    connect(btnBackup, &QPushButton::clicked, this, &AdminDatabasePage::onBackupDatabase);
    connect(btnRestore, &QPushButton::clicked, this, &AdminDatabasePage::onRestoreDatabase);
    connect(btnPurge, &QPushButton::clicked, this, &AdminDatabasePage::onPurgeDatabase);
    connect(btnImportAll, &QPushButton::clicked, this, &AdminDatabasePage::onImportAllPending);
    connect(chkAutoBackup, &QCheckBox::toggled, this, &AdminDatabasePage::onAutoBackupToggled);

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

    infoRefreshTimer = new QTimer(this);
    infoRefreshTimer->setInterval(5000);
    connect(infoRefreshTimer, &QTimer::timeout, this, &AdminDatabasePage::refreshInfo);
    infoRefreshTimer->start();

    refreshInfo();
    refreshPendingCounts();
}

AdminDatabasePage::~AdminDatabasePage() { 
    delete ui; 
}

void AdminDatabasePage::applyTheme() {
    // Theme is applied via global ThemeManager
}

void AdminDatabasePage::reloadTranslations() {
    refreshInfo();
    refreshPendingCounts();
}

QSqlDatabase AdminDatabasePage::db() const {
    return QSqlDatabase::database();
}

QString AdminDatabasePage::pendingDirPath() const {
    return QDir::currentPath() + "/pending";
}

QString AdminDatabasePage::pendingFilePath(const QString &name) const {
    return pendingDirPath() + "/" + name;
}

bool AdminDatabasePage::ensurePendingFiles() {
    QDir dir(pendingDirPath());
    if (!dir.exists()) {
        if (!QDir().mkpath(dir.absolutePath())) return false;
    }
    const QStringList files = {
        "residents_pending.json",
        "incidents_pending.json",
        "finance_pending.json",
        "locaux_pending.json",
        "transport_pending.json"
    };
    for (const QString &f : files) {
        const QString path = pendingFilePath(f);
        if (!QFile::exists(path)) {
            QFile file(path);
            if (file.open(QIODevice::WriteOnly)) {
                QJsonArray empty;
                file.write(QJsonDocument(empty).toJson(QJsonDocument::Compact));
                file.close();
            }
        }
    }
    return true;
}

int AdminDatabasePage::countJsonItems(const QString &filePath) const {
    QFile f(filePath);
    if (!f.open(QIODevice::ReadOnly)) return 0;
    const auto doc = QJsonDocument::fromJson(f.readAll());
    if (!doc.isArray()) return 0;
    return doc.array().size();
}

void AdminDatabasePage::refreshPendingCounts() {
    lblResidentsCount->setText(tr("(%1 éléments)").arg(countJsonItems(pendingFilePath("residents_pending.json"))));
    lblIncidentsCount->setText(tr("(%1 éléments)").arg(countJsonItems(pendingFilePath("incidents_pending.json"))));
    lblFinanceCount->setText(tr("(%1 éléments)").arg(countJsonItems(pendingFilePath("finance_pending.json"))));
    lblLocauxCount->setText(tr("(%1 éléments)").arg(countJsonItems(pendingFilePath("locaux_pending.json"))));
    lblTransportCount->setText(tr("(%1 éléments)").arg(countJsonItems(pendingFilePath("transport_pending.json"))));
}

void AdminDatabasePage::refreshInfo() {
    auto database = db();
    const bool ok = database.isOpen();
    lblStatus->setText(ok ? tr("Connecté") : tr("Non connecté"));
    lblUser->setText(database.userName());

    // Size via user_segments
    if (ok) {
        QSqlQuery q(database);
        if (q.prepare("SELECT SUM(bytes)/1024/1024 FROM user_segments")) {
            if (q.exec() && q.next()) {
                const double mb = q.value(0).toDouble();
                lblSize->setText(QString::number(mb, 'f', 2) + " MB");
            } else {
                lblSize->setText(tr("N/A"));
            }
        }
    } else {
        lblSize->setText(tr("N/A"));
    }

    QSettings s;
    const QString last = s.value("db/lastBackup", QString()).toString();
    lblLastBackup->setText(last.isEmpty() ? tr("Aucune") : last);
}

void AdminDatabasePage::onBackupDatabase() {
    const QString file = QFileDialog::getSaveFileName(this, tr("Sauvegarder en .sql"), QDir::currentPath()+"/backup.sql", tr("SQL (*.sql)"));
    if (file.isEmpty()) return;
    const bool ok = dumpDatabaseToSql(file);
    if (ok) {
        QSettings s; s.setValue("db/lastBackup", QDateTime::currentDateTime().toString(Qt::ISODate));
        showInfo(tr("Sauvegarde réussie: %1").arg(file));
        refreshInfo();
    } else {
        showWarning(tr("Échec de la sauvegarde"));
    }
}

void AdminDatabasePage::onRestoreDatabase() {
    const QString file = QFileDialog::getOpenFileName(this, tr("Restorer depuis .sql"), QDir::currentPath(), tr("SQL (*.sql)"));
    if (file.isEmpty()) return;
    if (restoreDatabaseFromSql(file)) {
        showInfo(tr("Restauration réussie"));
        refreshPendingCounts();
    } else {
        showWarning(tr("Échec de la restauration"));
    }
}

void AdminDatabasePage::onPurgeDatabase() {
    if (QMessageBox::question(this, tr("Confirmer"), tr("Vider toutes les tables ?")) != QMessageBox::Yes)
        return;
    if (truncateAllTables()) {
        showInfo(tr("Base vidée."));
    } else {
        showWarning(tr("Échec du vidage."));
    }
}

void AdminDatabasePage::onImportAllPending() {
    const QStringList files = {
        "residents_pending.json",
        "incidents_pending.json",
        "finance_pending.json",
        "locaux_pending.json",
        "transport_pending.json"
    };
    int okCount = 0;
    for (const QString &f : files) {
        if (insertDataIntoOracle(pendingFilePath(f))) okCount++;
    }
    refreshPendingCounts();
    showInfo(tr("Import terminé: %1/%2 fichiers").arg(okCount).arg(files.size()));
}

void AdminDatabasePage::onViewPending(const QString &fileName) {
    const QString path = pendingFilePath(fileName);
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) { showWarning(tr("Impossible d'ouvrir %1").arg(fileName)); return; }
    const QByteArray data = f.readAll(); f.close();
    QMessageBox::information(this, tr("Contenu de %1").arg(fileName), QString::fromUtf8(data).left(3000));
}

void AdminDatabasePage::onImportPending(const QString &fileName) {
    if (insertDataIntoOracle(pendingFilePath(fileName))) {
        refreshPendingCounts();
        showInfo(tr("Import de %1 réussi").arg(fileName));
    }
}

void AdminDatabasePage::onClearPending(const QString &fileName) {
    const QString path = pendingFilePath(fileName);
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate)) { showWarning(tr("Impossible de vider %1").arg(fileName)); return; }
    QJsonArray empty;
    f.write(QJsonDocument(empty).toJson(QJsonDocument::Compact));
    f.close();
    refreshPendingCounts();
    showInfo(tr("%1 vidé").arg(fileName));
}

void AdminDatabasePage::onAutoBackupToggled(bool enabled) {
    QSettings s; s.setValue("ui/autoBackup", enabled);
}

void AdminDatabasePage::showSqlError(const QString &context, const QSqlError &err) {
    QMessageBox::critical(this, tr("Erreur SQL"), context + "\n" + err.text());
}

void AdminDatabasePage::showInfo(const QString &message) { 
    QMessageBox::information(this, tr("Info"), message); 
}

void AdminDatabasePage::showWarning(const QString &message) { 
    QMessageBox::warning(this, tr("Attention"), message); 
}

bool AdminDatabasePage::insertDataIntoOracle(const QString &filePath) {
    QFile f(filePath);
    if (!f.open(QIODevice::ReadOnly)) { showWarning(tr("Lecture échouée: %1").arg(filePath)); return false; }
    const auto doc = QJsonDocument::fromJson(f.readAll());
    f.close();
    if (!doc.isArray()) { showWarning(tr("Format JSON invalide")); return false; }
    const auto arr = doc.array();

    auto database = db();
    if (!database.isOpen()) { showWarning(tr("Base non connectée")); return false; }

    QString table;
    if (filePath.contains("residents_")) table = tableResidents();
    else if (filePath.contains("incidents_")) table = tableIncidents();
    else if (filePath.contains("finance_")) table = tableFinances();
    else if (filePath.contains("locaux_")) table = tableLocaux();
    else if (filePath.contains("transport_")) table = tableTransport();
    else { showWarning(tr("Table inconnue")); return false; }

    QSqlQuery q(database);
    database.transaction();
    for (const auto &v : arr) {
        if (!v.isObject()) continue;
        const QJsonObject o = v.toObject();
        // Generic insert using key/value pairs
        QStringList cols, binds;
        for (auto it = o.begin(); it != o.end(); ++it) {
            cols << it.key();
            binds << ":" + it.key();
        }
        const QString sql = QString("INSERT INTO %1 (%2) VALUES (%3)")
                                .arg(table)
                                .arg(cols.join(","))
                                .arg(binds.join(","));
        if (!q.prepare(sql)) { database.rollback(); showSqlError(tr("Préparation"), q.lastError()); return false; }
        for (auto it = o.begin(); it != o.end(); ++it) {
            q.bindValue(":" + it.key(), it.value().toVariant());
        }
        if (!q.exec()) { database.rollback(); showSqlError(tr("Insertion"), q.lastError()); return false; }
    }
    if (!database.commit()) { showSqlError(tr("Commit"), database.lastError()); return false; }

    // Clear file after successful import
    QFile out(filePath);
    if (out.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        out.write(QJsonDocument(QJsonArray()).toJson(QJsonDocument::Compact));
        out.close();
    }
    return true;
}

bool AdminDatabasePage::dumpDatabaseToSql(const QString &filePath) {
    auto database = db();
    if (!database.isOpen()) { showWarning(tr("Base non connectée")); return false; }
    QFile f(filePath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate)) return false;
    QTextStream out(&f);
    out.setEncoding(QStringConverter::Utf8);

    const QStringList tables = { tableResidents(), tableIncidents(), tableFinances(), tableLocaux(), tableTransport() };
    for (const QString &t : tables) {
        QSqlQuery q(database);
        if (!q.exec("SELECT * FROM " + t)) { showSqlError(tr("Lecture table ") + t, q.lastError()); f.close(); return false; }
        const int cols = q.record().count();
        while (q.next()) {
            QStringList values;
            for (int i=0;i<cols;++i) {
                const QVariant v = q.value(i);
                if (v.isNull()) values << "NULL";
                else values << QString("'%1'").arg(QString(v.toString()).replace("'", "''"));
            }
            // Simple insert dump
            out << "INSERT INTO " << t << " VALUES (" << values.join(",") << ");\n";
        }
    }
    f.close();
    return true;
}

bool AdminDatabasePage::restoreDatabaseFromSql(const QString &filePath) {
    auto database = db();
    if (!database.isOpen()) { showWarning(tr("Base non connectée")); return false; }
    QFile f(filePath);
    if (!f.open(QIODevice::ReadOnly)) return false;
    QTextStream in(&f);
    in.setEncoding(QStringConverter::Utf8);
    QString sql;
    QSqlQuery q(database);
    database.transaction();
    while (!in.atEnd()) {
        sql += in.readLine() + "\n";
        if (sql.trimmed().endsWith(';')) {
            if (!q.exec(sql)) { database.rollback(); showSqlError(tr("Exécution"), q.lastError()); f.close(); return false; }
            sql.clear();
        }
    }
    f.close();
    if (!database.commit()) { showSqlError(tr("Commit"), database.lastError()); return false; }
    return true;
}

bool AdminDatabasePage::truncateAllTables() {
    auto database = db();
    if (!database.isOpen()) { showWarning(tr("Base non connectée")); return false; }
    QSqlQuery q(database);
    const QStringList tables = { tableResidents(), tableIncidents(), tableFinances(), tableLocaux(), tableTransport() };
    database.transaction();
    for (const QString &t : tables) {
        if (!q.exec("TRUNCATE TABLE " + t)) { database.rollback(); showSqlError(tr("TRONQUER ") + t, q.lastError()); return false; }
    }
    if (!database.commit()) { showSqlError(tr("Commit"), database.lastError()); return false; }
    return true;
}