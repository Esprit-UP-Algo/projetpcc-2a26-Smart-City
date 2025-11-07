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
#include <QtWidgets/QFrame>
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
    QVBoxLayout *mainLayout;
    QFrame *headerFrame;
    QVBoxLayout *headerLayout;
    QLabel *titleLabel;
    QLabel *subtitleLabel;
    QFrame *formFrame;
    QHBoxLayout *columnsLayout;
    QFormLayout *leftFormLayout;
    QLabel *labelCin;
    QHBoxLayout *cinLayout;
    QLineEdit *cinEdit;
    QLabel *cinRequired;
    QLabel *labelNom;
    QHBoxLayout *nomLayout;
    QLineEdit *nomEdit;
    QLabel *nomRequired;
    QLabel *labelPrenom;
    QHBoxLayout *prenomLayout;
    QLineEdit *prenomEdit;
    QLabel *prenomRequired;
    QLabel *labelSexe;
    QHBoxLayout *sexeLayout;
    QComboBox *sexeCombo;
    QLabel *sexeRequired;
    QFormLayout *rightFormLayout;
    QLabel *labelTelephone;
    QLineEdit *telephoneEdit;
    QLabel *labelEmail;
    QLineEdit *emailEdit;
    QLabel *labelAppartement;
    QHBoxLayout *appartementLayout;
    QLineEdit *appartementEdit;
    QLabel *appartementRequired;
    QLabel *labelEtage;
    QSpinBox *etageSpinBox;
    QFrame *dateButtonFrame;
    QHBoxLayout *dateButtonLayout;
    QLabel *labelDateEntree;
    QDateEdit *dateEntreeEdit;
    QLabel *dateRequired;
    QSpacerItem *dateSpacer;
    QPushButton *cancelButton;
    QPushButton *saveButton;

    void setupUi(QDialog *ResidentDialog)
    {
        if (ResidentDialog->objectName().isEmpty())
            ResidentDialog->setObjectName("ResidentDialog");
        ResidentDialog->resize(750, 400);
        ResidentDialog->setStyleSheet(QString::fromUtf8("background-color: #F8FAFC; font-family: 'Segoe UI', Arial, sans-serif;"));
        mainLayout = new QVBoxLayout(ResidentDialog);
        mainLayout->setSpacing(15);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(20, 20, 20, 20);
        headerFrame = new QFrame(ResidentDialog);
        headerFrame->setObjectName("headerFrame");
        headerFrame->setFrameShape(QFrame::NoFrame);
        headerFrame->setStyleSheet(QString::fromUtf8("background-color: white; border-radius: 10px; padding: 15px; box-shadow: 0 2px 8px rgba(0,0,0,0.1);"));
        headerLayout = new QVBoxLayout(headerFrame);
        headerLayout->setSpacing(5);
        headerLayout->setObjectName("headerLayout");
        titleLabel = new QLabel(headerFrame);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setStyleSheet(QString::fromUtf8("font-size: 18px; font-weight: bold; color: #2C3E50;"));
        titleLabel->setAlignment(Qt::AlignCenter);

        headerLayout->addWidget(titleLabel);

        subtitleLabel = new QLabel(headerFrame);
        subtitleLabel->setObjectName("subtitleLabel");
        subtitleLabel->setStyleSheet(QString::fromUtf8("font-size: 11px; color: #6B7280; font-style: italic;"));
        subtitleLabel->setAlignment(Qt::AlignCenter);

        headerLayout->addWidget(subtitleLabel);


        mainLayout->addWidget(headerFrame);

        formFrame = new QFrame(ResidentDialog);
        formFrame->setObjectName("formFrame");
        formFrame->setFrameShape(QFrame::NoFrame);
        formFrame->setStyleSheet(QString::fromUtf8("background-color: white; border-radius: 10px; padding: 20px; box-shadow: 0 2px 8px rgba(0,0,0,0.1);"));
        columnsLayout = new QHBoxLayout(formFrame);
        columnsLayout->setSpacing(25);
        columnsLayout->setObjectName("columnsLayout");
        leftFormLayout = new QFormLayout();
        leftFormLayout->setObjectName("leftFormLayout");
        leftFormLayout->setHorizontalSpacing(10);
        leftFormLayout->setVerticalSpacing(12);
        labelCin = new QLabel(formFrame);
        labelCin->setObjectName("labelCin");
        labelCin->setStyleSheet(QString::fromUtf8("font-weight: bold; color: #374151; font-size: 13px;"));

        leftFormLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, labelCin);

        cinLayout = new QHBoxLayout();
        cinLayout->setObjectName("cinLayout");
        cinEdit = new QLineEdit(formFrame);
        cinEdit->setObjectName("cinEdit");
        cinEdit->setStyleSheet(QString::fromUtf8("border: 2px solid #E5E7EB; border-radius: 6px; padding: 8px; font-size: 13px; background-color: #F9FAFB;"));
        cinEdit->setMinimumSize(QSize(160, 32));

        cinLayout->addWidget(cinEdit);

        cinRequired = new QLabel(formFrame);
        cinRequired->setObjectName("cinRequired");
        cinRequired->setStyleSheet(QString::fromUtf8("color: #EF4444; font-weight: bold; font-size: 14px;"));

        cinLayout->addWidget(cinRequired);


        leftFormLayout->setLayout(0, QFormLayout::ItemRole::FieldRole, cinLayout);

        labelNom = new QLabel(formFrame);
        labelNom->setObjectName("labelNom");
        labelNom->setStyleSheet(QString::fromUtf8("font-weight: bold; color: #374151; font-size: 13px;"));

        leftFormLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, labelNom);

        nomLayout = new QHBoxLayout();
        nomLayout->setObjectName("nomLayout");
        nomEdit = new QLineEdit(formFrame);
        nomEdit->setObjectName("nomEdit");
        nomEdit->setStyleSheet(QString::fromUtf8("border: 2px solid #E5E7EB; border-radius: 6px; padding: 8px; font-size: 13px; background-color: #F9FAFB;"));
        nomEdit->setMinimumSize(QSize(160, 32));

        nomLayout->addWidget(nomEdit);

        nomRequired = new QLabel(formFrame);
        nomRequired->setObjectName("nomRequired");
        nomRequired->setStyleSheet(QString::fromUtf8("color: #EF4444; font-weight: bold; font-size: 14px;"));

        nomLayout->addWidget(nomRequired);


        leftFormLayout->setLayout(1, QFormLayout::ItemRole::FieldRole, nomLayout);

        labelPrenom = new QLabel(formFrame);
        labelPrenom->setObjectName("labelPrenom");
        labelPrenom->setStyleSheet(QString::fromUtf8("font-weight: bold; color: #374151; font-size: 13px;"));

        leftFormLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, labelPrenom);

        prenomLayout = new QHBoxLayout();
        prenomLayout->setObjectName("prenomLayout");
        prenomEdit = new QLineEdit(formFrame);
        prenomEdit->setObjectName("prenomEdit");
        prenomEdit->setStyleSheet(QString::fromUtf8("border: 2px solid #E5E7EB; border-radius: 6px; padding: 8px; font-size: 13px; background-color: #F9FAFB;"));
        prenomEdit->setMinimumSize(QSize(160, 32));

        prenomLayout->addWidget(prenomEdit);

        prenomRequired = new QLabel(formFrame);
        prenomRequired->setObjectName("prenomRequired");
        prenomRequired->setStyleSheet(QString::fromUtf8("color: #EF4444; font-weight: bold; font-size: 14px;"));

        prenomLayout->addWidget(prenomRequired);


        leftFormLayout->setLayout(2, QFormLayout::ItemRole::FieldRole, prenomLayout);

        labelSexe = new QLabel(formFrame);
        labelSexe->setObjectName("labelSexe");
        labelSexe->setStyleSheet(QString::fromUtf8("font-weight: bold; color: #374151; font-size: 13px;"));

        leftFormLayout->setWidget(3, QFormLayout::ItemRole::LabelRole, labelSexe);

        sexeLayout = new QHBoxLayout();
        sexeLayout->setObjectName("sexeLayout");
        sexeCombo = new QComboBox(formFrame);
        sexeCombo->addItem(QString());
        sexeCombo->addItem(QString());
        sexeCombo->addItem(QString());
        sexeCombo->setObjectName("sexeCombo");
        sexeCombo->setStyleSheet(QString::fromUtf8("border: 2px solid #E5E7EB; border-radius: 6px; padding: 8px; font-size: 13px; background-color: #F9FAFB;"));
        sexeCombo->setMinimumSize(QSize(160, 32));

        sexeLayout->addWidget(sexeCombo);

        sexeRequired = new QLabel(formFrame);
        sexeRequired->setObjectName("sexeRequired");
        sexeRequired->setStyleSheet(QString::fromUtf8("color: #EF4444; font-weight: bold; font-size: 14px;"));

        sexeLayout->addWidget(sexeRequired);


        leftFormLayout->setLayout(3, QFormLayout::ItemRole::FieldRole, sexeLayout);


        columnsLayout->addLayout(leftFormLayout);

        rightFormLayout = new QFormLayout();
        rightFormLayout->setObjectName("rightFormLayout");
        rightFormLayout->setHorizontalSpacing(10);
        rightFormLayout->setVerticalSpacing(12);
        labelTelephone = new QLabel(formFrame);
        labelTelephone->setObjectName("labelTelephone");
        labelTelephone->setStyleSheet(QString::fromUtf8("font-weight: bold; color: #374151; font-size: 13px;"));

        rightFormLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, labelTelephone);

        telephoneEdit = new QLineEdit(formFrame);
        telephoneEdit->setObjectName("telephoneEdit");
        telephoneEdit->setStyleSheet(QString::fromUtf8("border: 2px solid #E5E7EB; border-radius: 6px; padding: 8px; font-size: 13px; background-color: #F9FAFB;"));
        telephoneEdit->setMinimumSize(QSize(180, 32));

        rightFormLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, telephoneEdit);

        labelEmail = new QLabel(formFrame);
        labelEmail->setObjectName("labelEmail");
        labelEmail->setStyleSheet(QString::fromUtf8("font-weight: bold; color: #374151; font-size: 13px;"));

        rightFormLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, labelEmail);

        emailEdit = new QLineEdit(formFrame);
        emailEdit->setObjectName("emailEdit");
        emailEdit->setStyleSheet(QString::fromUtf8("border: 2px solid #E5E7EB; border-radius: 6px; padding: 8px; font-size: 13px; background-color: #F9FAFB;"));
        emailEdit->setMinimumSize(QSize(180, 32));

        rightFormLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, emailEdit);

        labelAppartement = new QLabel(formFrame);
        labelAppartement->setObjectName("labelAppartement");
        labelAppartement->setStyleSheet(QString::fromUtf8("font-weight: bold; color: #374151; font-size: 13px;"));

        rightFormLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, labelAppartement);

        appartementLayout = new QHBoxLayout();
        appartementLayout->setObjectName("appartementLayout");
        appartementEdit = new QLineEdit(formFrame);
        appartementEdit->setObjectName("appartementEdit");
        appartementEdit->setStyleSheet(QString::fromUtf8("border: 2px solid #E5E7EB; border-radius: 6px; padding: 8px; font-size: 13px; background-color: #F9FAFB;"));
        appartementEdit->setMinimumSize(QSize(160, 32));

        appartementLayout->addWidget(appartementEdit);

        appartementRequired = new QLabel(formFrame);
        appartementRequired->setObjectName("appartementRequired");
        appartementRequired->setStyleSheet(QString::fromUtf8("color: #EF4444; font-weight: bold; font-size: 14px;"));

        appartementLayout->addWidget(appartementRequired);


        rightFormLayout->setLayout(2, QFormLayout::ItemRole::FieldRole, appartementLayout);

        labelEtage = new QLabel(formFrame);
        labelEtage->setObjectName("labelEtage");
        labelEtage->setStyleSheet(QString::fromUtf8("font-weight: bold; color: #374151; font-size: 13px;"));

        rightFormLayout->setWidget(3, QFormLayout::ItemRole::LabelRole, labelEtage);

        etageSpinBox = new QSpinBox(formFrame);
        etageSpinBox->setObjectName("etageSpinBox");
        etageSpinBox->setStyleSheet(QString::fromUtf8("border: 2px solid #E5E7EB; border-radius: 6px; padding: 8px; font-size: 13px; background-color: #F9FAFB;"));
        etageSpinBox->setMinimum(0);
        etageSpinBox->setMaximum(50);
        etageSpinBox->setMinimumSize(QSize(180, 32));

        rightFormLayout->setWidget(3, QFormLayout::ItemRole::FieldRole, etageSpinBox);


        columnsLayout->addLayout(rightFormLayout);


        mainLayout->addWidget(formFrame);

        dateButtonFrame = new QFrame(ResidentDialog);
        dateButtonFrame->setObjectName("dateButtonFrame");
        dateButtonFrame->setFrameShape(QFrame::NoFrame);
        dateButtonFrame->setStyleSheet(QString::fromUtf8("background-color: white; border-radius: 10px; padding: 15px; box-shadow: 0 2px 8px rgba(0,0,0,0.1);"));
        dateButtonLayout = new QHBoxLayout(dateButtonFrame);
        dateButtonLayout->setSpacing(15);
        dateButtonLayout->setObjectName("dateButtonLayout");
        labelDateEntree = new QLabel(dateButtonFrame);
        labelDateEntree->setObjectName("labelDateEntree");
        labelDateEntree->setStyleSheet(QString::fromUtf8("font-weight: bold; color: #374151; font-size: 13px;"));

        dateButtonLayout->addWidget(labelDateEntree);

        dateEntreeEdit = new QDateEdit(dateButtonFrame);
        dateEntreeEdit->setObjectName("dateEntreeEdit");
        dateEntreeEdit->setStyleSheet(QString::fromUtf8("border: 2px solid #E5E7EB; border-radius: 6px; padding: 8px; font-size: 13px; background-color: #F9FAFB;"));
        dateEntreeEdit->setCalendarPopup(true);
        dateEntreeEdit->setMinimumSize(QSize(140, 32));

        dateButtonLayout->addWidget(dateEntreeEdit);

        dateRequired = new QLabel(dateButtonFrame);
        dateRequired->setObjectName("dateRequired");
        dateRequired->setStyleSheet(QString::fromUtf8("color: #EF4444; font-weight: bold; font-size: 14px;"));

        dateButtonLayout->addWidget(dateRequired);

        dateSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        dateButtonLayout->addItem(dateSpacer);

        cancelButton = new QPushButton(dateButtonFrame);
        cancelButton->setObjectName("cancelButton");
        cancelButton->setStyleSheet(QString::fromUtf8("background-color: #6B7280; color: white; border: none; border-radius: 6px; font-weight: bold; font-size: 12px; padding: 8px 16px;"));
        cancelButton->setMinimumSize(QSize(90, 32));

        dateButtonLayout->addWidget(cancelButton);

        saveButton = new QPushButton(dateButtonFrame);
        saveButton->setObjectName("saveButton");
        saveButton->setStyleSheet(QString::fromUtf8("background-color: #3168E0; color: white; border: none; border-radius: 6px; font-weight: bold; font-size: 12px; padding: 8px 16px;"));
        saveButton->setMinimumSize(QSize(110, 32));

        dateButtonLayout->addWidget(saveButton);


        mainLayout->addWidget(dateButtonFrame);


        retranslateUi(ResidentDialog);
        QObject::connect(saveButton, &QPushButton::clicked, ResidentDialog, qOverload<>(&QDialog::accept));
        QObject::connect(cancelButton, &QPushButton::clicked, ResidentDialog, qOverload<>(&QDialog::reject));

        saveButton->setDefault(true);


        QMetaObject::connectSlotsByName(ResidentDialog);
    } // setupUi

    void retranslateUi(QDialog *ResidentDialog)
    {
        ResidentDialog->setWindowTitle(QCoreApplication::translate("ResidentDialog", "Gestion des R\303\251sidents", nullptr));
        titleLabel->setText(QCoreApplication::translate("ResidentDialog", "\360\237\221\244 Ajouter un R\303\251sident", nullptr));
        subtitleLabel->setText(QCoreApplication::translate("ResidentDialog", "Les champs marqu\303\251s d'un ast\303\251risque (*) sont obligatoires", nullptr));
        labelCin->setText(QCoreApplication::translate("ResidentDialog", "CIN *", nullptr));
        cinEdit->setPlaceholderText(QCoreApplication::translate("ResidentDialog", "Ex: 12345678", nullptr));
        cinRequired->setText(QCoreApplication::translate("ResidentDialog", "*", nullptr));
        labelNom->setText(QCoreApplication::translate("ResidentDialog", "Nom *", nullptr));
        nomEdit->setPlaceholderText(QCoreApplication::translate("ResidentDialog", "Ex: Dupont", nullptr));
        nomRequired->setText(QCoreApplication::translate("ResidentDialog", "*", nullptr));
        labelPrenom->setText(QCoreApplication::translate("ResidentDialog", "Pr\303\251nom *", nullptr));
        prenomEdit->setPlaceholderText(QCoreApplication::translate("ResidentDialog", "Ex: Jean", nullptr));
        prenomRequired->setText(QCoreApplication::translate("ResidentDialog", "*", nullptr));
        labelSexe->setText(QCoreApplication::translate("ResidentDialog", "Sexe *", nullptr));
        sexeCombo->setItemText(0, QCoreApplication::translate("ResidentDialog", "S\303\251lectionner...", nullptr));
        sexeCombo->setItemText(1, QCoreApplication::translate("ResidentDialog", "M", nullptr));
        sexeCombo->setItemText(2, QCoreApplication::translate("ResidentDialog", "F", nullptr));

        sexeRequired->setText(QCoreApplication::translate("ResidentDialog", "*", nullptr));
        labelTelephone->setText(QCoreApplication::translate("ResidentDialog", "T\303\251l\303\251phone", nullptr));
        telephoneEdit->setPlaceholderText(QCoreApplication::translate("ResidentDialog", "Ex: +216 12 345 678", nullptr));
        labelEmail->setText(QCoreApplication::translate("ResidentDialog", "Email", nullptr));
        emailEdit->setPlaceholderText(QCoreApplication::translate("ResidentDialog", "Ex: jean.dupont@email.com", nullptr));
        labelAppartement->setText(QCoreApplication::translate("ResidentDialog", "Appartement *", nullptr));
        appartementEdit->setPlaceholderText(QCoreApplication::translate("ResidentDialog", "Ex: A101, B205", nullptr));
        appartementRequired->setText(QCoreApplication::translate("ResidentDialog", "*", nullptr));
        labelEtage->setText(QCoreApplication::translate("ResidentDialog", "\303\211tage", nullptr));
        labelDateEntree->setText(QCoreApplication::translate("ResidentDialog", "Date d'entr\303\251e *", nullptr));
        dateRequired->setText(QCoreApplication::translate("ResidentDialog", "*", nullptr));
        cancelButton->setText(QCoreApplication::translate("ResidentDialog", "\342\235\214 Annuler", nullptr));
        saveButton->setText(QCoreApplication::translate("ResidentDialog", "\360\237\222\276 Enregistrer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ResidentDialog: public Ui_ResidentDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RESIDENTDIALOG_H
