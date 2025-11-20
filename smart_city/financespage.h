#ifndef FINANCESPAGE_H
#define FINANCESPAGE_H

#include <QWidget>
#include <QVariantMap>
#include <QPair>
// (optionnel mais propre)
// #include <QList>

namespace Ui {
class FinancesPage;
}

class FinancesPage : public QWidget
{
    Q_OBJECT

public:
    explicit FinancesPage(QWidget *parent = nullptr);
    ~FinancesPage();

signals:
    void refreshRequested();

private slots:
    // Onglets
    void onTabAdd();
    void onTabList();
    void onTabStats();

    // Formulaire
    void onSaveForm();
    void onCancelForm();

    // Table
    void onEditClicked();
    void onDeleteClicked();

    // Recherche / Tri
    void onFilterTextChanged(const QString &text);
    void onSortIndexChanged(int index);

    // ⭐⭐ AJOUT POUR L’EXPORT PDF ⭐⭐
    void onExportPdfClicked();   // <-- important pour que le slot du .cpp soit reconnu

private:
    Ui::FinancesPage *ui;
    QString currentTransactionCode;

    // Fonctions internes
    void setupTableHeaders();
    void setupUiBehavior();
    void setupValidation();
    void reloadTransactions();

    void clearForm();
    void setTransactions(const QList<QVariantMap> &rows);
    void loadTransaction(const QVariantMap &transaction);

    bool validateForm();
    void showError(const QString &message);
    void hideError();

    // Analyse et statistiques
    QPair<int, QString> calculateFinancialScore(double revenues, double expenses);
    void calculateAndDisplayScore();
    int sortColumnForIndex(int index) const;

    // 🔥 AJOUTS NECESSAIRES (sans modifier la structure)
    void loadResidentCINs();      // charge les CIN dans la ComboBox
    void refreshCINCombo();       // tu peux l’implémenter plus tard si besoin
};

#endif // FINANCESPAGE_H
