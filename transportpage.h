#ifndef TRANSPORTPAGE_H
#define TRANSPORTPAGE_H

#include <QWidget>
#include <QVariant>
#include <QVariantMap>
#include <QList>

QT_BEGIN_NAMESPACE
namespace Ui {
class TransportPage;
}
QT_END_NAMESPACE

class TransportPage : public QWidget
{
    Q_OBJECT

public:
    explicit TransportPage(QWidget *parent = nullptr);
    ~TransportPage();

    void setVehicules(const QList<QVariantMap> &rows);

signals:
    void refreshRequested();

private slots:
    void onTabAdd();
    void onTabList();
    void onTabStats();
    void onSaveForm();
    void onCancelForm();
    void onFilterTextChanged(const QString &text);
    void onSortIndexChanged(int index);
    void onExportPdf();
    void onEditClicked();
    void onDeleteClicked();
    void onIdTextChanged();
    void onCapaciteTextChanged();
    void onDataChanged();

private:
    void setupTableHeaders();
    void setupUiBehavior();
    void setupValidation();
    void clearForm();
    void showEmptyTable();
    void loadAllVehicules();
    int sortColumnForIndex(int index) const;
    bool validateForm();
    bool validateId(const QString &id);
    bool validateCapacite(const QString &capacite);
    void showError(const QString &message);
    void hideError();
    void loadVehicule(const QVariantMap &v);
    void calculateAndDisplayStatistics();
    void populateHistoriqueTable();

    // 🔥 AJOUT POUR LA COMBOBOX DES CODES FINANCIERS
    void loadFinancialCodes();
    void refreshCodeUniqueCombo();

private:
    Ui::TransportPage *ui;
};

#endif // TRANSPORTPAGE_H
