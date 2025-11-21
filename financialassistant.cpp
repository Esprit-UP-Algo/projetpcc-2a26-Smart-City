// financialassistant.cpp
#include "financialassistant.h"
#include <QDate>
#include <QJsonDocument>
#include <QJsonObject>

FinancialAssistant::FinancialAssistant(const QString &apiKey, QObject *parent)
    : QObject(parent)
{
    aiChat = new OpenAIChat(apiKey, this);
    connect(aiChat, &OpenAIChat::responseReceived, this, &FinancialAssistant::handleAIResponse);
    connect(aiChat, &OpenAIChat::errorOccurred, this, &FinancialAssistant::handleAIError);
}

void FinancialAssistant::askQuestion(const QString &question)
{
    QString context = prepareFinancialContext();
    QString prompt = QString("Question de l'utilisateur: %1\n\n"
                             "Informations financières actuelles:\n%2")
                         .arg(question, context);

    aiChat->sendMessage(prompt);
}

void FinancialAssistant::getFinancialAnalysis()
{
    QString context = prepareFinancialContext();
    QString analysisRequest = QString(
                                  "Analyse complète de la situation financière avec les données suivantes:\n\n"
                                  "%1\n\n"
                                  "Fournis une analyse détaillée comprenant:\n"
                                  "1. Résumé des finances actuelles\n"
                                  "2. Points forts et points à améliorer\n"
                                  "3. Recommandations personnalisées\n"
                                  "4. Conseils pour optimiser le budget"
                                  ).arg(context);

    aiChat->sendMessage(analysisRequest);
}

QString FinancialAssistant::prepareFinancialContext() const
{
    QDate today = QDate::currentDate();
    QVariantMap stats = DatabaseManager::instance().getFinancialStatistics(today.year(), today.month());
    double revenues = stats.value("revenu").toDouble();
    double expenses = stats.value("depense").toDouble();
    double balance = revenues - expenses;

    // Récupérer toutes les transactions
    QList<QVariantMap> allTransactions = DatabaseManager::instance().getAllTransactions();

    // Trier les transactions par date (du plus récent au plus ancien)
    std::sort(allTransactions.begin(), allTransactions.end(),
              [](const QVariantMap &a, const QVariantMap &b) {
                  QDate dateA = QDate::fromString(a["date_transaction"].toString(), "yyyy-MM-dd");
                  QDate dateB = QDate::fromString(b["date_transaction"].toString(), "yyyy-MM-dd");
                  return dateA > dateB; // Tri décroissant
              });

    // Prendre les 5 premières transactions (les plus récentes)
    QList<QVariantMap> recentTransactions = allTransactions.mid(0, 5);

    QString transactionsList;
    for (const auto &t : recentTransactions) {
        transactionsList += QString("- %1: %2 (%3) - %4 DT\n")
        .arg(t["date_transaction"].toString(),
             t["type"].toString(),
             t["categorie"].toString(),
             QString::number(t["montant"].toDouble(), 'f', 2));
    }

    return QString(
               "Situation financière actuelle:\n"
               "--------------------------------\n"
               "💵 Revenus du mois: %1 DT\n"
               "💸 Dépenses du mois: %2 DT\n"
               "💰 Solde actuel: %3 DT\n\n"
               "Dernières transactions:\n%4"
               ).arg(QString::number(revenues, 'f', 2),
             QString::number(expenses, 'f', 2),
             QString::number(balance, 'f', 2),
             transactionsList.isEmpty() ? "Aucune transaction récente" : transactionsList);
}

void FinancialAssistant::handleAIResponse(const QString &response)
{
    emit responseReceived(response);
}

void FinancialAssistant::handleAIError(const QString &error)
{
    emit errorOccurred("Erreur de l'assistant financier: " + error);
}
