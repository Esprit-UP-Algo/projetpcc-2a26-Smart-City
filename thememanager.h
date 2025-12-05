#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QObject>
#include <QString>
#include <QPointer>
#include <QSettings>

enum class AppTheme { Light, Dark };

class ThemeManager : public QObject {
    Q_OBJECT
public:
    static ThemeManager &instance();

    AppTheme currentTheme() const { return m_currentTheme; }
    QString currentThemeKey() const { return m_currentTheme == AppTheme::Light ? "light" : "dark"; }

    void initialize(); // load from QSettings
    void setTheme(AppTheme theme); // change and persist
    void applyTheme(QWidget *root); // apply stylesheet to hierarchy root

    QString lightStyleSheet() const;  // exported for fine-grain usage
    QString darkStyleSheet() const;   // exported for fine-grain usage

signals:
    void themeChanged(AppTheme newTheme);

private:
    explicit ThemeManager(QObject *parent = nullptr);
    void persist();
    QString buildCommonVariables() const;

    AppTheme m_currentTheme;
    QSettings m_settings; // organization/app inferred from QCoreApplication::setOrganizationName
};

#endif // THEMEMANAGER_H
