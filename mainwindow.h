#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "userrole.h"     // contains struct UserInfo + enum UserRole

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class NexoraDashboard;

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
    void onLoginRequested();

private:
    Ui::MainWindow *ui;
    bool isLoggingIn;
    NexoraDashboard *m_currentDashboard;

    QList<UserInfo> m_users;   // uses struct from userrole.h
    int m_lastServiceId;       // last SERVICE_ID read from Oracle

    // --- Login flow ---
    void performLogin();
    bool validateCredentials(const QString &username, const QString &password);
    UserRole getUserRole(const QString &username) const;
    QString getUserDisplayName(const QString &username) const;
    void openDashboardWithRole(UserRole role,
                               const QString &userEmail,
                               const QString &userName);

    // --- UI helpers ---
    void setupUsers();
    void showErrorMessage(const QString &message);
    void shakeWidget(QWidget *widget);
    void applyTheme();
    void reloadTranslations();

    // Map SERVICE_ID → UserRole
    UserRole roleFromServiceId(int serviceId) const;
};

#endif // MAINWINDOW_H
