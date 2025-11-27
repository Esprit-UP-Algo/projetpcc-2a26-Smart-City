#ifndef NEXORADASHBOARD_H
#define NEXORADASHBOARD_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include "userrole.h"
class ThemeManager;
class LanguageManager;

QT_BEGIN_NAMESPACE
namespace Ui { class NexoraDashboard; }
QT_END_NAMESPACE

class ResidentsPage;
class FinancesPage;
class IncidentsPage;
class TransportPage;
class LocauxPage;
class SettingsPage;
class AdminDatabasePage;
class AdminDatabaseInfoPage;
class AdminFilesPage;

class NexoraDashboard : public QMainWindow
{
    Q_OBJECT

public:
    // FINAL → Only ONE valid constructor
    NexoraDashboard(UserRole userRole,
                    const QString &userEmail,
                    const QString &userName,
                    QWidget *parent = nullptr);

    ~NexoraDashboard();

signals:
    void loginRequested();

private slots:
    void onResidentsClicked();
    void onFinancesClicked();
    void onIncidentsClicked();
    void onTransportsClicked();
    void onLocauxClicked();
    void onSettingsClicked();
    void onAdminInfoClicked();
    void onAdminFilesClicked();
    void onLogoutClicked();

private:
    Ui::NexoraDashboard *ui;

    // User info
    UserRole m_userRole;
    QString m_userEmail;
    QString m_userName;

    // Pages
    QStackedWidget *pageStack;
    ResidentsPage *residentsPage;
    FinancesPage *financesPage;
    IncidentsPage *incidentsPage;
    TransportPage *transportsPage;
    LocauxPage *locauxPage;
    SettingsPage *settingsPage;
    AdminDatabasePage *adminDbPage;
    AdminDatabaseInfoPage *adminInfoPage;
    AdminFilesPage *adminFilesPage;

    // Animation
    QPropertyAnimation *pageTransition;
    QGraphicsOpacityEffect *pageOpacity;

    // Internal logic
    void setupUI();
    void setupNavigation();
    void setupPages();
    void setupAnimations();
    void setupRoleBasedAccess();
    void configureButtonVisibility();
    void showPage(int pageIndex);
    void returnToLogin();

    // Theme/Language
    void hookGlobalManagers();
    void applyTheme();
    void reloadTranslations();
    void onDatabaseClicked();
};

#endif // NEXORADASHBOARD_H
