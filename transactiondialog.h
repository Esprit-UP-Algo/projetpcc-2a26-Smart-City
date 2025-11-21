#ifndef TRANSACTIONDIALOG_H
#define TRANSACTIONDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QVariantMap>

class TransactionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TransactionDialog(QWidget *parent = nullptr);
    explicit TransactionDialog(const QVariantMap &transactionData, QWidget *parent = nullptr);
    
    QVariantMap getTransactionData() const;

private:
    void setupUi();
    void populateFields(const QVariantMap &data);
    bool validateInput();
    
    QLineEdit *codeEdit;
    QDoubleSpinBox *montantSpinBox;
    QDateEdit *dateEdit;
    QComboBox *typeCombo;
    QComboBox *categorieCombo;
    QLineEdit *descriptionEdit;
    
    QPushButton *saveButton;
    QPushButton *cancelButton;
    
    bool isEditMode;
};

#endif // TRANSACTIONDIALOG_H
