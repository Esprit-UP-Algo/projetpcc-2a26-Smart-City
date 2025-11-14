#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
class SettingsPage;
}

class SettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsPage(QWidget *parent = nullptr);
    ~SettingsPage();

private slots:
    void on_profileButton_clicked();
    void on_themeButton_clicked();
    void on_notificationsButton_clicked();
    void on_securityButton_clicked();
    void on_databaseButton_clicked();
    void on_aboutButton_clicked();
    
    void on_saveProfileButton_clicked();
    void on_changePasswordButton_clicked();
    void on_backupButton_clicked();
    void on_restoreButton_clicked();
    void on_clearDataButton_clicked();
    
    void on_lightThemeRadio_clicked();
    void on_darkThemeRadio_clicked();
    void on_languageComboBox_currentIndexChanged(int index);

private:
    Ui::SettingsPage *ui;
    
    void switchToPage(int index);
    void highlightMenuButton(QPushButton *button);
    void resetMenuButtons();
};

#endif // SETTINGSPAGE_H
