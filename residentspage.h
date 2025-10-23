#ifndef RESIDENTSPAGE_H
#define RESIDENTSPAGE_H

#include <QWidget>
#include <QVariantMap>

QT_BEGIN_NAMESPACE
namespace Ui { class ResidentsPage; }
QT_END_NAMESPACE

class ResidentsPage : public QWidget
{
    Q_OBJECT

public:
    explicit ResidentsPage(QWidget *parent = nullptr);
    ~ResidentsPage();

private slots:
    void onAddResidentClicked();
    void onEditResidentClicked();
    void onDeleteResidentClicked();
    void onSearchTextChanged(const QString &text);
    void onSortChanged(int index);
    void onExportPdfClicked();
    void onTableSelectionChanged();

private:
    Ui::ResidentsPage *ui;

    // Initialisation
    void setupConnections();
    void setupTable();
    void setupInitialData();

    // Gestion CRUD
    void loadResidents();
    void addResidentToTable(const QVariantMap &residentData);
    QVariantMap getResidentDataFromRow(int row);
    void updateTableRow(int row, const QVariantMap &residentData);

    // Statistiques
    void updateStatistics();
    int calculateNewResidentsThisMonth();
    double calculateSatisfactionScore();
};

#endif // RESIDENTSPAGE_H
