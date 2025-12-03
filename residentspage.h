#ifndef RESIDENTSPAGE_H
#define RESIDENTSPAGE_H

#include <QWidget>
#include <QVariant>
#include <QVariantMap>
#include <QList>

#include "arduino.h"   // <<--------- IMPORTANT
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

    // PDF
    void onExportPdfClicked();

    // 🔵 SLOT RFID — Reçoit UID depuis Arduino
    void onArduinoData(QString data);

private:
    // UI setup
    void setupTableHeaders();
    void setupUiBehavior();
    void setupValidation();
    void setupProfessionalCharts();

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

    // Statistics
    void createResidentStatisticsCharts();
    void updateChartsData();
    void updateChartsWithCurrentData();
    void addSatisfactionChart();
    void createRealStatisticsCharts();
    QChartView* createResidentsPieChart();
    QChartView* createSatisfactionCurveChart();
    QChartView* createMonthlyTrendsChart();

    void calculateAndDisplaySatisfactionScore();
    QPair<int, QString> calculateSatisfactionScore();

private:
    Ui::ResidentsPage *ui;

    // QtCharts members
    QWidget *chartWidget;
    QWidget *chartsContainer;
    QChartView *residentsPieChart;
    QChartView *satisfactionCurveChart;
    QChartView *monthlyTrendsChart;

    // THEME + LANGUAGE
    void applyTheme();
    void reloadTranslations();

    // 🔵 ARDUINO
    Arduino *arduino;      // OBJET ARDUINO
    int deniedCount = 0;   // compteur pour 3x DENIED → incident
};

#endif // RESIDENTSPAGE_H
