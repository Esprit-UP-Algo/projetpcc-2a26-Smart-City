#ifndef RESIDENTDIALOG_H
#define RESIDENTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDateEdit>
#include <QPushButton>
#include <QVariantMap>

class ResidentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResidentDialog(QWidget *parent = nullptr, const QVariantMap &residentData = QVariantMap());
    ~ResidentDialog();

    QVariantMap getResidentData() const;
    bool isEditMode() const { return editMode; }

private slots:
    void onSaveClicked();
    void onCancelClicked();

private:
    void setupUI();
    void loadResidentData(const QVariantMap &data);
    bool validateForm();

    // Form fields
    QLineEdit *cinEdit;
    QLineEdit *nomEdit;
    QLineEdit *prenomEdit;
    QComboBox *sexeCombo;
    QLineEdit *telephoneEdit;
    QLineEdit *emailEdit;
    QLineEdit *appartementEdit;
    QSpinBox *etageSpinBox;
    QComboBox *statutCombo;
    QDateEdit *dateEntreeEdit;

    QPushButton *saveButton;
    QPushButton *cancelButton;

    bool editMode;
    QVariantMap residentData;
};

#endif // RESIDENTDIALOG_H
