#ifndef ADMINDATABASEPAGE_H
#define ADMINDATABASEPAGE_H

#include <QWidget>
#include <QPointer>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QTimer>
#include <QSettings>

class QLabel;
class QPushButton;
class QCheckBox;
class QGroupBox;
class QGridLayout;

namespace Ui { class AdminDatabasePage; }

class AdminDatabasePage : public QWidget
{
    Q_OBJECT
public:
    explicit AdminDatabasePage(QWidget *parent = nullptr);
    ~AdminDatabasePage() override;

public slots:
    void applyTheme();
    void reloadTranslations();

private slots:
    void refreshInfo();
    void refreshPendingCounts();

    void onBackupDatabase();
    void onRestoreDatabase();
    void onPurgeDatabase();
    void onImportAllPending();

    void onViewPending(const QString &fileName);
    void onImportPending(const QString &fileName);
    void onClearPending(const QString &fileName);

    void onAutoBackupToggled(bool enabled);

private:
    Ui::AdminDatabasePage *ui;
    
    // Helpers
    QSqlDatabase db() const;
    bool ensurePendingFiles();
    QString pendingDirPath() const;
    QString pendingFilePath(const QString &name) const;
    int countJsonItems(const QString &filePath) const;
    bool insertDataIntoOracle(const QString &filePath);
    void showSqlError(const QString &context, const QSqlError &err);
    void showInfo(const QString &message);
    void showWarning(const QString &message);

    bool dumpDatabaseToSql(const QString &filePath);
    bool restoreDatabaseFromSql(const QString &filePath);
    bool truncateAllTables();

    // UI elements bound from .ui
    QLabel *lblType;
    QLabel *lblDsn;
    QLabel *lblStatus;
    QLabel *lblUser;
    QLabel *lblSize;
    QLabel *lblLastBackup;

    // Pending rows
    QLabel *lblResidentsCount;
    QLabel *lblIncidentsCount;
    QLabel *lblFinanceCount;
    QLabel *lblLocauxCount;
    QLabel *lblTransportCount;

    QPushButton *btnResidentsView;
    QPushButton *btnResidentsImport;
    QPushButton *btnResidentsClear;

    QPushButton *btnIncidentsView;
    QPushButton *btnIncidentsImport;
    QPushButton *btnIncidentsClear;

    QPushButton *btnFinanceView;
    QPushButton *btnFinanceImport;
    QPushButton *btnFinanceClear;

    QPushButton *btnLocauxView;
    QPushButton *btnLocauxImport;
    QPushButton *btnLocauxClear;

    QPushButton *btnTransportView;
    QPushButton *btnTransportImport;
    QPushButton *btnTransportClear;

    // Global actions
    QPushButton *btnBackup;
    QPushButton *btnRestore;
    QPushButton *btnPurge;
    QPushButton *btnImportAll;
    QCheckBox *chkAutoBackup;

    QTimer *infoRefreshTimer;
};

#endif // ADMINDATABASEPAGE_H