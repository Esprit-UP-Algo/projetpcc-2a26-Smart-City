#ifndef TRANSACTIONDIALOG_H
#define TRANSACTIONDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QDateEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QVariantMap>

class TransactionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TransactionDialog(QWidget *parent = nullptr);
    explicit TransactionDialog(const QVariantMap &transactionData, QWidget *parent = nullptr);
    QVariantMap getTransactionData() const;

private:
    bool isEditMode;
    QLineEdit *codeEdit;
    QDoubleSpinBox *montantSpinBox;
    QDateEdit *dateEdit;
    QComboBox *typeCombo;
    QComboBox *categorieCombo;
    QLineEdit *descriptionEdit;
    QPushButton *saveButton;
    QPushButton *cancelButton;
    QLabel *errorLabel;

    void setupUi();
    void populateFields(const QVariantMap &data);

    // Fonctions désactivées
    void validateForm();
    void updateSaveButton();
    bool validateInput();
    void showError(const QString &);
    void clearError();

private slots:
    void onSaveClicked();   // 👈
};

#endif // TRANSACTIONDIALOG_H
