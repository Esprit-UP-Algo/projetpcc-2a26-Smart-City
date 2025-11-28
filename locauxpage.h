#ifndef LOCAUXPAGE_H
#define LOCAUXPAGE_H

#include <QWidget>
#include <QList>
#include <QVariantMap>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QPrinter>
#include <QTextDocument>
#include <QDate>
#include <QCalendarWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QChartView>
#include <QPieSeries>
#include <QBarSeries>
#include <QBarSet>
#include <QValueAxis>

QT_BEGIN_NAMESPACE
namespace Ui { class LocauxPage; }
QT_END_NAMESPACE

class LocauxPage : public QWidget
{
    Q_OBJECT

public:
    explicit LocauxPage(QWidget *parent = nullptr);
    ~LocauxPage();

    void setLocaux(const QList<QVariantMap> &rows);

signals:
    void refreshRequested();

private slots:
    // Tabs
    void onTabAdd();
    void onTabList();
    void onTabStats();

    // Form actions
    void onSaveForm();
    void onCancelForm();

    // List toolbar
    void onFilterTextChanged(const QString &text);
    void onSortIndexChanged(int index);
    void onExportPdfClicked();

    // Row actions
    void onEditClicked();
    void onDeleteClicked();

    // 🏆 FONCTIONS INNOVANTES
    void onVisualPlanningClicked();
    void onSmartSimulatorClicked();
    void onGenerateOccupancyReport();
    void onQuickReservation();

private:
    Ui::LocauxPage *ui;

    // Helpers
    void setupUiBehavior();
    void setupAdvancedSort();
    void loadTable();
    void clearForm();
    void showError(const QString &msg);
    void hideError();
    bool validateForm();
    int sortColumnForIndex(int index) const;
    void loadLocal(const QVariantMap &m);
    double calculerSuperficieMoyenne();
    void afficherStatistiquesGraphique();

    // Advanced features
    void sortTable(const QString &column, Qt::SortOrder order);
    void showAdvancedStats();
    QVariantMap getLocalDetails(const QString &localId);
    bool exportToPDF(const QString &fileName);
    QString generateHTMLForPDF();

    // 🥇 FONCTIONS INNOVANTES - PLANNING VISUEL
    void setupVisualPlanning();
    void updateVisualCalendar();
    void showLocalAvailability(const QString& localId);
    QList<QDate> getLocalReservations(const QString& localId);
    void addQuickReservation(const QString& localId, const QDate& startDate, const QDate& endDate);

    // 🥇 FONCTIONS INNOVANTES - SIMULATEUR INTELLIGENT
    void setupSmartSimulator();
    void runSmartSimulation();
    QList<QVariantMap> findOptimalLocaux(const QString& activityType, int capacity, const QDate& date);
    double calculateOccupancyRate(const QString& localId);
    QList<QVariantMap> suggestAlternativeLocaux(const QString& preferredLocal, const QDate& date);
    void generateSmartRecommendations();

    // Base de données étendue pour les réservations
    void createReservationsTable();
    bool addReservation(const QString& localId, const QDate& startDate, const QDate& endDate, const QString& purpose);
    QList<QVariantMap> getLocalReservationsDetails(const QString& localId);
};
#endif // LOCAUXPAGE_H
