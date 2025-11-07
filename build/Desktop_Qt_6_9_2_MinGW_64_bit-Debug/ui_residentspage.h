/********************************************************************************
** Form generated from reading UI file 'residentspage.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RESIDENTSPAGE_H
#define UI_RESIDENTSPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
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

class Ui_ResidentsPage
{
public:
    QVBoxLayout *mainLayout;
    QLabel *pageTitle;
    QHBoxLayout *statisticsLayout;
    QFrame *totalResidentsCard;
    QVBoxLayout *totalResidentsLayout;
    QHBoxLayout *totalHeaderLayout;
    QLabel *totalIcon;
    QSpacerItem *totalSpacer;
    QLabel *totalResidentsValue;
    QLabel *totalResidentsLabel;
    QFrame *newResidentsCard;
    QVBoxLayout *newResidentsLayout;
    QHBoxLayout *newHeaderLayout;
    QLabel *newIcon;
    QSpacerItem *newSpacer;
    QLabel *newResidentsValue;
    QLabel *newResidentsLabel;
    QFrame *satisfactionCard;
    QVBoxLayout *satisfactionLayout;
    QHBoxLayout *satisfactionHeaderLayout;
    QLabel *satisfactionIcon;
    QSpacerItem *satisfactionSpacer;
    QLabel *satisfactionValue;
    QLabel *satisfactionLabel;
    QLabel *statsExplanation;
    QFrame *tableSection;
    QVBoxLayout *tableSectionLayout;
    QHBoxLayout *tableControlsLayout;
    QLineEdit *searchLineEdit;
    QComboBox *sortComboBox;
    QSpacerItem *tableControlsSpacer;
    QTableWidget *residentsTable;
    QHBoxLayout *actionButtonsLayout;
    QPushButton *addResidentButton;
    QPushButton *editResidentButton;
    QPushButton *deleteResidentButton;
    QPushButton *exportPdfButton;
    QSpacerItem *actionButtonsSpacer;
    QFrame *metiersAvancesSection;
    QVBoxLayout *metiersAvancesLayout;
    QLabel *metiersTitle;
    QHBoxLayout *metiersCardsLayout;
    QFrame *stabiliteCard;
    QVBoxLayout *stabiliteLayout;
    QLabel *stabiliteTitle;
    QLabel *stabiliteScore;
    QLabel *stabiliteRules;
    QLabel *stabiliteDescription;
    QFrame *alertesCard;
    QVBoxLayout *alertesLayout;
    QLabel *alertesTitle;
    QLabel *alertesContrats;
    QLabel *alertesNouveaux;
    QLabel *alertesIncidents;

    void setupUi(QWidget *ResidentsPage)
    {
        if (ResidentsPage->objectName().isEmpty())
            ResidentsPage->setObjectName("ResidentsPage");
        ResidentsPage->resize(1400, 900);
        ResidentsPage->setStyleSheet(QString::fromUtf8("background-color: #E9F1FF; font-family: 'Segoe UI', Arial, sans-serif;"));
        mainLayout = new QVBoxLayout(ResidentsPage);
        mainLayout->setSpacing(20);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(20, 20, 20, 20);
        pageTitle = new QLabel(ResidentsPage);
        pageTitle->setObjectName("pageTitle");
        pageTitle->setStyleSheet(QString::fromUtf8("font-size: 32px; font-weight: bold; color: #2C3E50; margin-bottom: 10px;"));

        mainLayout->addWidget(pageTitle);

        statisticsLayout = new QHBoxLayout();
        statisticsLayout->setSpacing(20);
        statisticsLayout->setObjectName("statisticsLayout");
        totalResidentsCard = new QFrame(ResidentsPage);
        totalResidentsCard->setObjectName("totalResidentsCard");
        totalResidentsCard->setFrameShape(QFrame::NoFrame);
        totalResidentsCard->setStyleSheet(QString::fromUtf8("background-color: white; border-radius: 15px; border: none; box-shadow: 0 4px 12px rgba(0,0,0,0.1);"));
        totalResidentsLayout = new QVBoxLayout(totalResidentsCard);
        totalResidentsLayout->setSpacing(10);
        totalResidentsLayout->setObjectName("totalResidentsLayout");
        totalResidentsLayout->setContentsMargins(25, 25, 25, 25);
        totalHeaderLayout = new QHBoxLayout();
        totalHeaderLayout->setObjectName("totalHeaderLayout");
        totalIcon = new QLabel(totalResidentsCard);
        totalIcon->setObjectName("totalIcon");
        totalIcon->setStyleSheet(QString::fromUtf8("font-size: 24px;"));

        totalHeaderLayout->addWidget(totalIcon);

        totalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        totalHeaderLayout->addItem(totalSpacer);


        totalResidentsLayout->addLayout(totalHeaderLayout);

        totalResidentsValue = new QLabel(totalResidentsCard);
        totalResidentsValue->setObjectName("totalResidentsValue");
        totalResidentsValue->setStyleSheet(QString::fromUtf8("font-size: 36px; font-weight: bold; color: #3168E0;"));

        totalResidentsLayout->addWidget(totalResidentsValue);

        totalResidentsLabel = new QLabel(totalResidentsCard);
        totalResidentsLabel->setObjectName("totalResidentsLabel");
        totalResidentsLabel->setStyleSheet(QString::fromUtf8("font-size: 14px; color: #6B7280; font-weight: 500;"));

        totalResidentsLayout->addWidget(totalResidentsLabel);


        statisticsLayout->addWidget(totalResidentsCard);

        newResidentsCard = new QFrame(ResidentsPage);
        newResidentsCard->setObjectName("newResidentsCard");
        newResidentsCard->setFrameShape(QFrame::NoFrame);
        newResidentsCard->setStyleSheet(QString::fromUtf8("background-color: white; border-radius: 15px; border: none; box-shadow: 0 4px 12px rgba(0,0,0,0.1);"));
        newResidentsLayout = new QVBoxLayout(newResidentsCard);
        newResidentsLayout->setSpacing(10);
        newResidentsLayout->setObjectName("newResidentsLayout");
        newResidentsLayout->setContentsMargins(25, 25, 25, 25);
        newHeaderLayout = new QHBoxLayout();
        newHeaderLayout->setObjectName("newHeaderLayout");
        newIcon = new QLabel(newResidentsCard);
        newIcon->setObjectName("newIcon");
        newIcon->setStyleSheet(QString::fromUtf8("font-size: 24px;"));

        newHeaderLayout->addWidget(newIcon);

        newSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        newHeaderLayout->addItem(newSpacer);


        newResidentsLayout->addLayout(newHeaderLayout);

        newResidentsValue = new QLabel(newResidentsCard);
        newResidentsValue->setObjectName("newResidentsValue");
        newResidentsValue->setStyleSheet(QString::fromUtf8("font-size: 36px; font-weight: bold; color: #10B981;"));

        newResidentsLayout->addWidget(newResidentsValue);

        newResidentsLabel = new QLabel(newResidentsCard);
        newResidentsLabel->setObjectName("newResidentsLabel");
        newResidentsLabel->setStyleSheet(QString::fromUtf8("font-size: 14px; color: #6B7280; font-weight: 500;"));

        newResidentsLayout->addWidget(newResidentsLabel);


        statisticsLayout->addWidget(newResidentsCard);

        satisfactionCard = new QFrame(ResidentsPage);
        satisfactionCard->setObjectName("satisfactionCard");
        satisfactionCard->setFrameShape(QFrame::NoFrame);
        satisfactionCard->setStyleSheet(QString::fromUtf8("background-color: white; border-radius: 15px; border: none; box-shadow: 0 4px 12px rgba(0,0,0,0.1);"));
        satisfactionLayout = new QVBoxLayout(satisfactionCard);
        satisfactionLayout->setSpacing(10);
        satisfactionLayout->setObjectName("satisfactionLayout");
        satisfactionLayout->setContentsMargins(25, 25, 25, 25);
        satisfactionHeaderLayout = new QHBoxLayout();
        satisfactionHeaderLayout->setObjectName("satisfactionHeaderLayout");
        satisfactionIcon = new QLabel(satisfactionCard);
        satisfactionIcon->setObjectName("satisfactionIcon");
        satisfactionIcon->setStyleSheet(QString::fromUtf8("font-size: 24px;"));

        satisfactionHeaderLayout->addWidget(satisfactionIcon);

        satisfactionSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        satisfactionHeaderLayout->addItem(satisfactionSpacer);


        satisfactionLayout->addLayout(satisfactionHeaderLayout);

        satisfactionValue = new QLabel(satisfactionCard);
        satisfactionValue->setObjectName("satisfactionValue");
        satisfactionValue->setStyleSheet(QString::fromUtf8("font-size: 36px; font-weight: bold; color: #FBBF24;"));

        satisfactionLayout->addWidget(satisfactionValue);

        satisfactionLabel = new QLabel(satisfactionCard);
        satisfactionLabel->setObjectName("satisfactionLabel");
        satisfactionLabel->setStyleSheet(QString::fromUtf8("font-size: 14px; color: #6B7280; font-weight: 500;"));

        satisfactionLayout->addWidget(satisfactionLabel);


        statisticsLayout->addWidget(satisfactionCard);


        mainLayout->addLayout(statisticsLayout);

        statsExplanation = new QLabel(ResidentsPage);
        statsExplanation->setObjectName("statsExplanation");
        statsExplanation->setStyleSheet(QString::fromUtf8("font-size: 12px; color: #9CA3AF; font-style: italic;"));
        statsExplanation->setAlignment(Qt::AlignCenter);

        mainLayout->addWidget(statsExplanation);

        tableSection = new QFrame(ResidentsPage);
        tableSection->setObjectName("tableSection");
        tableSection->setFrameShape(QFrame::NoFrame);
        tableSection->setStyleSheet(QString::fromUtf8("background-color: white; border-radius: 15px; border: none; box-shadow: 0 4px 12px rgba(0,0,0,0.1);"));
        tableSectionLayout = new QVBoxLayout(tableSection);
        tableSectionLayout->setSpacing(20);
        tableSectionLayout->setObjectName("tableSectionLayout");
        tableSectionLayout->setContentsMargins(30, 30, 30, 30);
        tableControlsLayout = new QHBoxLayout();
        tableControlsLayout->setSpacing(15);
        tableControlsLayout->setObjectName("tableControlsLayout");
        searchLineEdit = new QLineEdit(tableSection);
        searchLineEdit->setObjectName("searchLineEdit");
        searchLineEdit->setStyleSheet(QString::fromUtf8("border: 2px solid #E5E7EB; border-radius: 10px; padding: 12px 15px; font-size: 14px; background-color: #F9FAFB;"));
        searchLineEdit->setMinimumSize(QSize(300, 45));

        tableControlsLayout->addWidget(searchLineEdit);

        sortComboBox = new QComboBox(tableSection);
        sortComboBox->addItem(QString());
        sortComboBox->addItem(QString());
        sortComboBox->addItem(QString());
        sortComboBox->addItem(QString());
        sortComboBox->setObjectName("sortComboBox");
        sortComboBox->setStyleSheet(QString::fromUtf8("border: 2px solid #E5E7EB; border-radius: 10px; padding: 12px 15px; font-size: 14px; background-color: #F9FAFB;"));
        sortComboBox->setMinimumSize(QSize(200, 45));

        tableControlsLayout->addWidget(sortComboBox);

        tableControlsSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        tableControlsLayout->addItem(tableControlsSpacer);


        tableSectionLayout->addLayout(tableControlsLayout);

        residentsTable = new QTableWidget(tableSection);
        if (residentsTable->columnCount() < 10)
            residentsTable->setColumnCount(10);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        residentsTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        residentsTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        residentsTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        residentsTable->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        residentsTable->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        residentsTable->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        residentsTable->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        residentsTable->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        residentsTable->setHorizontalHeaderItem(8, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        residentsTable->setHorizontalHeaderItem(9, __qtablewidgetitem9);
        residentsTable->setObjectName("residentsTable");
        residentsTable->setStyleSheet(QString::fromUtf8("\n"
"QTableWidget {\n"
"    border: 1px solid #E5E7EB;\n"
"    border-radius: 10px;\n"
"    background-color: white;\n"
"    gridline-color: #F3F4F6;\n"
"    font-size: 14px;\n"
"}\n"
"QTableWidget::item {\n"
"    padding: 12px;\n"
"    border-bottom: 1px solid #F3F4F6;\n"
"}\n"
"QTableWidget::item:selected {\n"
"    background-color: #EBF4FF;\n"
"    color: #1E40AF;\n"
"}\n"
"QHeaderView::section {\n"
"    background-color: #F8FAFC;\n"
"    color: #374151;\n"
"    font-weight: bold;\n"
"    font-size: 14px;\n"
"    border: none;\n"
"    border-bottom: 2px solid #E5E7EB;\n"
"    padding: 15px 12px;\n"
"}\n"
"QTableWidget::item:alternate {\n"
"    background-color: #F8FAFC;\n"
"}\n"
"         "));
        residentsTable->setAlternatingRowColors(true);
        residentsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        residentsTable->setSelectionMode(QAbstractItemView::SingleSelection);
        residentsTable->setShowGrid(false);

        tableSectionLayout->addWidget(residentsTable);

        actionButtonsLayout = new QHBoxLayout();
        actionButtonsLayout->setSpacing(15);
        actionButtonsLayout->setObjectName("actionButtonsLayout");
        addResidentButton = new QPushButton(tableSection);
        addResidentButton->setObjectName("addResidentButton");
        addResidentButton->setStyleSheet(QString::fromUtf8("background-color: #3168E0; color: white; border: none; border-radius: 10px; font-weight: bold; font-size: 14px; padding: 12px 25px;"));
        addResidentButton->setMinimumSize(QSize(120, 45));

        actionButtonsLayout->addWidget(addResidentButton);

        editResidentButton = new QPushButton(tableSection);
        editResidentButton->setObjectName("editResidentButton");
        editResidentButton->setStyleSheet(QString::fromUtf8("background-color: #FBBF24; color: white; border: none; border-radius: 10px; font-weight: bold; font-size: 14px; padding: 12px 25px;"));
        editResidentButton->setMinimumSize(QSize(120, 45));

        actionButtonsLayout->addWidget(editResidentButton);

        deleteResidentButton = new QPushButton(tableSection);
        deleteResidentButton->setObjectName("deleteResidentButton");
        deleteResidentButton->setStyleSheet(QString::fromUtf8("background-color: #E74C3C; color: white; border: none; border-radius: 10px; font-weight: bold; font-size: 14px; padding: 12px 25px;"));
        deleteResidentButton->setMinimumSize(QSize(120, 45));

        actionButtonsLayout->addWidget(deleteResidentButton);

        exportPdfButton = new QPushButton(tableSection);
        exportPdfButton->setObjectName("exportPdfButton");
        exportPdfButton->setStyleSheet(QString::fromUtf8("background-color: #8E44AD; color: white; border: none; border-radius: 10px; font-weight: bold; font-size: 14px; padding: 12px 25px;"));
        exportPdfButton->setMinimumSize(QSize(140, 45));

        actionButtonsLayout->addWidget(exportPdfButton);

        actionButtonsSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        actionButtonsLayout->addItem(actionButtonsSpacer);


        tableSectionLayout->addLayout(actionButtonsLayout);


        mainLayout->addWidget(tableSection);

        metiersAvancesSection = new QFrame(ResidentsPage);
        metiersAvancesSection->setObjectName("metiersAvancesSection");
        metiersAvancesSection->setFrameShape(QFrame::NoFrame);
        metiersAvancesSection->setStyleSheet(QString::fromUtf8("background-color: white; border-radius: 15px; border: none; box-shadow: 0 4px 12px rgba(0,0,0,0.1);"));
        metiersAvancesLayout = new QVBoxLayout(metiersAvancesSection);
        metiersAvancesLayout->setSpacing(20);
        metiersAvancesLayout->setObjectName("metiersAvancesLayout");
        metiersAvancesLayout->setContentsMargins(30, 30, 30, 30);
        metiersTitle = new QLabel(metiersAvancesSection);
        metiersTitle->setObjectName("metiersTitle");
        metiersTitle->setStyleSheet(QString::fromUtf8("font-size: 20px; font-weight: bold; color: #2C3E50;"));

        metiersAvancesLayout->addWidget(metiersTitle);

        metiersCardsLayout = new QHBoxLayout();
        metiersCardsLayout->setSpacing(20);
        metiersCardsLayout->setObjectName("metiersCardsLayout");
        stabiliteCard = new QFrame(metiersAvancesSection);
        stabiliteCard->setObjectName("stabiliteCard");
        stabiliteCard->setFrameShape(QFrame::NoFrame);
        stabiliteCard->setStyleSheet(QString::fromUtf8("background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #EBF4FF, stop:1 #DBEAFE); border-radius: 12px; border: 1px solid #BFDBFE;"));
        stabiliteLayout = new QVBoxLayout(stabiliteCard);
        stabiliteLayout->setSpacing(15);
        stabiliteLayout->setObjectName("stabiliteLayout");
        stabiliteLayout->setContentsMargins(20, 20, 20, 20);
        stabiliteTitle = new QLabel(stabiliteCard);
        stabiliteTitle->setObjectName("stabiliteTitle");
        stabiliteTitle->setStyleSheet(QString::fromUtf8("font-size: 16px; font-weight: bold; color: #1E40AF;"));

        stabiliteLayout->addWidget(stabiliteTitle);

        stabiliteScore = new QLabel(stabiliteCard);
        stabiliteScore->setObjectName("stabiliteScore");
        stabiliteScore->setStyleSheet(QString::fromUtf8("font-size: 24px; font-weight: bold; color: #3168E0;"));

        stabiliteLayout->addWidget(stabiliteScore);

        stabiliteRules = new QLabel(stabiliteCard);
        stabiliteRules->setObjectName("stabiliteRules");
        stabiliteRules->setStyleSheet(QString::fromUtf8("font-size: 12px; color: #4B5563; line-height: 1.5;"));
        stabiliteRules->setWordWrap(true);

        stabiliteLayout->addWidget(stabiliteRules);

        stabiliteDescription = new QLabel(stabiliteCard);
        stabiliteDescription->setObjectName("stabiliteDescription");
        stabiliteDescription->setStyleSheet(QString::fromUtf8("font-size: 11px; color: #6B7280; font-style: italic;"));
        stabiliteDescription->setWordWrap(true);

        stabiliteLayout->addWidget(stabiliteDescription);


        metiersCardsLayout->addWidget(stabiliteCard);

        alertesCard = new QFrame(metiersAvancesSection);
        alertesCard->setObjectName("alertesCard");
        alertesCard->setFrameShape(QFrame::NoFrame);
        alertesCard->setStyleSheet(QString::fromUtf8("background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #FEF3E2, stop:1 #FDE68A); border-radius: 12px; border: 1px solid #F59E0B;"));
        alertesLayout = new QVBoxLayout(alertesCard);
        alertesLayout->setSpacing(15);
        alertesLayout->setObjectName("alertesLayout");
        alertesLayout->setContentsMargins(20, 20, 20, 20);
        alertesTitle = new QLabel(alertesCard);
        alertesTitle->setObjectName("alertesTitle");
        alertesTitle->setStyleSheet(QString::fromUtf8("font-size: 16px; font-weight: bold; color: #B45309;"));

        alertesLayout->addWidget(alertesTitle);

        alertesContrats = new QLabel(alertesCard);
        alertesContrats->setObjectName("alertesContrats");
        alertesContrats->setStyleSheet(QString::fromUtf8("font-size: 14px; color: #92400E; font-weight: 500;"));
        alertesContrats->setWordWrap(true);

        alertesLayout->addWidget(alertesContrats);

        alertesNouveaux = new QLabel(alertesCard);
        alertesNouveaux->setObjectName("alertesNouveaux");
        alertesNouveaux->setStyleSheet(QString::fromUtf8("font-size: 14px; color: #92400E; font-weight: 500;"));
        alertesNouveaux->setWordWrap(true);

        alertesLayout->addWidget(alertesNouveaux);

        alertesIncidents = new QLabel(alertesCard);
        alertesIncidents->setObjectName("alertesIncidents");
        alertesIncidents->setStyleSheet(QString::fromUtf8("font-size: 14px; color: #92400E; font-weight: 500;"));
        alertesIncidents->setWordWrap(true);

        alertesLayout->addWidget(alertesIncidents);


        metiersCardsLayout->addWidget(alertesCard);


        metiersAvancesLayout->addLayout(metiersCardsLayout);


        mainLayout->addWidget(metiersAvancesSection);


        retranslateUi(ResidentsPage);

        QMetaObject::connectSlotsByName(ResidentsPage);
    } // setupUi

    void retranslateUi(QWidget *ResidentsPage)
    {
        ResidentsPage->setWindowTitle(QCoreApplication::translate("ResidentsPage", "Gestion des R\303\251sidents", nullptr));
        pageTitle->setText(QCoreApplication::translate("ResidentsPage", "\360\237\221\245 Gestion des R\303\251sidents", nullptr));
        totalIcon->setText(QCoreApplication::translate("ResidentsPage", "\360\237\221\250\342\200\215\360\237\221\251\342\200\215\360\237\221\247\342\200\215\360\237\221\246", nullptr));
        totalResidentsValue->setText(QCoreApplication::translate("ResidentsPage", "0", nullptr));
        totalResidentsLabel->setText(QCoreApplication::translate("ResidentsPage", "Total R\303\251sidents", nullptr));
        newIcon->setText(QCoreApplication::translate("ResidentsPage", "\360\237\217\240", nullptr));
        newResidentsValue->setText(QCoreApplication::translate("ResidentsPage", "0", nullptr));
        newResidentsLabel->setText(QCoreApplication::translate("ResidentsPage", "Nouveaux ce mois", nullptr));
#if QT_CONFIG(tooltip)
        satisfactionCard->setToolTip(QCoreApplication::translate("ResidentsPage", "Calcul\303\251 automatiquement selon la stabilit\303\251 et les incidents", nullptr));
#endif // QT_CONFIG(tooltip)
        satisfactionIcon->setText(QCoreApplication::translate("ResidentsPage", "\360\237\222\254", nullptr));
        satisfactionValue->setText(QCoreApplication::translate("ResidentsPage", "85%", nullptr));
        satisfactionLabel->setText(QCoreApplication::translate("ResidentsPage", "Satisfaction Moyenne", nullptr));
        statsExplanation->setText(QCoreApplication::translate("ResidentsPage", "Bas\303\251 sur les donn\303\251es des r\303\251sidents actifs", nullptr));
        searchLineEdit->setPlaceholderText(QCoreApplication::translate("ResidentsPage", "\360\237\224\215 Rechercher par nom ou CIN...", nullptr));
        sortComboBox->setItemText(0, QCoreApplication::translate("ResidentsPage", "Trier par CIN", nullptr));
        sortComboBox->setItemText(1, QCoreApplication::translate("ResidentsPage", "Trier par Nom", nullptr));
        sortComboBox->setItemText(2, QCoreApplication::translate("ResidentsPage", "Trier par Appartement", nullptr));
        sortComboBox->setItemText(3, QCoreApplication::translate("ResidentsPage", "Trier par Date d'entr\303\251e", nullptr));

        QTableWidgetItem *___qtablewidgetitem = residentsTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("ResidentsPage", "CIN", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = residentsTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("ResidentsPage", "Nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = residentsTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("ResidentsPage", "Pr\303\251nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = residentsTable->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("ResidentsPage", "Sexe", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = residentsTable->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("ResidentsPage", "T\303\251l\303\251phone", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = residentsTable->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("ResidentsPage", "Email", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = residentsTable->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("ResidentsPage", "Date d'entr\303\251e", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = residentsTable->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("ResidentsPage", "Appartement", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = residentsTable->horizontalHeaderItem(8);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("ResidentsPage", "\303\211tage", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = residentsTable->horizontalHeaderItem(9);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("ResidentsPage", "Statut", nullptr));
        addResidentButton->setText(QCoreApplication::translate("ResidentsPage", "\342\236\225 Nouveau", nullptr));
        editResidentButton->setText(QCoreApplication::translate("ResidentsPage", "\342\234\217\357\270\217 Modifier", nullptr));
        deleteResidentButton->setText(QCoreApplication::translate("ResidentsPage", "\360\237\227\221\357\270\217 Supprimer", nullptr));
        exportPdfButton->setText(QCoreApplication::translate("ResidentsPage", "\360\237\223\204 Exporter PDF", nullptr));
#if QT_CONFIG(tooltip)
        exportPdfButton->setToolTip(QCoreApplication::translate("ResidentsPage", "G\303\251n\303\251rer la fiche des r\303\251sidents au format PDF", nullptr));
#endif // QT_CONFIG(tooltip)
        metiersTitle->setText(QCoreApplication::translate("ResidentsPage", "\360\237\247\240 M\303\251tiers Avanc\303\251s \342\200\223 Intelligence R\303\251sidentielle", nullptr));
        stabiliteTitle->setText(QCoreApplication::translate("ResidentsPage", "\360\237\223\212 Stabilit\303\251 & Satisfaction", nullptr));
        stabiliteScore->setText(QCoreApplication::translate("ResidentsPage", "Score comportemental : 82/100", nullptr));
        stabiliteRules->setText(QCoreApplication::translate("ResidentsPage", "+10 \342\206\222 Renouvellement de contrat\n"
"\342\210\22220 \342\206\222 D\303\251part anticip\303\251  \n"
"\342\210\2225 \342\206\222 Signalement d'incident\n"
"+5 \342\206\222 Aucun incident sur 6 mois", nullptr));
        stabiliteDescription->setText(QCoreApplication::translate("ResidentsPage", "Score comportemental estim\303\251 automatiquement selon les actions du r\303\251sident.", nullptr));
        alertesTitle->setText(QCoreApplication::translate("ResidentsPage", "\360\237\224\224 Alertes Contrat & Notifications", nullptr));
        alertesContrats->setText(QCoreApplication::translate("ResidentsPage", "\360\237\223\213 3 contrats arrivent \303\240 \303\251ch\303\251ance cette semaine", nullptr));
        alertesNouveaux->setText(QCoreApplication::translate("ResidentsPage", "\360\237\217\240 2 nouveaux r\303\251sidents \303\240 accueillir", nullptr));
        alertesIncidents->setText(QCoreApplication::translate("ResidentsPage", "\342\232\240\357\270\217 1 incident en cours de traitement", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ResidentsPage: public Ui_ResidentsPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RESIDENTSPAGE_H
