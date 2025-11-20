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
    void showEmptyTable();
    int sortColumnForIndex(int index) const;

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
    void onAfficherStatistiques();
    void onHoraireTextChanged();
    void onHistoriqueClicked();

private:
    void setupTableHeaders();
    void setupUiBehavior();
    void setupValidation();
    void clearForm();
    void loadAllVehicules();
    bool validateForm();
    bool validateId(const QString &id);
    bool validateCapacite(const QString &capacite);
    bool validateHoraire(const QString &horaire);
    void showError(const QString &message);
    void hideError();
    void loadVehicule(const QVariantMap &v);
    void calculateAndDisplayStatistics();
    double calculerKilometrage(const QString& type, double tempsUtilise) const;
    void calculerPourcentagesKilometrage();
    void afficherGraphiqueKilometrage();
    void afficherHistoriqueComplet();

    // NOUVELLES méthodes pour l'historique SQL
    void sauvegarderHistoriqueSQL(const QVariantMap &vehicule, const QString &action, const QString &notes = "");
    QList<QVariantMap> getHistoriqueCompletSQL();
    QString detecterModifications(const QVariantMap &ancien, const QVariantMap &nouveau);
private:
    Ui::TransportPage *ui;
};

#endif // TRANSPORTPAGE_H
