#include "settingsmanager.h"
#include <QCryptographicHash>
#include <QCoreApplication>

SettingsManager &SettingsManager::instance()
{
    static SettingsManager inst;
    return inst;
}

SettingsManager::SettingsManager(QObject *parent)
    : QObject(parent),
    m_settings(QCoreApplication::organizationName(),
               QCoreApplication::applicationName())
{
}

QString SettingsManager::hashPassword(const QString &plain) const
{
    return QString(
        QCryptographicHash::hash(plain.toUtf8(),
                                 QCryptographicHash::Sha256)
            .toHex());
}

QString SettingsManager::getPasswordForUser(const QString &username) const
{
    return m_settings.value("passwords/" + username, "").toString();
}

void SettingsManager::setPasswordForUser(const QString &username,
                                         const QString &plainPassword)
{
    m_settings.setValue("passwords/" + username,
                        hashPassword(plainPassword));
}
