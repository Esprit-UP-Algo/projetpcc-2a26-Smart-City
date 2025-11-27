#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include "userrole.h"
class ThemeManager;
class LanguageManager;

class NexoraDashboard;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void on_loginButton_clicked();
    void on_fullscreenButton_clicked();
    void onLoginRequested();     // Called when Dashboard signals logout

private:
    Ui::MainWindow *ui;

    // Login system
    void performLogin();
    void showErrorMessage(const QString &message);
    void shakeWidget(QWidget *widget);

    // User database
    void setupUsers();
    bool validateCredentials(const QString &email, const QString &password);
    UserRole getUserRole(const QString &email) const;
    QString getUserDisplayName(const QString &email) const;

    // Dashboard workflow
    void openDashboardWithRole(UserRole role, const QString &userEmail, const QString &userName);

    bool isLoggingIn;
    QList<UserInfo> m_users;

    // Current dashboard instance
    NexoraDashboard *m_currentDashboard;

    // Theme/Language hooks
    void applyTheme();
    void reloadTranslations();
};

#endif // MAINWINDOW_H
