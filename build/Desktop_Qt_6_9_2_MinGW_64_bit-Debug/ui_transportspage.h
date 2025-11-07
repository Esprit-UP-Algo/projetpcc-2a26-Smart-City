/********************************************************************************
** Form generated from reading UI file 'transportspage.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRANSPORTSPAGE_H
#define UI_TRANSPORTSPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TransportsPage
{
public:
    QVBoxLayout *mainLayout;
    QLabel *titleLabel;
    QHBoxLayout *topRowLayout;
    QWidget *formCardWidget;
    QVBoxLayout *formCardLayout;
    QLabel *formCardTitle;
    QPushButton *addVehiculeButton;
    QLabel *fleetInfoTitle;
    QLabel *totalVehiculesLabel;
    QLabel *disponiblesLabel;
    QLabel *reservesLabel;
    QLabel *maintenanceLabel;
    QSpacerItem *formCardSpacer;
    QWidget *tableCardWidget;
    QVBoxLayout *tableCardLayout;
    QHBoxLayout *tableHeaderLayout;
    QLabel *tableCardTitle;
    QSpacerItem *tableHeaderSpacer;
    QPushButton *exportPDFButton;
    QPushButton *historiqueButton;
    QHBoxLayout *searchLayout;
    QLineEdit *searchLineEdit;
    QPushButton *searchButton;
    QComboBox *sortComboBox;
    QTableWidget *vehiculesTable;
    QHBoxLayout *tableActionsLayout;
    QSpacerItem *tableActionsSpacer;
    QPushButton *modifyButton;
    QPushButton *deleteButton;
    QWidget *statsCardWidget;
    QVBoxLayout *statsCardLayout;
    QLabel *statsCardTitle;
    QHBoxLayout *indicatorsLayout;
    QLabel *tauxUtilisationLabel;
    QLabel *kmTotalLabel;
    QLabel *tempsMoyenLabel;
    QWidget *chartContainer;
    QVBoxLayout *chartContainerLayout;

    void setupUi(QWidget *TransportsPage)
    {
        if (TransportsPage->objectName().isEmpty())
            TransportsPage->setObjectName("TransportsPage");
        TransportsPage->resize(1200, 800);
        TransportsPage->setStyleSheet(QString::fromUtf8("background-color: #F5F7FA;"));
        mainLayout = new QVBoxLayout(TransportsPage);
        mainLayout->setSpacing(20);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(20, 20, 20, 20);
        titleLabel = new QLabel(TransportsPage);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setStyleSheet(QString::fromUtf8("font-size: 28px; font-weight: bold; color: #2C3E50;"));

        mainLayout->addWidget(titleLabel);

        topRowLayout = new QHBoxLayout();
        topRowLayout->setSpacing(20);
        topRowLayout->setObjectName("topRowLayout");
        formCardWidget = new QWidget(TransportsPage);
        formCardWidget->setObjectName("formCardWidget");
        formCardWidget->setStyleSheet(QString::fromUtf8("background-color: white; border-radius: 15px;"));
        formCardLayout = new QVBoxLayout(formCardWidget);
        formCardLayout->setSpacing(15);
        formCardLayout->setObjectName("formCardLayout");
        formCardLayout->setContentsMargins(25, 25, 25, 25);
        formCardTitle = new QLabel(formCardWidget);
        formCardTitle->setObjectName("formCardTitle");
        formCardTitle->setStyleSheet(QString::fromUtf8("font-size: 18px; font-weight: bold; color: #2C3E50;"));

        formCardLayout->addWidget(formCardTitle);

        addVehiculeButton = new QPushButton(formCardWidget);
        addVehiculeButton->setObjectName("addVehiculeButton");
        addVehiculeButton->setMinimumSize(QSize(0, 50));
        addVehiculeButton->setStyleSheet(QString::fromUtf8("background-color: #3498DB; color: white; border: none; border-radius: 10px; font-size: 16px; font-weight: bold;"));

        formCardLayout->addWidget(addVehiculeButton);

        fleetInfoTitle = new QLabel(formCardWidget);
        fleetInfoTitle->setObjectName("fleetInfoTitle");

        formCardLayout->addWidget(fleetInfoTitle);

        totalVehiculesLabel = new QLabel(formCardWidget);
        totalVehiculesLabel->setObjectName("totalVehiculesLabel");
        totalVehiculesLabel->setStyleSheet(QString::fromUtf8("font-size: 15px; color: #3498DB; font-weight: bold; padding: 10px; background-color: #EBF5FB; border-radius: 8px;"));

        formCardLayout->addWidget(totalVehiculesLabel);

        disponiblesLabel = new QLabel(formCardWidget);
        disponiblesLabel->setObjectName("disponiblesLabel");
        disponiblesLabel->setStyleSheet(QString::fromUtf8("font-size: 14px; color: #27AE60; padding: 10px; background-color: #E8F8F5; border-radius: 8px;"));

        formCardLayout->addWidget(disponiblesLabel);

        reservesLabel = new QLabel(formCardWidget);
        reservesLabel->setObjectName("reservesLabel");
        reservesLabel->setStyleSheet(QString::fromUtf8("font-size: 14px; color: #F39C12; padding: 10px; background-color: #FEF5E7; border-radius: 8px;"));

        formCardLayout->addWidget(reservesLabel);

        maintenanceLabel = new QLabel(formCardWidget);
        maintenanceLabel->setObjectName("maintenanceLabel");
        maintenanceLabel->setStyleSheet(QString::fromUtf8("font-size: 14px; color: #E74C3C; padding: 10px; background-color: #FADBD8; border-radius: 8px;"));

        formCardLayout->addWidget(maintenanceLabel);

        formCardSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        formCardLayout->addItem(formCardSpacer);


        topRowLayout->addWidget(formCardWidget);

        tableCardWidget = new QWidget(TransportsPage);
        tableCardWidget->setObjectName("tableCardWidget");
        tableCardWidget->setStyleSheet(QString::fromUtf8("background-color: white; border-radius: 15px;"));
        tableCardLayout = new QVBoxLayout(tableCardWidget);
        tableCardLayout->setSpacing(15);
        tableCardLayout->setObjectName("tableCardLayout");
        tableCardLayout->setContentsMargins(25, 25, 25, 25);
        tableHeaderLayout = new QHBoxLayout();
        tableHeaderLayout->setObjectName("tableHeaderLayout");
        tableCardTitle = new QLabel(tableCardWidget);
        tableCardTitle->setObjectName("tableCardTitle");
        tableCardTitle->setStyleSheet(QString::fromUtf8("font-size: 18px; font-weight: bold; color: #2C3E50;"));

        tableHeaderLayout->addWidget(tableCardTitle);

        tableHeaderSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        tableHeaderLayout->addItem(tableHeaderSpacer);

        exportPDFButton = new QPushButton(tableCardWidget);
        exportPDFButton->setObjectName("exportPDFButton");
        exportPDFButton->setMinimumSize(QSize(0, 35));
        exportPDFButton->setStyleSheet(QString::fromUtf8("background-color: #3498DB; color: white; border: none; border-radius: 8px; padding: 0 20px; font-weight: bold;"));

        tableHeaderLayout->addWidget(exportPDFButton);

        historiqueButton = new QPushButton(tableCardWidget);
        historiqueButton->setObjectName("historiqueButton");
        historiqueButton->setMinimumSize(QSize(0, 35));
        historiqueButton->setStyleSheet(QString::fromUtf8("background-color: #3498DB; color: white; border: none; border-radius: 8px; padding: 0 20px; font-weight: bold;"));

        tableHeaderLayout->addWidget(historiqueButton);


        tableCardLayout->addLayout(tableHeaderLayout);

        searchLayout = new QHBoxLayout();
        searchLayout->setObjectName("searchLayout");
        searchLineEdit = new QLineEdit(tableCardWidget);
        searchLineEdit->setObjectName("searchLineEdit");
        searchLineEdit->setMinimumSize(QSize(0, 40));
        searchLineEdit->setStyleSheet(QString::fromUtf8("border: 2px solid #BDC3C7; border-radius: 8px; padding-left: 15px; font-size: 14px;"));

        searchLayout->addWidget(searchLineEdit);

        searchButton = new QPushButton(tableCardWidget);
        searchButton->setObjectName("searchButton");
        searchButton->setMinimumSize(QSize(0, 40));
        searchButton->setStyleSheet(QString::fromUtf8("background-color: #3498DB; color: white; border: none; border-radius: 8px; padding: 0 25px; font-weight: bold;"));

        searchLayout->addWidget(searchButton);

        sortComboBox = new QComboBox(tableCardWidget);
        sortComboBox->addItem(QString());
        sortComboBox->addItem(QString());
        sortComboBox->addItem(QString());
        sortComboBox->setObjectName("sortComboBox");
        sortComboBox->setMinimumSize(QSize(0, 40));
        sortComboBox->setStyleSheet(QString::fromUtf8("border: 2px solid #BDC3C7; border-radius: 8px; padding-left: 10px; font-size: 14px;"));

        searchLayout->addWidget(sortComboBox);


        tableCardLayout->addLayout(searchLayout);

        vehiculesTable = new QTableWidget(tableCardWidget);
        if (vehiculesTable->columnCount() < 7)
            vehiculesTable->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        vehiculesTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        vehiculesTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        vehiculesTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        vehiculesTable->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        vehiculesTable->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        vehiculesTable->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        vehiculesTable->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        vehiculesTable->setObjectName("vehiculesTable");
        vehiculesTable->setStyleSheet(QString::fromUtf8("border: 1px solid #E0E0E0; border-radius: 8px; gridline-color: #E0E0E0;"));
        vehiculesTable->setAlternatingRowColors(true);
        vehiculesTable->setSelectionBehavior(QAbstractItemView::SelectRows);

        tableCardLayout->addWidget(vehiculesTable);

        tableActionsLayout = new QHBoxLayout();
        tableActionsLayout->setObjectName("tableActionsLayout");
        tableActionsSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        tableActionsLayout->addItem(tableActionsSpacer);

        modifyButton = new QPushButton(tableCardWidget);
        modifyButton->setObjectName("modifyButton");
        modifyButton->setMinimumSize(QSize(0, 40));
        modifyButton->setStyleSheet(QString::fromUtf8("background-color: #F39C12; color: white; border: none; border-radius: 8px; padding: 0 20px; font-weight: bold;"));

        tableActionsLayout->addWidget(modifyButton);

        deleteButton = new QPushButton(tableCardWidget);
        deleteButton->setObjectName("deleteButton");
        deleteButton->setMinimumSize(QSize(0, 40));
        deleteButton->setStyleSheet(QString::fromUtf8("background-color: #95A5A6; color: white; border: none; border-radius: 8px; padding: 0 20px; font-weight: bold;"));

        tableActionsLayout->addWidget(deleteButton);


        tableCardLayout->addLayout(tableActionsLayout);


        topRowLayout->addWidget(tableCardWidget);


        mainLayout->addLayout(topRowLayout);

        statsCardWidget = new QWidget(TransportsPage);
        statsCardWidget->setObjectName("statsCardWidget");
        statsCardWidget->setStyleSheet(QString::fromUtf8("background-color: white; border-radius: 15px;"));
        statsCardLayout = new QVBoxLayout(statsCardWidget);
        statsCardLayout->setSpacing(15);
        statsCardLayout->setObjectName("statsCardLayout");
        statsCardLayout->setContentsMargins(25, 25, 25, 25);
        statsCardTitle = new QLabel(statsCardWidget);
        statsCardTitle->setObjectName("statsCardTitle");
        statsCardTitle->setStyleSheet(QString::fromUtf8("font-size: 18px; font-weight: bold; color: #2C3E50;"));

        statsCardLayout->addWidget(statsCardTitle);

        indicatorsLayout = new QHBoxLayout();
        indicatorsLayout->setObjectName("indicatorsLayout");
        tauxUtilisationLabel = new QLabel(statsCardWidget);
        tauxUtilisationLabel->setObjectName("tauxUtilisationLabel");
        tauxUtilisationLabel->setStyleSheet(QString::fromUtf8("font-size: 14px; color: #3498DB; font-weight: bold; padding: 10px; background-color: #EBF5FB; border-radius: 8px;"));

        indicatorsLayout->addWidget(tauxUtilisationLabel);

        kmTotalLabel = new QLabel(statsCardWidget);
        kmTotalLabel->setObjectName("kmTotalLabel");
        kmTotalLabel->setStyleSheet(QString::fromUtf8("font-size: 14px; color: #27AE60; font-weight: bold; padding: 10px; background-color: #E8F8F5; border-radius: 8px;"));

        indicatorsLayout->addWidget(kmTotalLabel);

        tempsMoyenLabel = new QLabel(statsCardWidget);
        tempsMoyenLabel->setObjectName("tempsMoyenLabel");
        tempsMoyenLabel->setStyleSheet(QString::fromUtf8("font-size: 14px; color: #F39C12; font-weight: bold; padding: 10px; background-color: #FEF5E7; border-radius: 8px;"));

        indicatorsLayout->addWidget(tempsMoyenLabel);


        statsCardLayout->addLayout(indicatorsLayout);

        chartContainer = new QWidget(statsCardWidget);
        chartContainer->setObjectName("chartContainer");
        chartContainer->setMinimumSize(QSize(0, 200));
        chartContainerLayout = new QVBoxLayout(chartContainer);
        chartContainerLayout->setSpacing(0);
        chartContainerLayout->setObjectName("chartContainerLayout");
        chartContainerLayout->setContentsMargins(0, 0, 0, 0);

        statsCardLayout->addWidget(chartContainer);


        mainLayout->addWidget(statsCardWidget);


        retranslateUi(TransportsPage);

        QMetaObject::connectSlotsByName(TransportsPage);
    } // setupUi

    void retranslateUi(QWidget *TransportsPage)
    {
        TransportsPage->setWindowTitle(QCoreApplication::translate("TransportsPage", "Transports", nullptr));
        titleLabel->setText(QCoreApplication::translate("TransportsPage", "\360\237\232\227 Gestion des Transports", nullptr));
        formCardTitle->setText(QCoreApplication::translate("TransportsPage", "\360\237\232\231 Ajouter un V\303\251hicule", nullptr));
        addVehiculeButton->setText(QCoreApplication::translate("TransportsPage", "\342\236\225 Nouveau V\303\251hicule", nullptr));
        fleetInfoTitle->setText(QCoreApplication::translate("TransportsPage", "<html><head/><body><p><span style=\" font-weight:600; font-size:16px;\">\360\237\223\212 Info Flotte</span></p></body></html>", nullptr));
        totalVehiculesLabel->setText(QCoreApplication::translate("TransportsPage", "\360\237\232\227 Total v\303\251hicules: 0", nullptr));
        disponiblesLabel->setText(QCoreApplication::translate("TransportsPage", "\342\234\205 Disponibles: 0", nullptr));
        reservesLabel->setText(QCoreApplication::translate("TransportsPage", "\360\237\224\222 R\303\251serv\303\251s: 0", nullptr));
        maintenanceLabel->setText(QCoreApplication::translate("TransportsPage", "\360\237\224\247 Maintenance: 0", nullptr));
        tableCardTitle->setText(QCoreApplication::translate("TransportsPage", "\360\237\232\231 Liste de la Flotte", nullptr));
        exportPDFButton->setText(QCoreApplication::translate("TransportsPage", "\360\237\223\204 Export PDF", nullptr));
        historiqueButton->setText(QCoreApplication::translate("TransportsPage", "\360\237\223\234 Historique", nullptr));
#if QT_CONFIG(tooltip)
        historiqueButton->setToolTip(QCoreApplication::translate("TransportsPage", "\360\237\223\234 Historique - Afficher l'historique d\303\251taill\303\251 des v\303\251hicules et analytics avanc\303\251s", nullptr));
#endif // QT_CONFIG(tooltip)
        searchLineEdit->setPlaceholderText(QCoreApplication::translate("TransportsPage", "\360\237\224\215 Rechercher par ID ou type...", nullptr));
        searchButton->setText(QCoreApplication::translate("TransportsPage", "Rechercher", nullptr));
        sortComboBox->setItemText(0, QCoreApplication::translate("TransportsPage", "Trier par Type", nullptr));
        sortComboBox->setItemText(1, QCoreApplication::translate("TransportsPage", "Trier par Statut", nullptr));
        sortComboBox->setItemText(2, QCoreApplication::translate("TransportsPage", "Trier par Kilom\303\251trage", nullptr));

        QTableWidgetItem *___qtablewidgetitem = vehiculesTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("TransportsPage", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = vehiculesTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("TransportsPage", "Type", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = vehiculesTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("TransportsPage", "Capacit\303\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = vehiculesTable->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("TransportsPage", "Zone", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = vehiculesTable->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("TransportsPage", "Statut", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = vehiculesTable->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("TransportsPage", "Kilom\303\251trage", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = vehiculesTable->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("TransportsPage", "Temps Utilis\303\251", nullptr));
        modifyButton->setText(QCoreApplication::translate("TransportsPage", "\342\234\217\357\270\217 Modifier", nullptr));
        deleteButton->setText(QCoreApplication::translate("TransportsPage", "\360\237\227\221\357\270\217 Supprimer", nullptr));
        statsCardTitle->setText(QCoreApplication::translate("TransportsPage", "\360\237\223\212 Statistiques d'Utilisation", nullptr));
        tauxUtilisationLabel->setText(QCoreApplication::translate("TransportsPage", "Taux d'utilisation: 0%", nullptr));
        kmTotalLabel->setText(QCoreApplication::translate("TransportsPage", "KM Total: 0 km", nullptr));
        tempsMoyenLabel->setText(QCoreApplication::translate("TransportsPage", "Temps moyen: 0h", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TransportsPage: public Ui_TransportsPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRANSPORTSPAGE_H
