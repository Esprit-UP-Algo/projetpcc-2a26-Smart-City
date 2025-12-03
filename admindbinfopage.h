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
    explicit AdminDatabaseInfoPage(QWidget *parent = nullptr);
    ~AdminDatabaseInfoPage();

public slots:
    void refreshInfo();          // Refresh DB info + stats

private:
    Ui::AdminDatabaseInfoPage *ui;

    /* ──────────────────────────────
     *  Database Info Labels
     * ────────────────────────────── */
    QLabel *labelType;
    QLabel *labelDsn;
    QLabel *labelStatus;
    QLabel *labelUser;
    QLabel *labelSize;
    QLabel *valueBackup;

    /* ──────────────────────────────
     *  Statistics Labels
     * ────────────────────────────── */
    QLabel *countResidents;
    QLabel *countIncidents;
    QLabel *countFinance;
    QLabel *countLocaux;
    QLabel *countTransport;
    QLabel *countTotal;

    /* ──────────────────────────────
     *  Timer for auto-refresh
     * ────────────────────────────── */
    QTimer *infoRefreshTimer;

    /* ──────────────────────────────
     *  Internal Methods
     * ────────────────────────────── */
    void updateDatabaseInfo();
    void updateStatistics();

    /* ──────────────────────────────
     *  NEW (reserved for logs later)
     * ────────────────────────────── */
    void updateInfoStyling();    // Better styling (colors, fonts)
    void updateTotalRecordStyle();   // Dynamic color update
};

#endif // ADMINDBINFOPAGE_H
