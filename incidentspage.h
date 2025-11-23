#ifndef INCIDENTSPAGE_H
#define INCIDENTSPAGE_H

#include <QWidget>
#include <QtCharts>

QT_BEGIN_NAMESPACE
namespace Ui { class IncidentsPage; }
QT_END_NAMESPACE

class IncidentsPage : public QWidget
{
    Q_OBJECT
public:
    explicit IncidentsPage(QWidget *parent = nullptr);
    ~IncidentsPage();

private slots:
    void onSaveIncident();
    void onSearch();
    void onDeleteIncident();
    void onEditIncident();
    void onExportPdf();
    void onSortIndexChanged(int index);

private:
    void refreshList();
    void refreshStats();
    void setupChart();
    void clearForm();
    int getNextId();

    Ui::IncidentsPage *ui;
    int currentEditId = -1; // -1 = nouveau, >0 = modification
};

#endif // INCIDENTSPAGE_H
