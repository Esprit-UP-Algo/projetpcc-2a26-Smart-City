#ifndef FINANCESPAGE_H
#define FINANCESPAGE_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QLabel>
#include <QVariantMap>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

QT_BEGIN_NAMESPACE
namespace Ui {
class FinancesPage;
}
QT_END_NAMESPACE

class FinancesPage : public QWidget
{
    Q_OBJECT

public:
    explicit FinancesPage(QWidget *parent = nullptr);
    ~FinancesPage();

    // Public utility methods
    void loadTransactions();
    void refreshStatistics();

private slots:
    // ---- Connected automatically by Qt via .ui ----
    void on_addTransactionButton_clicked();
    void on_searchButton_clicked();
    void on_sortComboBox_currentIndexChanged(int index); // ✅ corrected name
    void on_modifyButton_clicked();
    void on_deleteButton_clicked();
    void on_exportPDFButton_clicked();
    void on_sendMessageButton_clicked();

private:
    // ---- Initialization helpers ----
    void setupUi();
    void createTransactionFormCard();
    void createTransactionListCard();
    void createStatisticsCard();
    void createAssistantCard();

    // ---- Logic ----
    void updateChart();
    void updateFinancialIndicators();
    QString getAssistantResponse(const QString &question);

    // ---- UI ----
    Ui::FinancesPage *ui;

    // ---- Widgets ----
    QTableWidget *transactionsTable;
    QLineEdit *searchLineEdit;
    QComboBox *sortComboBox;
    QTextEdit *chatTextEdit;
    QLineEdit *questionLineEdit;

    // ---- Chart ----
    QChart *chart;
    QChartView *chartView;

    // ---- Labels ----
    QLabel *revenueLabel;
    QLabel *expensesLabel;
    QLabel *balanceLabel;
    QLabel *statusBadge;
    QLabel *processingDelayLabel;
};

#endif // FINANCESPAGE_H
