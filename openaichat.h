// openaichat.h
#ifndef OPENAICHAT_H
#define OPENAICHAT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

class OpenAIChat : public QObject
{
    Q_OBJECT
public:
    explicit OpenAIChat(const QString &apiKey, QObject *parent = nullptr);
    void sendMessage(const QString &message, const QString &context = "");

signals:
    void responseReceived(const QString &response);
    void errorOccurred(const QString &error);

private slots:
    void onFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *networkManager;
    QString apiKey;
    QString systemPrompt;
};

#endif // OPENAICHAT_H