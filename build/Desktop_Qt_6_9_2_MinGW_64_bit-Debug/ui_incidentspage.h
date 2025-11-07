/********************************************************************************
** Form generated from reading UI file 'incidentspage.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INCIDENTSPAGE_H
#define UI_INCIDENTSPAGE_H

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

class Ui_IncidentsPage
{
public:
    QVBoxLayout *mainLayout;
    QLabel *titleLabel;
    QHBoxLayout *topRowLayout;
    QWidget *formCardWidget;
    QVBoxLayout *formCardLayout;
    QLabel *formCardTitle;
    QPushButton *addIncidentButton;
    QLabel *dashboardTitle;
    QLabel *infoLabel;
    QSpacerItem *formCardSpacer;
    QWidget *tableCardWidget;
    QVBoxLayout *tableCardLayout;
    QHBoxLayout *tableHeaderLayout;
    QLabel *tableCardTitle;
    QSpacerItem *tableHeaderSpacer;
    QPushButton *exportPDFButton;
    QHBoxLayout *searchLayout;
    QLineEdit *searchLineEdit;
    QPushButton *searchButton;
    QComboBox *sortComboBox;
    QTableWidget *incidentsTable;
    QHBoxLayout *tableActionsLayout;
    QSpacerItem *tableActionsSpacer;
    QPushButton *modifyButton;
    QPushButton *deleteButton;
    QWidget *statsCardWidget;
    QVBoxLayout *statsCardLayout;
    QLabel *statsCardTitle;
    QHBoxLayout *indicatorsLayout;
    QLabel *totalLabel;
    QLabel *traitesLabel;
    QLabel *enAttenteLabel;
    QLabel *enCoursLabel;
    QWidget *chartContainer;
    QVBoxLayout *chartContainerLayout;

    void setupUi(QWidget *IncidentsPage)
    {
        if (IncidentsPage->objectName().isEmpty())
            IncidentsPage->setObjectName("IncidentsPage");
        IncidentsPage->resize(1200, 800);
        IncidentsPage->setStyleSheet(QString::fromUtf8("background-color: #F5F7FA;"));
        mainLayout = new QVBoxLayout(IncidentsPage);
        mainLayout->setSpacing(20);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(20, 20, 20, 20);
        titleLabel = new QLabel(IncidentsPage);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setStyleSheet(QString::fromUtf8("font-size: 28px; font-weight: bold; color: #2C3E50;"));

        mainLayout->addWidget(titleLabel);

        topRowLayout = new QHBoxLayout();
        topRowLayout->setSpacing(20);
        topRowLayout->setObjectName("topRowLayout");
        formCardWidget = new QWidget(IncidentsPage);
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

        addIncidentButton = new QPushButton(formCardWidget);
        addIncidentButton->setObjectName("addIncidentButton");
        addIncidentButton->setMinimumSize(QSize(0, 50));
        addIncidentButton->setStyleSheet(QString::fromUtf8("background-color: #E74C3C; color: white; border: none; border-radius: 10px; font-size: 16px; font-weight: bold;"));

        formCardLayout->addWidget(addIncidentButton);

        dashboardTitle = new QLabel(formCardWidget);
        dashboardTitle->setObjectName("dashboardTitle");

        formCardLayout->addWidget(dashboardTitle);

        infoLabel = new QLabel(formCardWidget);
        infoLabel->setObjectName("infoLabel");
        infoLabel->setWordWrap(true);
        infoLabel->setStyleSheet(QString::fromUtf8("color: #7F8C8D; font-size: 12px; padding: 12px; background-color: #FCF3CF; border-radius: 8px;"));

        formCardLayout->addWidget(infoLabel);

        formCardSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        formCardLayout->addItem(formCardSpacer);


        topRowLayout->addWidget(formCardWidget);

        tableCardWidget = new QWidget(IncidentsPage);
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
        exportPDFButton->setStyleSheet(QString::fromUtf8("background-color: #E74C3C; color: white; border: none; border-radius: 8px; padding: 0 20px; font-weight: bold;"));

        tableHeaderLayout->addWidget(exportPDFButton);


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
        searchButton->setStyleSheet(QString::fromUtf8("background-color: #E74C3C; color: white; border: none; border-radius: 8px; padding: 0 25px; font-weight: bold;"));

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

        incidentsTable = new QTableWidget(tableCardWidget);
        if (incidentsTable->columnCount() < 6)
            incidentsTable->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        incidentsTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        incidentsTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        incidentsTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        incidentsTable->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        incidentsTable->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        incidentsTable->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        incidentsTable->setObjectName("incidentsTable");
        incidentsTable->setStyleSheet(QString::fromUtf8("border: 1px solid #E0E0E0; border-radius: 8px; gridline-color: #E0E0E0;"));
        incidentsTable->setAlternatingRowColors(true);
        incidentsTable->setSelectionBehavior(QAbstractItemView::SelectRows);

        tableCardLayout->addWidget(incidentsTable);

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

        statsCardWidget = new QWidget(IncidentsPage);
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
        totalLabel = new QLabel(statsCardWidget);
        totalLabel->setObjectName("totalLabel");
        totalLabel->setStyleSheet(QString::fromUtf8("font-size: 14px; color: #2C3E50; font-weight: bold; padding: 10px; background-color: #EBF5FB; border-radius: 8px;"));

        indicatorsLayout->addWidget(totalLabel);

        traitesLabel = new QLabel(statsCardWidget);
        traitesLabel->setObjectName("traitesLabel");
        traitesLabel->setStyleSheet(QString::fromUtf8("font-size: 14px; color: #27AE60; font-weight: bold; padding: 10px; background-color: #E8F8F5; border-radius: 8px;"));

        indicatorsLayout->addWidget(traitesLabel);

        enAttenteLabel = new QLabel(statsCardWidget);
        enAttenteLabel->setObjectName("enAttenteLabel");
        enAttenteLabel->setStyleSheet(QString::fromUtf8("font-size: 14px; color: #E74C3C; font-weight: bold; padding: 10px; background-color: #FADBD8; border-radius: 8px;"));

        indicatorsLayout->addWidget(enAttenteLabel);

        enCoursLabel = new QLabel(statsCardWidget);
        enCoursLabel->setObjectName("enCoursLabel");
        enCoursLabel->setStyleSheet(QString::fromUtf8("font-size: 14px; color: #F39C12; font-weight: bold; padding: 10px; background-color: #FEF5E7; border-radius: 8px;"));

        indicatorsLayout->addWidget(enCoursLabel);


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


        retranslateUi(IncidentsPage);

        QMetaObject::connectSlotsByName(IncidentsPage);
    } // setupUi

    void retranslateUi(QWidget *IncidentsPage)
    {
        IncidentsPage->setWindowTitle(QCoreApplication::translate("IncidentsPage", "Incidents", nullptr));
        titleLabel->setText(QCoreApplication::translate("IncidentsPage", "\360\237\232\250 Gestion des Incidents", nullptr));
        formCardTitle->setText(QCoreApplication::translate("IncidentsPage", "\342\232\240\357\270\217 D\303\251clarer un Incident", nullptr));
        addIncidentButton->setText(QCoreApplication::translate("IncidentsPage", "\342\236\225 Nouvel Incident", nullptr));
        dashboardTitle->setText(QCoreApplication::translate("IncidentsPage", "<html><head/><body><p><span style=\" font-weight:600; font-size:16px;\">\360\237\223\212 Dashboard Temps R\303\251el</span></p></body></html>", nullptr));
        infoLabel->setText(QCoreApplication::translate("IncidentsPage", "<html><head/><body><p><span style=\" font-weight:600;\">Types d'incidents:</span><br/>\342\200\242 Panne \303\251lectricit\303\251<br/>\342\200\242 Fuite eau<br/>\342\200\242 Accident<br/>\342\200\242 Incendie<br/>\342\200\242 Ascenseur<br/>\342\200\242 S\303\251curit\303\251</p></body></html>", nullptr));
        tableCardTitle->setText(QCoreApplication::translate("IncidentsPage", "\360\237\223\213 Liste des Incidents", nullptr));
        exportPDFButton->setText(QCoreApplication::translate("IncidentsPage", "\360\237\223\204 Export PDF", nullptr));
        searchLineEdit->setPlaceholderText(QCoreApplication::translate("IncidentsPage", "\360\237\224\215 Rechercher par type ou localisation...", nullptr));
        searchButton->setText(QCoreApplication::translate("IncidentsPage", "Rechercher", nullptr));
        sortComboBox->setItemText(0, QCoreApplication::translate("IncidentsPage", "Trier par Gravit\303\251", nullptr));
        sortComboBox->setItemText(1, QCoreApplication::translate("IncidentsPage", "Trier par Date", nullptr));
        sortComboBox->setItemText(2, QCoreApplication::translate("IncidentsPage", "Trier par Type", nullptr));

        QTableWidgetItem *___qtablewidgetitem = incidentsTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("IncidentsPage", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = incidentsTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("IncidentsPage", "Type", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = incidentsTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("IncidentsPage", "Localisation", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = incidentsTable->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("IncidentsPage", "Date & Heure", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = incidentsTable->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("IncidentsPage", "Gravit\303\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = incidentsTable->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("IncidentsPage", "Statut", nullptr));
        modifyButton->setText(QCoreApplication::translate("IncidentsPage", "\342\234\217\357\270\217 Modifier", nullptr));
        deleteButton->setText(QCoreApplication::translate("IncidentsPage", "\360\237\227\221\357\270\217 Supprimer", nullptr));
        statsCardTitle->setText(QCoreApplication::translate("IncidentsPage", "\360\237\223\212 Statistiques & \303\211volution", nullptr));
        totalLabel->setText(QCoreApplication::translate("IncidentsPage", "Total: 0", nullptr));
        traitesLabel->setText(QCoreApplication::translate("IncidentsPage", "Trait\303\251s: 0", nullptr));
        enAttenteLabel->setText(QCoreApplication::translate("IncidentsPage", "En Attente: 0", nullptr));
        enCoursLabel->setText(QCoreApplication::translate("IncidentsPage", "En Cours: 0", nullptr));
    } // retranslateUi

};

namespace Ui {
    class IncidentsPage: public Ui_IncidentsPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INCIDENTSPAGE_H
