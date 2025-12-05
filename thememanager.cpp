#include "thememanager.h"
#include <QCoreApplication>
#include <QWidget>

// ---------------- SINGLETON ----------------
ThemeManager &ThemeManager::instance() {
    static ThemeManager inst;
    return inst;
}

ThemeManager::ThemeManager(QObject *parent)
    : QObject(parent),
    m_currentTheme(AppTheme::Light),
    m_settings(QCoreApplication::organizationName(),
               QCoreApplication::applicationName())
{}

// ---------------- INITIALISATION ----------------
void ThemeManager::initialize() {
    const QString key = m_settings.value("ui/theme", "light").toString();
    m_currentTheme = (key == "dark" ? AppTheme::Dark : AppTheme::Light);
}

// ---------------- PERSISTENCE ----------------
void ThemeManager::persist() {
    m_settings.setValue("ui/theme", currentThemeKey());
}

// ---------------- APPLY THEME ----------------
void ThemeManager::setTheme(AppTheme theme) {
    if (m_currentTheme == theme) return;

    m_currentTheme = theme;
    persist();
    emit themeChanged(theme);
}

// VERY IMPORTANT : Light theme = CLEAR stylesheet (use .ui)
void ThemeManager::applyTheme(QWidget *root) {
    if (!root) return;

    if (m_currentTheme == AppTheme::Light) {
        // ❗ Light Mode → No custom stylesheet → UI stays original (blue)
        root->setStyleSheet("");
        return;
    }

    // Dark Mode
    root->setStyleSheet(darkStyleSheet());
}

// ---------------- DARK MODE ----------------
QString ThemeManager::darkStyleSheet() const {
    return QString(
        "QWidget {"
        "  background-color: #0F1F2F;"
        "  color: #E6EEF5;"
        "  font-family:'Segoe UI'; font-size:14px;"
        "}"
        "QFrame#sidebarFrame {"
        "  background: qlineargradient(x1:0,y1:0,x2:0,y2:1, stop:0 #143854, stop:1 #0E2A42);"
        "  border-right:1px solid #1E3D56;"
        "}"
        "QPushButton {"
        "  background:#1F3A52; color:#E6EEF5;"
        "  border:none; border-radius:8px; padding:8px 14px;"
        "}"
        "QPushButton:hover { background:#295071; }"
        "QPushButton:checked { background:#4DA3FF; color:white; }"
        "QLineEdit, QComboBox {"
        "  background:#173042; border:1px solid #28516B;"
        "  border-radius:6px; padding:6px; color:#E6EEF5;"
        "}"
        "QScrollBar:vertical { width:12px; background:transparent; }"
        "QScrollBar::handle:vertical { background:#28516B; border-radius:6px; }"
        );
}

// (unused)
QString ThemeManager::buildCommonVariables() const { return ""; }
