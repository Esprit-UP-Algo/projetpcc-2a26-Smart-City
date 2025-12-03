#ifndef LANGUAGEMANAGER_H
#define LANGUAGEMANAGER_H

#include <QObject>
#include <QTranslator>
#include <QPointer>
#include <QSettings>
#include <QString>

class LanguageManager : public QObject {
    Q_OBJECT
public:
    static LanguageManager &instance();

    void initialize(); // load saved language
    void setLanguage(const QString &langCode); // 'fr' or 'en'
    QString currentLanguage() const { return m_currentLanguage; }

signals:
    void languageChanged(const QString &langCode);

private:
    explicit LanguageManager(QObject *parent = nullptr);
    void persist();
    QString translationResourceFor(const QString &langCode) const;

    QTranslator m_translator;
    QString m_currentLanguage;
    QSettings m_settings;
};

#endif // LANGUAGEMANAGER_H
