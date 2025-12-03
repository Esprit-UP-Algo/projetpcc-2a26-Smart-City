#include "languagemanager.h"
#include <QCoreApplication>
#include <QDebug>

LanguageManager &LanguageManager::instance() {
    static LanguageManager inst; return inst;
}

LanguageManager::LanguageManager(QObject *parent)
    : QObject(parent),
      m_currentLanguage("fr"),
      m_settings(QCoreApplication::organizationName(), QCoreApplication::applicationName()) {}

void LanguageManager::initialize() {
    m_currentLanguage = m_settings.value("ui/language", "fr").toString();
    setLanguage(m_currentLanguage); // ensure translator loaded
}

void LanguageManager::persist() {
    m_settings.setValue("ui/language", m_currentLanguage);
}

QString LanguageManager::translationResourceFor(const QString &langCode) const {
    if (langCode == "en") return ":/translations/nexora_en.qm"; // ensure resource exists
    return ":/translations/nexora_fr.qm"; // default
}

void LanguageManager::setLanguage(const QString &langCode) {
    QString normalized = (langCode == "en" ? "en" : "fr");
    if (normalized == m_currentLanguage && m_translator.isEmpty()) {
        // first load still required if translator empty
    } else if (normalized == m_currentLanguage) {
        return; // nothing to do
    }

    QCoreApplication::removeTranslator(&m_translator);
    bool loaded = m_translator.load(translationResourceFor(normalized));
    if (!loaded) {
        // Fallback to relative disk path
        const QString rel = QString("translations/nexora_%1.qm").arg(normalized);
        loaded = m_translator.load(rel);
    }
    if (!loaded) {
        qWarning() << "Failed to load translation for" << normalized << "(resource or disk)";
    } else {
        QCoreApplication::installTranslator(&m_translator);
    }
    m_currentLanguage = normalized;
    persist();
    emit languageChanged(m_currentLanguage);
}
