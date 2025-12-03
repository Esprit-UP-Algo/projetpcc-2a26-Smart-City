#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include <QSettings>
#include <QString>

class SettingsManager : public QObject
{
    Q_OBJECT

public:
    static SettingsManager &instance();

    // Password management
    QString getPasswordForUser(const QString &username) const;
    void setPasswordForUser(const QString &username, const QString &plainPassword);

    // Hash utility (SHA-256)
    QString hashPassword(const QString &plain) const;

private:
    explicit SettingsManager(QObject *parent = nullptr);

    QSettings m_settings;  // stored in QSettings → persistent
};

#endif // SETTINGSMANAGER_H
