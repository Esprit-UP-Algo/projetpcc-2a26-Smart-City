#ifndef FINANCIALASSISTANT_H
#define FINANCIALASSISTANT_H

#include "openaichat.h"
#include "databasemanager.h"
#include <QObject>

class FinancialAssistant : public QObject
{
    Q_OBJECT
public:
    explicit FinancialAssistant(const QString &apiKey, QObject *parent = nullptr);
    void askQuestion(const QString &question);
    void getFinancialAnalysis();

signals:
    void responseReceived(const QString &response);
    void errorOccurred(const QString &error);

private slots:
    void handleAIResponse(const QString &response);
    void handleAIError(const QString &error);

private:
    OpenAIChat *aiChat;
    QString prepareFinancialContext() const;
    QString analyzeSpendingPatterns() const;
    QString generateBudgetAdvice() const;
    void generateSimulatedResponse(const QString &question, const QString &context);
};

#endif // FINANCIALASSISTANT_H