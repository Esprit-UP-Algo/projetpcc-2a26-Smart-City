/********************************************************************************
** Form generated from reading UI file 'locauxpage.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOCAUXPAGE_H
#define UI_LOCAUXPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LocauxPage
{
public:
    QVBoxLayout *mainLayout;
    QLabel *titleLabel;
    QHBoxLayout *topRowLayout;
    QFrame *formCard;
    QVBoxLayout *formLayout;
    QLabel *formTitleLabel;
    QFormLayout *fieldsLayout;
    QLabel *idLabel;
    QLineEdit *idLineEdit;
    QLabel *nomLabel;
    QLineEdit *nomLineEdit;
    QLabel *capaciteLabel;
    QSpinBox *capaciteSpinBox;
    QLabel *adresseLabel;
    QLineEdit *adresseLineEdit;
    QLabel *occupantLabel;
    QLineEdit *occupantLineEdit;
    QLabel *superficieLabel;
    QSpinBox *superficieSpinBox;
    QLabel *typeLabel;
    QComboBox *typeComboBox;
    QLabel *statutLabel;
    QComboBox *statutComboBox;
    QLabel *ouvertLabel;
    QComboBox *ouvertComboBox;
    QHBoxLayout *buttonsLayout;
    QPushButton *ajouterButton;
    QPushButton *modifierButton;
    QPushButton *supprimerButton;
    QPushButton *consulterButton;
    QSpacerItem *formVerticalSpacer;
    QFrame *tableCard;
    QVBoxLayout *tableLayout;
    QLabel *tableTitleLabel;
    QHBoxLayout *searchLayout;
    QLineEdit *searchLineEdit;
    QComboBox *sortComboBox;
    QPushButton *exportButton;
    QTableWidget *locauxTableWidget;
    QHBoxLayout *bottomRowLayout;
    QFrame *statsCard;
    QVBoxLayout *statsLayout;
    QLabel *statsTitleLabel;
    QLabel *statsDescriptionLabel;
    QWidget *statsContainer;
    QGridLayout *statsGridLayout;
    QLabel *statDisponible;
    QLabel *statOccupe;
    QLabel *statMaintenance;
    QLabel *statTotal;
    QFrame *innovationCard;
    QVBoxLayout *innovationLayout;
    QLabel *innovationTitleLabel;
    QLabel *innovationDescriptionLabel;
    QTextEdit *notificationsTextEdit;
    QSpacerItem *innovationVerticalSpacer;

    void setupUi(QWidget *LocauxPage)
    {
        if (LocauxPage->objectName().isEmpty())
            LocauxPage->setObjectName("LocauxPage");
        LocauxPage->resize(1200, 800);
        LocauxPage->setStyleSheet(QString::fromUtf8("background-color: #F5F7FA;"));
        mainLayout = new QVBoxLayout(LocauxPage);
        mainLayout->setSpacing(20);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(20, 20, 20, 20);
        titleLabel = new QLabel(LocauxPage);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setStyleSheet(QString::fromUtf8("font-size: 28px; font-weight: bold; color: #2C3E50;"));

        mainLayout->addWidget(titleLabel);

        topRowLayout = new QHBoxLayout();
        topRowLayout->setSpacing(20);
        topRowLayout->setObjectName("topRowLayout");
        formCard = new QFrame(LocauxPage);
        formCard->setObjectName("formCard");
        formCard->setStyleSheet(QString::fromUtf8("QFrame {\n"
"    background-color: white;\n"
"    border-radius: 15px;\n"
"    padding: 20px;\n"
"}"));
        formCard->setFrameShape(QFrame::Shape::StyledPanel);
        formLayout = new QVBoxLayout(formCard);
        formLayout->setSpacing(15);
        formLayout->setObjectName("formLayout");
        formLayout->setContentsMargins(25, 25, 25, 25);
        formTitleLabel = new QLabel(formCard);
        formTitleLabel->setObjectName("formTitleLabel");
        formTitleLabel->setStyleSheet(QString::fromUtf8("font-size: 18px; font-weight: bold; color: #2C3E50; margin-bottom: 10px;"));

        formLayout->addWidget(formTitleLabel);

        fieldsLayout = new QFormLayout();
        fieldsLayout->setObjectName("fieldsLayout");
        fieldsLayout->setHorizontalSpacing(15);
        fieldsLayout->setVerticalSpacing(10);
        idLabel = new QLabel(formCard);
        idLabel->setObjectName("idLabel");
        idLabel->setStyleSheet(QString::fromUtf8("font-weight: bold; color: #34495E;"));

        fieldsLayout->setWidget(0, QFormLayout::LabelRole, idLabel);

        idLineEdit = new QLineEdit(formCard);
        idLineEdit->setObjectName("idLineEdit");
        idLineEdit->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    border: 2px solid #BDC3C7;\n"
"    border-radius: 8px;\n"
"    padding: 10px;\n"
"    font-size: 14px;\n"
"}\n"
"QLineEdit:focus {\n"
"    border-color: #3498DB;\n"
"}"));

        fieldsLayout->setWidget(0, QFormLayout::FieldRole, idLineEdit);

        nomLabel = new QLabel(formCard);
        nomLabel->setObjectName("nomLabel");
        nomLabel->setStyleSheet(QString::fromUtf8("font-weight: bold; color: #34495E;"));

        fieldsLayout->setWidget(1, QFormLayout::LabelRole, nomLabel);

        nomLineEdit = new QLineEdit(formCard);
        nomLineEdit->setObjectName("nomLineEdit");
        nomLineEdit->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    border: 2px solid #BDC3C7;\n"
"    border-radius: 8px;\n"
"    padding: 10px;\n"
"    font-size: 14px;\n"
"}\n"
"QLineEdit:focus {\n"
"    border-color: #3498DB;\n"
"}"));

        fieldsLayout->setWidget(1, QFormLayout::FieldRole, nomLineEdit);

        capaciteLabel = new QLabel(formCard);
        capaciteLabel->setObjectName("capaciteLabel");
        capaciteLabel->setStyleSheet(QString::fromUtf8("font-weight: bold; color: #34495E;"));

        fieldsLayout->setWidget(2, QFormLayout::LabelRole, capaciteLabel);

        capaciteSpinBox = new QSpinBox(formCard);
        capaciteSpinBox->setObjectName("capaciteSpinBox");
        capaciteSpinBox->setStyleSheet(QString::fromUtf8("QSpinBox {\n"
"    border: 2px solid #BDC3C7;\n"
"    border-radius: 8px;\n"
"    padding: 10px;\n"
"    font-size: 14px;\n"
"}\n"
"QSpinBox:focus {\n"
"    border-color: #3498DB;\n"
"}"));
        capaciteSpinBox->setMinimum(1);
        capaciteSpinBox->setMaximum(1000);

        fieldsLayout->setWidget(2, QFormLayout::FieldRole, capaciteSpinBox);

        adresseLabel = new QLabel(formCard);
        adresseLabel->setObjectName("adresseLabel");
        adresseLabel->setStyleSheet(QString::fromUtf8("font-weight: bold; color: #34495E;"));

        fieldsLayout->setWidget(3, QFormLayout::LabelRole, adresseLabel);

        adresseLineEdit = new QLineEdit(formCard);
        adresseLineEdit->setObjectName("adresseLineEdit");
        adresseLineEdit->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    border: 2px solid #BDC3C7;\n"
"    border-radius: 8px;\n"
"    padding: 10px;\n"
"    font-size: 14px;\n"
"}\n"
"QLineEdit:focus {\n"
"    border-color: #3498DB;\n"
"}"));

        fieldsLayout->setWidget(3, QFormLayout::FieldRole, adresseLineEdit);

        occupantLabel = new QLabel(formCard);
        occupantLabel->setObjectName("occupantLabel");
        occupantLabel->setStyleSheet(QString::fromUtf8("font-weight: bold; color: #34495E;"));

        fieldsLayout->setWidget(4, QFormLayout::LabelRole, occupantLabel);

        occupantLineEdit = new QLineEdit(formCard);
        occupantLineEdit->setObjectName("occupantLineEdit");
        occupantLineEdit->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    border: 2px solid #BDC3C7;\n"
"    border-radius: 8px;\n"
"    padding: 10px;\n"
"    font-size: 14px;\n"
"}\n"
"QLineEdit:focus {\n"
"    border-color: #3498DB;\n"
"}"));

        fieldsLayout->setWidget(4, QFormLayout::FieldRole, occupantLineEdit);

        superficieLabel = new QLabel(formCard);
        superficieLabel->setObjectName("superficieLabel");
        superficieLabel->setStyleSheet(QString::fromUtf8("font-weight: bold; color: #34495E;"));

        fieldsLayout->setWidget(5, QFormLayout::LabelRole, superficieLabel);

        superficieSpinBox = new QSpinBox(formCard);
        superficieSpinBox->setObjectName("superficieSpinBox");
        superficieSpinBox->setStyleSheet(QString::fromUtf8("QSpinBox {\n"
"    border: 2px solid #BDC3C7;\n"
"    border-radius: 8px;\n"
"    padding: 10px;\n"
"    font-size: 14px;\n"
"}\n"
"QSpinBox:focus {\n"
"    border-color: #3498DB;\n"
"}"));
        superficieSpinBox->setMinimum(1);
        superficieSpinBox->setMaximum(10000);

        fieldsLayout->setWidget(5, QFormLayout::FieldRole, superficieSpinBox);

        typeLabel = new QLabel(formCard);
        typeLabel->setObjectName("typeLabel");
        typeLabel->setStyleSheet(QString::fromUtf8("font-weight: bold; color: #34495E;"));

        fieldsLayout->setWidget(6, QFormLayout::LabelRole, typeLabel);

        typeComboBox = new QComboBox(formCard);
        typeComboBox->addItem(QString());
        typeComboBox->addItem(QString());
        typeComboBox->addItem(QString());
        typeComboBox->addItem(QString());
        typeComboBox->addItem(QString());
        typeComboBox->addItem(QString());
        typeComboBox->addItem(QString());
        typeComboBox->addItem(QString());
        typeComboBox->addItem(QString());
        typeComboBox->setObjectName("typeComboBox");
        typeComboBox->setStyleSheet(QString::fromUtf8("QComboBox {\n"
"    border: 2px solid #BDC3C7;\n"
"    border-radius: 8px;\n"
"    padding: 10px;\n"
"    font-size: 14px;\n"
"}\n"
"QComboBox:focus {\n"
"    border-color: #3498DB;\n"
"}"));

        fieldsLayout->setWidget(6, QFormLayout::FieldRole, typeComboBox);

        statutLabel = new QLabel(formCard);
        statutLabel->setObjectName("statutLabel");
        statutLabel->setStyleSheet(QString::fromUtf8("font-weight: bold; color: #34495E;"));

        fieldsLayout->setWidget(7, QFormLayout::LabelRole, statutLabel);

        statutComboBox = new QComboBox(formCard);
        statutComboBox->addItem(QString());
        statutComboBox->addItem(QString());
        statutComboBox->addItem(QString());
        statutComboBox->setObjectName("statutComboBox");
        statutComboBox->setStyleSheet(QString::fromUtf8("QComboBox {\n"
"    border: 2px solid #BDC3C7;\n"
"    border-radius: 8px;\n"
"    padding: 10px;\n"
"    font-size: 14px;\n"
"}\n"
"QComboBox:focus {\n"
"    border-color: #3498DB;\n"
"}"));

        fieldsLayout->setWidget(7, QFormLayout::FieldRole, statutComboBox);

        ouvertLabel = new QLabel(formCard);
        ouvertLabel->setObjectName("ouvertLabel");
        ouvertLabel->setStyleSheet(QString::fromUtf8("font-weight: bold; color: #34495E;"));

        fieldsLayout->setWidget(8, QFormLayout::LabelRole, ouvertLabel);

        ouvertComboBox = new QComboBox(formCard);
        ouvertComboBox->addItem(QString());
        ouvertComboBox->addItem(QString());
        ouvertComboBox->setObjectName("ouvertComboBox");
        ouvertComboBox->setStyleSheet(QString::fromUtf8("QComboBox {\n"
"    border: 2px solid #BDC3C7;\n"
"    border-radius: 8px;\n"
"    padding: 10px;\n"
"    font-size: 14px;\n"
"}\n"
"QComboBox:focus {\n"
"    border-color: #3498DB;\n"
"}"));

        fieldsLayout->setWidget(8, QFormLayout::FieldRole, ouvertComboBox);


        formLayout->addLayout(fieldsLayout);

        buttonsLayout = new QHBoxLayout();
        buttonsLayout->setSpacing(10);
        buttonsLayout->setObjectName("buttonsLayout");
        ajouterButton = new QPushButton(formCard);
        ajouterButton->setObjectName("ajouterButton");
        ajouterButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #27AE60;\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    padding: 12px 20px;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #229954;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #1E8449;\n"
"}"));

        buttonsLayout->addWidget(ajouterButton);

        modifierButton = new QPushButton(formCard);
        modifierButton->setObjectName("modifierButton");
        modifierButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #F39C12;\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    padding: 12px 20px;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #E67E22;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #D68910;\n"
"}"));

        buttonsLayout->addWidget(modifierButton);

        supprimerButton = new QPushButton(formCard);
        supprimerButton->setObjectName("supprimerButton");
        supprimerButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #E74C3C;\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    padding: 12px 20px;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #C0392B;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #A93226;\n"
"}"));

        buttonsLayout->addWidget(supprimerButton);

        consulterButton = new QPushButton(formCard);
        consulterButton->setObjectName("consulterButton");
        consulterButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #3498DB;\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    padding: 12px 20px;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #2980B9;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #21618C;\n"
"}"));

        buttonsLayout->addWidget(consulterButton);


        formLayout->addLayout(buttonsLayout);

        formVerticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        formLayout->addItem(formVerticalSpacer);


        topRowLayout->addWidget(formCard);

        tableCard = new QFrame(LocauxPage);
        tableCard->setObjectName("tableCard");
        tableCard->setStyleSheet(QString::fromUtf8("QFrame {\n"
"    background-color: white;\n"
"    border-radius: 15px;\n"
"    padding: 20px;\n"
"}"));
        tableCard->setFrameShape(QFrame::Shape::StyledPanel);
        tableLayout = new QVBoxLayout(tableCard);
        tableLayout->setSpacing(15);
        tableLayout->setObjectName("tableLayout");
        tableLayout->setContentsMargins(25, 25, 25, 25);
        tableTitleLabel = new QLabel(tableCard);
        tableTitleLabel->setObjectName("tableTitleLabel");
        tableTitleLabel->setStyleSheet(QString::fromUtf8("font-size: 18px; font-weight: bold; color: #2C3E50; margin-bottom: 10px;"));

        tableLayout->addWidget(tableTitleLabel);

        searchLayout = new QHBoxLayout();
        searchLayout->setSpacing(10);
        searchLayout->setObjectName("searchLayout");
        searchLineEdit = new QLineEdit(tableCard);
        searchLineEdit->setObjectName("searchLineEdit");
        searchLineEdit->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    border: 2px solid #BDC3C7;\n"
"    border-radius: 8px;\n"
"    padding: 10px;\n"
"    font-size: 14px;\n"
"}\n"
"QLineEdit:focus {\n"
"    border-color: #3498DB;\n"
"}"));

        searchLayout->addWidget(searchLineEdit);

        sortComboBox = new QComboBox(tableCard);
        sortComboBox->addItem(QString());
        sortComboBox->addItem(QString());
        sortComboBox->setObjectName("sortComboBox");
        sortComboBox->setStyleSheet(QString::fromUtf8("QComboBox {\n"
"    border: 2px solid #BDC3C7;\n"
"    border-radius: 8px;\n"
"    padding: 10px;\n"
"    font-size: 14px;\n"
"    min-width: 150px;\n"
"}\n"
"QComboBox:focus {\n"
"    border-color: #3498DB;\n"
"}"));

        searchLayout->addWidget(sortComboBox);

        exportButton = new QPushButton(tableCard);
        exportButton->setObjectName("exportButton");
        exportButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #9B59B6;\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    padding: 10px 15px;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #8E44AD;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #7D3C98;\n"
"}"));

        searchLayout->addWidget(exportButton);


        tableLayout->addLayout(searchLayout);

        locauxTableWidget = new QTableWidget(tableCard);
        if (locauxTableWidget->columnCount() < 8)
            locauxTableWidget->setColumnCount(8);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        locauxTableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        locauxTableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        locauxTableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        locauxTableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        locauxTableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        locauxTableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        locauxTableWidget->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        locauxTableWidget->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        locauxTableWidget->setObjectName("locauxTableWidget");
        locauxTableWidget->setMinimumSize(QSize(0, 450));
        locauxTableWidget->setStyleSheet(QString::fromUtf8("QTableWidget {\n"
"    border: 2px solid #E0E0E0;\n"
"    border-radius: 10px;\n"
"    gridline-color: #E0E0E0;\n"
"    background-color: white;\n"
"    alternate-background-color: #F8F9FA;\n"
"    selection-background-color: #E3F2FD;\n"
"    font-size: 14px;\n"
"}\n"
"QHeaderView::section {\n"
"    background-color: #3498DB;\n"
"    color: white;\n"
"    padding: 15px;\n"
"    border: none;\n"
"    font-weight: bold;\n"
"    font-size: 15px;\n"
"}\n"
"QTableWidget::item {\n"
"    padding: 12px;\n"
"    border-bottom: 1px solid #E0E0E0;\n"
"    font-size: 14px;\n"
"}\n"
"QTableWidget::item:selected {\n"
"    background-color: #E3F2FD;\n"
"    color: #1976D2;\n"
"}"));
        locauxTableWidget->setAlternatingRowColors(true);
        locauxTableWidget->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        locauxTableWidget->setSortingEnabled(true);

        tableLayout->addWidget(locauxTableWidget);


        topRowLayout->addWidget(tableCard);


        mainLayout->addLayout(topRowLayout);

        bottomRowLayout = new QHBoxLayout();
        bottomRowLayout->setSpacing(20);
        bottomRowLayout->setObjectName("bottomRowLayout");
        statsCard = new QFrame(LocauxPage);
        statsCard->setObjectName("statsCard");
        statsCard->setStyleSheet(QString::fromUtf8("QFrame {\n"
"    background-color: white;\n"
"    border-radius: 15px;\n"
"    padding: 20px;\n"
"}"));
        statsCard->setFrameShape(QFrame::Shape::StyledPanel);
        statsLayout = new QVBoxLayout(statsCard);
        statsLayout->setSpacing(15);
        statsLayout->setObjectName("statsLayout");
        statsLayout->setContentsMargins(25, 25, 25, 25);
        statsTitleLabel = new QLabel(statsCard);
        statsTitleLabel->setObjectName("statsTitleLabel");
        statsTitleLabel->setStyleSheet(QString::fromUtf8("font-size: 18px; font-weight: bold; color: #2C3E50; margin-bottom: 10px;"));

        statsLayout->addWidget(statsTitleLabel);

        statsDescriptionLabel = new QLabel(statsCard);
        statsDescriptionLabel->setObjectName("statsDescriptionLabel");
        statsDescriptionLabel->setStyleSheet(QString::fromUtf8("font-size: 14px; color: #7F8C8D; margin-bottom: 15px;"));
        statsDescriptionLabel->setWordWrap(true);

        statsLayout->addWidget(statsDescriptionLabel);

        statsContainer = new QWidget(statsCard);
        statsContainer->setObjectName("statsContainer");
        statsContainer->setMinimumSize(QSize(0, 300));
        statsContainer->setStyleSheet(QString::fromUtf8("background-color: #F8F9FA; border-radius: 10px; border: 1px solid #E0E0E0;"));
        statsGridLayout = new QGridLayout(statsContainer);
        statsGridLayout->setSpacing(15);
        statsGridLayout->setObjectName("statsGridLayout");
        statsGridLayout->setContentsMargins(20, 20, 20, 20);
        statDisponible = new QLabel(statsContainer);
        statDisponible->setObjectName("statDisponible");
        statDisponible->setStyleSheet(QString::fromUtf8("background-color: #D5F4E6; color: #27AE60; padding: 20px; border-radius: 10px; font-size: 16px; font-weight: bold; text-align: center;"));
        statDisponible->setAlignment(Qt::AlignmentFlag::AlignCenter);

        statsGridLayout->addWidget(statDisponible, 0, 0, 1, 1);

        statOccupe = new QLabel(statsContainer);
        statOccupe->setObjectName("statOccupe");
        statOccupe->setStyleSheet(QString::fromUtf8("background-color: #FCF3CF; color: #F39C12; padding: 20px; border-radius: 10px; font-size: 16px; font-weight: bold; text-align: center;"));
        statOccupe->setAlignment(Qt::AlignmentFlag::AlignCenter);

        statsGridLayout->addWidget(statOccupe, 0, 1, 1, 1);

        statMaintenance = new QLabel(statsContainer);
        statMaintenance->setObjectName("statMaintenance");
        statMaintenance->setStyleSheet(QString::fromUtf8("background-color: #FADBD8; color: #E74C3C; padding: 20px; border-radius: 10px; font-size: 16px; font-weight: bold; text-align: center;"));
        statMaintenance->setAlignment(Qt::AlignmentFlag::AlignCenter);

        statsGridLayout->addWidget(statMaintenance, 1, 0, 1, 1);

        statTotal = new QLabel(statsContainer);
        statTotal->setObjectName("statTotal");
        statTotal->setStyleSheet(QString::fromUtf8("background-color: #EBF3FD; color: #3498DB; padding: 20px; border-radius: 10px; font-size: 16px; font-weight: bold; text-align: center;"));
        statTotal->setAlignment(Qt::AlignmentFlag::AlignCenter);

        statsGridLayout->addWidget(statTotal, 1, 1, 1, 1);


        statsLayout->addWidget(statsContainer);


        bottomRowLayout->addWidget(statsCard);

        innovationCard = new QFrame(LocauxPage);
        innovationCard->setObjectName("innovationCard");
        innovationCard->setStyleSheet(QString::fromUtf8("QFrame {\n"
"    background-color: white;\n"
"    border-radius: 15px;\n"
"    padding: 20px;\n"
"}"));
        innovationCard->setFrameShape(QFrame::Shape::StyledPanel);
        innovationLayout = new QVBoxLayout(innovationCard);
        innovationLayout->setSpacing(15);
        innovationLayout->setObjectName("innovationLayout");
        innovationLayout->setContentsMargins(25, 25, 25, 25);
        innovationTitleLabel = new QLabel(innovationCard);
        innovationTitleLabel->setObjectName("innovationTitleLabel");
        innovationTitleLabel->setStyleSheet(QString::fromUtf8("font-size: 18px; font-weight: bold; color: #2C3E50; margin-bottom: 10px;"));

        innovationLayout->addWidget(innovationTitleLabel);

        innovationDescriptionLabel = new QLabel(innovationCard);
        innovationDescriptionLabel->setObjectName("innovationDescriptionLabel");
        innovationDescriptionLabel->setStyleSheet(QString::fromUtf8("font-size: 14px; color: #7F8C8D; margin-bottom: 15px;"));
        innovationDescriptionLabel->setWordWrap(true);

        innovationLayout->addWidget(innovationDescriptionLabel);

        notificationsTextEdit = new QTextEdit(innovationCard);
        notificationsTextEdit->setObjectName("notificationsTextEdit");
        notificationsTextEdit->setStyleSheet(QString::fromUtf8("QTextEdit {\n"
"    border: 2px solid #E8E8E8;\n"
"    border-radius: 8px;\n"
"    padding: 15px;\n"
"    background-color: #FAFAFA;\n"
"    font-size: 14px;\n"
"}\n"
"QTextEdit:focus {\n"
"    border-color: #3498DB;\n"
"}"));
        notificationsTextEdit->setReadOnly(true);

        innovationLayout->addWidget(notificationsTextEdit);

        innovationVerticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        innovationLayout->addItem(innovationVerticalSpacer);


        bottomRowLayout->addWidget(innovationCard);


        mainLayout->addLayout(bottomRowLayout);


        retranslateUi(LocauxPage);

        QMetaObject::connectSlotsByName(LocauxPage);
    } // setupUi

    void retranslateUi(QWidget *LocauxPage)
    {
        LocauxPage->setWindowTitle(QCoreApplication::translate("LocauxPage", "Locaux", nullptr));
        titleLabel->setText(QCoreApplication::translate("LocauxPage", "\360\237\217\240 Gestion des Locaux", nullptr));
        formTitleLabel->setText(QCoreApplication::translate("LocauxPage", "\360\237\217\242 Ajouter un Local", nullptr));
        idLabel->setText(QCoreApplication::translate("LocauxPage", "ID Local:", nullptr));
        idLineEdit->setPlaceholderText(QCoreApplication::translate("LocauxPage", "Saisir l'ID du local", nullptr));
        nomLabel->setText(QCoreApplication::translate("LocauxPage", "Nom:", nullptr));
        nomLineEdit->setPlaceholderText(QCoreApplication::translate("LocauxPage", "Nom du local", nullptr));
        capaciteLabel->setText(QCoreApplication::translate("LocauxPage", "Capacit\303\251:", nullptr));
        capaciteSpinBox->setSuffix(QCoreApplication::translate("LocauxPage", " personnes", nullptr));
        adresseLabel->setText(QCoreApplication::translate("LocauxPage", "Adresse:", nullptr));
        adresseLineEdit->setPlaceholderText(QCoreApplication::translate("LocauxPage", "Adresse compl\303\250te", nullptr));
        occupantLabel->setText(QCoreApplication::translate("LocauxPage", "Nom de l'occupant:", nullptr));
        occupantLineEdit->setPlaceholderText(QCoreApplication::translate("LocauxPage", "Nom de l'occupant actuel", nullptr));
        superficieLabel->setText(QCoreApplication::translate("LocauxPage", "Superficie (m\302\262):", nullptr));
        superficieSpinBox->setSuffix(QCoreApplication::translate("LocauxPage", " m\302\262", nullptr));
        typeLabel->setText(QCoreApplication::translate("LocauxPage", "Type:", nullptr));
        typeComboBox->setItemText(0, QCoreApplication::translate("LocauxPage", "Jardin", nullptr));
        typeComboBox->setItemText(1, QCoreApplication::translate("LocauxPage", "Salle de sport", nullptr));
        typeComboBox->setItemText(2, QCoreApplication::translate("LocauxPage", "Salle de spectacle", nullptr));
        typeComboBox->setItemText(3, QCoreApplication::translate("LocauxPage", "Restaurant/Caf\303\251", nullptr));
        typeComboBox->setItemText(4, QCoreApplication::translate("LocauxPage", "H\303\264pital", nullptr));
        typeComboBox->setItemText(5, QCoreApplication::translate("LocauxPage", "Magasin", nullptr));
        typeComboBox->setItemText(6, QCoreApplication::translate("LocauxPage", "Parking", nullptr));
        typeComboBox->setItemText(7, QCoreApplication::translate("LocauxPage", "Bureau", nullptr));
        typeComboBox->setItemText(8, QCoreApplication::translate("LocauxPage", "Appartement", nullptr));

        statutLabel->setText(QCoreApplication::translate("LocauxPage", "Statut:", nullptr));
        statutComboBox->setItemText(0, QCoreApplication::translate("LocauxPage", "Disponible", nullptr));
        statutComboBox->setItemText(1, QCoreApplication::translate("LocauxPage", "Occup\303\251", nullptr));
        statutComboBox->setItemText(2, QCoreApplication::translate("LocauxPage", "En maintenance", nullptr));

        ouvertLabel->setText(QCoreApplication::translate("LocauxPage", "Ouvert:", nullptr));
        ouvertComboBox->setItemText(0, QCoreApplication::translate("LocauxPage", "Oui", nullptr));
        ouvertComboBox->setItemText(1, QCoreApplication::translate("LocauxPage", "Non", nullptr));

        ajouterButton->setText(QCoreApplication::translate("LocauxPage", "\342\236\225 Ajouter", nullptr));
        modifierButton->setText(QCoreApplication::translate("LocauxPage", "\342\234\217\357\270\217 Modifier", nullptr));
        supprimerButton->setText(QCoreApplication::translate("LocauxPage", "\360\237\227\221\357\270\217 Supprimer", nullptr));
        consulterButton->setText(QCoreApplication::translate("LocauxPage", "\360\237\221\201\357\270\217 Consulter", nullptr));
        tableTitleLabel->setText(QCoreApplication::translate("LocauxPage", "\360\237\223\213 Liste des Locaux", nullptr));
        searchLineEdit->setPlaceholderText(QCoreApplication::translate("LocauxPage", "\360\237\224\215 Rechercher par ID, type, adresse...", nullptr));
        sortComboBox->setItemText(0, QCoreApplication::translate("LocauxPage", "Trier par ID", nullptr));
        sortComboBox->setItemText(1, QCoreApplication::translate("LocauxPage", "Trier par Statut", nullptr));

        exportButton->setText(QCoreApplication::translate("LocauxPage", "\360\237\223\212 Export Excel", nullptr));
#if QT_CONFIG(tooltip)
        exportButton->setToolTip(QCoreApplication::translate("LocauxPage", "Exporter un rapport comparatif sur les co\303\273ts des types de locaux.", nullptr));
#endif // QT_CONFIG(tooltip)
        QTableWidgetItem *___qtablewidgetitem = locauxTableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("LocauxPage", "\360\237\206\224 ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = locauxTableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("LocauxPage", "\360\237\217\242 Nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = locauxTableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("LocauxPage", "\360\237\217\267\357\270\217 Type", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = locauxTableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("LocauxPage", "\360\237\221\245 Capacit\303\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = locauxTableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("LocauxPage", "\360\237\223\220 Superficie", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = locauxTableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("LocauxPage", "\360\237\223\215 Adresse", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = locauxTableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("LocauxPage", "\360\237\223\212 Statut", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = locauxTableWidget->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("LocauxPage", "\360\237\225\220 Ouvert", nullptr));
        statsTitleLabel->setText(QCoreApplication::translate("LocauxPage", "\360\237\223\212 Statistiques des Locaux", nullptr));
        statsDescriptionLabel->setText(QCoreApplication::translate("LocauxPage", "Visualiser l'\303\251tat actuel des locaux (disponibles, occup\303\251s, en maintenance).", nullptr));
        statDisponible->setText(QCoreApplication::translate("LocauxPage", "\342\234\205 Disponibles\n"
"3 locaux", nullptr));
        statOccupe->setText(QCoreApplication::translate("LocauxPage", "\360\237\224\266 Occup\303\251s\n"
"2 locaux", nullptr));
        statMaintenance->setText(QCoreApplication::translate("LocauxPage", "\360\237\224\247 En maintenance\n"
"1 local", nullptr));
        statTotal->setText(QCoreApplication::translate("LocauxPage", "\360\237\217\242 Total\n"
"6 locaux", nullptr));
        innovationTitleLabel->setText(QCoreApplication::translate("LocauxPage", "\360\237\222\241 Notifications automatiques", nullptr));
        innovationDescriptionLabel->setText(QCoreApplication::translate("LocauxPage", "Alertes en cas de maintenance ou r\303\251servation.", nullptr));
        notificationsTextEdit->setHtml(QCoreApplication::translate("LocauxPage", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:14px; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; color:#2c3e50;\">\360\237\224\224 Notifications r\303\251centes</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span styl"
                        "e=\" color:#e74c3c; font-weight:600;\">\360\237\224\247 Maintenance programm\303\251e</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#7f8c8d;\">Th\303\251\303\242tre Municipal - Maintenance du syst\303\250me de climatisation pr\303\251vue demain</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#27ae60; font-weight:600;\">\342\234\205 Local disponible</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#7f8c8d;\">Jardin Central - D\303\251sormais disponible pour r\303\251servations</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; mar"
                        "gin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#f39c12; font-weight:600;\">\360\237\223\213 Nouvelle r\303\251servation</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#7f8c8d;\">Salle Sport Alpha - R\303\251serv\303\251e pour \303\251v\303\251nement sportif le 15 octobre</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#3498db; font-weight:600;\">\360\237\223\212 Rapport mensuel</span></p>\n"
"<p style=\" margin-top:0px; margi"
                        "n-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#7f8c8d;\">Taux d'occupation des locaux: 85% ce mois-ci</span></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LocauxPage: public Ui_LocauxPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOCAUXPAGE_H
