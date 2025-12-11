#ifndef FINANCESPAGE_H
#define FINANCESPAGE_H

#include <QWidget>
#include <QVariantMap>
#include <QPair>
#include <QTextCharFormat>
#include <QSqlQuery>
#include <QSqlError>
#include "arduino.h"

class FinancialAssistant;
class QChartView;
class QBarSeries;
class QBarSet;
class QBarCategoryAxis;
class QValueAxis;

QT_BEGIN_NAMESPACE
namespace Ui { class FinancesPage; }
QT_END_NAMESPACE

class FinancesPage : public QWidget
{
    Q_OBJECT

public:
    FinancesPage(QWidget *parent = nullptr);
    ~FinancesPage();
    
    // Méthode publique pour forcer la mise à jour depuis TransportPage
    void forceRefreshFromExternalTransaction(const QString &vehicleCode, double amount);

public slots:
    void reloadTransactions(); // Méthode publique pour recharger les transactions
    void onArduinoDataReceived(const QByteArray &data); // Déplacé vers public

signals:
    void arduinoResponseNeeded(const QString &response); // Signal pour envoyer réponse via MainWindow

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
    void onPreviewQRCode();
    void onGeneratePaymentQR();

    // ===== GESTION BORNE ARDUINO ===== (DÉPLACÉ VERS PUBLIC SLOTS)
    void processVehicleTransaction(const QString &vehicleCode, double amount);
    void displayTransactionResult(const QString &vehicleCode, double amount, bool success);

    // Slots pour les sous-onglets
    void onSubTabStat();
    void onSubTabChat();

    // Slots pour le chatbot
    void onSendMessage();
    void onAIResponse(const QString &response);
    void onAIError(const QString &error);

    // Slots pour Arduino (déclarés plus haut dans la section GESTION BORNE ARDUINO)

private:
    Ui::FinancesPage *ui;
    FinancialAssistant *financialAssistant;
    QString currentTransactionCode;
    bool usePieChart; // Pour alterner entre pie et bar charts

    // ===== BORNE ARDUINO TRANSPORT =====
    Arduino *arduinoBorne;
    QString pendingVehicleCode;
    double vehicleTransactionAmount = 5.0; // Montant par défaut pour recharge

    void setupTableHeaders();
    void setupUiBehavior();
    void setupValidation();
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
