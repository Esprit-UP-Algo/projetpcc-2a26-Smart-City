#ifndef DASHBOARDWINDOW_H
#define DASHBOARDWINDOW_H

#include <QMainWindow>
#include <QGraphicsOpacityEffect>
#include <QStackedWidget>

class ResidentsPage;
class FinancesPage;
class IncidentsPage;
class TransportsPage;
class LocauxPage;
class SettingsPage;

namespace Ui {
class DashboardWindow;
}

class DashboardWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DashboardWindow(QWidget *parent = nullptr);
    ~DashboardWindow();

private slots:
    void on_dashboardButton_clicked();
    void on_residentsButton_clicked();
    void on_financesButton_clicked();
    void on_transportsButton_clicked();
    void on_incidentsButton_clicked();
    void on_locauxButton_clicked();
    void on_settingsButton_clicked();
    void on_exportPdfButton_clicked();
    void on_exportCsvButton_clicked();
    void on_notificationsButton_clicked();
    void on_searchButton_clicked();
    void on_exitButton_clicked();
    void on_searchLineEdit_returnPressed();

private:
    Ui::DashboardWindow *ui;
    QGraphicsOpacityEffect *contentOpacity;
    QStackedWidget *stackedWidget;
    ResidentsPage *residentsPage;
    FinancesPage *financesPage;
    IncidentsPage *incidentsPage;
    TransportsPage *transportsPage;
    LocauxPage *locauxPage;
    SettingsPage *settingsPage;
    
    void animatePageTransition(const QString &newTitle);
    void setupPages();
};

#endif // DASHBOARDWINDOW_H
