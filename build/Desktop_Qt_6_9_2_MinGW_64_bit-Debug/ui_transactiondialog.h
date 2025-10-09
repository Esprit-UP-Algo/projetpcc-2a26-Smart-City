/********************************************************************************
** Form generated from reading UI file 'transactiondialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRANSACTIONDIALOG_H
#define UI_TRANSACTIONDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_TransactionDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *dialogTitle;
    QFormLayout *formLayout;
    QLabel *codeFieldLabel;
    QLineEdit *codeEdit;
    QLabel *montantFieldLabel;
    QDoubleSpinBox *montantSpinBox;
    QLabel *dateFieldLabel;
    QDateEdit *dateEdit;
    QLabel *typeFieldLabel;
    QComboBox *typeCombo;
    QLabel *categorieFieldLabel;
    QComboBox *categorieCombo;
    QLabel *modePaiementFieldLabel;
    QComboBox *modePaiementCombo;
    QHBoxLayout *buttonLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelButton;
    QPushButton *saveButton;

    void setupUi(QDialog *TransactionDialog)
    {
        if (TransactionDialog->objectName().isEmpty())
            TransactionDialog->setObjectName("TransactionDialog");
        TransactionDialog->resize(600, 1000);
        TransactionDialog->setStyleSheet(QString::fromUtf8("background-color: white;"));
        verticalLayout = new QVBoxLayout(TransactionDialog);
        verticalLayout->setSpacing(25);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(40, 40, 40, 40);
        dialogTitle = new QLabel(TransactionDialog);
        dialogTitle->setObjectName("dialogTitle");
        dialogTitle->setStyleSheet(QString::fromUtf8("font-size: 24px; font-weight: bold; color: #2C3E50;"));

        verticalLayout->addWidget(dialogTitle);

        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        formLayout->setLabelAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);
        formLayout->setHorizontalSpacing(20);
        formLayout->setVerticalSpacing(18);
        codeFieldLabel = new QLabel(TransactionDialog);
        codeFieldLabel->setObjectName("codeFieldLabel");
        codeFieldLabel->setStyleSheet(QString::fromUtf8("font-size: 15px; font-weight: bold; color: #2C3E50;"));

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, codeFieldLabel);

        codeEdit = new QLineEdit(TransactionDialog);
        codeEdit->setObjectName("codeEdit");
        codeEdit->setMinimumSize(QSize(0, 45));
        codeEdit->setStyleSheet(QString::fromUtf8("padding: 12px; border: 2px solid #BDC3C7; border-radius: 8px; font-size: 15px;"));

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, codeEdit);

        montantFieldLabel = new QLabel(TransactionDialog);
        montantFieldLabel->setObjectName("montantFieldLabel");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, montantFieldLabel);

        montantSpinBox = new QDoubleSpinBox(TransactionDialog);
        montantSpinBox->setObjectName("montantSpinBox");
        montantSpinBox->setMinimumSize(QSize(0, 45));
        montantSpinBox->setStyleSheet(QString::fromUtf8("padding: 12px; border: 2px solid #BDC3C7; border-radius: 8px; font-size: 15px;"));
        montantSpinBox->setDecimals(2);
        montantSpinBox->setMaximum(1000000.000000000000000);

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, montantSpinBox);

        dateFieldLabel = new QLabel(TransactionDialog);
        dateFieldLabel->setObjectName("dateFieldLabel");

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, dateFieldLabel);

        dateEdit = new QDateEdit(TransactionDialog);
        dateEdit->setObjectName("dateEdit");
        dateEdit->setMinimumSize(QSize(0, 45));
        dateEdit->setStyleSheet(QString::fromUtf8("padding: 12px; border: 2px solid #BDC3C7; border-radius: 8px; font-size: 15px;"));
        dateEdit->setCalendarPopup(true);

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, dateEdit);

        typeFieldLabel = new QLabel(TransactionDialog);
        typeFieldLabel->setObjectName("typeFieldLabel");

        formLayout->setWidget(3, QFormLayout::ItemRole::LabelRole, typeFieldLabel);

        typeCombo = new QComboBox(TransactionDialog);
        typeCombo->addItem(QString());
        typeCombo->addItem(QString());
        typeCombo->setObjectName("typeCombo");
        typeCombo->setMinimumSize(QSize(0, 45));
        typeCombo->setStyleSheet(QString::fromUtf8("padding: 12px; border: 2px solid #BDC3C7; border-radius: 8px; font-size: 15px;"));

        formLayout->setWidget(3, QFormLayout::ItemRole::FieldRole, typeCombo);

        categorieFieldLabel = new QLabel(TransactionDialog);
        categorieFieldLabel->setObjectName("categorieFieldLabel");

        formLayout->setWidget(4, QFormLayout::ItemRole::LabelRole, categorieFieldLabel);

        categorieCombo = new QComboBox(TransactionDialog);
        categorieCombo->addItem(QString());
        categorieCombo->addItem(QString());
        categorieCombo->addItem(QString());
        categorieCombo->addItem(QString());
        categorieCombo->addItem(QString());
        categorieCombo->setObjectName("categorieCombo");
        categorieCombo->setMinimumSize(QSize(0, 45));
        categorieCombo->setStyleSheet(QString::fromUtf8("padding: 12px; border: 2px solid #BDC3C7; border-radius: 8px; font-size: 15px;"));

        formLayout->setWidget(4, QFormLayout::ItemRole::FieldRole, categorieCombo);

        modePaiementFieldLabel = new QLabel(TransactionDialog);
        modePaiementFieldLabel->setObjectName("modePaiementFieldLabel");

        formLayout->setWidget(5, QFormLayout::ItemRole::LabelRole, modePaiementFieldLabel);

        modePaiementCombo = new QComboBox(TransactionDialog);
        modePaiementCombo->addItem(QString());
        modePaiementCombo->addItem(QString());
        modePaiementCombo->addItem(QString());
        modePaiementCombo->addItem(QString());
        modePaiementCombo->setObjectName("modePaiementCombo");
        modePaiementCombo->setMinimumSize(QSize(0, 45));
        modePaiementCombo->setStyleSheet(QString::fromUtf8("padding: 12px; border: 2px solid #BDC3C7; border-radius: 8px; font-size: 15px;"));

        formLayout->setWidget(5, QFormLayout::ItemRole::FieldRole, modePaiementCombo);


        verticalLayout->addLayout(formLayout);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setSpacing(15);
        buttonLayout->setObjectName("buttonLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonLayout->addItem(horizontalSpacer);

        cancelButton = new QPushButton(TransactionDialog);
        cancelButton->setObjectName("cancelButton");
        cancelButton->setMinimumSize(QSize(120, 50));
        cancelButton->setStyleSheet(QString::fromUtf8("background-color: #95A5A6; color: white; border: none; border-radius: 10px; font-size: 15px; font-weight: bold; padding: 0 35px;"));

        buttonLayout->addWidget(cancelButton);

        saveButton = new QPushButton(TransactionDialog);
        saveButton->setObjectName("saveButton");
        saveButton->setMinimumSize(QSize(140, 50));
        saveButton->setStyleSheet(QString::fromUtf8("background-color: #4A90E2; color: white; border: none; border-radius: 10px; font-size: 15px; font-weight: bold; padding: 0 35px;"));

        buttonLayout->addWidget(saveButton);


        verticalLayout->addLayout(buttonLayout);


        retranslateUi(TransactionDialog);
        QObject::connect(cancelButton, &QPushButton::clicked, TransactionDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(TransactionDialog);
    } // setupUi

    void retranslateUi(QDialog *TransactionDialog)
    {
        TransactionDialog->setWindowTitle(QCoreApplication::translate("TransactionDialog", "Transaction", nullptr));
        dialogTitle->setText(QCoreApplication::translate("TransactionDialog", "Nouvelle Transaction", nullptr));
        codeFieldLabel->setText(QCoreApplication::translate("TransactionDialog", "Code *:", nullptr));
        codeEdit->setPlaceholderText(QCoreApplication::translate("TransactionDialog", "Ex: TRN001", nullptr));
        montantFieldLabel->setText(QCoreApplication::translate("TransactionDialog", "Montant *:", nullptr));
        montantSpinBox->setSuffix(QCoreApplication::translate("TransactionDialog", " DT", nullptr));
        dateFieldLabel->setText(QCoreApplication::translate("TransactionDialog", "Date *:", nullptr));
        dateEdit->setDisplayFormat(QCoreApplication::translate("TransactionDialog", "dd/MM/yyyy", nullptr));
        typeFieldLabel->setText(QCoreApplication::translate("TransactionDialog", "Type *:", nullptr));
        typeCombo->setItemText(0, QCoreApplication::translate("TransactionDialog", "Entree", nullptr));
        typeCombo->setItemText(1, QCoreApplication::translate("TransactionDialog", "Sortie", nullptr));

        categorieFieldLabel->setText(QCoreApplication::translate("TransactionDialog", "Cat\303\251gorie *:", nullptr));
        categorieCombo->setItemText(0, QCoreApplication::translate("TransactionDialog", "Loyer", nullptr));
        categorieCombo->setItemText(1, QCoreApplication::translate("TransactionDialog", "Charges", nullptr));
        categorieCombo->setItemText(2, QCoreApplication::translate("TransactionDialog", "Services", nullptr));
        categorieCombo->setItemText(3, QCoreApplication::translate("TransactionDialog", "Maintenance", nullptr));
        categorieCombo->setItemText(4, QCoreApplication::translate("TransactionDialog", "Autre", nullptr));

        modePaiementFieldLabel->setText(QCoreApplication::translate("TransactionDialog", "Mode de paiement *:", nullptr));
        modePaiementCombo->setItemText(0, QCoreApplication::translate("TransactionDialog", "Especes", nullptr));
        modePaiementCombo->setItemText(1, QCoreApplication::translate("TransactionDialog", "Carte", nullptr));
        modePaiementCombo->setItemText(2, QCoreApplication::translate("TransactionDialog", "Virement", nullptr));
        modePaiementCombo->setItemText(3, QCoreApplication::translate("TransactionDialog", "Cheque", nullptr));

        cancelButton->setText(QCoreApplication::translate("TransactionDialog", "Annuler", nullptr));
        saveButton->setText(QCoreApplication::translate("TransactionDialog", "Enregistrer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TransactionDialog: public Ui_TransactionDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRANSACTIONDIALOG_H
