#ifndef RESIDENTSPAGE_H
#define RESIDENTSPAGE_H

#include <QWidget>
#include <QVariant>
#include <QVariantMap>
#include <QList>

QT_BEGIN_NAMESPACE
namespace Ui { class ResidentsPage; }
QT_END_NAMESPACE

class ResidentsPage : public QWidget
{
    Q_OBJECT

public:
    explicit ResidentsPage(QWidget *parent = nullptr);
    ~ResidentsPage();

    // Controller can still inject data if you want
    void setResidents(const QList<QVariantMap> &rows);

signals:
    // Emitted at startup and when returning to list (optional)
    void refreshRequested();

private slots:
    // Navigation
    void onTabAdd();
    void onTabList();
    void onTabStats();

    // Form
    void onSaveForm();
    void onCancelForm();

    // List toolbar
    void onFilterTextChanged(const QString &text);
    void onSortIndexChanged(int index);

    // Row actions
    void onEditClicked();
    void onDeleteClicked();

    // Form validation
    void onCinTextChanged();
    void onEmailTextChanged();

private:
    // UI setup
    void setupTableHeaders();
    void setupUiBehavior();
    void setupValidation();

    // Helpers
    void clearForm();
    int  sortColumnForIndex(int index) const;

    // Validation methods
    bool validateForm();
    bool validateCin(const QString &cin);
    bool validateEmail(const QString &email);
    void showError(const QString &message);
    void hideError();

    // ✅ The missing declaration that fixes your build
    void loadResident(const QVariantMap &r);

    // Smart statistics calculation
    void calculateAndDisplaySatisfactionScore();
    QPair<int, QString> calculateSatisfactionScore();

private:
    Ui::ResidentsPage *ui;
};

#endif // RESIDENTSPAGE_H
