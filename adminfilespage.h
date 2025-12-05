#ifndef ADMINFILESPAGE_H
#define ADMINFILESPAGE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class AdminFilesPage; }
QT_END_NAMESPACE

class AdminFilesPage : public QWidget
{
    Q_OBJECT

public:
    AdminFilesPage(QWidget *parent = nullptr);
    ~AdminFilesPage();

public slots:
    void refreshPendingCounts();
    void onBackupDatabase();
    void onRestoreDatabase();
    void onPurgeDatabase();
    void onImportAllPending();
    void onAutoBackupToggled(bool checked);
    void onViewPending(const QString& file);
    void onImportPending(const QString& file);
    void onClearPending(const QString& file);

private:
    Ui::AdminFilesPage *ui;
    
    // Pending file count labels
    QLabel *countResidents;
    QLabel *countIncidents;
    QLabel *countFinance;
    QLabel *countLocaux;
    QLabel *countTransport;
    
    // Action buttons for individual files
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
    
    // Global action buttons
    QPushButton *btnBackup;
    QPushButton *btnRestore;
    QPushButton *btnPurge;
    QPushButton *btnImportAll;
    QCheckBox *chkAutoBackup;
    
    QTimer *pendingRefreshTimer;
    
    void ensurePendingFiles();
    int countPendingElements(const QString& filename);
};

#endif // ADMINFILESPAGE_H