#ifndef FINANCESPAGE_H
#define FINANCESPAGE_H
#include <QWidget>
namespace Ui {
class FinancesPage;
}
class FinancesPage : public QWidget
{
    Q_OBJECT
public:
    explicit FinancesPage(QWidget *parent = nullptr);
    ~FinancesPage();
    void loadTransactions();
    void refreshStatistics();
private slots:
    void on_addTransactionButton_clicked();
    void on_searchButton_clicked();
    void on_sortComboBox_currentIndexChanged(int index);
    void on_modifyButton_clicked();
    void on_deleteButton_clicked();
    void on_exportPDFButton_clicked();
    void on_sendButton_clicked();
private:
    void setupUi();
    void updateFinancialIndicators();
    void updateChart();
    QString getAssistantResponse(const QString &question);
    Ui::FinancesPage *ui;
};
#endif // FINANCESPAGE_H
