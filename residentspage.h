#ifndef RESIDENTSPAGE_H
#define RESIDENTSPAGE_H

#include <QWidget>
#include <QVariant>
#include <QVariantMap>
#include <QList>

QT_BEGIN_NAMESPACE
namespace Ui { class ResidentsPage; }
class QWidget;
class QChartView;
QT_END_NAMESPACE

class ResidentsPage : public QWidget
{
    Q_OBJECT

public:
    explicit ResidentsPage(QWidget *parent = nullptr);
    ~ResidentsPage();

    void setResidents(const QList<QVariantMap> &rows);

signals:
    void refreshRequested();

private slots:
    // Navigation
    void onTabAdd();
    void onTabList();
    void onTabStats();

    // Form
    void onSaveForm();
    void onCancelForm();

    // List toolbar
    void onFilterTextChanged(const QString &text);
    void onSortIndexChanged(int index);

    // Row actions
    void onEditClicked();
    void onDeleteClicked();

    // Validation
    void onCinTextChanged();
    void onEmailTextChanged();

    // >>> AJOUT pour l’export PDF
    void onExportPdfClicked();
    // <<< FIN AJOUT

private:
    // UI setup
    void setupTableHeaders();
    void setupUiBehavior();
    void setupValidation();
    void setupProfessionalCharts(); // <<<< NOUVEAU pour charts

    // Helpers
    void clearForm();
    int  sortColumnForIndex(int index) const;

    // Validation methods
    bool validateForm();
    bool validateCin(const QString &cin);
    bool validateEmail(const QString &email);
    void showError(const QString &message);
    void hideError();

    // Load resident into form
    void loadResident(const QVariantMap &r);

    // Professional Statistics with QtCharts
    void createResidentStatisticsCharts(); // <<<< NOUVEAU
    void updateChartsData(); // <<<< NOUVEAU
    void updateChartsWithCurrentData(); // <<<< NOUVEAU - Refresh charts
    void addSatisfactionChart(); // <<<< NOUVEAU - Add satisfaction chart to stats page
    void createRealStatisticsCharts(); // <<<< NOUVEAU - Real QtCharts implementation
    QChartView* createResidentsPieChart(); // <<<< NOUVEAU
    QChartView* createSatisfactionCurveChart(); // <<<< NOUVEAU
    QChartView* createMonthlyTrendsChart(); // <<<< NOUVEAU

    // PDF export handled by onExportPdfClicked()

    // Statistics
    void calculateAndDisplaySatisfactionScore();
    QPair<int, QString> calculateSatisfactionScore();

    // (Old PDF helpers removed)

private:
    Ui::ResidentsPage *ui;

    // QtCharts member variables
    QWidget *chartWidget;
    QWidget *chartsContainer; // Reference to UI container
    QChartView *residentsPieChart;
    QChartView *satisfactionCurveChart;
    QChartView *monthlyTrendsChart;

    // Theme/Language helpers
    void applyTheme();
    void reloadTranslations();
};

#endif // RESIDENTSPAGE_H
