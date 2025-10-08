/********************************************************************************
** Form generated from reading UI file 'incidentdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INCIDENTDIALOG_H
#define UI_INCIDENTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateTimeEdit>
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

class Ui_IncidentDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *dialogTitle;
    QFormLayout *formLayout;
    QLabel *typeFieldLabel;
    QComboBox *typeCombo;
    QLabel *localisationFieldLabel;
    QLineEdit *localisationEdit;
    QLabel *dateHeureFieldLabel;
    QDateTimeEdit *dateHeureEdit;
    QLabel *graviteFieldLabel;
    QSpinBox *graviteSpinBox;
    QLabel *statutFieldLabel;
    QComboBox *statutCombo;
    QLabel *infoLabel;
    QHBoxLayout *buttonLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelButton;
    QPushButton *saveButton;

    void setupUi(QDialog *IncidentDialog)
    {
        if (IncidentDialog->objectName().isEmpty())
            IncidentDialog->setObjectName("IncidentDialog");
        IncidentDialog->resize(600, 715);
        IncidentDialog->setStyleSheet(QString::fromUtf8("background-color: white;"));
        verticalLayout = new QVBoxLayout(IncidentDialog);
        verticalLayout->setSpacing(25);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(40, 40, 40, 40);
        dialogTitle = new QLabel(IncidentDialog);
        dialogTitle->setObjectName("dialogTitle");
        dialogTitle->setStyleSheet(QString::fromUtf8("font-size: 24px; font-weight: bold; color: #2C3E50;"));

        verticalLayout->addWidget(dialogTitle);

        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        formLayout->setLabelAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);
        formLayout->setHorizontalSpacing(20);
        formLayout->setVerticalSpacing(18);
        typeFieldLabel = new QLabel(IncidentDialog);
        typeFieldLabel->setObjectName("typeFieldLabel");
        typeFieldLabel->setStyleSheet(QString::fromUtf8("font-size: 15px; font-weight: bold; color: #2C3E50;"));

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, typeFieldLabel);

        typeCombo = new QComboBox(IncidentDialog);
        typeCombo->addItem(QString());
        typeCombo->addItem(QString());
        typeCombo->addItem(QString());
        typeCombo->addItem(QString());
        typeCombo->addItem(QString());
        typeCombo->addItem(QString());
        typeCombo->addItem(QString());
        typeCombo->setObjectName("typeCombo");
        typeCombo->setMinimumSize(QSize(0, 45));
        typeCombo->setStyleSheet(QString::fromUtf8("padding: 12px; border: 2px solid #BDC3C7; border-radius: 8px; font-size: 15px;"));

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, typeCombo);

        localisationFieldLabel = new QLabel(IncidentDialog);
        localisationFieldLabel->setObjectName("localisationFieldLabel");
        localisationFieldLabel->setStyleSheet(QString::fromUtf8("font-size: 15px; font-weight: bold; color: #2C3E50;"));

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, localisationFieldLabel);

        localisationEdit = new QLineEdit(IncidentDialog);
        localisationEdit->setObjectName("localisationEdit");
        localisationEdit->setMinimumSize(QSize(0, 45));
        localisationEdit->setStyleSheet(QString::fromUtf8("padding: 12px; border: 2px solid #BDC3C7; border-radius: 8px; font-size: 15px;"));

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, localisationEdit);

        dateHeureFieldLabel = new QLabel(IncidentDialog);
        dateHeureFieldLabel->setObjectName("dateHeureFieldLabel");
        dateHeureFieldLabel->setStyleSheet(QString::fromUtf8("font-size: 15px; font-weight: bold; color: #2C3E50;"));

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, dateHeureFieldLabel);

        dateHeureEdit = new QDateTimeEdit(IncidentDialog);
        dateHeureEdit->setObjectName("dateHeureEdit");
        dateHeureEdit->setMinimumSize(QSize(0, 45));
        dateHeureEdit->setStyleSheet(QString::fromUtf8("padding: 12px; border: 2px solid #BDC3C7; border-radius: 8px; font-size: 15px;"));
        dateHeureEdit->setCalendarPopup(true);

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, dateHeureEdit);

        graviteFieldLabel = new QLabel(IncidentDialog);
        graviteFieldLabel->setObjectName("graviteFieldLabel");
        graviteFieldLabel->setStyleSheet(QString::fromUtf8("font-size: 15px; font-weight: bold; color: #2C3E50;"));

        formLayout->setWidget(3, QFormLayout::ItemRole::LabelRole, graviteFieldLabel);

        graviteSpinBox = new QSpinBox(IncidentDialog);
        graviteSpinBox->setObjectName("graviteSpinBox");
        graviteSpinBox->setMinimumSize(QSize(0, 45));
        graviteSpinBox->setStyleSheet(QString::fromUtf8("padding: 12px; border: 2px solid #BDC3C7; border-radius: 8px; font-size: 15px;"));
        graviteSpinBox->setMinimum(1);
        graviteSpinBox->setMaximum(5);
        graviteSpinBox->setValue(3);

        formLayout->setWidget(3, QFormLayout::ItemRole::FieldRole, graviteSpinBox);

        statutFieldLabel = new QLabel(IncidentDialog);
        statutFieldLabel->setObjectName("statutFieldLabel");
        statutFieldLabel->setStyleSheet(QString::fromUtf8("font-size: 15px; font-weight: bold; color: #2C3E50;"));

        formLayout->setWidget(4, QFormLayout::ItemRole::LabelRole, statutFieldLabel);

        statutCombo = new QComboBox(IncidentDialog);
        statutCombo->addItem(QString());
        statutCombo->addItem(QString());
        statutCombo->addItem(QString());
        statutCombo->setObjectName("statutCombo");
        statutCombo->setMinimumSize(QSize(0, 45));
        statutCombo->setStyleSheet(QString::fromUtf8("padding: 12px; border: 2px solid #BDC3C7; border-radius: 8px; font-size: 15px;"));

        formLayout->setWidget(4, QFormLayout::ItemRole::FieldRole, statutCombo);


        verticalLayout->addLayout(formLayout);

        infoLabel = new QLabel(IncidentDialog);
        infoLabel->setObjectName("infoLabel");
        infoLabel->setStyleSheet(QString::fromUtf8("padding: 12px; background-color: #FCF3CF; border-radius: 8px; font-size: 13px; color: #7F8C8D;"));
        infoLabel->setWordWrap(true);

        verticalLayout->addWidget(infoLabel);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setSpacing(15);
        buttonLayout->setObjectName("buttonLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonLayout->addItem(horizontalSpacer);

        cancelButton = new QPushButton(IncidentDialog);
        cancelButton->setObjectName("cancelButton");
        cancelButton->setMinimumSize(QSize(120, 50));
        cancelButton->setStyleSheet(QString::fromUtf8("background-color: #95A5A6; color: white; border: none; border-radius: 10px; font-size: 15px; font-weight: bold; padding: 0 35px;"));

        buttonLayout->addWidget(cancelButton);

        saveButton = new QPushButton(IncidentDialog);
        saveButton->setObjectName("saveButton");
        saveButton->setMinimumSize(QSize(140, 50));
        saveButton->setStyleSheet(QString::fromUtf8("background-color: #E74C3C; color: white; border: none; border-radius: 10px; font-size: 15px; font-weight: bold; padding: 0 35px;"));

        buttonLayout->addWidget(saveButton);


        verticalLayout->addLayout(buttonLayout);


        retranslateUi(IncidentDialog);
        QObject::connect(cancelButton, &QPushButton::clicked, IncidentDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(IncidentDialog);
    } // setupUi

    void retranslateUi(QDialog *IncidentDialog)
    {
        IncidentDialog->setWindowTitle(QCoreApplication::translate("IncidentDialog", "Incident", nullptr));
        dialogTitle->setText(QCoreApplication::translate("IncidentDialog", "Nouvel Incident", nullptr));
        typeFieldLabel->setText(QCoreApplication::translate("IncidentDialog", "Type *:", nullptr));
        typeCombo->setItemText(0, QCoreApplication::translate("IncidentDialog", "Panne \303\211lectricit\303\251", nullptr));
        typeCombo->setItemText(1, QCoreApplication::translate("IncidentDialog", "Fuite Eau", nullptr));
        typeCombo->setItemText(2, QCoreApplication::translate("IncidentDialog", "Accident", nullptr));
        typeCombo->setItemText(3, QCoreApplication::translate("IncidentDialog", "Incendie", nullptr));
        typeCombo->setItemText(4, QCoreApplication::translate("IncidentDialog", "Ascenseur", nullptr));
        typeCombo->setItemText(5, QCoreApplication::translate("IncidentDialog", "S\303\251curit\303\251", nullptr));
        typeCombo->setItemText(6, QCoreApplication::translate("IncidentDialog", "Autre", nullptr));

        localisationFieldLabel->setText(QCoreApplication::translate("IncidentDialog", "Localisation *:", nullptr));
        localisationEdit->setPlaceholderText(QCoreApplication::translate("IncidentDialog", "Ex: B\303\242timent A - \303\211tage 3 - Appartement 301", nullptr));
        dateHeureFieldLabel->setText(QCoreApplication::translate("IncidentDialog", "Date & Heure *:", nullptr));
        dateHeureEdit->setDisplayFormat(QCoreApplication::translate("IncidentDialog", "dd/MM/yyyy HH:mm", nullptr));
        graviteFieldLabel->setText(QCoreApplication::translate("IncidentDialog", "Niveau de Gravit\303\251 *:", nullptr));
        statutFieldLabel->setText(QCoreApplication::translate("IncidentDialog", "Statut *:", nullptr));
        statutCombo->setItemText(0, QCoreApplication::translate("IncidentDialog", "En Attente", nullptr));
        statutCombo->setItemText(1, QCoreApplication::translate("IncidentDialog", "En Cours", nullptr));
        statutCombo->setItemText(2, QCoreApplication::translate("IncidentDialog", "Trait\303\251", nullptr));

        infoLabel->setText(QCoreApplication::translate("IncidentDialog", "<html><head/><body><p><span style=\" font-weight:600; color:#e74c3c;\">\342\232\240 Gravit\303\251:</span><br/>1 = Mineur | 2 = Faible | 3 = Moyen | 4 = Grave | 5 = Critique</p></body></html>", nullptr));
        cancelButton->setText(QCoreApplication::translate("IncidentDialog", "Annuler", nullptr));
        saveButton->setText(QCoreApplication::translate("IncidentDialog", "Enregistrer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class IncidentDialog: public Ui_IncidentDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INCIDENTDIALOG_H
