#include "admindbinfopage.h"
#include "ui_admindbinfopage.h"
#include "databasemanager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSettings>
#include <QDateTime>
#include <QDebug>

/* ============================================================
 *                CONSTRUCTOR
 * ============================================================ */
AdminDatabaseInfoPage::AdminDatabaseInfoPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AdminDatabaseInfoPage)
{
    ui->setupUi(this);

    // Map UI widgets
    labelType       = ui->labelType;
    labelDsn        = ui->labelDsn;
    labelStatus     = ui->labelStatus;
    labelUser       = ui->labelUser;
    labelSize       = ui->labelSize;
    valueBackup     = ui->valueBackup;

    countResidents  = ui->countResidents;
    countIncidents  = ui->countIncidents;
    countFinance    = ui->countFinance;
    countLocaux     = ui->countLocaux;
    countTransport  = ui->countTransport;
    countTotal      = ui->countTotal;

    // Auto refresh timer every 5 sec
    infoRefreshTimer = new QTimer(this);
    infoRefreshTimer->setInterval(5000);
    connect(infoRefreshTimer, &QTimer::timeout,
            this, &AdminDatabaseInfoPage::refreshInfo);
    infoRefreshTimer->start();

    // Initial refresh
    refreshInfo();
}

AdminDatabaseInfoPage::~AdminDatabaseInfoPage()
{
    delete ui;
}

/* ============================================================
 *                MAIN REFRESH METHOD
 * ============================================================ */
void AdminDatabaseInfoPage::refreshInfo()
{
    updateDatabaseInfo();
    updateStatistics();
    updateInfoStyling();
}

/* ============================================================
 *                UPDATE DATABASE INFO
 * ============================================================ */
void AdminDatabaseInfoPage::updateDatabaseInfo()
{
    auto &dbManager = DatabaseManager::instance();
    QSqlDatabase db = dbManager.database();

    /* ------------------ STATUS ------------------ */
    if (db.isOpen() && db.isValid()) {
        labelStatus->setText("Connecté");
        labelStatus->setStyleSheet(
            "font-size: 16px; font-weight: bold; color: #27AE60;"
            );
    } else {
        labelStatus->setText("Déconnecté");
        labelStatus->setStyleSheet(
            "font-size: 16px; font-weight: bold; color: #E74C3C;"
            );
    }

    /* ------------------ USER ------------------ */
    labelUser->setText(db.userName().isEmpty() ? "N/A" : db.userName());

    /* ------------------ DSN ------------------ */
    QString dsn = db.hostName().isEmpty()
                      ? (db.databaseName().isEmpty()
                             ? db.connectionName()
                             : db.databaseName())
                      : QString("%1:%2/%3")
                            .arg(db.hostName())
                            .arg(db.port())
                            .arg(db.databaseName());

    labelDsn->setText(dsn);

    /* ------------------ DB SIZE ------------------ */
    if (db.isOpen() && db.isValid()) {
        QSqlQuery q(db);

        if (q.exec("SELECT NVL(SUM(bytes),0) FROM user_segments")) {
            if (q.next()) {
                double bytes = q.value(0).toDouble();
                double mb = bytes / (1024.0 * 1024.0);
                double gb = mb / 1024.0;

                if (gb >= 1.0)
                    labelSize->setText(QString::number(gb, 'f', 2) + " GB");
                else
                    labelSize->setText(QString::number(mb, 'f', 2) + " MB");
            }
        } else {
            labelSize->setText("N/A");
        }
    } else {
        labelSize->setText("N/A");
    }

    /* ------------------ LAST BACKUP ------------------ */
    QSettings s;
    QString lastBackup = s.value("db/lastBackup").toString();

    if (lastBackup.isEmpty()) {
        valueBackup->setText("Aucune");
        valueBackup->setStyleSheet("color: #E74C3C; font-weight: bold;");
    } else {
        valueBackup->setText(lastBackup);
        valueBackup->setStyleSheet("color: #27AE60; font-weight: bold;");
    }
}

/* ============================================================
 *                UPDATE STATISTICS
 * ============================================================ */
void AdminDatabaseInfoPage::updateStatistics()
{
    auto &dbManager = DatabaseManager::instance();
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

    int total = 0;

    auto fetchCount = [&](const QString &table) {
        QSqlQuery query(db);
        query.prepare(QStringLiteral("SELECT COUNT(*) FROM %1").arg(table));
        if (query.exec() && query.next())
            return query.value(0).toInt();
        return 0;
    };

    int r = fetchCount("RESIDENTS");
    int i = fetchCount("INCIDENTS");
    int f = fetchCount("FINANCES");
    int l = fetchCount("LOCAUX");
    int t = fetchCount("TRANSPORT_VEHICULES");

    total = r + i + f + l + t;

    countResidents->setText(QString("%1 enregistrements").arg(r));
    countIncidents->setText(QString("%1 enregistrements").arg(i));
    countFinance->setText(QString("%1 enregistrements").arg(f));
    countLocaux->setText(QString("%1 enregistrements").arg(l));
    countTransport->setText(QString("%1 enregistrements").arg(t));
    countTotal->setText(QString("%1 enregistrements").arg(total));

    updateTotalRecordStyle();
}

/* ============================================================
 *               STYLING HELPERS
 * ============================================================ */
void AdminDatabaseInfoPage::updateInfoStyling()
{
    // Future: dynamic fade / animations
    // Clean style consistency for Nexora
    this->setStyleSheet(
        "QLabel { font-family: 'Segoe UI'; font-size: 15px; color: #2C3E50; }"
        );
}

void AdminDatabaseInfoPage::updateTotalRecordStyle()
{
    int total = countTotal->text().split(" ").first().toInt();

    if (total == 0) {
        countTotal->setStyleSheet("color: #E74C3C; font-weight: bold;");
    } else if (total < 10) {
        countTotal->setStyleSheet("color: #F1C40F; font-weight: bold;");
    } else {
        countTotal->setStyleSheet("color: #27AE60; font-weight: bold;");
    }
}
