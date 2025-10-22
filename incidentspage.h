#ifndef INCIDENTSPAGE_H
#define INCIDENTSPAGE_H

#include <QWidget>

namespace Ui {
class IncidentsPage;
}

class IncidentsPage : public QWidget
{
    Q_OBJECT

public:
    explicit IncidentsPage(QWidget *parent = nullptr);
    ~IncidentsPage();

private slots:
    void on_addIncidentButton_clicked();
    void on_searchButton_clicked();
    void on_sortComboBox_currentIndexChanged(int index);
    void on_modifyButton_clicked();
    void on_deleteButton_clicked();
    void on_exportPDFButton_clicked();

private:
    Ui::IncidentsPage *ui;

    void setupTable();
    void loadIncidents(const QString &term = QString());
    void refreshStatistics();
    void refreshDashboard();
    void setupChart();
};

#endif // INCIDENTSPAGE_H
