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
    
    void setupConnections();
    void setupTable();
    void setupInitialData();
    void updateSatisfactionScore();
    void addResidentToTable(const QVariantMap &residentData);
    QVariantMap getResidentDataFromRow(int row);
    void updateTableRow(int row, const QVariantMap &residentData);
};

#endif // RESIDENTSPAGE_H