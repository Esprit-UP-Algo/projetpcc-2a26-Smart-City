#ifndef DASHBOARDWINDOW_H
#define DASHBOARDWINDOW_H

#include <QMainWindow>
#include <QGraphicsOpacityEffect>
#include <QStackedWidget>

class IncidentsPage;
class TransportsPage;
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
    void on_incidentsButton_clicked();
    void on_transportsButton_clicked();
    void on_settingsButton_clicked();
    void on_exportPdfButton_clicked();
    void on_exportCsvButton_clicked();
    void on_notificationsButton_clicked();
    void on_exitButton_clicked();

private:
    Ui::DashboardWindow *ui;
    QGraphicsOpacityEffect *contentOpacity;
    QStackedWidget *stackedWidget;
    IncidentsPage *incidentsPage;
    TransportsPage *transportsPage;
    SettingsPage *settingsPage;

    void animatePageTransition(const QString &newTitle);
    void setupPages();
};

#endif // DASHBOARDWINDOW_H
