#ifndef TRANSPORTSPAGE_H
#define TRANSPORTSPAGE_H

#include <QWidget>

namespace Ui {
class TransportsPage;
}

class TransportsPage : public QWidget
{
    Q_OBJECT

public:
    explicit TransportsPage(QWidget *parent = nullptr);
    ~TransportsPage();

private slots:
    void on_addVehiculeButton_clicked();
    void on_searchButton_clicked();
    void on_sortComboBox_currentIndexChanged(int index);
    void on_modifyButton_clicked();
    void on_deleteButton_clicked();
    void on_exportPDFButton_clicked();
    void on_historiqueButton_clicked();

private:
    Ui::TransportsPage *ui;
    void setupTable();
    void loadVehicules();
    void refreshStatistics();
    void refreshFleetInfo();
    void setupChart();
    void sortVehiculesByType();
    void sortVehiculesByStatus();
};

#endif // TRANSPORTSPAGE_H
