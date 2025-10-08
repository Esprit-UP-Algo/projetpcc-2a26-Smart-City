/********************************************************************************
** Form generated from reading UI file 'vehiculedialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VEHICULEDIALOG_H
#define UI_VEHICULEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_VehiculeDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *dialogTitle;
    QFormLayout *formLayout;
    QLabel *idFieldLabel;
    QLineEdit *idEdit;
    QLabel *typeFieldLabel;
    QComboBox *typeCombo;
    QLabel *capaciteFieldLabel;
    QSpinBox *capaciteSpinBox;
    QLabel *zoneFieldLabel;
    QLineEdit *zoneEdit;
    QLabel *horaireFieldLabel;
    QLineEdit *horaireEdit;
    QLabel *statutFieldLabel;
    QComboBox *statutCombo;
    QLabel *kilometrageFieldLabel;
    QDoubleSpinBox *kilometrageSpinBox;
    QLabel *tempsFieldLabel;
    QDoubleSpinBox *tempsUtiliseSpinBox;
    QHBoxLayout *buttonLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelButton;
    QPushButton *saveButton;

    void setupUi(QDialog *VehiculeDialog)
    {
        if (VehiculeDialog->objectName().isEmpty())
            VehiculeDialog->setObjectName("VehiculeDialog");
        VehiculeDialog->resize(600, 550);
        VehiculeDialog->setStyleSheet(QString::fromUtf8("background-color: white;"));
        verticalLayout = new QVBoxLayout(VehiculeDialog);
        verticalLayout->setSpacing(25);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(40, 40, 40, 40);
        dialogTitle = new QLabel(VehiculeDialog);
        dialogTitle->setObjectName("dialogTitle");
        dialogTitle->setStyleSheet(QString::fromUtf8("font-size: 24px; font-weight: bold; color: #2C3E50;"));

        verticalLayout->addWidget(dialogTitle);

        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        formLayout->setHorizontalSpacing(20);
        formLayout->setVerticalSpacing(18);
        formLayout->setLabelAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        idFieldLabel = new QLabel(VehiculeDialog);
        idFieldLabel->setObjectName("idFieldLabel");
        idFieldLabel->setStyleSheet(QString::fromUtf8("font-size: 15px; font-weight: bold; color: #2C3E50;"));

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, idFieldLabel);

        idEdit = new QLineEdit(VehiculeDialog);
        idEdit->setObjectName("idEdit");
        idEdit->setMinimumSize(QSize(0, 45));
        idEdit->setStyleSheet(QString::fromUtf8("padding: 12px; border: 2px solid #BDC3C7; border-radius: 8px; font-size: 15px;"));

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, idEdit);

        typeFieldLabel = new QLabel(VehiculeDialog);
        typeFieldLabel->setObjectName("typeFieldLabel");
        typeFieldLabel->setStyleSheet(QString::fromUtf8("font-size: 15px; font-weight: bold; color: #2C3E50;"));

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, typeFieldLabel);

        typeCombo = new QComboBox(VehiculeDialog);
        typeCombo->addItem(QString());
        typeCombo->addItem(QString());
        typeCombo->addItem(QString());
        typeCombo->addItem(QString());
        typeCombo->addItem(QString());
        typeCombo->setObjectName("typeCombo");
        typeCombo->setMinimumSize(QSize(0, 45));
        typeCombo->setStyleSheet(QString::fromUtf8("padding: 12px; border: 2px solid #BDC3C7; border-radius: 8px; font-size: 15px;"));

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, typeCombo);

        capaciteFieldLabel = new QLabel(VehiculeDialog);
        capaciteFieldLabel->setObjectName("capaciteFieldLabel");
        capaciteFieldLabel->setStyleSheet(QString::fromUtf8("font-size: 15px; font-weight: bold; color: #2C3E50;"));

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, capaciteFieldLabel);

        capaciteSpinBox = new QSpinBox(VehiculeDialog);
        capaciteSpinBox->setObjectName("capaciteSpinBox");
        capaciteSpinBox->setMinimumSize(QSize(0, 45));
        capaciteSpinBox->setMinimum(1);
        capaciteSpinBox->setMaximum(100);
        capaciteSpinBox->setValue(4);
        capaciteSpinBox->setStyleSheet(QString::fromUtf8("padding: 12px; border: 2px solid #BDC3C7; border-radius: 8px; font-size: 15px;"));

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, capaciteSpinBox);

        zoneFieldLabel = new QLabel(VehiculeDialog);
        zoneFieldLabel->setObjectName("zoneFieldLabel");
        zoneFieldLabel->setStyleSheet(QString::fromUtf8("font-size: 15px; font-weight: bold; color: #2C3E50;"));

        formLayout->setWidget(3, QFormLayout::ItemRole::LabelRole, zoneFieldLabel);

        zoneEdit = new QLineEdit(VehiculeDialog);
        zoneEdit->setObjectName("zoneEdit");
        zoneEdit->setMinimumSize(QSize(0, 45));
        zoneEdit->setStyleSheet(QString::fromUtf8("padding: 12px; border: 2px solid #BDC3C7; border-radius: 8px; font-size: 15px;"));

        formLayout->setWidget(3, QFormLayout::ItemRole::FieldRole, zoneEdit);

        horaireFieldLabel = new QLabel(VehiculeDialog);
        horaireFieldLabel->setObjectName("horaireFieldLabel");
        horaireFieldLabel->setStyleSheet(QString::fromUtf8("font-size: 15px; font-weight: bold; color: #2C3E50;"));

        formLayout->setWidget(4, QFormLayout::ItemRole::LabelRole, horaireFieldLabel);

        horaireEdit = new QLineEdit(VehiculeDialog);
        horaireEdit->setObjectName("horaireEdit");
        horaireEdit->setMinimumSize(QSize(0, 45));
        horaireEdit->setStyleSheet(QString::fromUtf8("padding: 12px; border: 2px solid #BDC3C7; border-radius: 8px; font-size: 15px;"));

        formLayout->setWidget(4, QFormLayout::ItemRole::FieldRole, horaireEdit);

        statutFieldLabel = new QLabel(VehiculeDialog);
        statutFieldLabel->setObjectName("statutFieldLabel");
        statutFieldLabel->setStyleSheet(QString::fromUtf8("font-size: 15px; font-weight: bold; color: #2C3E50;"));

        formLayout->setWidget(5, QFormLayout::ItemRole::LabelRole, statutFieldLabel);

        statutCombo = new QComboBox(VehiculeDialog);
        statutCombo->addItem(QString());
        statutCombo->addItem(QString());
        statutCombo->addItem(QString());
        statutCombo->setObjectName("statutCombo");
        statutCombo->setMinimumSize(QSize(0, 45));
        statutCombo->setStyleSheet(QString::fromUtf8("padding: 12px; border: 2px solid #BDC3C7; border-radius: 8px; font-size: 15px;"));

        formLayout->setWidget(5, QFormLayout::ItemRole::FieldRole, statutCombo);

        kilometrageFieldLabel = new QLabel(VehiculeDialog);
        kilometrageFieldLabel->setObjectName("kilometrageFieldLabel");
        kilometrageFieldLabel->setStyleSheet(QString::fromUtf8("font-size: 15px; font-weight: bold; color: #2C3E50;"));

        formLayout->setWidget(6, QFormLayout::ItemRole::LabelRole, kilometrageFieldLabel);

        kilometrageSpinBox = new QDoubleSpinBox(VehiculeDialog);
        kilometrageSpinBox->setObjectName("kilometrageSpinBox");
        kilometrageSpinBox->setMinimumSize(QSize(0, 45));
        kilometrageSpinBox->setDecimals(1);
        kilometrageSpinBox->setMaximum(999999.000000000000000);
        kilometrageSpinBox->setStyleSheet(QString::fromUtf8("padding: 12px; border: 2px solid #BDC3C7; border-radius: 8px; font-size: 15px;"));

        formLayout->setWidget(6, QFormLayout::ItemRole::FieldRole, kilometrageSpinBox);

        tempsFieldLabel = new QLabel(VehiculeDialog);
        tempsFieldLabel->setObjectName("tempsFieldLabel");
        tempsFieldLabel->setStyleSheet(QString::fromUtf8("font-size: 15px; font-weight: bold; color: #2C3E50;"));

        formLayout->setWidget(7, QFormLayout::ItemRole::LabelRole, tempsFieldLabel);

        tempsUtiliseSpinBox = new QDoubleSpinBox(VehiculeDialog);
        tempsUtiliseSpinBox->setObjectName("tempsUtiliseSpinBox");
        tempsUtiliseSpinBox->setMinimumSize(QSize(0, 45));
        tempsUtiliseSpinBox->setDecimals(1);
        tempsUtiliseSpinBox->setMaximum(99999.000000000000000);
        tempsUtiliseSpinBox->setStyleSheet(QString::fromUtf8("padding: 12px; border: 2px solid #BDC3C7; border-radius: 8px; font-size: 15px;"));

        formLayout->setWidget(7, QFormLayout::ItemRole::FieldRole, tempsUtiliseSpinBox);


        verticalLayout->addLayout(formLayout);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setSpacing(15);
        buttonLayout->setObjectName("buttonLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonLayout->addItem(horizontalSpacer);

        cancelButton = new QPushButton(VehiculeDialog);
        cancelButton->setObjectName("cancelButton");
        cancelButton->setMinimumSize(QSize(120, 50));
        cancelButton->setStyleSheet(QString::fromUtf8("background-color: #95A5A6; color: white; border: none; border-radius: 10px; font-size: 15px; font-weight: bold; padding: 0 35px;"));

        buttonLayout->addWidget(cancelButton);

        saveButton = new QPushButton(VehiculeDialog);
        saveButton->setObjectName("saveButton");
        saveButton->setMinimumSize(QSize(140, 50));
        saveButton->setStyleSheet(QString::fromUtf8("background-color: #3498DB; color: white; border: none; border-radius: 10px; font-size: 15px; font-weight: bold; padding: 0 35px;"));

        buttonLayout->addWidget(saveButton);


        verticalLayout->addLayout(buttonLayout);


        retranslateUi(VehiculeDialog);
        QObject::connect(cancelButton, &QPushButton::clicked, VehiculeDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(VehiculeDialog);
    } // setupUi

    void retranslateUi(QDialog *VehiculeDialog)
    {
        VehiculeDialog->setWindowTitle(QCoreApplication::translate("VehiculeDialog", "V\303\251hicule", nullptr));
        dialogTitle->setText(QCoreApplication::translate("VehiculeDialog", "Nouveau V\303\251hicule", nullptr));
        idFieldLabel->setText(QCoreApplication::translate("VehiculeDialog", "ID V\303\251hicule *:", nullptr));
        idEdit->setPlaceholderText(QCoreApplication::translate("VehiculeDialog", "Ex: VH001", nullptr));
        typeFieldLabel->setText(QCoreApplication::translate("VehiculeDialog", "Type *:", nullptr));
        typeCombo->setItemText(0, QCoreApplication::translate("VehiculeDialog", "Taxi", nullptr));
        typeCombo->setItemText(1, QCoreApplication::translate("VehiculeDialog", "Bus", nullptr));
        typeCombo->setItemText(2, QCoreApplication::translate("VehiculeDialog", "Voiture", nullptr));
        typeCombo->setItemText(3, QCoreApplication::translate("VehiculeDialog", "Navette", nullptr));
        typeCombo->setItemText(4, QCoreApplication::translate("VehiculeDialog", "Utilitaire", nullptr));

        capaciteFieldLabel->setText(QCoreApplication::translate("VehiculeDialog", "Capacit\303\251 *:", nullptr));
        capaciteSpinBox->setSuffix(QCoreApplication::translate("VehiculeDialog", " places", nullptr));
        zoneFieldLabel->setText(QCoreApplication::translate("VehiculeDialog", "Zone Desservie *:", nullptr));
        zoneEdit->setPlaceholderText(QCoreApplication::translate("VehiculeDialog", "Ex: Zone Nord, Centre-ville", nullptr));
        horaireFieldLabel->setText(QCoreApplication::translate("VehiculeDialog", "Horaire Pr\303\251vue *:", nullptr));
        horaireEdit->setPlaceholderText(QCoreApplication::translate("VehiculeDialog", "Ex: 08:00 - 18:00", nullptr));
        statutFieldLabel->setText(QCoreApplication::translate("VehiculeDialog", "Statut *:", nullptr));
        statutCombo->setItemText(0, QCoreApplication::translate("VehiculeDialog", "Disponible", nullptr));
        statutCombo->setItemText(1, QCoreApplication::translate("VehiculeDialog", "R\303\251serv\303\251", nullptr));
        statutCombo->setItemText(2, QCoreApplication::translate("VehiculeDialog", "Maintenance", nullptr));

        kilometrageFieldLabel->setText(QCoreApplication::translate("VehiculeDialog", "Kilom\303\251trage *:", nullptr));
        kilometrageSpinBox->setSuffix(QCoreApplication::translate("VehiculeDialog", " km", nullptr));
        tempsFieldLabel->setText(QCoreApplication::translate("VehiculeDialog", "Temps Utilis\303\251 *:", nullptr));
        tempsUtiliseSpinBox->setSuffix(QCoreApplication::translate("VehiculeDialog", " heures", nullptr));
        cancelButton->setText(QCoreApplication::translate("VehiculeDialog", "Annuler", nullptr));
        saveButton->setText(QCoreApplication::translate("VehiculeDialog", "Enregistrer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VehiculeDialog: public Ui_VehiculeDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VEHICULEDIALOG_H
