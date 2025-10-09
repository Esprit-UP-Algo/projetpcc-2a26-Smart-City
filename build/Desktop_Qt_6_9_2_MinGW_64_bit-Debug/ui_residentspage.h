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
    QLabel *titleLabel;
    QLabel *subtitleLabel;
    QPushButton *addResidentButton;
    QFrame *residentsListCard;
    QVBoxLayout *residentsListLayout;
    QLabel *residentsListTitle;
    QHBoxLayout *tableControlsLayout;
    QLineEdit *searchLineEdit;
    QComboBox *sortComboBox;
    QPushButton *exportPdfButton;
    QTableWidget *residentsTable;
    QHBoxLayout *tableButtonsLayout;
    QSpacerItem *tableButtonsSpacer;
    QPushButton *editResidentButton;
    QPushButton *deleteResidentButton;
    QFrame *satisfactionCard;
    QVBoxLayout *satisfactionLayout;
    QLabel *satisfactionTitle;
    QLabel *satisfactionDescription;
    QHBoxLayout *scoreRulesLayout;
    QLabel *renewalLabel;
    QLabel *departureLabel;
    QLabel *incidentLabel;
    QLabel *noIncidentLabel;
    QLabel *globalScoreLabel;
    QFrame *innovativeFeaturesCard;
    QVBoxLayout *innovativeFeaturesLayout;
    QLabel *innovativeFeaturesTitle;
    QLabel *notificationsFeature;
    QLabel *mapFeature;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *ResidentsPage)
    {
        if (ResidentsPage->objectName().isEmpty())
            ResidentsPage->setObjectName("ResidentsPage");
        ResidentsPage->resize(1200, 1080);
        ResidentsPage->setStyleSheet(QString::fromUtf8("background-color: #F5F7FA;"));
        mainLayout = new QVBoxLayout(ResidentsPage);
        mainLayout->setSpacing(20);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(20, 20, 20, 20);
        titleLabel = new QLabel(ResidentsPage);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setStyleSheet(QString::fromUtf8("font-size: 28px; font-weight: bold; color: #2C3E50;"));

        mainLayout->addWidget(titleLabel);

        subtitleLabel = new QLabel(ResidentsPage);
        subtitleLabel->setObjectName("subtitleLabel");
        subtitleLabel->setStyleSheet(QString::fromUtf8("font-size: 14px; color: #7F8C8D; margin-bottom: 10px;"));

        mainLayout->addWidget(subtitleLabel);

        addResidentButton = new QPushButton(ResidentsPage);
        addResidentButton->setObjectName("addResidentButton");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(addResidentButton->sizePolicy().hasHeightForWidth());
        addResidentButton->setSizePolicy(sizePolicy);
        addResidentButton->setMinimumSize(QSize(200, 50));
        addResidentButton->setStyleSheet(QString::fromUtf8("background-color: #3498DB; color: white; border-radius: 15px; font-weight: bold; font-size: 16px; padding: 15px 30px;"));

        mainLayout->addWidget(addResidentButton);

        residentsListCard = new QFrame(ResidentsPage);
        residentsListCard->setObjectName("residentsListCard");
        residentsListCard->setStyleSheet(QString::fromUtf8("background-color: white; border-radius: 15px;"));
        residentsListCard->setFrameShape(QFrame::Shape::NoFrame);
        residentsListLayout = new QVBoxLayout(residentsListCard);
        residentsListLayout->setSpacing(15);
        residentsListLayout->setObjectName("residentsListLayout");
        residentsListLayout->setContentsMargins(25, 25, 25, 25);
        residentsListTitle = new QLabel(residentsListCard);
        residentsListTitle->setObjectName("residentsListTitle");
        residentsListTitle->setStyleSheet(QString::fromUtf8("font-size: 20px; font-weight: bold; color: #2C3E50;"));

        residentsListLayout->addWidget(residentsListTitle);

        tableControlsLayout = new QHBoxLayout();
        tableControlsLayout->setSpacing(15);
        tableControlsLayout->setObjectName("tableControlsLayout");
        searchLineEdit = new QLineEdit(residentsListCard);
        searchLineEdit->setObjectName("searchLineEdit");
        searchLineEdit->setStyleSheet(QString::fromUtf8("border: 1px solid #BDC3C7; border-radius: 8px; padding: 10px; font-size: 14px;"));

        tableControlsLayout->addWidget(searchLineEdit);

        sortComboBox = new QComboBox(residentsListCard);
        sortComboBox->addItem(QString());
        sortComboBox->addItem(QString());
        sortComboBox->addItem(QString());
        sortComboBox->addItem(QString());
        sortComboBox->setObjectName("sortComboBox");
        sortComboBox->setStyleSheet(QString::fromUtf8("border: 1px solid #BDC3C7; border-radius: 8px; padding: 10px; font-size: 14px;"));

        tableControlsLayout->addWidget(sortComboBox);

        exportPdfButton = new QPushButton(residentsListCard);
        exportPdfButton->setObjectName("exportPdfButton");
        exportPdfButton->setStyleSheet(QString::fromUtf8("background-color: #8E44AD; color: white; border-radius: 8px; font-weight: bold; padding: 10px 20px;"));

        tableControlsLayout->addWidget(exportPdfButton);


        residentsListLayout->addLayout(tableControlsLayout);

        residentsTable = new QTableWidget(residentsListCard);
        if (residentsTable->columnCount() < 8)
            residentsTable->setColumnCount(8);
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
        residentsTable->setObjectName("residentsTable");
        residentsTable->setStyleSheet(QString::fromUtf8("border: 1px solid #BDC3C7; border-radius: 8px; alternate-background-color: #F8F9FA;"));
        residentsTable->setAlternatingRowColors(true);
        residentsTable->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        residentsTable->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        residentsListLayout->addWidget(residentsTable);

        tableButtonsLayout = new QHBoxLayout();
        tableButtonsLayout->setObjectName("tableButtonsLayout");
        tableButtonsSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        tableButtonsLayout->addItem(tableButtonsSpacer);

        editResidentButton = new QPushButton(residentsListCard);
        editResidentButton->setObjectName("editResidentButton");
        editResidentButton->setStyleSheet(QString::fromUtf8("background-color: #F39C12; color: white; border-radius: 8px; font-weight: bold; padding: 10px 20px;"));

        tableButtonsLayout->addWidget(editResidentButton);

        deleteResidentButton = new QPushButton(residentsListCard);
        deleteResidentButton->setObjectName("deleteResidentButton");
        deleteResidentButton->setStyleSheet(QString::fromUtf8("background-color: #E74C3C; color: white; border-radius: 8px; font-weight: bold; padding: 10px 20px;"));

        tableButtonsLayout->addWidget(deleteResidentButton);


        residentsListLayout->addLayout(tableButtonsLayout);


        mainLayout->addWidget(residentsListCard);

        satisfactionCard = new QFrame(ResidentsPage);
        satisfactionCard->setObjectName("satisfactionCard");
        satisfactionCard->setStyleSheet(QString::fromUtf8("background-color: white; border-radius: 15px;"));
        satisfactionCard->setFrameShape(QFrame::Shape::NoFrame);
        satisfactionLayout = new QVBoxLayout(satisfactionCard);
        satisfactionLayout->setSpacing(15);
        satisfactionLayout->setObjectName("satisfactionLayout");
        satisfactionLayout->setContentsMargins(25, 25, 25, 25);
        satisfactionTitle = new QLabel(satisfactionCard);
        satisfactionTitle->setObjectName("satisfactionTitle");
        satisfactionTitle->setStyleSheet(QString::fromUtf8("font-size: 20px; font-weight: bold; color: #2C3E50;"));

        satisfactionLayout->addWidget(satisfactionTitle);

        satisfactionDescription = new QLabel(satisfactionCard);
        satisfactionDescription->setObjectName("satisfactionDescription");
        satisfactionDescription->setStyleSheet(QString::fromUtf8("font-size: 14px; color: #7F8C8D;"));
        satisfactionDescription->setWordWrap(true);

        satisfactionLayout->addWidget(satisfactionDescription);

        scoreRulesLayout = new QHBoxLayout();
        scoreRulesLayout->setSpacing(15);
        scoreRulesLayout->setObjectName("scoreRulesLayout");
        renewalLabel = new QLabel(satisfactionCard);
        renewalLabel->setObjectName("renewalLabel");
        renewalLabel->setStyleSheet(QString::fromUtf8("background-color: #D5EDDA; color: #155724; border: 1px solid #C3E6CB; border-radius: 8px; padding: 10px; font-size: 12px; font-weight: bold;"));
        renewalLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        scoreRulesLayout->addWidget(renewalLabel);

        departureLabel = new QLabel(satisfactionCard);
        departureLabel->setObjectName("departureLabel");
        departureLabel->setStyleSheet(QString::fromUtf8("background-color: #F8D7DA; color: #721C24; border: 1px solid #F5C6CB; border-radius: 8px; padding: 10px; font-size: 12px; font-weight: bold;"));
        departureLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        scoreRulesLayout->addWidget(departureLabel);

        incidentLabel = new QLabel(satisfactionCard);
        incidentLabel->setObjectName("incidentLabel");
        incidentLabel->setStyleSheet(QString::fromUtf8("background-color: #FFF3CD; color: #856404; border: 1px solid #FFEAA7; border-radius: 8px; padding: 10px; font-size: 12px; font-weight: bold;"));
        incidentLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        scoreRulesLayout->addWidget(incidentLabel);

        noIncidentLabel = new QLabel(satisfactionCard);
        noIncidentLabel->setObjectName("noIncidentLabel");
        noIncidentLabel->setStyleSheet(QString::fromUtf8("background-color: #CCE5FF; color: #004085; border: 1px solid #B3D7FF; border-radius: 8px; padding: 10px; font-size: 12px; font-weight: bold;"));
        noIncidentLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        scoreRulesLayout->addWidget(noIncidentLabel);


        satisfactionLayout->addLayout(scoreRulesLayout);

        globalScoreLabel = new QLabel(satisfactionCard);
        globalScoreLabel->setObjectName("globalScoreLabel");
        globalScoreLabel->setStyleSheet(QString::fromUtf8("background-color: #E3F2FD; color: #1565C0; border: 1px solid #BBDEFB; border-radius: 10px; padding: 15px; font-size: 16px; font-weight: bold;"));
        globalScoreLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        globalScoreLabel->setWordWrap(true);

        satisfactionLayout->addWidget(globalScoreLabel);


        mainLayout->addWidget(satisfactionCard);

        innovativeFeaturesCard = new QFrame(ResidentsPage);
        innovativeFeaturesCard->setObjectName("innovativeFeaturesCard");
        innovativeFeaturesCard->setStyleSheet(QString::fromUtf8("background-color: white; border-radius: 15px; padding: 20px;"));
        innovativeFeaturesCard->setFrameShape(QFrame::Shape::NoFrame);
        innovativeFeaturesLayout = new QVBoxLayout(innovativeFeaturesCard);
        innovativeFeaturesLayout->setSpacing(12);
        innovativeFeaturesLayout->setObjectName("innovativeFeaturesLayout");
        innovativeFeaturesLayout->setContentsMargins(20, 20, 20, 20);
        innovativeFeaturesTitle = new QLabel(innovativeFeaturesCard);
        innovativeFeaturesTitle->setObjectName("innovativeFeaturesTitle");
        innovativeFeaturesTitle->setStyleSheet(QString::fromUtf8("font-size: 18px; font-weight: bold; color: #2C3E50;"));

        innovativeFeaturesLayout->addWidget(innovativeFeaturesTitle);

        notificationsFeature = new QLabel(innovativeFeaturesCard);
        notificationsFeature->setObjectName("notificationsFeature");
        notificationsFeature->setStyleSheet(QString::fromUtf8("font-size: 14px; color: #2C3E50;"));
        notificationsFeature->setWordWrap(true);

        innovativeFeaturesLayout->addWidget(notificationsFeature);

        mapFeature = new QLabel(innovativeFeaturesCard);
        mapFeature->setObjectName("mapFeature");
        mapFeature->setStyleSheet(QString::fromUtf8("font-size: 14px; color: #2C3E50;"));
        mapFeature->setWordWrap(true);

        innovativeFeaturesLayout->addWidget(mapFeature);


        mainLayout->addWidget(innovativeFeaturesCard);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        mainLayout->addItem(verticalSpacer);


        retranslateUi(ResidentsPage);

        QMetaObject::connectSlotsByName(ResidentsPage);
    } // setupUi

    void retranslateUi(QWidget *ResidentsPage)
    {
        ResidentsPage->setWindowTitle(QCoreApplication::translate("ResidentsPage", "R\303\251sidents", nullptr));
        titleLabel->setText(QCoreApplication::translate("ResidentsPage", "\360\237\221\245 Gestion des R\303\251sidents", nullptr));
        subtitleLabel->setText(QCoreApplication::translate("ResidentsPage", "Service de Gestion R\303\251sidentielle \342\200\223 Module du projet NEXORA Smart City", nullptr));
#if QT_CONFIG(tooltip)
        addResidentButton->setToolTip(QCoreApplication::translate("ResidentsPage", "Ajouter un nouveau r\303\251sident (ouvre la fen\303\252tre de saisie)", nullptr));
#endif // QT_CONFIG(tooltip)
        addResidentButton->setText(QCoreApplication::translate("ResidentsPage", "\342\236\225 Nouveau R\303\251sident", nullptr));
        residentsListTitle->setText(QCoreApplication::translate("ResidentsPage", "\360\237\223\213 Liste des R\303\251sidents", nullptr));
        searchLineEdit->setPlaceholderText(QCoreApplication::translate("ResidentsPage", "\360\237\224\215 Rechercher par nom ou CIN ...", nullptr));
        sortComboBox->setItemText(0, QCoreApplication::translate("ResidentsPage", "Trier par : CIN", nullptr));
        sortComboBox->setItemText(1, QCoreApplication::translate("ResidentsPage", "Trier par : Nom", nullptr));
        sortComboBox->setItemText(2, QCoreApplication::translate("ResidentsPage", "Trier par : Appartement", nullptr));
        sortComboBox->setItemText(3, QCoreApplication::translate("ResidentsPage", "Trier par : Date d'entr\303\251e", nullptr));

#if QT_CONFIG(tooltip)
        exportPdfButton->setToolTip(QCoreApplication::translate("ResidentsPage", "Exporter une fiche r\303\251sident avec logo NEXORA, d\303\251tails du contrat et signature administrateur", nullptr));
#endif // QT_CONFIG(tooltip)
        exportPdfButton->setText(QCoreApplication::translate("ResidentsPage", "\360\237\223\204 Exporter Facture PDF", nullptr));
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
        editResidentButton->setText(QCoreApplication::translate("ResidentsPage", "\360\237\226\212\357\270\217 Modifier", nullptr));
        deleteResidentButton->setText(QCoreApplication::translate("ResidentsPage", "\360\237\227\221\357\270\217 Supprimer", nullptr));
        satisfactionTitle->setText(QCoreApplication::translate("ResidentsPage", "\360\237\223\212 Score de satisfaction estim\303\251", nullptr));
        satisfactionDescription->setText(QCoreApplication::translate("ResidentsPage", "Le syst\303\250me estime automatiquement le niveau de satisfaction et de stabilit\303\251 des r\303\251sidents selon leur comportement, sans graphique.", nullptr));
        renewalLabel->setText(QCoreApplication::translate("ResidentsPage", "Renouvellement de contrat \342\206\222 +10 points", nullptr));
        departureLabel->setText(QCoreApplication::translate("ResidentsPage", "D\303\251part avant la date pr\303\251vue \342\206\222 \342\210\22220 points", nullptr));
        incidentLabel->setText(QCoreApplication::translate("ResidentsPage", "Signalement d'un incident \342\206\222 \342\210\2225 points", nullptr));
        noIncidentLabel->setText(QCoreApplication::translate("ResidentsPage", "Aucun incident sur 6 mois \342\206\222 +5 points", nullptr));
        globalScoreLabel->setText(QCoreApplication::translate("ResidentsPage", "Score global de satisfaction : 82 / 100 \342\200\224 Bonne stabilit\303\251. Quelques incidents mineurs signal\303\251s ce mois-ci.", nullptr));
        innovativeFeaturesTitle->setText(QCoreApplication::translate("ResidentsPage", "\360\237\222\241 Fonctionnalit\303\251s Innovantes", nullptr));
        notificationsFeature->setText(QCoreApplication::translate("ResidentsPage", "\360\237\224\224 Notifications personnalis\303\251es : rappels de fin de contrat, nouveaux entrants, \303\251ch\303\251ances.", nullptr));
        mapFeature->setText(QCoreApplication::translate("ResidentsPage", "\360\237\227\272\357\270\217 Carte interactive par \303\251tage : visualisation et filtrage rapides.", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ResidentsPage: public Ui_ResidentsPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RESIDENTSPAGE_H
