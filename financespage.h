#ifndef FINANCESPAGE_H
#define FINANCESPAGE_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QLabel>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QCloseEvent>   // ✅ nécessaire pour la sauvegarde à la fermeture

QT_BEGIN_NAMESPACE
namespace Ui { class FinancesPage; }
QT_END_NAMESPACE

class FinancesPage : public QWidget
{
    Q_OBJECT

public:
    explicit FinancesPage(QWidget *parent = nullptr);
    ~FinancesPage();

    void loadTransactions();
    void refreshStatistics();

protected:
    void closeEvent(QCloseEvent *event) override;   // ✅ sauvegarde automatique avant fermeture

private slots:
    void on_addTransactionButton_clicked();
    void on_searchButton_clicked();
    void on_sortComboChanged(int index);
    void on_modifyButton_clicked();
    void on_deleteButton_clicked();
    void on_exportPDFButton_clicked();
    void on_sendMessageButton_clicked();

private:
    void setupUi();
    void createTransactionFormCard();
    void createTransactionListCard();
    void createStatisticsCard();
    void createAssistantCard();
    void updateChart();
    void updateFinancialIndicators();
    QString getAssistantResponse(const QString &question);

    // ✅ Ajout : méthode interne pour sauvegarder toutes les transactions dans Oracle
    void saveAllToDatabase();

    Ui::FinancesPage *ui;

    // Widgets
    QWidget *formCardWidget;
    QWidget *tableCardWidget;
    QTableWidget *transactionsTable;
    QLineEdit *searchLineEdit;
    QComboBox *sortComboBox;

    QWidget *statsCardWidget;
    QChart *chart;
    QChartView *chartView;
    QLabel *revenueLabel;
    QLabel *expensesLabel;
    QLabel *balanceLabel;
    QLabel *statusBadge;
    QLabel *processingDelayLabel;

    QWidget *assistantCardWidget;
    QTextEdit *chatTextEdit;
    QLineEdit *questionLineEdit;
};

#endif // FINANCESPAGE_H
