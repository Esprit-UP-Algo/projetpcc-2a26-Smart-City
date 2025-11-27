#include "admindbinfopage.h"
#include "ui_admindbinfopage.h"
#include "databasemanager.h"
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlError>
#include <QSettings>

AdminDatabaseInfoPage::AdminDatabaseInfoPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AdminDatabaseInfoPage)
{
    ui->setupUi(this);

    // Map UI widgets to members
    labelType = ui->labelType;
    labelDsn = ui->labelDsn;
    labelStatus = ui->labelStatus;
    labelUser = ui->labelUser;
    labelSize = ui->labelSize;
    valueBackup = ui->valueBackup;

    // Map counter labels (old style)
    countResidents = ui->countResidents;
    countIncidents = ui->countIncidents;
    countFinance = ui->countFinance;
    countLocaux = ui->countLocaux;
    countTransport = ui->countTransport;
    countTotal = ui->countTotal;

    // Setup refresh timer
    infoRefreshTimer = new QTimer(this);
    infoRefreshTimer->setInterval(5000);
    connect(infoRefreshTimer, &QTimer::timeout, this, &AdminDatabaseInfoPage::refreshInfo);
    infoRefreshTimer->start();

    // Initial refresh
    refreshInfo();
}

AdminDatabaseInfoPage::~AdminDatabaseInfoPage()
{
    delete ui;
}

void AdminDatabaseInfoPage::refreshInfo()
{
    updateDatabaseInfo();
    updateStatistics();
}

void AdminDatabaseInfoPage::updateDatabaseInfo()
{
    auto& dbManager = DatabaseManager::instance();
    QSqlDatabase db = dbManager.database();
    
    // Update connection status
    if (db.isOpen() && db.isValid()) {
        labelStatus->setText("Connecté");
        labelStatus->setStyleSheet(
            "font-size: 16px; font-weight: bold; color: #27AE60; background: transparent;"
        );
    } else {
        labelStatus->setText("Déconnecté");
        labelStatus->setStyleSheet(
            "font-size: 16px; font-weight: bold; color: #E74C3C; background: transparent;"
        );
    }

    // User and DSN details
    labelUser->setText(db.userName());
    const QString dsn = db.hostName().isEmpty()
        ? (db.databaseName().isEmpty() ? db.connectionName() : db.databaseName())
        : QString("%1:%2/%3").arg(db.hostName()).arg(db.port()).arg(db.databaseName());
    labelDsn->setText(dsn);

    // Update database size (Oracle user_segments if available)
    if (db.isOpen() && db.isValid()) {
        QSqlQuery q(db);
        if (q.exec("SELECT NVL(SUM(bytes),0)/1024/1024 FROM user_segments")) {
            if (q.next()) {
                const double mb = q.value(0).toDouble();
                labelSize->setText(QString::number(mb, 'f', 2) + " MB");
            } else {
                labelSize->setText("N/A");
            }
        } else {
            // Fallback when not permitted/available
            labelSize->setText("N/A");
        }
    } else {
        labelSize->setText("N/A");
    }

    // Update last backup time from settings (shared with AdminDatabasePage)
    QSettings s;
    const QString lastBackup = s.value("db/lastBackup").toString();
    if (lastBackup.isEmpty()) {
        valueBackup->setText("Aucune");
        valueBackup->setStyleSheet("font-size: 16px; font-weight: bold; color: #E74C3C; background: transparent;");
    } else {
        valueBackup->setText(lastBackup);
        valueBackup->setStyleSheet("font-size: 16px; font-weight: bold; color: #27AE60; background: transparent;");
    }
}

void AdminDatabaseInfoPage::updateStatistics()
{
    auto& dbManager = DatabaseManager::instance();
    QSqlDatabase db = dbManager.database();

    if (!db.isOpen() || !db.isValid()) {
        countResidents->setText("0 enregistrements");
        countIncidents->setText("0 enregistrements");
        countFinance->setText("0 enregistrements");
        countLocaux->setText("0 enregistrements");
        countTransport->setText("0 enregistrements");
        countTotal->setText("0 enregistrements");
        return;
    }

    QSqlQuery query(db);
    int totalCount = 0;
    int rCount = 0, iCount = 0, fCount = 0, lCount = 0, tCount = 0;

    query.prepare("SELECT COUNT(*) FROM RESIDENTS");
    if (query.exec() && query.next()) {
        rCount = query.value(0).toInt();
        countResidents->setText(QString("%1 enregistrements").arg(rCount));
        totalCount += rCount;
    } else {
        countResidents->setText("0 enregistrements");
    }

    query.prepare("SELECT COUNT(*) FROM INCIDENTS");
    if (query.exec() && query.next()) {
        iCount = query.value(0).toInt();
        countIncidents->setText(QString("%1 enregistrements").arg(iCount));
        totalCount += iCount;
    } else {
        countIncidents->setText("0 enregistrements");
    }

    query.prepare("SELECT COUNT(*) FROM FINANCES");
    if (query.exec() && query.next()) {
        fCount = query.value(0).toInt();
        countFinance->setText(QString("%1 enregistrements").arg(fCount));
        totalCount += fCount;
    } else {
        countFinance->setText("0 enregistrements");
    }

    query.prepare("SELECT COUNT(*) FROM LOCAUX");
    if (query.exec() && query.next()) {
        lCount = query.value(0).toInt();
        countLocaux->setText(QString("%1 enregistrements").arg(lCount));
        totalCount += lCount;
    } else {
        countLocaux->setText("0 enregistrements");
    }

    query.prepare("SELECT COUNT(*) FROM TRANSPORT_VEHICULES");
    if (query.exec() && query.next()) {
        tCount = query.value(0).toInt();
        countTransport->setText(QString("%1 enregistrements").arg(tCount));
        totalCount += tCount;
    } else {
        countTransport->setText("0 enregistrements");
    }

    countTotal->setText(QString("%1 enregistrements").arg(totalCount));
}

// Chart-based methods removed as we restored the old counters view