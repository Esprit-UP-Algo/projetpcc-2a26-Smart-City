#ifndef FINANCESPAGE_H
#define FINANCESPAGE_H

#include <QWidget>
#include <QVariantMap>
#include <QPair>
#include <QTextCharFormat>

class FinancialAssistant;
class QChartView;

QT_BEGIN_NAMESPACE
namespace Ui { class FinancesPage; }
QT_END_NAMESPACE

class FinancesPage : public QWidget
{
    Q_OBJECT

public:
    FinancesPage(QWidget *parent = nullptr);
    ~FinancesPage();

private slots:
    void onEditClicked();
    void onDeleteClicked();
    void onTabAdd();
    void onTabList();
    void onTabStats();
    void onSaveForm();
    void onCancelForm();
    void onFilterTextChanged(const QString &text);
    void onSortIndexChanged(int index);
    void onExportPdfClicked();

    // Slots pour les sous-onglets
    void onSubTabStat();
    void onSubTabChat();

    // Slots pour le chatbot
    void onSendMessage();
    void onAIResponse(const QString &response);
    void onAIError(const QString &error);

private:
    Ui::FinancesPage *ui;
    FinancialAssistant *financialAssistant;
    QString currentTransactionCode;

    void setupTableHeaders();
    void setupUiBehavior();
    void setupValidation();
    void reloadTransactions();
    void setTransactions(const QList<QVariantMap> &list);
    void loadTransaction(const QVariantMap &transaction);
    void loadResidentCINs();
    void calculateAndDisplayScore();
    void clearForm();
    bool validateForm();
    void showError(const QString &message);
    void hideError();
    int sortColumnForIndex(int index) const;
    QPair<int, QString> calculateFinancialScore(double revenues, double expenses);
    void addMessage(const QString &sender, const QString &message);
    void updateChart(QChartView *chartView, double revenues, double expenses);
};

#endif // FINANCESPAGE_H
