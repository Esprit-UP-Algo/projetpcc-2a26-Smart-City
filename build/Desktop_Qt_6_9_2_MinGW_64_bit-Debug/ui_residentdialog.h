/********************************************************************************
** Form generated from reading UI file 'residentdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RESIDENTDIALOG_H
#define UI_RESIDENTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ResidentDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *dialogTitle;
    QFormLayout *formLayout;
    QLabel *cinFieldLabel;
    QLineEdit *cinEdit;
    QLabel *nomFieldLabel;
    QLineEdit *nomEdit;
    QLabel *prenomFieldLabel;
    QLineEdit *prenomEdit;
    QLabel *sexeFieldLabel;
    QComboBox *sexeCombo;
    QLabel *telephoneFieldLabel;
    QLineEdit *telephoneEdit;
    QLabel *emailFieldLabel;
    QLineEdit *emailEdit;
    QLabel *appartementFieldLabel;
    QLineEdit *appartementEdit;
    QLabel *etageFieldLabel;
    QSpinBox *etageSpinBox;
    QLabel *statutFieldLabel;
    QComboBox *statutCombo;
    QLabel *dateEntreeFieldLabel;
    QDateEdit *dateEntreeEdit;
    QHBoxLayout *buttonLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelButton;
    QPushButton *saveButton;

    void setupUi(QDialog *ResidentDialog)
    {
        if (ResidentDialog->objectName().isEmpty())
            ResidentDialog->setObjectName("ResidentDialog");
        ResidentDialog->resize(1200, 800);
        ResidentDialog->setStyleSheet(QString::fromUtf8("background-color: white;"));
        verticalLayout = new QVBoxLayout(ResidentDialog);
        verticalLayout->setSpacing(20);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(30, 30, 30, 30);
        dialogTitle = new QLabel(ResidentDialog);
        dialogTitle->setObjectName("dialogTitle");
        dialogTitle->setStyleSheet(QString::fromUtf8("font-size: 20px; font-weight: bold; color: #2C3E50;"));

        verticalLayout->addWidget(dialogTitle);

        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        formLayout->setHorizontalSpacing(15);
        formLayout->setVerticalSpacing(15);
        formLayout->setLabelAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        cinFieldLabel = new QLabel(ResidentDialog);
        cinFieldLabel->setObjectName("cinFieldLabel");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, cinFieldLabel);

        cinEdit = new QLineEdit(ResidentDialog);
        cinEdit->setObjectName("cinEdit");
        cinEdit->setStyleSheet(QString::fromUtf8("padding: 10px; border: 1px solid #BDC3C7; border-radius: 8px; font-size: 14px;"));

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, cinEdit);

        nomFieldLabel = new QLabel(ResidentDialog);
        nomFieldLabel->setObjectName("nomFieldLabel");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, nomFieldLabel);

        nomEdit = new QLineEdit(ResidentDialog);
        nomEdit->setObjectName("nomEdit");
        nomEdit->setStyleSheet(QString::fromUtf8("padding: 10px; border: 1px solid #BDC3C7; border-radius: 8px; font-size: 14px;"));

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, nomEdit);

        prenomFieldLabel = new QLabel(ResidentDialog);
        prenomFieldLabel->setObjectName("prenomFieldLabel");

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, prenomFieldLabel);

        prenomEdit = new QLineEdit(ResidentDialog);
        prenomEdit->setObjectName("prenomEdit");
        prenomEdit->setStyleSheet(QString::fromUtf8("padding: 10px; border: 1px solid #BDC3C7; border-radius: 8px; font-size: 14px;"));

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, prenomEdit);

        sexeFieldLabel = new QLabel(ResidentDialog);
        sexeFieldLabel->setObjectName("sexeFieldLabel");

        formLayout->setWidget(3, QFormLayout::ItemRole::LabelRole, sexeFieldLabel);

        sexeCombo = new QComboBox(ResidentDialog);
        sexeCombo->addItem(QString());
        sexeCombo->addItem(QString());
        sexeCombo->setObjectName("sexeCombo");
        sexeCombo->setStyleSheet(QString::fromUtf8("padding: 10px; border: 1px solid #BDC3C7; border-radius: 8px; font-size: 14px;"));

        formLayout->setWidget(3, QFormLayout::ItemRole::FieldRole, sexeCombo);

        telephoneFieldLabel = new QLabel(ResidentDialog);
        telephoneFieldLabel->setObjectName("telephoneFieldLabel");

        formLayout->setWidget(4, QFormLayout::ItemRole::LabelRole, telephoneFieldLabel);

        telephoneEdit = new QLineEdit(ResidentDialog);
        telephoneEdit->setObjectName("telephoneEdit");
        telephoneEdit->setStyleSheet(QString::fromUtf8("padding: 10px; border: 1px solid #BDC3C7; border-radius: 8px; font-size: 14px;"));

        formLayout->setWidget(4, QFormLayout::ItemRole::FieldRole, telephoneEdit);

        emailFieldLabel = new QLabel(ResidentDialog);
        emailFieldLabel->setObjectName("emailFieldLabel");

        formLayout->setWidget(5, QFormLayout::ItemRole::LabelRole, emailFieldLabel);

        emailEdit = new QLineEdit(ResidentDialog);
        emailEdit->setObjectName("emailEdit");
        emailEdit->setStyleSheet(QString::fromUtf8("padding: 10px; border: 1px solid #BDC3C7; border-radius: 8px; font-size: 14px;"));

        formLayout->setWidget(5, QFormLayout::ItemRole::FieldRole, emailEdit);

        appartementFieldLabel = new QLabel(ResidentDialog);
        appartementFieldLabel->setObjectName("appartementFieldLabel");

        formLayout->setWidget(6, QFormLayout::ItemRole::LabelRole, appartementFieldLabel);

        appartementEdit = new QLineEdit(ResidentDialog);
        appartementEdit->setObjectName("appartementEdit");
        appartementEdit->setStyleSheet(QString::fromUtf8("padding: 10px; border: 1px solid #BDC3C7; border-radius: 8px; font-size: 14px;"));

        formLayout->setWidget(6, QFormLayout::ItemRole::FieldRole, appartementEdit);

        etageFieldLabel = new QLabel(ResidentDialog);
        etageFieldLabel->setObjectName("etageFieldLabel");

        formLayout->setWidget(7, QFormLayout::ItemRole::LabelRole, etageFieldLabel);

        etageSpinBox = new QSpinBox(ResidentDialog);
        etageSpinBox->setObjectName("etageSpinBox");
        etageSpinBox->setMinimum(0);
        etageSpinBox->setMaximum(20);
        etageSpinBox->setStyleSheet(QString::fromUtf8("padding: 10px; border: 1px solid #BDC3C7; border-radius: 8px; font-size: 14px;"));

        formLayout->setWidget(7, QFormLayout::ItemRole::FieldRole, etageSpinBox);

        statutFieldLabel = new QLabel(ResidentDialog);
        statutFieldLabel->setObjectName("statutFieldLabel");

        formLayout->setWidget(8, QFormLayout::ItemRole::LabelRole, statutFieldLabel);

        statutCombo = new QComboBox(ResidentDialog);
        statutCombo->addItem(QString());
        statutCombo->addItem(QString());
        statutCombo->setObjectName("statutCombo");
        statutCombo->setStyleSheet(QString::fromUtf8("padding: 10px; border: 1px solid #BDC3C7; border-radius: 8px; font-size: 14px;"));

        formLayout->setWidget(8, QFormLayout::ItemRole::FieldRole, statutCombo);

        dateEntreeFieldLabel = new QLabel(ResidentDialog);
        dateEntreeFieldLabel->setObjectName("dateEntreeFieldLabel");

        formLayout->setWidget(9, QFormLayout::ItemRole::LabelRole, dateEntreeFieldLabel);

        dateEntreeEdit = new QDateEdit(ResidentDialog);
        dateEntreeEdit->setObjectName("dateEntreeEdit");
        dateEntreeEdit->setCalendarPopup(true);
        dateEntreeEdit->setStyleSheet(QString::fromUtf8("padding: 10px; border: 1px solid #BDC3C7; border-radius: 8px; font-size: 14px;"));

        formLayout->setWidget(9, QFormLayout::ItemRole::FieldRole, dateEntreeEdit);


        verticalLayout->addLayout(formLayout);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setSpacing(15);
        buttonLayout->setObjectName("buttonLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonLayout->addItem(horizontalSpacer);

        cancelButton = new QPushButton(ResidentDialog);
        cancelButton->setObjectName("cancelButton");
        cancelButton->setMinimumSize(QSize(0, 45));
        cancelButton->setStyleSheet(QString::fromUtf8("background-color: #95A5A6; color: white; border: none; border-radius: 8px; font-size: 14px; font-weight: bold; padding: 0 30px;"));

        buttonLayout->addWidget(cancelButton);

        saveButton = new QPushButton(ResidentDialog);
        saveButton->setObjectName("saveButton");
        saveButton->setMinimumSize(QSize(0, 45));
        saveButton->setStyleSheet(QString::fromUtf8("background-color: #4A90E2; color: white; border: none; border-radius: 8px; font-size: 14px; font-weight: bold; padding: 0 30px;"));

        buttonLayout->addWidget(saveButton);


        verticalLayout->addLayout(buttonLayout);


        retranslateUi(ResidentDialog);
        QObject::connect(cancelButton, &QPushButton::clicked, ResidentDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(ResidentDialog);
    } // setupUi

    void retranslateUi(QDialog *ResidentDialog)
    {
        ResidentDialog->setWindowTitle(QCoreApplication::translate("ResidentDialog", "R\303\251sident", nullptr));
        dialogTitle->setText(QCoreApplication::translate("ResidentDialog", "Ajouter un R\303\251sident", nullptr));
        cinFieldLabel->setText(QCoreApplication::translate("ResidentDialog", "CIN *:", nullptr));
        cinEdit->setPlaceholderText(QCoreApplication::translate("ResidentDialog", "Ex: 12345678", nullptr));
        nomFieldLabel->setText(QCoreApplication::translate("ResidentDialog", "Nom *:", nullptr));
        nomEdit->setPlaceholderText(QCoreApplication::translate("ResidentDialog", "Ex: Dupont", nullptr));
        prenomFieldLabel->setText(QCoreApplication::translate("ResidentDialog", "Pr\303\251nom *:", nullptr));
        prenomEdit->setPlaceholderText(QCoreApplication::translate("ResidentDialog", "Ex: Jean", nullptr));
        sexeFieldLabel->setText(QCoreApplication::translate("ResidentDialog", "Sexe *:", nullptr));
        sexeCombo->setItemText(0, QCoreApplication::translate("ResidentDialog", "Homme", nullptr));
        sexeCombo->setItemText(1, QCoreApplication::translate("ResidentDialog", "Femme", nullptr));

        telephoneFieldLabel->setText(QCoreApplication::translate("ResidentDialog", "T\303\251l\303\251phone *:", nullptr));
        telephoneEdit->setPlaceholderText(QCoreApplication::translate("ResidentDialog", "Ex: +216 12 345 678", nullptr));
        emailFieldLabel->setText(QCoreApplication::translate("ResidentDialog", "Email *:", nullptr));
        emailEdit->setPlaceholderText(QCoreApplication::translate("ResidentDialog", "Ex: jean.dupont@email.com", nullptr));
        appartementFieldLabel->setText(QCoreApplication::translate("ResidentDialog", "Appartement *:", nullptr));
        appartementEdit->setPlaceholderText(QCoreApplication::translate("ResidentDialog", "Ex: A101", nullptr));
        etageFieldLabel->setText(QCoreApplication::translate("ResidentDialog", "\303\211tage *:", nullptr));
        statutFieldLabel->setText(QCoreApplication::translate("ResidentDialog", "Statut *:", nullptr));
        statutCombo->setItemText(0, QCoreApplication::translate("ResidentDialog", "Proprietaire", nullptr));
        statutCombo->setItemText(1, QCoreApplication::translate("ResidentDialog", "Locataire", nullptr));

        dateEntreeFieldLabel->setText(QCoreApplication::translate("ResidentDialog", "Date d'entr\303\251e *:", nullptr));
        dateEntreeEdit->setDisplayFormat(QCoreApplication::translate("ResidentDialog", "dd/MM/yyyy", nullptr));
        cancelButton->setText(QCoreApplication::translate("ResidentDialog", "Annuler", nullptr));
        saveButton->setText(QCoreApplication::translate("ResidentDialog", "Enregistrer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ResidentDialog: public Ui_ResidentDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RESIDENTDIALOG_H
