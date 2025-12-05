// openaichat.cpp
#include "openaichat.h"
#include <QNetworkRequest>
#include <QJsonArray>
#include <QDebug>

OpenAIChat::OpenAIChat(const QString &apiKey, QObject *parent)
    : QObject(parent), apiKey(apiKey)
{
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &OpenAIChat::onFinished);
    
    systemPrompt = "Tu es un assistant financier expert pour une application smart city. "
                  "Réponds de manière claire, précise et personnalisée aux questions financières.";
}

void OpenAIChat::sendMessage(const QString &message, const QString &context)
{
    QJsonObject json;
    json["model"] = "gpt-3.5-turbo";
    json["max_tokens"] = 150;
    json["temperature"] = 0.7;
    
    QJsonArray messages;
    QJsonObject systemMessage;
    systemMessage["role"] = "system";
    systemMessage["content"] = systemPrompt;
    messages.append(systemMessage);
    
    if (!context.isEmpty()) {
        QJsonObject contextMessage;
        contextMessage["role"] = "user";
        contextMessage["content"] = context;
        messages.append(contextMessage);
    }
    
    QJsonObject userMessage;
    userMessage["role"] = "user";
    userMessage["content"] = message;
    messages.append(userMessage);
    
    json["messages"] = messages;
    
    QNetworkRequest request(QUrl("https://api.openai.com/v1/chat/completions"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", QString("Bearer %1").arg(apiKey).toUtf8());
    
    QJsonDocument doc(json);
    networkManager->post(request, doc.toJson());
}

void OpenAIChat::onFinished(QNetworkReply *reply)
{
    reply->deleteLater();
    
    if (reply->error() != QNetworkReply::NoError) {
        emit errorOccurred(reply->errorString());
        return;
    }
    
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll(), &error);
    
    if (error.error != QJsonParseError::NoError) {
        emit errorOccurred("Erreur de parsing JSON: " + error.errorString());
        return;
    }
    
    QJsonObject response = doc.object();
    if (response.contains("choices")) {
        QJsonArray choices = response["choices"].toArray();
        if (!choices.isEmpty()) {
            QJsonObject choice = choices[0].toObject();
            QJsonObject message = choice["message"].toObject();
            QString content = message["content"].toString();
            emit responseReceived(content);
        } else {
            emit errorOccurred("Aucune réponse reçue");
        }
    } else if (response.contains("error")) {
        QJsonObject errorObj = response["error"].toObject();
        emit errorOccurred(errorObj["message"].toString());
    } else {
        emit errorOccurred("Format de réponse inattendu");
    }
}