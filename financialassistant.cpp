// financialassistant.cpp
#include "financialassistant.h"
#include "config.h"
#include "databasemanager.h"
#include <QDate>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include <QDebug>
#include <algorithm>

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
    
#if USE_SIMULATED_AI
    // Mode simulé activé
    QTimer::singleShot(1000, this, [this, question, context]() {
        generateSimulatedResponse(question, context);
    });
#else
    // Mode API OpenAI avec prompt amélioré
    QString prompt = QString(
        "Tu es un assistant financier expert pour une application de gestion smart city appelée NEXORA. "
        "Réponds UNIQUEMENT à la question posée par l'utilisateur de façon précise et personnalisée.\n\n"
        
        "QUESTION DE L'UTILISATEUR: %1\n\n"
        
        "CONTEXTE FINANCIER ACTUEL:\n%2\n\n"
        
        "INSTRUCTIONS:\n"
        "- Réponds DIRECTEMENT et SPÉCIFIQUEMENT à la question posée\n"
        "- Utilise les données financières fournies dans ta réponse\n"
        "- Donne des conseils concrets et personnalisés\n"
        "- Format ta réponse avec des emoji et une structure claire\n"
        "- Maximum 200 mots\n"
        "- Ne répète pas la question, réponds directement\n\n"
        
        "RÉPONSE PERSONNALISÉE:")
        .arg(question, context);
        
    qDebug() << "Envoi à OpenAI:" << prompt;
    aiChat->sendMessage(prompt);
#endif
}

void FinancialAssistant::generateSimulatedResponse(const QString &question, const QString &context)
{
    QString response;
    QString lowerQuestion = question.toLower();
    
    qDebug() << "===== CHATBOT DEBUG =====";
    qDebug() << "Question originale:" << question;
    qDebug() << "Question en minuscules:" << lowerQuestion;
    qDebug() << "Contexte passé:" << context;
    
    // Récupérer les données financières actuelles
    QList<QVariantMap> transactions = DatabaseManager::instance().getAllTransactions();
    double revenues = 0.0;
    double expenses = 0.0;
    
    for (const QVariantMap &trans : transactions) {
        QString type = trans.value("type").toString();
        double montant = trans.value("montant").toDouble();
        if (type.toLower().contains("entrée") || type == "Entrée") {
            revenues += montant;
        } else if (type.toLower().contains("sortie") || type == "Sortie") {
            expenses += montant;
        }
    }
    
    double balance = revenues - expenses;
    
    qDebug() << "Données calculées - Entrées:" << revenues << "Sorties:" << expenses << "Balance:" << balance;
    
    // Détection intelligente des mots-clés financiers avec variantes d'orthographe
    QStringList financeKeywords = {"budget", "budjet", "buget", "argent", "finance", "économie", "épargne", "dépense", "dépence", "revenu", "entrée", "sortie", 
                                   "solde", "montant", "coût", "cout", "prix", "facture", "paiement", "crédit", "débit", "investir",
                                   "économiser", "épargner", "dépenser", "optimiser", "gérer", "gerer", "planifier", "conseil", "aide",
                                   "financer", "financement", "budgétaire", "budgetaire", "financier", "financière", "gestion", "organisation"};
    
    bool isFinancialQuestion = false;
    QString detectedKeywords = "";
    for (const QString &keyword : financeKeywords) {
        if (lowerQuestion.contains(keyword)) {
            isFinancialQuestion = true;
            detectedKeywords += keyword + " ";
        }
    }
    
    qDebug() << "Question financière détectée:" << isFinancialQuestion;
    qDebug() << "Mots-clés trouvés:" << detectedKeywords.trimmed();
    
    // Génération de réponse intelligente adaptée à toute question financière
    if (isFinancialQuestion || lowerQuestion.contains("?")) {
        qDebug() << "RÉPONSE: Question financière analysée";
        
        // Générer une réponse dynamique et intelligente
        QString statusIcon = balance > 0 ? "✅" : balance == 0 ? "⚖️" : "⚠️";
        QString statusText = balance > 0 ? "Excellente situation financière !" : 
                            balance == 0 ? "Situation équilibrée" :
                            "Attention au déficit";
        
        // Construction d'une réponse complète et contextuelle
        response = QString("🤖 **Assistant Financier NEXORA**\\n\\n"
                          "**Votre question :** \"%1\"\\n\\n"
                          "**📊 Vos données actuelles :**\\n"
                          "• Revenus totaux : %2 DT\\n"
                          "• Dépenses totales : %3 DT\\n"
                          "• Solde disponible : %4 DT %5\\n\\n"
                          "**💡 Réponse personnalisée :**\\n")
                          .arg(question)
                          .arg(QString::number(revenues, 'f', 2))
                          .arg(QString::number(expenses, 'f', 2))
                          .arg(QString::number(balance, 'f', 2))
                          .arg(statusIcon);
        
        // Analyse spécifique selon les mots-clés détectés avec priorité
        if (lowerQuestion.contains("facture") || lowerQuestion.contains("planifier") || lowerQuestion.contains("planning") || lowerQuestion.contains("paiement")) {
            response += "📅 **Planification des factures :**\\n"
                       "• Créez un calendrier de paiements mensuel\\n"
                       "• Domiciliez vos factures récurrentes\\n"
                       "• Provisionnez " + QString::number(expenses * 1.1, 'f', 2) + " DT/mois pour les factures\\n"
                       "• Programmez des alertes 3 jours avant chaque échéance\\n"
                       "• Utilisez l'épargne automatique après les factures\\n";
        } else if (lowerQuestion.contains("dépense") || lowerQuestion.contains("dépence") || (lowerQuestion.contains("gérer") && !lowerQuestion.contains("budget")) || lowerQuestion.contains("gerer")) {
            response += "💼 **Guide de gestion des dépenses :**\\n"
                       "• Catégorisez : Fixe (" + QString::number(expenses * 0.6, 'f', 2) + " DT) / Variable (" + QString::number(expenses * 0.4, 'f', 2) + " DT)\\n"
                       "• Règle 50/30/20 : 50% besoins, 30% envies, 20% épargne\\n"
                       "• Limitez les achats impulsifs (règle 24h)\\n"
                       "• Utilisez des apps de suivi quotidien\\n"
                       "• Révisez chaque semaine vos dépenses\\n";
        } else if (lowerQuestion.contains("budjet") || lowerQuestion.contains("budget") || (lowerQuestion.contains("gérer") && lowerQuestion.contains("budget"))) {
            response += "📋 **Guide de gestion budgétaire :**\\n"
                       "• Établissez un budget mensuel de " + QString::number(revenues, 'f', 2) + " DT\\n"
                       "• Allouez max " + QString::number(revenues * 0.80, 'f', 2) + " DT aux dépenses\\n"
                       "• Gardez " + QString::number(revenues * 0.20, 'f', 2) + " DT pour l'épargne\\n"
                       "• Suivez quotidiennement vos entrées/sorties\\n"
                       "• Ajustez mensuellement selon vos besoins\\n";
        } else if (lowerQuestion.contains("optimiser") || lowerQuestion.contains("améliorer") || lowerQuestion.contains("mieux") || lowerQuestion.contains("cout") || lowerQuestion.contains("coût")) {
            response += "🔧 **Optimisation de vos coûts :**\\n"
                       "• Analysez vos 3 plus grosses dépenses\\n"
                       "• Renégociez vos contrats (assurance, abonnements)\\n"
                       "• Comparez les prix avant chaque achat\\n"
                       "• Objectif d'économies : " + QString::number(expenses * 0.10, 'f', 2) + " DT/mois\\n"
                       "• Éliminez les dépenses inutiles\\n";
        } else if (lowerQuestion.contains("épargn") || lowerQuestion.contains("économ")) {
            response += "💰 **Plan d'épargne recommandé :**\\n"
                       "• Objectif mensuel : " + QString::number(revenues * 0.20, 'f', 2) + " DT\\n"
                       "• Fonds d'urgence cible : " + QString::number(expenses * 3, 'f', 2) + " DT\\n"
                       "• Automatisez vos virements d'épargne\\n";
        } else if (lowerQuestion.contains("invest") || lowerQuestion.contains("placement")) {
            response += "📈 **Options d'investissement selon votre profil :**\\n"
                       "• Avec votre épargne disponible de " + QString::number(balance, 'f', 2) + " DT\\n"
                       "• Considérez des placements sécurisés d'abord\\n"
                       "• Diversifiez vos investissements\\n";
        } else if (lowerQuestion.contains("conseil") || lowerQuestion.contains("aide") || lowerQuestion.contains("recommand")) {
            response += "🎯 **Mes conseils pour votre situation :**\\n"
                       "• " + statusText + "\\n"
                       "• Planifiez vos dépenses à l'avance\\n"
                       "• Tenez un journal financier régulier\\n"
                       "• Fixez-vous des objectifs réalistes\\n";
        } else {
            response += "Analyse générale de votre situation :\\n"
                       "• " + statusText + "\\n"
                       "• Ratio d'épargne possible : " + QString::number((balance/revenues)*100, 'f', 1) + "%\\n"
                       "• Recommandation : surveillez vos dépenses et optimisez votre budget\\n";
        }
        
        response += "\\n📞 **N'hésitez pas à me poser d'autres questions financières !**";
    } else {
        qDebug() << "RÉPONSE: Question non financière";
        
        response = QString("🤖 **Assistant Financier NEXORA**\\n\\n"
                          "Je suis votre assistant financier spécialisé !\\n\\n"
                          "**Vos données actuelles :**\\n"
                          "• Revenus : %1 DT\\n"
                          "• Dépenses : %2 DT\\n"
                          "• Solde : %3 DT\\n\\n"
                          "**Posez-moi vos questions sur :**\\n"
                          "• Budget et planification financière\\n"
                          "• Épargne et investissements\\n"
                          "• Optimisation des dépenses\\n"
                          "• Conseils financiers personnalisés\\n\\n"
                          "💬 **Exemple :** \\\"Comment améliorer mon budget ?\\\"")
                          .arg(QString::number(revenues, 'f', 2))
                          .arg(QString::number(expenses, 'f', 2))
                          .arg(QString::number(balance, 'f', 2));
    }
    
    qDebug() << "Réponse générée, longueur:" << response.length();
    qDebug() << "===== FIN DEBUG =====";
    emit responseReceived(response);
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
    // Récupérer toutes les transactions pour une analyse complète
    QList<QVariantMap> allTransactions = DatabaseManager::instance().getAllTransactions();
    
    double totalRevenues = 0.0;
    double totalExpenses = 0.0;
    QMap<QString, double> categoryBreakdown;
    
    // Calculer les totaux et catégories
    for (const QVariantMap &trans : allTransactions) {
        QString type = trans.value("type").toString();
        double montant = trans.value("montant").toDouble();
        QString categorie = trans.value("categorie").toString();
        
        if (type.toLower().contains("entrée") || type == "Entrée") {
            totalRevenues += montant;
            categoryBreakdown["Entrées " + categorie] += montant;
        } else if (type.toLower().contains("sortie") || type == "Sortie") {
            totalExpenses += montant;
            categoryBreakdown["Sorties " + categorie] += montant;
        }
    }
    
    double balance = totalRevenues - totalExpenses;
    double savingRate = totalRevenues > 0 ? ((totalRevenues - totalExpenses) / totalRevenues) * 100 : 0;
    
    // Trier les transactions par date (du plus récent au plus ancien)
    std::sort(allTransactions.begin(), allTransactions.end(),
              [](const QVariantMap &a, const QVariantMap &b) {
                  QDate dateA = QDate::fromString(a["date_transaction"].toString(), "yyyy-MM-dd");
                  QDate dateB = QDate::fromString(b["date_transaction"].toString(), "yyyy-MM-dd");
                  return dateA > dateB;
              });

    // Prendre les 3 dernières transactions
    QStringList recentTransactions;
    for (int i = 0; i < qMin(3, allTransactions.size()); ++i) {
        const auto &t = allTransactions[i];
        recentTransactions << QString("%1: %2 %3 DT (%4)")
                              .arg(t["date_transaction"].toString())
                              .arg(t["type"].toString())
                              .arg(QString::number(t["montant"].toDouble(), 'f', 2))
                              .arg(t["categorie"].toString());
    }
    
    // Analyse des principales catégories de dépenses
    QString topCategories;
    QList<QString> sortedCategories = categoryBreakdown.keys();
    std::sort(sortedCategories.begin(), sortedCategories.end(),
              [&categoryBreakdown](const QString &a, const QString &b) {
                  return categoryBreakdown[a] > categoryBreakdown[b];
              });
    
    for (int i = 0; i < qMin(3, sortedCategories.size()); ++i) {
        topCategories += QString("- %1: %2 DT\n")
                         .arg(sortedCategories[i])
                         .arg(QString::number(categoryBreakdown[sortedCategories[i]], 'f', 2));
    }

    return QString(
               "=== DONNÉES FINANCIÈRES PERSONNELLES ===\n\n"
               "📊 BILAN GLOBAL:\n"
               "• Total entrées: %1 DT\n"
               "• Total sorties: %2 DT\n"
               "• Solde net: %3 DT\n"
               "• Taux d'épargne: %4%\n"
               "• Nombre de transactions: %5\n\n"
               
               "🏷️ PRINCIPALES CATÉGORIES:\n%6\n"
               
               "📅 DERNIÈRES TRANSACTIONS:\n%7\n\n"
               
               "💡 STATUT: %8"
               ).arg(QString::number(totalRevenues, 'f', 2))
                .arg(QString::number(totalExpenses, 'f', 2))
                .arg(QString::number(balance, 'f', 2))
                .arg(QString::number(savingRate, 'f', 1))
                .arg(allTransactions.size())
                .arg(topCategories.isEmpty() ? "Aucune catégorie détectée" : topCategories)
                .arg(recentTransactions.isEmpty() ? "Aucune transaction récente" : recentTransactions.join("\n"))
                .arg(balance > 0 ? "Situation financière positive" : 
                     balance == 0 ? "Situation équilibrée" : "Attention - déficit détecté");
}

void FinancialAssistant::handleAIResponse(const QString &response)
{
    emit responseReceived(response);
}

void FinancialAssistant::handleAIError(const QString &error)
{
    emit errorOccurred("Erreur de l'assistant financier: " + error);
}