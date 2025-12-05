#ifndef NEXORADASHBOARD_H
#define NEXORADASHBOARD_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include "userrole.h"

QT_BEGIN_NAMESPACE
namespace Ui { class NexoraDashboard; }
QT_END_NAMESPACE

class ResidentsPage;
class FinancesPage;
class IncidentsPage;
class TransportPage;
class LocauxPage;
class SettingsPage;
class AdminDatabaseInfoPage;
class AdminFilesPage;

class NexoraDashboard : public QMainWindow
{
    Q_OBJECT

public:
    explicit NexoraDashboard(UserRole userRole,
                             const QString &userEmail,
                             const QString &userName,
                             QWidget *parent = nullptr);

    ~NexoraDashboard();

signals:
    void loginRequested();

private slots:
    // Sidebar navigation
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
    AdminDatabaseInfoPage *adminInfoPage;
    AdminFilesPage *adminFilesPage;

    // Page transitions
    QPropertyAnimation *pageTransition;
    QGraphicsOpacityEffect *pageOpacity;

    // Initialization routines
    void setupUI();
    void setupNavigation();
    void setupPages();
    void setupAnimations();
    void setupRoleBasedAccess();
    void configureButtonVisibility();
    void updateUserDisplayInfo();
    void showPage(int pageIndex);
    void returnToLogin();

    // Theme / Language
    void hookGlobalManagers();
    void applyTheme();
    void reloadTranslations();
};

#endif // NEXORADASHBOARD_H
