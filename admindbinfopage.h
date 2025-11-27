#ifndef ADMINDBINFOPAGE_H
#define ADMINDBINFOPAGE_H

#include <QWidget>
#include <QLabel>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class AdminDatabaseInfoPage; }
QT_END_NAMESPACE

class AdminDatabaseInfoPage : public QWidget
{
    Q_OBJECT

public:
    AdminDatabaseInfoPage(QWidget *parent = nullptr);
    ~AdminDatabaseInfoPage();

public slots:
    void refreshInfo();

private:
    Ui::AdminDatabaseInfoPage *ui;
    
    // Labels for database info
    QLabel *labelType;
    QLabel *labelDsn;
    QLabel *labelStatus;
    QLabel *labelUser;
    QLabel *labelSize;
    QLabel *valueBackup;

    // Labels for statistics (old style)
    QLabel *countResidents;
    QLabel *countIncidents;
    QLabel *countFinance;
    QLabel *countLocaux;
    QLabel *countTransport;
    QLabel *countTotal;

    QTimer *infoRefreshTimer;

    void updateDatabaseInfo();
    void updateStatistics();
};

#endif // ADMINDBINFOPAGE_H