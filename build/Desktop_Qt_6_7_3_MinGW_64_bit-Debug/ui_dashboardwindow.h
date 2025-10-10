/********************************************************************************
** Form generated from reading UI file 'dashboardwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DASHBOARDWINDOW_H
#define UI_DASHBOARDWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DashboardWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QFrame *sidebarFrame;
    QVBoxLayout *verticalLayout;
    QLabel *logoLabel;
    QSpacerItem *verticalSpacer_top;
    QPushButton *dashboardButton;
    QPushButton *residentsButton;
    QPushButton *financesButton;
    QPushButton *transportsButton;
    QPushButton *incidentsButton;
    QPushButton *locauxButton;
    QPushButton *settingsButton;
    QSpacerItem *verticalSpacer_bottom;
    QWidget *mainContentWidget;
    QVBoxLayout *verticalLayout_2;
    QFrame *headerFrame;
    QHBoxLayout *horizontalLayout_2;
    QLabel *titleLabel;
    QSpacerItem *horizontalSpacer;
    QLineEdit *searchLineEdit;
    QPushButton *searchButton;
    QPushButton *exitButton;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_3;
    QFrame *topCardsFrame;
    QHBoxLayout *horizontalLayout_3;
    QFrame *profileCard;
    QVBoxLayout *verticalLayout_4;
    QLabel *profileImageLabel;
    QLabel *profileNameLabel;
    QLabel *registrationLabel;
    QLabel *locationLabel;
    QLabel *birthLabel;
    QLabel *emailLabel;
    QFrame *paymentCard;
    QVBoxLayout *verticalLayout_5;
    QLabel *paymentTitleLabel;
    QLabel *cardNumberLabel;
    QLabel *cardNumberValue;
    QLabel *paymentMethodsLabel;
    QSpacerItem *verticalSpacer_2;
    QFrame *bottomCardsFrame;
    QHBoxLayout *horizontalLayout_4;
    QFrame *coursesCard;
    QVBoxLayout *verticalLayout_6;
    QLabel *coursesTitleLabel;
    QLabel *courseInfoLabel;
    QLabel *courseInfo2Label;
    QLabel *courseInfo3Label;
    QFrame *premiumCard;
    QVBoxLayout *verticalLayout_7;
    QLabel *premiumTitleLabel;
    QLabel *premiumBenefit1;
    QLabel *premiumBenefit2;
    QLabel *premiumBenefit3;
    QLabel *premiumBenefit4;
    QSpacerItem *verticalSpacer_3;
    QPushButton *subscribeButton;
    QSpacerItem *verticalSpacer_4;

    void setupUi(QMainWindow *DashboardWindow)
    {
        if (DashboardWindow->objectName().isEmpty())
            DashboardWindow->setObjectName("DashboardWindow");
        DashboardWindow->resize(1920, 1080);
        DashboardWindow->setStyleSheet(QString::fromUtf8("QMainWindow {\n"
"    background-color: #F5F7FA;\n"
"}"));
        centralwidget = new QWidget(DashboardWindow);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setStyleSheet(QString::fromUtf8("QWidget#centralwidget {\n"
"    background-color: #F5F7FA;\n"
"}"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        sidebarFrame = new QFrame(centralwidget);
        sidebarFrame->setObjectName("sidebarFrame");
        sidebarFrame->setMinimumSize(QSize(280, 0));
        sidebarFrame->setMaximumSize(QSize(280, 16777215));
        sidebarFrame->setStyleSheet(QString::fromUtf8("QFrame#sidebarFrame {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,\n"
"                                stop:0 #4A90E2, stop:0.3 #6BA3E8, stop:0.7 #8BB6EE, stop:1 #ABC9F4);\n"
"    border: none;\n"
"}"));
        sidebarFrame->setFrameShape(QFrame::Shape::StyledPanel);
        verticalLayout = new QVBoxLayout(sidebarFrame);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 40, 0, 40);
        logoLabel = new QLabel(sidebarFrame);
        logoLabel->setObjectName("logoLabel");
        logoLabel->setStyleSheet(QString::fromUtf8(""));
        logoLabel->setPixmap(QPixmap(QString::fromUtf8(":/nexora.png")));
        logoLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(logoLabel);

        verticalSpacer_top = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_top);

        dashboardButton = new QPushButton(sidebarFrame);
        dashboardButton->setObjectName("dashboardButton");
        dashboardButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgba(255, 255, 255, 0.2);\n"
"    color: white;\n"
"    border: 2px solid rgba(255, 255, 255, 0.3);\n"
"    border-radius: 15px 0px 0px 15px;\n"
"    padding: 18px 25px;\n"
"    margin: 8px 15px 8px 20px;\n"
"    text-align: left;\n"
"    font-size: 15px;\n"
"    font-weight: 500;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: rgba(255, 255, 255, 0.3);\n"
"    border: 2px solid rgba(255, 255, 255, 0.5);\n"
"    margin-right: -10px;\n"
"    padding-right: 40px;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgba(255, 255, 255, 0.4);\n"
"    margin-right: -5px;\n"
"}\n"
"QPushButton:checked {\n"
"    background-color: #F5F7FA;\n"
"    color: #4A90E2;\n"
"    font-weight: bold;\n"
"    border: 3px solid #F5F7FA;\n"
"    margin-right: -15px;\n"
"    padding-right: 45px;\n"
"}"));
        QIcon icon;
        icon.addFile(QString::fromUtf8("."), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        dashboardButton->setIcon(icon);
        dashboardButton->setIconSize(QSize(24, 24));
        dashboardButton->setCheckable(true);
        dashboardButton->setChecked(true);

        verticalLayout->addWidget(dashboardButton);

        residentsButton = new QPushButton(sidebarFrame);
        residentsButton->setObjectName("residentsButton");
        residentsButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgba(255, 255, 255, 0.1);\n"
"    color: white;\n"
"    border: 2px solid rgba(255, 255, 255, 0.2);\n"
"    border-radius: 15px 0px 0px 15px;\n"
"    padding: 18px 25px;\n"
"    margin: 8px 15px 8px 20px;\n"
"    text-align: left;\n"
"    font-size: 15px;\n"
"    font-weight: 500;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: rgba(255, 255, 255, 0.3);\n"
"    border: 2px solid rgba(255, 255, 255, 0.5);\n"
"    margin-right: -10px;\n"
"    padding-right: 40px;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgba(255, 255, 255, 0.4);\n"
"    margin-right: -5px;\n"
"}\n"
"QPushButton:checked {\n"
"    background-color: #F5F7FA;\n"
"    color: #4A90E2;\n"
"    font-weight: bold;\n"
"    border: 3px solid #F5F7FA;\n"
"    margin-right: -15px;\n"
"    padding-right: 45px;\n"
"}"));
        residentsButton->setCheckable(true);

        verticalLayout->addWidget(residentsButton);

        financesButton = new QPushButton(sidebarFrame);
        financesButton->setObjectName("financesButton");
        financesButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: transparent;\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 15px 0px 0px 15px;\n"
"    padding: 18px 25px;\n"
"    margin: 8px 15px 8px 20px;\n"
"    text-align: left;\n"
"    font-size: 15px;\n"
"    font-weight: 500;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: rgba(255, 255, 255, 0.3);\n"
"    border: 2px solid rgba(255, 255, 255, 0.5);\n"
"    margin-right: -10px;\n"
"    padding-right: 40px;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgba(255, 255, 255, 0.25);\n"
"}\n"
"QPushButton:checked {\n"
"    background-color: #F5F7FA;\n"
"    color: #4A90E2;\n"
"    font-weight: bold;\n"
"    border: 3px solid #F5F7FA;\n"
"    margin-right: -15px;\n"
"    padding-right: 45px;\n"
"}"));
        financesButton->setCheckable(true);

        verticalLayout->addWidget(financesButton);

        transportsButton = new QPushButton(sidebarFrame);
        transportsButton->setObjectName("transportsButton");
        transportsButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: transparent;\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 15px 0px 0px 15px;\n"
"    padding: 18px 25px;\n"
"    margin: 8px 15px 8px 20px;\n"
"    text-align: left;\n"
"    font-size: 15px;\n"
"    font-weight: 500;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: rgba(255, 255, 255, 0.3);\n"
"    border: 2px solid rgba(255, 255, 255, 0.5);\n"
"    margin-right: -10px;\n"
"    padding-right: 40px;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgba(255, 255, 255, 0.25);\n"
"}\n"
"QPushButton:checked {\n"
"    background-color: #F5F7FA;\n"
"    color: #4A90E2;\n"
"    font-weight: bold;\n"
"    border: 3px solid #F5F7FA;\n"
"    margin-right: -15px;\n"
"    padding-right: 45px;\n"
"}"));
        transportsButton->setCheckable(true);

        verticalLayout->addWidget(transportsButton);

        incidentsButton = new QPushButton(sidebarFrame);
        incidentsButton->setObjectName("incidentsButton");
        incidentsButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: transparent;\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 15px 0px 0px 15px;\n"
"    padding: 18px 25px;\n"
"    margin: 8px 15px 8px 20px;\n"
"    text-align: left;\n"
"    font-size: 15px;\n"
"    font-weight: 500;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: rgba(255, 255, 255, 0.3);\n"
"    border: 2px solid rgba(255, 255, 255, 0.5);\n"
"    margin-right: -10px;\n"
"    padding-right: 40px;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgba(255, 255, 255, 0.25);\n"
"}\n"
"QPushButton:checked {\n"
"    background-color: #F5F7FA;\n"
"    color: #4A90E2;\n"
"    font-weight: bold;\n"
"    border: 3px solid #F5F7FA;\n"
"    margin-right: -15px;\n"
"    padding-right: 45px;\n"
"}"));
        incidentsButton->setCheckable(true);

        verticalLayout->addWidget(incidentsButton);

        locauxButton = new QPushButton(sidebarFrame);
        locauxButton->setObjectName("locauxButton");
        locauxButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: transparent;\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 15px 0px 0px 15px;\n"
"    padding: 18px 25px;\n"
"    margin: 8px 15px 8px 20px;\n"
"    text-align: left;\n"
"    font-size: 15px;\n"
"    font-weight: 500;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: rgba(255, 255, 255, 0.3);\n"
"    border: 2px solid rgba(255, 255, 255, 0.5);\n"
"    margin-right: -10px;\n"
"    padding-right: 40px;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgba(255, 255, 255, 0.25);\n"
"}\n"
"QPushButton:checked {\n"
"    background-color: #F5F7FA;\n"
"    color: #4A90E2;\n"
"    font-weight: bold;\n"
"    border: 3px solid #F5F7FA;\n"
"    margin-right: -15px;\n"
"    padding-right: 45px;\n"
"}"));
        locauxButton->setCheckable(true);

        verticalLayout->addWidget(locauxButton);

        settingsButton = new QPushButton(sidebarFrame);
        settingsButton->setObjectName("settingsButton");
        settingsButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: transparent;\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 15px 0px 0px 15px;\n"
"    padding: 18px 25px;\n"
"    margin: 8px 15px 8px 20px;\n"
"    text-align: left;\n"
"    font-size: 15px;\n"
"    font-weight: 500;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: rgba(255, 255, 255, 0.3);\n"
"    border: 2px solid rgba(255, 255, 255, 0.5);\n"
"    margin-right: -10px;\n"
"    padding-right: 40px;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgba(255, 255, 255, 0.25);\n"
"}\n"
"QPushButton:checked {\n"
"    background-color: #F5F7FA;\n"
"    color: #4A90E2;\n"
"    font-weight: bold;\n"
"    border: 3px solid #F5F7FA;\n"
"    margin-right: -15px;\n"
"    padding-right: 45px;\n"
"}"));
        settingsButton->setCheckable(true);

        verticalLayout->addWidget(settingsButton);

        verticalSpacer_bottom = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_bottom);


        horizontalLayout->addWidget(sidebarFrame);

        mainContentWidget = new QWidget(centralwidget);
        mainContentWidget->setObjectName("mainContentWidget");
        mainContentWidget->setStyleSheet(QString::fromUtf8("QWidget#mainContentWidget {\n"
"    background-color: #F5F7FA;\n"
"}"));
        verticalLayout_2 = new QVBoxLayout(mainContentWidget);
        verticalLayout_2->setSpacing(25);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(40, 30, 40, 30);
        headerFrame = new QFrame(mainContentWidget);
        headerFrame->setObjectName("headerFrame");
        headerFrame->setStyleSheet(QString::fromUtf8("QFrame {\n"
"    background-color: transparent;\n"
"    border: none;\n"
"}"));
        headerFrame->setFrameShape(QFrame::Shape::StyledPanel);
        horizontalLayout_2 = new QHBoxLayout(headerFrame);
        horizontalLayout_2->setSpacing(20);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        titleLabel = new QLabel(headerFrame);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 28px;\n"
"    font-weight: bold;\n"
"    color: #2C3E50;\n"
"}"));

        horizontalLayout_2->addWidget(titleLabel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        searchLineEdit = new QLineEdit(headerFrame);
        searchLineEdit->setObjectName("searchLineEdit");
        searchLineEdit->setMinimumSize(QSize(300, 45));
        searchLineEdit->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    background-color: white;\n"
"    border: 2px solid #E0E6ED;\n"
"    border-radius: 22px;\n"
"    padding: 10px 20px;\n"
"    font-size: 14px;\n"
"    color: #2C3E50;\n"
"}\n"
"QLineEdit:focus {\n"
"    border-color: #4A90E2;\n"
"}"));
        searchLineEdit->setClearButtonEnabled(true);

        horizontalLayout_2->addWidget(searchLineEdit);

        searchButton = new QPushButton(headerFrame);
        searchButton->setObjectName("searchButton");
        searchButton->setMinimumSize(QSize(45, 45));
        searchButton->setMaximumSize(QSize(45, 45));
        searchButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #4A90E2;\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 22px;\n"
"    font-size: 16px;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #5B4C95;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #4B3C85;\n"
"}"));

        horizontalLayout_2->addWidget(searchButton);

        exitButton = new QPushButton(headerFrame);
        exitButton->setObjectName("exitButton");
        exitButton->setMinimumSize(QSize(45, 45));
        exitButton->setMaximumSize(QSize(45, 45));
        exitButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: white;\n"
"    color: #4A90E2;\n"
"    border: 2px solid #E0E6ED;\n"
"    border-radius: 22px;\n"
"    font-size: 20px;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #F0F0F0;\n"
"    border-color: #4A90E2;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #E0E0E0;\n"
"}"));

        horizontalLayout_2->addWidget(exitButton);


        verticalLayout_2->addWidget(headerFrame);

        scrollArea = new QScrollArea(mainContentWidget);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setStyleSheet(QString::fromUtf8("QScrollArea {\n"
"    background-color: transparent;\n"
"    border: none;\n"
"}\n"
"QScrollBar:vertical {\n"
"    background-color: #F5F7FA;\n"
"    width: 10px;\n"
"    margin: 0px;\n"
"    border-radius: 5px;\n"
"}\n"
"QScrollBar::handle:vertical {\n"
"    background-color: #C0C7D1;\n"
"    border-radius: 5px;\n"
"    min-height: 30px;\n"
"}\n"
"QScrollBar::handle:vertical:hover {\n"
"    background-color: #A0A7B1;\n"
"}\n"
"QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {\n"
"    height: 0px;\n"
"}\n"
"QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {\n"
"    background: none;\n"
"}"));
        scrollArea->setFrameShape(QFrame::Shape::NoFrame);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 1560, 950));
        verticalLayout_3 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_3->setSpacing(25);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(0, 0, 10, 0);
        topCardsFrame = new QFrame(scrollAreaWidgetContents);
        topCardsFrame->setObjectName("topCardsFrame");
        topCardsFrame->setStyleSheet(QString::fromUtf8("QFrame {\n"
"    background-color: transparent;\n"
"    border: none;\n"
"}"));
        topCardsFrame->setFrameShape(QFrame::Shape::StyledPanel);
        horizontalLayout_3 = new QHBoxLayout(topCardsFrame);
        horizontalLayout_3->setSpacing(25);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        profileCard = new QFrame(topCardsFrame);
        profileCard->setObjectName("profileCard");
        profileCard->setMinimumSize(QSize(0, 350));
        profileCard->setStyleSheet(QString::fromUtf8("QFrame#profileCard {\n"
"    background-color: white;\n"
"    border-radius: 20px;\n"
"    border: 1px solid #E8EDF5;\n"
"}"));
        profileCard->setFrameShape(QFrame::Shape::StyledPanel);
        verticalLayout_4 = new QVBoxLayout(profileCard);
        verticalLayout_4->setSpacing(15);
        verticalLayout_4->setObjectName("verticalLayout_4");
        verticalLayout_4->setContentsMargins(30, 30, 30, 30);
        profileImageLabel = new QLabel(profileCard);
        profileImageLabel->setObjectName("profileImageLabel");
        profileImageLabel->setMinimumSize(QSize(120, 120));
        profileImageLabel->setMaximumSize(QSize(120, 120));
        profileImageLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,\n"
"                                stop:0 #8BB6EE, stop:1 #4A90E2);\n"
"    border-radius: 60px;\n"
"    font-size: 60px;\n"
"    color: white;\n"
"}"));
        profileImageLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_4->addWidget(profileImageLabel);

        profileNameLabel = new QLabel(profileCard);
        profileNameLabel->setObjectName("profileNameLabel");
        profileNameLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: #2C3E50;\n"
"}"));
        profileNameLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_4->addWidget(profileNameLabel);

        registrationLabel = new QLabel(profileCard);
        registrationLabel->setObjectName("registrationLabel");
        registrationLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 13px;\n"
"    color: #95A5A6;\n"
"}"));
        registrationLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_4->addWidget(registrationLabel);

        locationLabel = new QLabel(profileCard);
        locationLabel->setObjectName("locationLabel");
        locationLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 13px;\n"
"    color: #95A5A6;\n"
"}"));
        locationLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_4->addWidget(locationLabel);

        birthLabel = new QLabel(profileCard);
        birthLabel->setObjectName("birthLabel");
        birthLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 13px;\n"
"    color: #95A5A6;\n"
"}"));
        birthLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_4->addWidget(birthLabel);

        emailLabel = new QLabel(profileCard);
        emailLabel->setObjectName("emailLabel");
        emailLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 13px;\n"
"    color: #95A5A6;\n"
"}"));
        emailLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_4->addWidget(emailLabel);


        horizontalLayout_3->addWidget(profileCard);

        paymentCard = new QFrame(topCardsFrame);
        paymentCard->setObjectName("paymentCard");
        paymentCard->setMinimumSize(QSize(0, 350));
        paymentCard->setStyleSheet(QString::fromUtf8("QFrame#paymentCard {\n"
"    background-color: white;\n"
"    border-radius: 20px;\n"
"    border: 1px solid #E8EDF5;\n"
"}"));
        paymentCard->setFrameShape(QFrame::Shape::StyledPanel);
        verticalLayout_5 = new QVBoxLayout(paymentCard);
        verticalLayout_5->setSpacing(20);
        verticalLayout_5->setObjectName("verticalLayout_5");
        verticalLayout_5->setContentsMargins(30, 30, 30, 30);
        paymentTitleLabel = new QLabel(paymentCard);
        paymentTitleLabel->setObjectName("paymentTitleLabel");
        paymentTitleLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 18px;\n"
"    font-weight: bold;\n"
"    color: #2C3E50;\n"
"}"));

        verticalLayout_5->addWidget(paymentTitleLabel);

        cardNumberLabel = new QLabel(paymentCard);
        cardNumberLabel->setObjectName("cardNumberLabel");
        cardNumberLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 14px;\n"
"    color: #7F8C8D;\n"
"    margin-bottom: 5px;\n"
"}"));

        verticalLayout_5->addWidget(cardNumberLabel);

        cardNumberValue = new QLabel(paymentCard);
        cardNumberValue->setObjectName("cardNumberValue");
        cardNumberValue->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 14px;\n"
"    color: #2C3E50;\n"
"    background-color: #F8F9FA;\n"
"    padding: 12px 20px;\n"
"    border-radius: 10px;\n"
"}"));

        verticalLayout_5->addWidget(cardNumberValue);

        paymentMethodsLabel = new QLabel(paymentCard);
        paymentMethodsLabel->setObjectName("paymentMethodsLabel");
        paymentMethodsLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 24px;\n"
"    color: #2C3E50;\n"
"    padding: 15px;\n"
"}"));
        paymentMethodsLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_5->addWidget(paymentMethodsLabel);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_2);


        horizontalLayout_3->addWidget(paymentCard);


        verticalLayout_3->addWidget(topCardsFrame);

        bottomCardsFrame = new QFrame(scrollAreaWidgetContents);
        bottomCardsFrame->setObjectName("bottomCardsFrame");
        bottomCardsFrame->setStyleSheet(QString::fromUtf8("QFrame {\n"
"    background-color: transparent;\n"
"    border: none;\n"
"}"));
        bottomCardsFrame->setFrameShape(QFrame::Shape::StyledPanel);
        horizontalLayout_4 = new QHBoxLayout(bottomCardsFrame);
        horizontalLayout_4->setSpacing(25);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        coursesCard = new QFrame(bottomCardsFrame);
        coursesCard->setObjectName("coursesCard");
        coursesCard->setMinimumSize(QSize(0, 400));
        coursesCard->setStyleSheet(QString::fromUtf8("QFrame#coursesCard {\n"
"    background-color: white;\n"
"    border-radius: 20px;\n"
"    border: 1px solid #E8EDF5;\n"
"}"));
        coursesCard->setFrameShape(QFrame::Shape::StyledPanel);
        verticalLayout_6 = new QVBoxLayout(coursesCard);
        verticalLayout_6->setSpacing(20);
        verticalLayout_6->setObjectName("verticalLayout_6");
        verticalLayout_6->setContentsMargins(30, 30, 30, 30);
        coursesTitleLabel = new QLabel(coursesCard);
        coursesTitleLabel->setObjectName("coursesTitleLabel");
        coursesTitleLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: #2C3E50;\n"
"}"));

        verticalLayout_6->addWidget(coursesTitleLabel);

        courseInfoLabel = new QLabel(coursesCard);
        courseInfoLabel->setObjectName("courseInfoLabel");
        courseInfoLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 14px;\n"
"    color: #7F8C8D;\n"
"    background-color: #F8F9FA;\n"
"    padding: 20px;\n"
"    border-radius: 15px;\n"
"    line-height: 1.6;\n"
"}"));
        courseInfoLabel->setWordWrap(true);

        verticalLayout_6->addWidget(courseInfoLabel);

        courseInfo2Label = new QLabel(coursesCard);
        courseInfo2Label->setObjectName("courseInfo2Label");
        courseInfo2Label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 14px;\n"
"    color: #7F8C8D;\n"
"    background-color: #F8F9FA;\n"
"    padding: 20px;\n"
"    border-radius: 15px;\n"
"    line-height: 1.6;\n"
"}"));
        courseInfo2Label->setWordWrap(true);

        verticalLayout_6->addWidget(courseInfo2Label);

        courseInfo3Label = new QLabel(coursesCard);
        courseInfo3Label->setObjectName("courseInfo3Label");
        courseInfo3Label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 14px;\n"
"    color: #7F8C8D;\n"
"    background-color: #FFF4F4;\n"
"    padding: 20px;\n"
"    border-radius: 15px;\n"
"    line-height: 1.6;\n"
"}"));
        courseInfo3Label->setWordWrap(true);

        verticalLayout_6->addWidget(courseInfo3Label);


        horizontalLayout_4->addWidget(coursesCard);

        premiumCard = new QFrame(bottomCardsFrame);
        premiumCard->setObjectName("premiumCard");
        premiumCard->setMinimumSize(QSize(0, 400));
        premiumCard->setStyleSheet(QString::fromUtf8("QFrame#premiumCard {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                                stop:0 #6BA3E8, stop:1 #4A90E2);\n"
"    border-radius: 20px;\n"
"    border: none;\n"
"}"));
        premiumCard->setFrameShape(QFrame::Shape::StyledPanel);
        verticalLayout_7 = new QVBoxLayout(premiumCard);
        verticalLayout_7->setSpacing(20);
        verticalLayout_7->setObjectName("verticalLayout_7");
        verticalLayout_7->setContentsMargins(35, 35, 35, 35);
        premiumTitleLabel = new QLabel(premiumCard);
        premiumTitleLabel->setObjectName("premiumTitleLabel");
        premiumTitleLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 22px;\n"
"    font-weight: bold;\n"
"    color: white;\n"
"    line-height: 1.3;\n"
"}"));
        premiumTitleLabel->setWordWrap(true);

        verticalLayout_7->addWidget(premiumTitleLabel);

        premiumBenefit1 = new QLabel(premiumCard);
        premiumBenefit1->setObjectName("premiumBenefit1");
        premiumBenefit1->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 15px;\n"
"    color: white;\n"
"    padding-left: 10px;\n"
"}"));

        verticalLayout_7->addWidget(premiumBenefit1);

        premiumBenefit2 = new QLabel(premiumCard);
        premiumBenefit2->setObjectName("premiumBenefit2");
        premiumBenefit2->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 15px;\n"
"    color: white;\n"
"    padding-left: 10px;\n"
"}"));
        premiumBenefit2->setWordWrap(true);

        verticalLayout_7->addWidget(premiumBenefit2);

        premiumBenefit3 = new QLabel(premiumCard);
        premiumBenefit3->setObjectName("premiumBenefit3");
        premiumBenefit3->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 15px;\n"
"    color: white;\n"
"    padding-left: 10px;\n"
"}"));

        verticalLayout_7->addWidget(premiumBenefit3);

        premiumBenefit4 = new QLabel(premiumCard);
        premiumBenefit4->setObjectName("premiumBenefit4");
        premiumBenefit4->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 15px;\n"
"    color: white;\n"
"    padding-left: 10px;\n"
"}"));
        premiumBenefit4->setWordWrap(true);

        verticalLayout_7->addWidget(premiumBenefit4);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_7->addItem(verticalSpacer_3);

        subscribeButton = new QPushButton(premiumCard);
        subscribeButton->setObjectName("subscribeButton");
        subscribeButton->setMinimumSize(QSize(0, 50));
        subscribeButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: white;\n"
"    color: #4A90E2;\n"
"    border: none;\n"
"    border-radius: 25px;\n"
"    padding: 15px 30px;\n"
"    font-size: 16px;\n"
"    font-weight: bold;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #F5F5F5;\n"
"    transform: scale(1.05);\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #E0E0E0;\n"
"}"));

        verticalLayout_7->addWidget(subscribeButton);


        horizontalLayout_4->addWidget(premiumCard);


        verticalLayout_3->addWidget(bottomCardsFrame);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_4);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_2->addWidget(scrollArea);


        horizontalLayout->addWidget(mainContentWidget);

        DashboardWindow->setCentralWidget(centralwidget);

        retranslateUi(DashboardWindow);

        QMetaObject::connectSlotsByName(DashboardWindow);
    } // setupUi

    void retranslateUi(QMainWindow *DashboardWindow)
    {
        DashboardWindow->setWindowTitle(QCoreApplication::translate("DashboardWindow", "SUCCESS - Smart City Dashboard", nullptr));
        logoLabel->setText(QString());
        dashboardButton->setText(QCoreApplication::translate("DashboardWindow", "   Tableau de bord", nullptr));
        residentsButton->setText(QCoreApplication::translate("DashboardWindow", "   Residents", nullptr));
        financesButton->setText(QCoreApplication::translate("DashboardWindow", "   Finances", nullptr));
        transportsButton->setText(QCoreApplication::translate("DashboardWindow", "   Transports", nullptr));
        incidentsButton->setText(QCoreApplication::translate("DashboardWindow", "   Incidents", nullptr));
        locauxButton->setText(QCoreApplication::translate("DashboardWindow", "   Locaux", nullptr));
        settingsButton->setText(QCoreApplication::translate("DashboardWindow", "   Parametres", nullptr));
        titleLabel->setText(QCoreApplication::translate("DashboardWindow", "\320\237\321\200\320\276\321\204\320\270\320\273\321\214", nullptr));
        searchLineEdit->setPlaceholderText(QCoreApplication::translate("DashboardWindow", "\360\237\224\215 Rechercher dans NEXORA...", nullptr));
        searchButton->setText(QCoreApplication::translate("DashboardWindow", "\360\237\224\215", nullptr));
        exitButton->setText(QCoreApplication::translate("DashboardWindow", "\342\216\213", nullptr));
        profileImageLabel->setText(QCoreApplication::translate("DashboardWindow", "\360\237\221\244", nullptr));
        profileNameLabel->setText(QCoreApplication::translate("DashboardWindow", "Smart City User", nullptr));
        registrationLabel->setText(QCoreApplication::translate("DashboardWindow", "\320\224\320\260\321\202\320\260 \321\200\320\265\320\263\320\270\321\201\321\202\321\200\320\260\321\206\320\270\320\270: 24 \320\275\320\276\321\217\320\261\321\200\321\217 2022 \320\263.", nullptr));
        locationLabel->setText(QCoreApplication::translate("DashboardWindow", "\320\241\321\202\321\200\320\260\320\275\320\260, \320\263\320\276\321\200\320\276\320\264: \320\221\320\276\320\273\320\263\320\260\321\200\320\270\321\217, \320\241\320\276\321\204\320\270\321\217", nullptr));
        birthLabel->setText(QCoreApplication::translate("DashboardWindow", "\320\224\320\260\321\202\320\260 \321\200\320\276\320\266\320\264\320\265\320\275\320\270\321\217: 08.04.1993 \320\263.", nullptr));
        emailLabel->setText(QCoreApplication::translate("DashboardWindow", "E-mail: smartcity@mail.ru", nullptr));
        paymentTitleLabel->setText(QCoreApplication::translate("DashboardWindow", "\320\237\320\273\320\260\321\202\320\265\320\266\320\275\321\213\320\265 \320\264\320\260\320\275\320\275\321\213\320\265", nullptr));
        cardNumberLabel->setText(QCoreApplication::translate("DashboardWindow", "\320\235\320\276\320\274\320\265\321\200 \320\272\320\260\321\200\321\202\320\276\321\207\320\272\320\270:", nullptr));
        cardNumberValue->setText(QCoreApplication::translate("DashboardWindow", "236 *** *** *** 265", nullptr));
        paymentMethodsLabel->setText(QCoreApplication::translate("DashboardWindow", "\360\237\222\263 VISA   \360\237\224\264 MasterCard   \360\237\223\261 GPay", nullptr));
        coursesTitleLabel->setText(QCoreApplication::translate("DashboardWindow", "\320\234\320\276\320\270 \320\272\321\203\321\200\321\201\321\213", nullptr));
        courseInfoLabel->setText(QCoreApplication::translate("DashboardWindow", "\360\237\223\232 UX/UI Design \342\200\224 \321\201\320\260\320\271\321\202\321\213\n"
"\320\232\320\276\320\274\320\277\320\276\320\267\320\270\321\206\320\270\321\217, \321\202\320\270\320\277\320\276\320\263\321\200\320\260\321\204\320\270\320\272\320\260, \321\202\320\265\320\276\321\200\320\270\321\217 \321\206\320\262\320\265\321\202\320\260...\n"
"68 \321\203\321\200\320\276\320\272\320\276\320\262  \342\234\205 \320\227\320\260\320\262\320\265\321\200\321\210\320\265\320\275", nullptr));
        courseInfo2Label->setText(QCoreApplication::translate("DashboardWindow", "\360\237\223\261 UX/UI Design \342\200\224 \320\277\321\200\320\270\320\273\320\276\320\266\320\265\320\275\320\270\321\217\n"
"\320\224\320\270\320\267\320\260\320\271\320\275 \320\270\320\275\321\202\320\265\321\200\321\204\320\265\320\271\321\201\320\260 \320\274\320\276\320\261\320\270\320\273\321\214\320\275\321\213\321\205 \320\277\321\200\320\270\320\273\320\276\320\266\320\265\320\275\320\270\320\271...\n"
"12 \321\203\321\200\320\276\320\272\320\276\320\262  \342\234\205 \320\227\320\260\320\262\320\265\321\200\321\210\320\265\320\275", nullptr));
        courseInfo3Label->setText(QCoreApplication::translate("DashboardWindow", "\360\237\216\254 UX/UI Design \342\200\224 \320\260\320\275\320\270\320\274\320\260\321\206\320\270\321\217\n"
"\320\220\320\275\320\270\320\274\320\260\321\206\320\270\321\217 \321\215\320\273\320\265\320\274\320\265\320\275\321\202\320\276\320\262 \320\262 \320\270\320\275\321\202\320\265\321\200\321\204\320\265\320\271\321\201\320\265...\n"
"12 \321\203\321\200\320\276\320\272\320\276\320\262  \360\237\225\220 \320\235\320\260\321\207\320\260\321\202: 13.06.2023\320\263.", nullptr));
        premiumTitleLabel->setText(QCoreApplication::translate("DashboardWindow", "\320\230\320\275\320\264\320\270\320\262\320\270\320\264\321\203\320\260\320\273\321\214\320\275\320\260\321\217\n"
"\320\277\320\276\320\264\320\277\320\270\321\201\320\272\320\260 Success\n"
"Premium", nullptr));
        premiumBenefit1->setText(QCoreApplication::translate("DashboardWindow", "\342\200\242 1 \320\274\320\265\321\201\320\265\321\206 Premium \320\261\320\265\321\201\320\277\320\273\320\260\321\202\320\275\320\276", nullptr));
        premiumBenefit2->setText(QCoreApplication::translate("DashboardWindow", "\342\200\242 2 \320\274\320\265\321\201\321\217\321\206\320\260 \320\264\320\273\321\217 \321\203\321\207\320\265\320\275\320\270\320\272\320\276\320\262\n"
"  \320\270 \321\201\321\202\321\203\320\264\320\265\320\275\321\202\320\276\320\262 \320\261\320\265\321\201\320\277\320\273\320\260\321\202\320\275\320\276", nullptr));
        premiumBenefit3->setText(QCoreApplication::translate("DashboardWindow", "\342\200\242 \320\236\321\202\320\274\320\265\320\275\320\260 \320\262 \320\273\321\216\320\261\320\276\320\271 \320\274\320\276\320\274\320\265\320\275\321\202", nullptr));
        premiumBenefit4->setText(QCoreApplication::translate("DashboardWindow", "\342\200\242 \320\233\321\203\321\207\321\210\320\270\320\265 \320\260\320\272\321\206\320\270\320\270, \321\201\320\272\320\270\320\264\320\272\320\270\n"
"  \320\270 \320\277\321\200\320\265\320\264\320\273\320\276\320\266\320\265\320\275\320\270\321\217 \320\265\320\266\320\265\320\274\320\265\321\201\321\217\321\207\320\275\320\276", nullptr));
        subscribeButton->setText(QCoreApplication::translate("DashboardWindow", "\320\237\320\276\320\264\320\277\320\270\321\201\320\260\321\202\321\214\321\201\321\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DashboardWindow: public Ui_DashboardWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DASHBOARDWINDOW_H
