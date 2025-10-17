/********************************************************************************
** Form generated from reading UI file 'welcomedialog.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WELCOMEDIALOG_H
#define UI_WELCOMEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_WelcomeDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *welcomeFrame;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer;
    QLabel *welcomeLabel;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QDialog *WelcomeDialog)
    {
        if (WelcomeDialog->objectName().isEmpty())
            WelcomeDialog->setObjectName("WelcomeDialog");
        WelcomeDialog->resize(1280, 720);
        WelcomeDialog->setStyleSheet(QString::fromUtf8("\n"
"    QDialog {\n"
"        background: qlineargradient(\n"
"            x1:0, y1:0, x2:1, y2:1,\n"
"            stop:0 #DCEBFF,\n"
"            stop:1 #B7D1FF\n"
"        );\n"
"    }\n"
"   "));
        verticalLayout = new QVBoxLayout(WelcomeDialog);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        welcomeFrame = new QFrame(WelcomeDialog);
        welcomeFrame->setObjectName("welcomeFrame");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(welcomeFrame->sizePolicy().hasHeightForWidth());
        welcomeFrame->setSizePolicy(sizePolicy);
        welcomeFrame->setStyleSheet(QString::fromUtf8("\n"
"       QFrame#welcomeFrame {\n"
"           background-color: rgba(255, 255, 255, 0.3);\n"
"           border-radius: 30px;\n"
"           margin: 60px;\n"
"       }\n"
"      "));
        verticalLayout_2 = new QVBoxLayout(welcomeFrame);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setAlignment(Qt::AlignmentFlag::AlignCenter);
        verticalSpacer = new QSpacerItem(20, 100, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        welcomeLabel = new QLabel(welcomeFrame);
        welcomeLabel->setObjectName("welcomeLabel");
        welcomeLabel->setMinimumSize(QSize(600, 200));
        welcomeLabel->setStyleSheet(QString::fromUtf8("\n"
"          QLabel {\n"
"              font-family: \"Segoe UI Semibold\";\n"
"              font-size: 72px;\n"
"              font-weight: bold;\n"
"              color: #1E3A8A;\n"
"              background-color: rgba(255, 255, 255, 0.8);\n"
"              border: 4px solid #1E3A8A;\n"
"              border-radius: 30px;\n"
"              padding: 40px;\n"
"              qproperty-alignment: AlignCenter;\n"
"          }\n"
"         "));
        welcomeLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_2->addWidget(welcomeLabel);

        verticalSpacer_2 = new QSpacerItem(20, 100, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);


        verticalLayout->addWidget(welcomeFrame);


        retranslateUi(WelcomeDialog);

        QMetaObject::connectSlotsByName(WelcomeDialog);
    } // setupUi

    void retranslateUi(QDialog *WelcomeDialog)
    {
        WelcomeDialog->setWindowTitle(QCoreApplication::translate("WelcomeDialog", "Bienvenue", nullptr));
        welcomeLabel->setText(QCoreApplication::translate("WelcomeDialog", "Bienvenue", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WelcomeDialog: public Ui_WelcomeDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WELCOMEDIALOG_H
