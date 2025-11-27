#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QLabel>

#include "userrole.h"
#include "thememanager.h"
#include "languagemanager.h"

namespace Ui {
class SettingsPage;
}

class SettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsPage(UserRole userRole = UserRole::NONE,
                          const QString &currentUser = "",
                          QWidget *parent = nullptr);
    ~SettingsPage();

    void setUserRole(UserRole role);
    void setCurrentUser(const QString &user);
    void loadUserData();

signals:
    void themeChanged(const QString &themeName);
    void languageChanged(const QString &languageCode);
    void userProfileUpdated();
    void passwordChanged();

private slots:
    // Menu gauche
    void on_profileButton_clicked();
    void on_themeButton_clicked();
    void on_notificationsButton_clicked();
    void on_securityButton_clicked();
    void on_databaseButton_clicked();
    void on_aboutButton_clicked();

    // Boutons d’action
    void on_saveProfileButton_clicked();
    void on_changePasswordButton_clicked();
    void on_backupButton_clicked();
    void on_restoreButton_clicked();
    void on_clearDataButton_clicked();

    // Thème / Langue
    void on_lightThemeRadio_clicked();
    void on_darkThemeRadio_clicked();
    void on_languageComboBox_currentIndexChanged(int index);

private:
    Ui::SettingsPage *ui;
    UserRole m_userRole;
    QString  m_currentUser;
    QString  m_currentTheme;   // "Clair" ou "Sombre"

    // Init
    void setupUI();
    void setupConnections();

    // Logique selon rôle
    void updateUIForRole();
    void updateNotificationsForRole();
    void updateAboutSection();

    // Navigation entre pages
    void switchToPage(int index);
    void highlightMenuButton(QPushButton *button);
    void resetMenuButtons();

    // Thème / Messages
    void applyTheme(const QString &themeName);
    void showSuccess(const QString &message);
    void showError(const QString &message);
};

#endif // SETTINGSPAGE_H
