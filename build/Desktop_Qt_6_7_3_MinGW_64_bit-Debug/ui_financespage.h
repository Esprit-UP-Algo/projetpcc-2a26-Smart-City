/********************************************************************************
** Form generated from reading UI file 'financespage.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FINANCESPAGE_H
#define UI_FINANCESPAGE_H

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
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FinancesPage
{
public:
    QVBoxLayout *mainLayout;
    QLabel *titleLabel;
    QHBoxLayout *topRowLayout;
    QWidget *formCardWidget;
    QVBoxLayout *formCardLayout;
    QLabel *formCardTitle;
    QPushButton *addTransactionButton;
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
    QTableWidget *transactionsTable;
    QHBoxLayout *tableActionsLayout;
    QSpacerItem *tableActionsSpacer;
    QPushButton *modifyButton;
    QPushButton *deleteButton;
    QHBoxLayout *bottomRowLayout;
    QWidget *statsCardWidget;
    QVBoxLayout *statsCardLayout;
    QLabel *statsCardTitle;
    QFrame *separator1;
    QLabel *stat1Title;
    QLabel *stat1Description;
    QHBoxLayout *indicatorsLayout;
    QLabel *revenueLabel;
    QLabel *expensesLabel;
    QLabel *balanceLabel;
    QFrame *separator2;
    QLabel *stat2Title;
    QLabel *stat2Description;
    QLabel *processingDelayLabel;
    QFrame *separator3;
    QLabel *statusBadge;
    QWidget *chartContainer;
    QVBoxLayout *chartContainerLayout;
    QWidget *assistantCardWidget;
    QVBoxLayout *assistantCardLayout;
    QLabel *assistantCardTitle;
    QTextEdit *chatTextEdit;
    QHBoxLayout *inputLayout;
    QLineEdit *questionLineEdit;
    QPushButton *sendButton;
    QLabel *actionsLabel;
    QHBoxLayout *quickActionsLayout;
    QPushButton *budgetButton;
    QPushButton *simulateButton;
    QPushButton *reportButton;

    void setupUi(QWidget *FinancesPage)
    {
        if (FinancesPage->objectName().isEmpty())
            FinancesPage->setObjectName("FinancesPage");
        FinancesPage->resize(1200, 1206);
        FinancesPage->setStyleSheet(QString::fromUtf8("background-color: #F5F7FA;"));
        mainLayout = new QVBoxLayout(FinancesPage);
        mainLayout->setSpacing(20);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(20, 20, 20, 20);
        titleLabel = new QLabel(FinancesPage);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setStyleSheet(QString::fromUtf8("font-size: 28px; font-weight: bold; color: #2C3E50;"));

        mainLayout->addWidget(titleLabel);

        topRowLayout = new QHBoxLayout();
        topRowLayout->setSpacing(20);
        topRowLayout->setObjectName("topRowLayout");
        formCardWidget = new QWidget(FinancesPage);
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

        addTransactionButton = new QPushButton(formCardWidget);
        addTransactionButton->setObjectName("addTransactionButton");
        addTransactionButton->setMinimumSize(QSize(0, 50));
        addTransactionButton->setStyleSheet(QString::fromUtf8("background-color: #4A90E2; color: white; border: none; border-radius: 10px; font-size: 16px; font-weight: bold;"));

        formCardLayout->addWidget(addTransactionButton);

        infoLabel = new QLabel(formCardWidget);
        infoLabel->setObjectName("infoLabel");
        infoLabel->setStyleSheet(QString::fromUtf8("color: #7F8C8D; font-size: 13px; padding: 15px; background-color: #ECF0F1; border-radius: 8px;"));
        infoLabel->setWordWrap(true);

        formCardLayout->addWidget(infoLabel);

        formCardSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        formCardLayout->addItem(formCardSpacer);


        topRowLayout->addWidget(formCardWidget);

        tableCardWidget = new QWidget(FinancesPage);
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
        exportPDFButton->setStyleSheet(QString::fromUtf8("background-color: #9B59B6; color: white; border: none; border-radius: 8px; padding: 0 20px; font-weight: bold;"));

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
        searchButton->setStyleSheet(QString::fromUtf8("background-color: #4A90E2; color: white; border: none; border-radius: 8px; padding: 0 25px; font-weight: bold;"));

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

        transactionsTable = new QTableWidget(tableCardWidget);
        if (transactionsTable->columnCount() < 7)
            transactionsTable->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        transactionsTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        transactionsTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        transactionsTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        transactionsTable->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        transactionsTable->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        transactionsTable->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        transactionsTable->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        transactionsTable->setObjectName("transactionsTable");
        transactionsTable->setStyleSheet(QString::fromUtf8("border: 1px solid #E0E0E0; border-radius: 8px; gridline-color: #E0E0E0;"));
        transactionsTable->setAlternatingRowColors(true);
        transactionsTable->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        tableCardLayout->addWidget(transactionsTable);

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
        deleteButton->setStyleSheet(QString::fromUtf8("background-color: #E74C3C; color: white; border: none; border-radius: 8px; padding: 0 20px; font-weight: bold;"));

        tableActionsLayout->addWidget(deleteButton);


        tableCardLayout->addLayout(tableActionsLayout);


        topRowLayout->addWidget(tableCardWidget);


        mainLayout->addLayout(topRowLayout);

        bottomRowLayout = new QHBoxLayout();
        bottomRowLayout->setSpacing(20);
        bottomRowLayout->setObjectName("bottomRowLayout");
        statsCardWidget = new QWidget(FinancesPage);
        statsCardWidget->setObjectName("statsCardWidget");
        statsCardWidget->setStyleSheet(QString::fromUtf8("background-color: white; border-radius: 15px;"));
        statsCardLayout = new QVBoxLayout(statsCardWidget);
        statsCardLayout->setSpacing(15);
        statsCardLayout->setObjectName("statsCardLayout");
        statsCardLayout->setContentsMargins(25, 25, 25, 25);
        statsCardTitle = new QLabel(statsCardWidget);
        statsCardTitle->setObjectName("statsCardTitle");
        statsCardTitle->setStyleSheet(QString::fromUtf8("font-size: 18px; font-weight: bold; color: #2C3E50; margin-bottom: 15px;"));

        statsCardLayout->addWidget(statsCardTitle);

        separator1 = new QFrame(statsCardWidget);
        separator1->setObjectName("separator1");
        separator1->setStyleSheet(QString::fromUtf8("color: #BDC3C7; margin: 5px 0;"));
        separator1->setFrameShape(QFrame::Shape::HLine);
        separator1->setFrameShadow(QFrame::Shadow::Sunken);

        statsCardLayout->addWidget(separator1);

        stat1Title = new QLabel(statsCardWidget);
        stat1Title->setObjectName("stat1Title");
        stat1Title->setStyleSheet(QString::fromUtf8("font-size: 15px; font-weight: bold; color: #2C3E50; margin: 15px 0 8px 0;"));

        statsCardLayout->addWidget(stat1Title);

        stat1Description = new QLabel(statsCardWidget);
        stat1Description->setObjectName("stat1Description");
        stat1Description->setStyleSheet(QString::fromUtf8("font-size: 12px; color: #7F8C8D; margin-bottom: 12px; padding-left: 20px;"));
        stat1Description->setWordWrap(true);

        statsCardLayout->addWidget(stat1Description);

        indicatorsLayout = new QHBoxLayout();
        indicatorsLayout->setSpacing(10);
        indicatorsLayout->setObjectName("indicatorsLayout");
        revenueLabel = new QLabel(statsCardWidget);
        revenueLabel->setObjectName("revenueLabel");
        revenueLabel->setStyleSheet(QString::fromUtf8("font-size: 13px; color: #27AE60; font-weight: bold; padding: 8px 12px; background-color: #E8F8F5; border-radius: 6px;"));

        indicatorsLayout->addWidget(revenueLabel);

        expensesLabel = new QLabel(statsCardWidget);
        expensesLabel->setObjectName("expensesLabel");
        expensesLabel->setStyleSheet(QString::fromUtf8("font-size: 13px; color: #E74C3C; font-weight: bold; padding: 8px 12px; background-color: #FADBD8; border-radius: 6px;"));

        indicatorsLayout->addWidget(expensesLabel);

        balanceLabel = new QLabel(statsCardWidget);
        balanceLabel->setObjectName("balanceLabel");
        balanceLabel->setStyleSheet(QString::fromUtf8("font-size: 13px; color: #2C3E50; font-weight: bold; padding: 8px 12px; background-color: #EBF5FB; border-radius: 6px;"));

        indicatorsLayout->addWidget(balanceLabel);


        statsCardLayout->addLayout(indicatorsLayout);

        separator2 = new QFrame(statsCardWidget);
        separator2->setObjectName("separator2");
        separator2->setStyleSheet(QString::fromUtf8("color: #BDC3C7; margin: 15px 0 10px 0;"));
        separator2->setFrameShape(QFrame::Shape::HLine);
        separator2->setFrameShadow(QFrame::Shadow::Sunken);

        statsCardLayout->addWidget(separator2);

        stat2Title = new QLabel(statsCardWidget);
        stat2Title->setObjectName("stat2Title");
        stat2Title->setStyleSheet(QString::fromUtf8("font-size: 15px; font-weight: bold; color: #2C3E50; margin: 10px 0 8px 0;"));

        statsCardLayout->addWidget(stat2Title);

        stat2Description = new QLabel(statsCardWidget);
        stat2Description->setObjectName("stat2Description");
        stat2Description->setStyleSheet(QString::fromUtf8("font-size: 12px; color: #7F8C8D; margin-bottom: 12px; padding-left: 20px;"));
        stat2Description->setWordWrap(true);

        statsCardLayout->addWidget(stat2Description);

        processingDelayLabel = new QLabel(statsCardWidget);
        processingDelayLabel->setObjectName("processingDelayLabel");
        processingDelayLabel->setStyleSheet(QString::fromUtf8("font-size: 15px; font-weight: bold; color: #3498DB; background-color: #EBF3FD; padding: 12px 20px; border-radius: 8px; border: 2px solid #3498DB; margin: 5px 0;"));
        processingDelayLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        statsCardLayout->addWidget(processingDelayLabel);

        separator3 = new QFrame(statsCardWidget);
        separator3->setObjectName("separator3");
        separator3->setStyleSheet(QString::fromUtf8("color: #BDC3C7; margin: 15px 0 10px 0;"));
        separator3->setFrameShape(QFrame::Shape::HLine);
        separator3->setFrameShadow(QFrame::Shadow::Sunken);

        statsCardLayout->addWidget(separator3);

        statusBadge = new QLabel(statsCardWidget);
        statusBadge->setObjectName("statusBadge");
        statusBadge->setStyleSheet(QString::fromUtf8("font-size: 16px; font-weight: bold; color: white; background-color: #27AE60; padding: 10px; border-radius: 8px;"));
        statusBadge->setAlignment(Qt::AlignmentFlag::AlignCenter);

        statsCardLayout->addWidget(statusBadge);

        chartContainer = new QWidget(statsCardWidget);
        chartContainer->setObjectName("chartContainer");
        chartContainer->setMinimumSize(QSize(0, 250));
        chartContainerLayout = new QVBoxLayout(chartContainer);
        chartContainerLayout->setSpacing(0);
        chartContainerLayout->setObjectName("chartContainerLayout");
        chartContainerLayout->setContentsMargins(0, 0, 0, 0);

        statsCardLayout->addWidget(chartContainer);


        bottomRowLayout->addWidget(statsCardWidget);

        assistantCardWidget = new QWidget(FinancesPage);
        assistantCardWidget->setObjectName("assistantCardWidget");
        assistantCardWidget->setStyleSheet(QString::fromUtf8("background-color: white; border-radius: 15px;"));
        assistantCardLayout = new QVBoxLayout(assistantCardWidget);
        assistantCardLayout->setSpacing(15);
        assistantCardLayout->setObjectName("assistantCardLayout");
        assistantCardLayout->setContentsMargins(25, 25, 25, 25);
        assistantCardTitle = new QLabel(assistantCardWidget);
        assistantCardTitle->setObjectName("assistantCardTitle");
        assistantCardTitle->setStyleSheet(QString::fromUtf8("font-size: 18px; font-weight: bold; color: #2C3E50;"));

        assistantCardLayout->addWidget(assistantCardTitle);

        chatTextEdit = new QTextEdit(assistantCardWidget);
        chatTextEdit->setObjectName("chatTextEdit");
        chatTextEdit->setStyleSheet(QString::fromUtf8("border: 2px solid #E0E0E0; border-radius: 8px; padding: 10px; font-size: 13px; background-color: #FAFAFA;"));
        chatTextEdit->setReadOnly(true);

        assistantCardLayout->addWidget(chatTextEdit);

        inputLayout = new QHBoxLayout();
        inputLayout->setObjectName("inputLayout");
        questionLineEdit = new QLineEdit(assistantCardWidget);
        questionLineEdit->setObjectName("questionLineEdit");
        questionLineEdit->setMinimumSize(QSize(0, 40));
        questionLineEdit->setStyleSheet(QString::fromUtf8("border: 2px solid #BDC3C7; border-radius: 8px; padding-left: 15px; font-size: 14px;"));

        inputLayout->addWidget(questionLineEdit);

        sendButton = new QPushButton(assistantCardWidget);
        sendButton->setObjectName("sendButton");
        sendButton->setMinimumSize(QSize(0, 40));
        sendButton->setStyleSheet(QString::fromUtf8("background-color: #9B59B6; color: white; border: none; border-radius: 8px; padding: 0 25px; font-weight: bold;"));

        inputLayout->addWidget(sendButton);


        assistantCardLayout->addLayout(inputLayout);

        actionsLabel = new QLabel(assistantCardWidget);
        actionsLabel->setObjectName("actionsLabel");
        actionsLabel->setStyleSheet(QString::fromUtf8("font-size: 12px; color: #7F8C8D;"));

        assistantCardLayout->addWidget(actionsLabel);

        quickActionsLayout = new QHBoxLayout();
        quickActionsLayout->setObjectName("quickActionsLayout");
        budgetButton = new QPushButton(assistantCardWidget);
        budgetButton->setObjectName("budgetButton");
        budgetButton->setMinimumSize(QSize(0, 30));
        budgetButton->setStyleSheet(QString::fromUtf8("background-color: #ECF0F1; color: #2C3E50; border: none; border-radius: 6px; font-size: 11px; padding: 0 10px;"));

        quickActionsLayout->addWidget(budgetButton);

        simulateButton = new QPushButton(assistantCardWidget);
        simulateButton->setObjectName("simulateButton");
        simulateButton->setMinimumSize(QSize(0, 30));
        simulateButton->setStyleSheet(QString::fromUtf8("background-color: #ECF0F1; color: #2C3E50; border: none; border-radius: 6px; font-size: 11px; padding: 0 10px;"));

        quickActionsLayout->addWidget(simulateButton);

        reportButton = new QPushButton(assistantCardWidget);
        reportButton->setObjectName("reportButton");
        reportButton->setMinimumSize(QSize(0, 30));
        reportButton->setStyleSheet(QString::fromUtf8("background-color: #ECF0F1; color: #2C3E50; border: none; border-radius: 6px; font-size: 11px; padding: 0 10px;"));

        quickActionsLayout->addWidget(reportButton);


        assistantCardLayout->addLayout(quickActionsLayout);


        bottomRowLayout->addWidget(assistantCardWidget);


        mainLayout->addLayout(bottomRowLayout);


        retranslateUi(FinancesPage);

        QMetaObject::connectSlotsByName(FinancesPage);
    } // setupUi

    void retranslateUi(QWidget *FinancesPage)
    {
        FinancesPage->setWindowTitle(QCoreApplication::translate("FinancesPage", "Finances", nullptr));
        titleLabel->setText(QCoreApplication::translate("FinancesPage", "\360\237\222\260 Gestion Financi\303\250re", nullptr));
        formCardTitle->setText(QCoreApplication::translate("FinancesPage", "\360\237\223\235 Ajouter une Transaction", nullptr));
        addTransactionButton->setText(QCoreApplication::translate("FinancesPage", "\342\236\225 Nouvelle Transaction", nullptr));
        infoLabel->setText(QCoreApplication::translate("FinancesPage", "<html><head/><body><p><span style=\" font-weight:600;\">Types de transactions:</span><br/>\342\200\242 <span style=\" color:#27ae60;\">Entr\303\251e</span>: Loyer, Services<br/>\342\200\242 <span style=\" color:#e74c3c;\">Sortie</span>: Charges, Maintenance<br/><br/><span style=\" font-weight:600;\">Modes de paiement:</span><br/>Esp\303\250ces, Carte, Virement, Ch\303\250que</p></body></html>", nullptr));
        tableCardTitle->setText(QCoreApplication::translate("FinancesPage", "\360\237\223\212 Liste des Transactions", nullptr));
        exportPDFButton->setText(QCoreApplication::translate("FinancesPage", "\360\237\223\204 Exporter PDF", nullptr));
        searchLineEdit->setPlaceholderText(QCoreApplication::translate("FinancesPage", "\360\237\224\215 Rechercher par code...", nullptr));
        searchButton->setText(QCoreApplication::translate("FinancesPage", "Rechercher", nullptr));
        sortComboBox->setItemText(0, QCoreApplication::translate("FinancesPage", "Trier par Date", nullptr));
        sortComboBox->setItemText(1, QCoreApplication::translate("FinancesPage", "Trier par Code", nullptr));
        sortComboBox->setItemText(2, QCoreApplication::translate("FinancesPage", "Trier par Montant", nullptr));

        QTableWidgetItem *___qtablewidgetitem = transactionsTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("FinancesPage", "Code", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = transactionsTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("FinancesPage", "Montant", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = transactionsTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("FinancesPage", "Date", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = transactionsTable->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("FinancesPage", "Type", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = transactionsTable->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("FinancesPage", "Cat\303\251gorie", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = transactionsTable->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("FinancesPage", "Mode Paiement", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = transactionsTable->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("FinancesPage", "Actions", nullptr));
        modifyButton->setText(QCoreApplication::translate("FinancesPage", "\342\234\217\357\270\217 Modifier", nullptr));
        deleteButton->setText(QCoreApplication::translate("FinancesPage", "\360\237\227\221\357\270\217 Supprimer", nullptr));
        statsCardTitle->setText(QCoreApplication::translate("FinancesPage", "Statistiques (Module Finances)", nullptr));
        stat1Title->setText(QCoreApplication::translate("FinancesPage", "\360\237\223\210 \303\211volution mensuelle des revenus et d\303\251penses", nullptr));
        stat1Description->setText(QCoreApplication::translate("FinancesPage", "Identifier si les revenus couvrent les d\303\251penses et rep\303\251rer les d\303\251ficits/exc\303\251dents", nullptr));
        revenueLabel->setText(QCoreApplication::translate("FinancesPage", "\360\237\223\210 Revenus: 0 DT", nullptr));
        expensesLabel->setText(QCoreApplication::translate("FinancesPage", "\360\237\223\211 D\303\251penses: 0 DT", nullptr));
        balanceLabel->setText(QCoreApplication::translate("FinancesPage", "\360\237\222\260 Solde: 0 DT", nullptr));
        stat2Title->setText(QCoreApplication::translate("FinancesPage", "\342\217\261\357\270\217 D\303\251lai moyen de traitement des transactions", nullptr));
        stat2Description->setText(QCoreApplication::translate("FinancesPage", "\303\211valuer la rapidit\303\251 du service financier et optimiser le flux de validation", nullptr));
        processingDelayLabel->setText(QCoreApplication::translate("FinancesPage", "\342\217\261\357\270\217 D\303\251lai moyen : 2,4 jours", nullptr));
        statusBadge->setText(QCoreApplication::translate("FinancesPage", "Exc\303\251dent", nullptr));
        assistantCardTitle->setText(QCoreApplication::translate("FinancesPage", "\360\237\244\226 Assistant Financier", nullptr));
        chatTextEdit->setHtml(QCoreApplication::translate("FinancesPage", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:13px; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:9pt; font-style:italic; color:#7f8c8d;\">Bonjour! Je suis votre assistant financier. Posez-moi vos questions!</span></p></body></html>", nullptr));
        questionLineEdit->setPlaceholderText(QCoreApplication::translate("FinancesPage", "\360\237\222\254 Posez votre question...", nullptr));
        sendButton->setText(QCoreApplication::translate("FinancesPage", "Envoyer", nullptr));
        actionsLabel->setText(QCoreApplication::translate("FinancesPage", "<html><head/><body><p><span style=\" font-weight:600;\">Actions rapides:</span></p></body></html>", nullptr));
        budgetButton->setText(QCoreApplication::translate("FinancesPage", "\360\237\222\274 Cr\303\251er Budget", nullptr));
        simulateButton->setText(QCoreApplication::translate("FinancesPage", "\360\237\223\212 Simuler Sc\303\251nario", nullptr));
        reportButton->setText(QCoreApplication::translate("FinancesPage", "\360\237\223\235 Rapport", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FinancesPage: public Ui_FinancesPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FINANCESPAGE_H
