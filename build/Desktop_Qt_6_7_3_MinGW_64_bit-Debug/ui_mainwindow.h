/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QFrame *mainFrame;
    QHBoxLayout *horizontalLayout_2;
    QFrame *leftFrame;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *headerLayout;
    QLabel *logoLabel;
    QSpacerItem *horizontalSpacer;
    QPushButton *fullscreenButton;
    QSpacerItem *verticalSpacer;
    QLabel *titleLabel;
    QLabel *subtitleLabel;
    QSpacerItem *verticalSpacer_2;
    QLabel *emailLabel;
    QLineEdit *emailLineEdit;
    QLabel *passwordLabel;
    QLineEdit *passwordLineEdit;
    QLabel *errorLabel;
    QSpacerItem *verticalSpacer_3;
    QPushButton *loginButton;
    QSpacerItem *verticalSpacer_4;
    QLabel *illustrationLabel;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1280, 956);
        MainWindow->setStyleSheet(QString::fromUtf8("\n"
"    QMainWindow {\n"
"        background-color: #A8C9F0;\n"
"    }\n"
"   "));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setSpacing(50);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(50, 50, 50, 50);
        mainFrame = new QFrame(centralwidget);
        mainFrame->setObjectName("mainFrame");
        mainFrame->setStyleSheet(QString::fromUtf8("\n"
"        QFrame#mainFrame {\n"
"            background-color: #E8F4FD;\n"
"            border: 5px solid #1E3A8A;\n"
"            border-radius: 30px;\n"
"        }\n"
"       "));
        horizontalLayout_2 = new QHBoxLayout(mainFrame);
        horizontalLayout_2->setSpacing(20);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(20, 20, 20, 20);
        leftFrame = new QFrame(mainFrame);
        leftFrame->setObjectName("leftFrame");
        leftFrame->setMinimumSize(QSize(450, 0));
        leftFrame->setMaximumSize(QSize(500, 16777215));
        verticalLayout_2 = new QVBoxLayout(leftFrame);
        verticalLayout_2->setSpacing(25);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(40, 40, 40, 40);
        headerLayout = new QHBoxLayout();
        headerLayout->setObjectName("headerLayout");
        logoLabel = new QLabel(leftFrame);
        logoLabel->setObjectName("logoLabel");
        logoLabel->setStyleSheet(QString::fromUtf8("\n"
"                QLabel {\n"
"                    font-size: 26px;\n"
"                    font-weight: bold;\n"
"                    color: #1E3A8A;\n"
"                    letter-spacing: 2px;\n"
"                }\n"
"               "));
        logoLabel->setPixmap(QPixmap(QString::fromUtf8(":/nexorab.png")));

        headerLayout->addWidget(logoLabel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        headerLayout->addItem(horizontalSpacer);

        fullscreenButton = new QPushButton(leftFrame);
        fullscreenButton->setObjectName("fullscreenButton");
        fullscreenButton->setMinimumSize(QSize(40, 40));
        fullscreenButton->setMaximumSize(QSize(40, 40));
        fullscreenButton->setStyleSheet(QString::fromUtf8("\n"
"                QPushButton {\n"
"                    background-color: transparent;\n"
"                    border: 2px solid #1E3A8A;\n"
"                    border-radius: 8px;\n"
"                    color: #1E3A8A;\n"
"                    font-size: 18px;\n"
"                }\n"
"                QPushButton:hover {\n"
"                    background-color: rgba(30, 58, 138, 0.1);\n"
"                }\n"
"                QPushButton:pressed {\n"
"                    background-color: rgba(30, 58, 138, 0.2);\n"
"                }\n"
"               "));

        headerLayout->addWidget(fullscreenButton);


        verticalLayout_2->addLayout(headerLayout);

        verticalSpacer = new QSpacerItem(20, 30, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout_2->addItem(verticalSpacer);

        titleLabel = new QLabel(leftFrame);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setStyleSheet(QString::fromUtf8("\n"
"              QLabel {\n"
"                  font-size: 52px;\n"
"                  font-weight: bold;\n"
"                  color: #1E3A8A;\n"
"              }\n"
"             "));

        verticalLayout_2->addWidget(titleLabel);

        subtitleLabel = new QLabel(leftFrame);
        subtitleLabel->setObjectName("subtitleLabel");
        subtitleLabel->setStyleSheet(QString::fromUtf8("\n"
"              QLabel {\n"
"                  font-size: 16px;\n"
"                  color: #1E3A8A;\n"
"                  line-height: 1.6;\n"
"              }\n"
"             "));
        subtitleLabel->setWordWrap(true);

        verticalLayout_2->addWidget(subtitleLabel);

        verticalSpacer_2 = new QSpacerItem(20, 30, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_2);

        emailLabel = new QLabel(leftFrame);
        emailLabel->setObjectName("emailLabel");
        emailLabel->setStyleSheet(QString::fromUtf8("\n"
"              QLabel {\n"
"                  font-size: 15px;\n"
"                  color: #1E3A8A;\n"
"                  font-weight: 600;\n"
"              }\n"
"             "));

        verticalLayout_2->addWidget(emailLabel);

        emailLineEdit = new QLineEdit(leftFrame);
        emailLineEdit->setObjectName("emailLineEdit");
        emailLineEdit->setMinimumSize(QSize(0, 50));
        emailLineEdit->setStyleSheet(QString::fromUtf8("\n"
"              QLineEdit {\n"
"                  background-color: #FFFFFF;\n"
"                  border: 2px solid #93C5FD;\n"
"                  border-radius: 10px;\n"
"                  padding: 10px 15px;\n"
"                  font-size: 15px;\n"
"                  color: #1E3A8A;\n"
"              }\n"
"              QLineEdit:focus {\n"
"                  border: 2px solid #3B82F6;\n"
"                  background-color: #F0F9FF;\n"
"              }\n"
"             "));

        verticalLayout_2->addWidget(emailLineEdit);

        passwordLabel = new QLabel(leftFrame);
        passwordLabel->setObjectName("passwordLabel");
        passwordLabel->setStyleSheet(QString::fromUtf8("\n"
"              QLabel {\n"
"                  font-size: 15px;\n"
"                  color: #1E3A8A;\n"
"                  font-weight: 600;\n"
"              }\n"
"             "));

        verticalLayout_2->addWidget(passwordLabel);

        passwordLineEdit = new QLineEdit(leftFrame);
        passwordLineEdit->setObjectName("passwordLineEdit");
        passwordLineEdit->setMinimumSize(QSize(0, 50));
        passwordLineEdit->setStyleSheet(QString::fromUtf8("\n"
"              QLineEdit {\n"
"                  background-color: #FFFFFF;\n"
"                  border: 2px solid #93C5FD;\n"
"                  border-radius: 10px;\n"
"                  padding: 10px 15px;\n"
"                  font-size: 15px;\n"
"                  color: #1E3A8A;\n"
"              }\n"
"              QLineEdit:focus {\n"
"                  border: 2px solid #3B82F6;\n"
"                  background-color: #F0F9FF;\n"
"              }\n"
"             "));
        passwordLineEdit->setEchoMode(QLineEdit::EchoMode::Password);

        verticalLayout_2->addWidget(passwordLineEdit);

        errorLabel = new QLabel(leftFrame);
        errorLabel->setObjectName("errorLabel");
        errorLabel->setStyleSheet(QString::fromUtf8("\n"
"              QLabel {\n"
"                  color: #DC2626;\n"
"                  font-size: 14px;\n"
"                  font-weight: 500;\n"
"                  padding: 5px;\n"
"              }\n"
"             "));
        errorLabel->setWordWrap(true);

        verticalLayout_2->addWidget(errorLabel);

        verticalSpacer_3 = new QSpacerItem(20, 10, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_3);

        loginButton = new QPushButton(leftFrame);
        loginButton->setObjectName("loginButton");
        loginButton->setMinimumSize(QSize(0, 55));
        loginButton->setStyleSheet(QString::fromUtf8("\n"
"              QPushButton {\n"
"                  background-color: #3B82F6;\n"
"                  color: white;\n"
"                  border: none;\n"
"                  border-radius: 12px;\n"
"                  font-size: 18px;\n"
"                  font-weight: bold;\n"
"                  letter-spacing: 1px;\n"
"              }\n"
"              QPushButton:hover {\n"
"                  background-color: #2563EB;\n"
"              }\n"
"              QPushButton:pressed {\n"
"                  background-color: #1D4ED8;\n"
"              }\n"
"             "));

        verticalLayout_2->addWidget(loginButton);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_4);


        horizontalLayout_2->addWidget(leftFrame);

        illustrationLabel = new QLabel(mainFrame);
        illustrationLabel->setObjectName("illustrationLabel");
        illustrationLabel->setMinimumSize(QSize(500, 500));
        illustrationLabel->setStyleSheet(QString::fromUtf8("\n"
"           QLabel {\n"
"               background-color: transparent;\n"
"           }\n"
"          "));
        illustrationLabel->setPixmap(QPixmap(QString::fromUtf8(":/pic1")));
        illustrationLabel->setScaledContents(true);
        illustrationLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout_2->addWidget(illustrationLabel);


        verticalLayout->addWidget(mainFrame);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "NEXORA - Connexion", nullptr));
        logoLabel->setText(QString());
#if QT_CONFIG(tooltip)
        fullscreenButton->setToolTip(QCoreApplication::translate("MainWindow", "Basculer plein \303\251cran", nullptr));
#endif // QT_CONFIG(tooltip)
        fullscreenButton->setText(QCoreApplication::translate("MainWindow", "\342\233\266", nullptr));
        titleLabel->setText(QCoreApplication::translate("MainWindow", "Connexion", nullptr));
        subtitleLabel->setText(QCoreApplication::translate("MainWindow", "Bienvenue", nullptr));
        emailLabel->setText(QCoreApplication::translate("MainWindow", "Username", nullptr));
        emailLineEdit->setText(QString());
        emailLineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Entrez votre username", nullptr));
        passwordLabel->setText(QCoreApplication::translate("MainWindow", "Mot de passe", nullptr));
        passwordLineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Entrez votre mot de passe", nullptr));
        errorLabel->setText(QString());
        loginButton->setText(QCoreApplication::translate("MainWindow", "CONNEXION", nullptr));
        illustrationLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
