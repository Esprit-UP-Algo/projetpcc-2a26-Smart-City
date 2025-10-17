/********************************************************************************
** Form generated from reading UI file 'settingspage.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGSPAGE_H
#define UI_SETTINGSPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingsPage
{
public:
    QVBoxLayout *mainLayout;
    QLabel *titleLabel;
    QHBoxLayout *contentLayout;
    QWidget *leftCardWidget;
    QVBoxLayout *leftCardLayout;
    QLabel *menuTitle;
    QPushButton *profileButton;
    QPushButton *themeButton;
    QPushButton *notificationsButton;
    QPushButton *securityButton;
    QPushButton *databaseButton;
    QPushButton *aboutButton;
    QSpacerItem *menuSpacer;
    QStackedWidget *settingsStackedWidget;
    QWidget *profilePage;
    QVBoxLayout *profilePageLayout;
    QWidget *profileCardWidget;
    QVBoxLayout *profileCardLayout;
    QLabel *profileTitle;
    QFormLayout *profileFormLayout;
    QLabel *usernameLabel;
    QLineEdit *usernameLineEdit;
    QLabel *roleLabel;
    QLineEdit *roleLineEdit;
    QLabel *emailLabel;
    QLineEdit *emailLineEdit;
    QLabel *phoneLabel;
    QLineEdit *phoneLineEdit;
    QHBoxLayout *profileButtonsLayout;
    QSpacerItem *profileButtonsSpacer;
    QPushButton *saveProfileButton;
    QSpacerItem *profilePageSpacer;
    QWidget *themePage;
    QVBoxLayout *themePageLayout;
    QWidget *themeCardWidget;
    QVBoxLayout *themeCardLayout;
    QLabel *themeTitle;
    QLabel *themeSubtitle;
    QHBoxLayout *themesLayout;
    QRadioButton *lightThemeRadio;
    QRadioButton *darkThemeRadio;
    QRadioButton *autoThemeRadio;
    QFrame *line1;
    QLabel *languageLabel;
    QComboBox *languageComboBox;
    QSpacerItem *themePageSpacer;
    QWidget *notificationsPage;
    QVBoxLayout *notificationsPageLayout;
    QWidget *notifCardWidget;
    QVBoxLayout *notifCardLayout;
    QLabel *notifTitle;
    QCheckBox *enableNotificationsCheckBox;
    QCheckBox *soundCheckBox;
    QCheckBox *emailNotifCheckBox;
    QFrame *line2;
    QLabel *notifTypesLabel;
    QCheckBox *residentsNotifCheckBox;
    QCheckBox *transactionsNotifCheckBox;
    QCheckBox *incidentsNotifCheckBox;
    QCheckBox *transportsNotifCheckBox;
    QSpacerItem *notifPageSpacer;
    QWidget *securityPage;
    QVBoxLayout *securityPageLayout;
    QWidget *securityCardWidget;
    QVBoxLayout *securityCardLayout;
    QLabel *securityTitle;
    QPushButton *changePasswordButton;
    QCheckBox *autoLogoutCheckBox;
    QCheckBox *twoFactorCheckBox;
    QFrame *line3;
    QLabel *sessionLabel;
    QLabel *sessionInfoLabel;
    QSpacerItem *securityPageSpacer;
    QWidget *databasePage;
    QVBoxLayout *databasePageLayout;
    QWidget *dbCardWidget;
    QVBoxLayout *dbCardLayout;
    QLabel *dbTitle;
    QLabel *dbInfoLabel;
    QLabel *dbStatsLabel;
    QHBoxLayout *dbButtonsLayout;
    QPushButton *backupButton;
    QPushButton *restoreButton;
    QPushButton *clearDataButton;
    QFrame *line4;
    QCheckBox *autoBackupCheckBox;
    QSpacerItem *dbPageSpacer;
    QWidget *aboutPage;
    QVBoxLayout *aboutPageLayout;
    QWidget *aboutCardWidget;
    QVBoxLayout *aboutCardLayout;
    QLabel *logoAboutLabel;
    QLabel *versionLabel;
    QFrame *line5;
    QLabel *descriptionLabel;
    QFrame *line6;
    QLabel *featuresLabel;
    QFrame *line7;
    QLabel *copyrightLabel;
    QSpacerItem *aboutPageSpacer;

    void setupUi(QWidget *SettingsPage)
    {
        if (SettingsPage->objectName().isEmpty())
            SettingsPage->setObjectName("SettingsPage");
        SettingsPage->resize(1200, 800);
        SettingsPage->setStyleSheet(QString::fromUtf8("background-color: #F5F7FA;"));
        mainLayout = new QVBoxLayout(SettingsPage);
        mainLayout->setSpacing(20);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(20, 20, 20, 20);
        titleLabel = new QLabel(SettingsPage);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setStyleSheet(QString::fromUtf8("font-size: 28px; font-weight: bold; color: #2C3E50;"));

        mainLayout->addWidget(titleLabel);

        contentLayout = new QHBoxLayout();
        contentLayout->setSpacing(20);
        contentLayout->setObjectName("contentLayout");
        leftCardWidget = new QWidget(SettingsPage);
        leftCardWidget->setObjectName("leftCardWidget");
        leftCardWidget->setMinimumSize(QSize(300, 0));
        leftCardWidget->setMaximumSize(QSize(300, 16777215));
        leftCardWidget->setStyleSheet(QString::fromUtf8("background-color: white; border-radius: 15px;"));
        leftCardLayout = new QVBoxLayout(leftCardWidget);
        leftCardLayout->setSpacing(10);
        leftCardLayout->setObjectName("leftCardLayout");
        leftCardLayout->setContentsMargins(15, 25, 15, 25);
        menuTitle = new QLabel(leftCardWidget);
        menuTitle->setObjectName("menuTitle");
        menuTitle->setStyleSheet(QString::fromUtf8("font-size: 16px; font-weight: bold; color: #7F8C8D; padding: 10px;"));

        leftCardLayout->addWidget(menuTitle);

        profileButton = new QPushButton(leftCardWidget);
        profileButton->setObjectName("profileButton");
        profileButton->setMinimumSize(QSize(0, 50));
        profileButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #4A90E2;\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 10px;\n"
"    padding: 10px;\n"
"    text-align: left;\n"
"    padding-left: 20px;\n"
"    font-size: 15px;\n"
"    font-weight: bold;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #3A7BC8;\n"
"}"));

        leftCardLayout->addWidget(profileButton);

        themeButton = new QPushButton(leftCardWidget);
        themeButton->setObjectName("themeButton");
        themeButton->setMinimumSize(QSize(0, 50));
        themeButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #ECF0F1;\n"
"    color: #2C3E50;\n"
"    border: none;\n"
"    border-radius: 10px;\n"
"    padding: 10px;\n"
"    text-align: left;\n"
"    padding-left: 20px;\n"
"    font-size: 15px;\n"
"    font-weight: bold;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #4A90E2;\n"
"    color: white;\n"
"}"));

        leftCardLayout->addWidget(themeButton);

        notificationsButton = new QPushButton(leftCardWidget);
        notificationsButton->setObjectName("notificationsButton");
        notificationsButton->setMinimumSize(QSize(0, 50));
        notificationsButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #ECF0F1;\n"
"    color: #2C3E50;\n"
"    border: none;\n"
"    border-radius: 10px;\n"
"    padding: 10px;\n"
"    text-align: left;\n"
"    padding-left: 20px;\n"
"    font-size: 15px;\n"
"    font-weight: bold;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #4A90E2;\n"
"    color: white;\n"
"}"));

        leftCardLayout->addWidget(notificationsButton);

        securityButton = new QPushButton(leftCardWidget);
        securityButton->setObjectName("securityButton");
        securityButton->setMinimumSize(QSize(0, 50));
        securityButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #ECF0F1;\n"
"    color: #2C3E50;\n"
"    border: none;\n"
"    border-radius: 10px;\n"
"    padding: 10px;\n"
"    text-align: left;\n"
"    padding-left: 20px;\n"
"    font-size: 15px;\n"
"    font-weight: bold;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #4A90E2;\n"
"    color: white;\n"
"}"));

        leftCardLayout->addWidget(securityButton);

        databaseButton = new QPushButton(leftCardWidget);
        databaseButton->setObjectName("databaseButton");
        databaseButton->setMinimumSize(QSize(0, 50));
        databaseButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #ECF0F1;\n"
"    color: #2C3E50;\n"
"    border: none;\n"
"    border-radius: 10px;\n"
"    padding: 10px;\n"
"    text-align: left;\n"
"    padding-left: 20px;\n"
"    font-size: 15px;\n"
"    font-weight: bold;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #4A90E2;\n"
"    color: white;\n"
"}"));

        leftCardLayout->addWidget(databaseButton);

        aboutButton = new QPushButton(leftCardWidget);
        aboutButton->setObjectName("aboutButton");
        aboutButton->setMinimumSize(QSize(0, 50));
        aboutButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #ECF0F1;\n"
"    color: #2C3E50;\n"
"    border: none;\n"
"    border-radius: 10px;\n"
"    padding: 10px;\n"
"    text-align: left;\n"
"    padding-left: 20px;\n"
"    font-size: 15px;\n"
"    font-weight: bold;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #4A90E2;\n"
"    color: white;\n"
"}"));

        leftCardLayout->addWidget(aboutButton);

        menuSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        leftCardLayout->addItem(menuSpacer);


        contentLayout->addWidget(leftCardWidget);

        settingsStackedWidget = new QStackedWidget(SettingsPage);
        settingsStackedWidget->setObjectName("settingsStackedWidget");
        profilePage = new QWidget();
        profilePage->setObjectName("profilePage");
        profilePageLayout = new QVBoxLayout(profilePage);
        profilePageLayout->setSpacing(20);
        profilePageLayout->setObjectName("profilePageLayout");
        profileCardWidget = new QWidget(profilePage);
        profileCardWidget->setObjectName("profileCardWidget");
        profileCardWidget->setStyleSheet(QString::fromUtf8("background-color: white; border-radius: 15px;"));
        profileCardLayout = new QVBoxLayout(profileCardWidget);
        profileCardLayout->setSpacing(20);
        profileCardLayout->setObjectName("profileCardLayout");
        profileCardLayout->setContentsMargins(30, 30, 30, 30);
        profileTitle = new QLabel(profileCardWidget);
        profileTitle->setObjectName("profileTitle");
        profileTitle->setStyleSheet(QString::fromUtf8("font-size: 22px; font-weight: bold; color: #2C3E50;"));

        profileCardLayout->addWidget(profileTitle);

        profileFormLayout = new QFormLayout();
        profileFormLayout->setObjectName("profileFormLayout");
        profileFormLayout->setHorizontalSpacing(20);
        profileFormLayout->setVerticalSpacing(15);
        usernameLabel = new QLabel(profileCardWidget);
        usernameLabel->setObjectName("usernameLabel");
        usernameLabel->setStyleSheet(QString::fromUtf8("font-size: 15px; font-weight: bold; color: #7F8C8D;"));

        profileFormLayout->setWidget(0, QFormLayout::LabelRole, usernameLabel);

        usernameLineEdit = new QLineEdit(profileCardWidget);
        usernameLineEdit->setObjectName("usernameLineEdit");
        usernameLineEdit->setReadOnly(true);
        usernameLineEdit->setStyleSheet(QString::fromUtf8("padding: 10px; border: 2px solid #E0E0E0; border-radius: 8px; font-size: 14px; background-color: #F9F9F9;"));

        profileFormLayout->setWidget(0, QFormLayout::FieldRole, usernameLineEdit);

        roleLabel = new QLabel(profileCardWidget);
        roleLabel->setObjectName("roleLabel");
        roleLabel->setStyleSheet(QString::fromUtf8("font-size: 15px; font-weight: bold; color: #7F8C8D;"));

        profileFormLayout->setWidget(1, QFormLayout::LabelRole, roleLabel);

        roleLineEdit = new QLineEdit(profileCardWidget);
        roleLineEdit->setObjectName("roleLineEdit");
        roleLineEdit->setReadOnly(true);
        roleLineEdit->setStyleSheet(QString::fromUtf8("padding: 10px; border: 2px solid #E0E0E0; border-radius: 8px; font-size: 14px; background-color: #F9F9F9;"));

        profileFormLayout->setWidget(1, QFormLayout::FieldRole, roleLineEdit);

        emailLabel = new QLabel(profileCardWidget);
        emailLabel->setObjectName("emailLabel");
        emailLabel->setStyleSheet(QString::fromUtf8("font-size: 15px; font-weight: bold; color: #7F8C8D;"));

        profileFormLayout->setWidget(2, QFormLayout::LabelRole, emailLabel);

        emailLineEdit = new QLineEdit(profileCardWidget);
        emailLineEdit->setObjectName("emailLineEdit");
        emailLineEdit->setStyleSheet(QString::fromUtf8("padding: 10px; border: 2px solid #E0E0E0; border-radius: 8px; font-size: 14px;"));

        profileFormLayout->setWidget(2, QFormLayout::FieldRole, emailLineEdit);

        phoneLabel = new QLabel(profileCardWidget);
        phoneLabel->setObjectName("phoneLabel");
        phoneLabel->setStyleSheet(QString::fromUtf8("font-size: 15px; font-weight: bold; color: #7F8C8D;"));

        profileFormLayout->setWidget(3, QFormLayout::LabelRole, phoneLabel);

        phoneLineEdit = new QLineEdit(profileCardWidget);
        phoneLineEdit->setObjectName("phoneLineEdit");
        phoneLineEdit->setStyleSheet(QString::fromUtf8("padding: 10px; border: 2px solid #E0E0E0; border-radius: 8px; font-size: 14px;"));

        profileFormLayout->setWidget(3, QFormLayout::FieldRole, phoneLineEdit);


        profileCardLayout->addLayout(profileFormLayout);

        profileButtonsLayout = new QHBoxLayout();
        profileButtonsLayout->setObjectName("profileButtonsLayout");
        profileButtonsSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        profileButtonsLayout->addItem(profileButtonsSpacer);

        saveProfileButton = new QPushButton(profileCardWidget);
        saveProfileButton->setObjectName("saveProfileButton");
        saveProfileButton->setMinimumSize(QSize(0, 45));
        saveProfileButton->setStyleSheet(QString::fromUtf8("background-color: #27AE60; color: white; border: none; border-radius: 10px; padding: 0 30px; font-size: 15px; font-weight: bold;"));

        profileButtonsLayout->addWidget(saveProfileButton);


        profileCardLayout->addLayout(profileButtonsLayout);


        profilePageLayout->addWidget(profileCardWidget);

        profilePageSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        profilePageLayout->addItem(profilePageSpacer);

        settingsStackedWidget->addWidget(profilePage);
        themePage = new QWidget();
        themePage->setObjectName("themePage");
        themePageLayout = new QVBoxLayout(themePage);
        themePageLayout->setObjectName("themePageLayout");
        themeCardWidget = new QWidget(themePage);
        themeCardWidget->setObjectName("themeCardWidget");
        themeCardWidget->setStyleSheet(QString::fromUtf8("background-color: white; border-radius: 15px;"));
        themeCardLayout = new QVBoxLayout(themeCardWidget);
        themeCardLayout->setSpacing(20);
        themeCardLayout->setObjectName("themeCardLayout");
        themeCardLayout->setContentsMargins(30, 30, 30, 30);
        themeTitle = new QLabel(themeCardWidget);
        themeTitle->setObjectName("themeTitle");
        themeTitle->setStyleSheet(QString::fromUtf8("font-size: 22px; font-weight: bold; color: #2C3E50;"));

        themeCardLayout->addWidget(themeTitle);

        themeSubtitle = new QLabel(themeCardWidget);
        themeSubtitle->setObjectName("themeSubtitle");
        themeSubtitle->setStyleSheet(QString::fromUtf8("font-size: 14px; color: #7F8C8D;"));

        themeCardLayout->addWidget(themeSubtitle);

        themesLayout = new QHBoxLayout();
        themesLayout->setSpacing(20);
        themesLayout->setObjectName("themesLayout");
        lightThemeRadio = new QRadioButton(themeCardWidget);
        lightThemeRadio->setObjectName("lightThemeRadio");
        lightThemeRadio->setChecked(true);
        lightThemeRadio->setStyleSheet(QString::fromUtf8("font-size: 15px; padding: 15px;"));

        themesLayout->addWidget(lightThemeRadio);

        darkThemeRadio = new QRadioButton(themeCardWidget);
        darkThemeRadio->setObjectName("darkThemeRadio");
        darkThemeRadio->setStyleSheet(QString::fromUtf8("font-size: 15px; padding: 15px;"));

        themesLayout->addWidget(darkThemeRadio);

        autoThemeRadio = new QRadioButton(themeCardWidget);
        autoThemeRadio->setObjectName("autoThemeRadio");
        autoThemeRadio->setStyleSheet(QString::fromUtf8("font-size: 15px; padding: 15px;"));

        themesLayout->addWidget(autoThemeRadio);


        themeCardLayout->addLayout(themesLayout);

        line1 = new QFrame(themeCardWidget);
        line1->setObjectName("line1");
        line1->setFrameShape(QFrame::Shape::HLine);
        line1->setFrameShadow(QFrame::Shadow::Sunken);

        themeCardLayout->addWidget(line1);

        languageLabel = new QLabel(themeCardWidget);
        languageLabel->setObjectName("languageLabel");
        languageLabel->setStyleSheet(QString::fromUtf8("font-size: 16px; font-weight: bold; color: #2C3E50;"));

        themeCardLayout->addWidget(languageLabel);

        languageComboBox = new QComboBox(themeCardWidget);
        languageComboBox->addItem(QString());
        languageComboBox->addItem(QString());
        languageComboBox->addItem(QString());
        languageComboBox->setObjectName("languageComboBox");
        languageComboBox->setMinimumSize(QSize(200, 45));
        languageComboBox->setStyleSheet(QString::fromUtf8("padding: 10px; border: 2px solid #E0E0E0; border-radius: 8px; font-size: 14px;"));

        themeCardLayout->addWidget(languageComboBox);


        themePageLayout->addWidget(themeCardWidget);

        themePageSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        themePageLayout->addItem(themePageSpacer);

        settingsStackedWidget->addWidget(themePage);
        notificationsPage = new QWidget();
        notificationsPage->setObjectName("notificationsPage");
        notificationsPageLayout = new QVBoxLayout(notificationsPage);
        notificationsPageLayout->setObjectName("notificationsPageLayout");
        notifCardWidget = new QWidget(notificationsPage);
        notifCardWidget->setObjectName("notifCardWidget");
        notifCardWidget->setStyleSheet(QString::fromUtf8("background-color: white; border-radius: 15px;"));
        notifCardLayout = new QVBoxLayout(notifCardWidget);
        notifCardLayout->setSpacing(20);
        notifCardLayout->setObjectName("notifCardLayout");
        notifCardLayout->setContentsMargins(30, 30, 30, 30);
        notifTitle = new QLabel(notifCardWidget);
        notifTitle->setObjectName("notifTitle");
        notifTitle->setStyleSheet(QString::fromUtf8("font-size: 22px; font-weight: bold; color: #2C3E50;"));

        notifCardLayout->addWidget(notifTitle);

        enableNotificationsCheckBox = new QCheckBox(notifCardWidget);
        enableNotificationsCheckBox->setObjectName("enableNotificationsCheckBox");
        enableNotificationsCheckBox->setChecked(true);
        enableNotificationsCheckBox->setStyleSheet(QString::fromUtf8("font-size: 15px; padding: 10px;"));

        notifCardLayout->addWidget(enableNotificationsCheckBox);

        soundCheckBox = new QCheckBox(notifCardWidget);
        soundCheckBox->setObjectName("soundCheckBox");
        soundCheckBox->setChecked(true);
        soundCheckBox->setStyleSheet(QString::fromUtf8("font-size: 15px; padding: 10px;"));

        notifCardLayout->addWidget(soundCheckBox);

        emailNotifCheckBox = new QCheckBox(notifCardWidget);
        emailNotifCheckBox->setObjectName("emailNotifCheckBox");
        emailNotifCheckBox->setStyleSheet(QString::fromUtf8("font-size: 15px; padding: 10px;"));

        notifCardLayout->addWidget(emailNotifCheckBox);

        line2 = new QFrame(notifCardWidget);
        line2->setObjectName("line2");
        line2->setFrameShape(QFrame::Shape::HLine);
        line2->setFrameShadow(QFrame::Shadow::Sunken);

        notifCardLayout->addWidget(line2);

        notifTypesLabel = new QLabel(notifCardWidget);
        notifTypesLabel->setObjectName("notifTypesLabel");
        notifTypesLabel->setStyleSheet(QString::fromUtf8("font-size: 16px; font-weight: bold; color: #2C3E50;"));

        notifCardLayout->addWidget(notifTypesLabel);

        residentsNotifCheckBox = new QCheckBox(notifCardWidget);
        residentsNotifCheckBox->setObjectName("residentsNotifCheckBox");
        residentsNotifCheckBox->setChecked(true);
        residentsNotifCheckBox->setStyleSheet(QString::fromUtf8("font-size: 14px; padding: 8px;"));

        notifCardLayout->addWidget(residentsNotifCheckBox);

        transactionsNotifCheckBox = new QCheckBox(notifCardWidget);
        transactionsNotifCheckBox->setObjectName("transactionsNotifCheckBox");
        transactionsNotifCheckBox->setChecked(true);
        transactionsNotifCheckBox->setStyleSheet(QString::fromUtf8("font-size: 14px; padding: 8px;"));

        notifCardLayout->addWidget(transactionsNotifCheckBox);

        incidentsNotifCheckBox = new QCheckBox(notifCardWidget);
        incidentsNotifCheckBox->setObjectName("incidentsNotifCheckBox");
        incidentsNotifCheckBox->setChecked(true);
        incidentsNotifCheckBox->setStyleSheet(QString::fromUtf8("font-size: 14px; padding: 8px;"));

        notifCardLayout->addWidget(incidentsNotifCheckBox);

        transportsNotifCheckBox = new QCheckBox(notifCardWidget);
        transportsNotifCheckBox->setObjectName("transportsNotifCheckBox");
        transportsNotifCheckBox->setChecked(true);
        transportsNotifCheckBox->setStyleSheet(QString::fromUtf8("font-size: 14px; padding: 8px;"));

        notifCardLayout->addWidget(transportsNotifCheckBox);


        notificationsPageLayout->addWidget(notifCardWidget);

        notifPageSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        notificationsPageLayout->addItem(notifPageSpacer);

        settingsStackedWidget->addWidget(notificationsPage);
        securityPage = new QWidget();
        securityPage->setObjectName("securityPage");
        securityPageLayout = new QVBoxLayout(securityPage);
        securityPageLayout->setObjectName("securityPageLayout");
        securityCardWidget = new QWidget(securityPage);
        securityCardWidget->setObjectName("securityCardWidget");
        securityCardWidget->setStyleSheet(QString::fromUtf8("background-color: white; border-radius: 15px;"));
        securityCardLayout = new QVBoxLayout(securityCardWidget);
        securityCardLayout->setSpacing(20);
        securityCardLayout->setObjectName("securityCardLayout");
        securityCardLayout->setContentsMargins(30, 30, 30, 30);
        securityTitle = new QLabel(securityCardWidget);
        securityTitle->setObjectName("securityTitle");
        securityTitle->setStyleSheet(QString::fromUtf8("font-size: 22px; font-weight: bold; color: #2C3E50;"));

        securityCardLayout->addWidget(securityTitle);

        changePasswordButton = new QPushButton(securityCardWidget);
        changePasswordButton->setObjectName("changePasswordButton");
        changePasswordButton->setMinimumSize(QSize(0, 50));
        changePasswordButton->setStyleSheet(QString::fromUtf8("background-color: #3498DB; color: white; border: none; border-radius: 10px; padding: 0 20px; font-size: 15px; font-weight: bold; text-align: left;"));

        securityCardLayout->addWidget(changePasswordButton);

        autoLogoutCheckBox = new QCheckBox(securityCardWidget);
        autoLogoutCheckBox->setObjectName("autoLogoutCheckBox");
        autoLogoutCheckBox->setStyleSheet(QString::fromUtf8("font-size: 14px; padding: 10px;"));

        securityCardLayout->addWidget(autoLogoutCheckBox);

        twoFactorCheckBox = new QCheckBox(securityCardWidget);
        twoFactorCheckBox->setObjectName("twoFactorCheckBox");
        twoFactorCheckBox->setStyleSheet(QString::fromUtf8("font-size: 14px; padding: 10px;"));

        securityCardLayout->addWidget(twoFactorCheckBox);

        line3 = new QFrame(securityCardWidget);
        line3->setObjectName("line3");
        line3->setFrameShape(QFrame::Shape::HLine);
        line3->setFrameShadow(QFrame::Shadow::Sunken);

        securityCardLayout->addWidget(line3);

        sessionLabel = new QLabel(securityCardWidget);
        sessionLabel->setObjectName("sessionLabel");
        sessionLabel->setStyleSheet(QString::fromUtf8("font-size: 16px; font-weight: bold; color: #2C3E50;"));

        securityCardLayout->addWidget(sessionLabel);

        sessionInfoLabel = new QLabel(securityCardWidget);
        sessionInfoLabel->setObjectName("sessionInfoLabel");
        sessionInfoLabel->setStyleSheet(QString::fromUtf8("font-size: 14px; color: #7F8C8D; padding: 10px; background-color: #F9F9F9; border-radius: 8px;"));

        securityCardLayout->addWidget(sessionInfoLabel);


        securityPageLayout->addWidget(securityCardWidget);

        securityPageSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        securityPageLayout->addItem(securityPageSpacer);

        settingsStackedWidget->addWidget(securityPage);
        databasePage = new QWidget();
        databasePage->setObjectName("databasePage");
        databasePageLayout = new QVBoxLayout(databasePage);
        databasePageLayout->setObjectName("databasePageLayout");
        dbCardWidget = new QWidget(databasePage);
        dbCardWidget->setObjectName("dbCardWidget");
        dbCardWidget->setStyleSheet(QString::fromUtf8("background-color: white; border-radius: 15px;"));
        dbCardLayout = new QVBoxLayout(dbCardWidget);
        dbCardLayout->setSpacing(20);
        dbCardLayout->setObjectName("dbCardLayout");
        dbCardLayout->setContentsMargins(30, 30, 30, 30);
        dbTitle = new QLabel(dbCardWidget);
        dbTitle->setObjectName("dbTitle");
        dbTitle->setStyleSheet(QString::fromUtf8("font-size: 22px; font-weight: bold; color: #2C3E50;"));

        dbCardLayout->addWidget(dbTitle);

        dbInfoLabel = new QLabel(dbCardWidget);
        dbInfoLabel->setObjectName("dbInfoLabel");
        dbInfoLabel->setStyleSheet(QString::fromUtf8("font-size: 16px; font-weight: bold; color: #2C3E50;"));

        dbCardLayout->addWidget(dbInfoLabel);

        dbStatsLabel = new QLabel(dbCardWidget);
        dbStatsLabel->setObjectName("dbStatsLabel");
        dbStatsLabel->setStyleSheet(QString::fromUtf8("font-size: 14px; color: #7F8C8D; padding: 15px; background-color: #F9F9F9; border-radius: 8px;"));

        dbCardLayout->addWidget(dbStatsLabel);

        dbButtonsLayout = new QHBoxLayout();
        dbButtonsLayout->setSpacing(15);
        dbButtonsLayout->setObjectName("dbButtonsLayout");
        backupButton = new QPushButton(dbCardWidget);
        backupButton->setObjectName("backupButton");
        backupButton->setMinimumSize(QSize(0, 50));
        backupButton->setStyleSheet(QString::fromUtf8("background-color: #27AE60; color: white; border: none; border-radius: 10px; padding: 0 20px; font-size: 15px; font-weight: bold;"));

        dbButtonsLayout->addWidget(backupButton);

        restoreButton = new QPushButton(dbCardWidget);
        restoreButton->setObjectName("restoreButton");
        restoreButton->setMinimumSize(QSize(0, 50));
        restoreButton->setStyleSheet(QString::fromUtf8("background-color: #3498DB; color: white; border: none; border-radius: 10px; padding: 0 20px; font-size: 15px; font-weight: bold;"));

        dbButtonsLayout->addWidget(restoreButton);

        clearDataButton = new QPushButton(dbCardWidget);
        clearDataButton->setObjectName("clearDataButton");
        clearDataButton->setMinimumSize(QSize(0, 50));
        clearDataButton->setStyleSheet(QString::fromUtf8("background-color: #E74C3C; color: white; border: none; border-radius: 10px; padding: 0 20px; font-size: 15px; font-weight: bold;"));

        dbButtonsLayout->addWidget(clearDataButton);


        dbCardLayout->addLayout(dbButtonsLayout);

        line4 = new QFrame(dbCardWidget);
        line4->setObjectName("line4");
        line4->setFrameShape(QFrame::Shape::HLine);
        line4->setFrameShadow(QFrame::Shadow::Sunken);

        dbCardLayout->addWidget(line4);

        autoBackupCheckBox = new QCheckBox(dbCardWidget);
        autoBackupCheckBox->setObjectName("autoBackupCheckBox");
        autoBackupCheckBox->setStyleSheet(QString::fromUtf8("font-size: 14px; padding: 10px;"));

        dbCardLayout->addWidget(autoBackupCheckBox);


        databasePageLayout->addWidget(dbCardWidget);

        dbPageSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        databasePageLayout->addItem(dbPageSpacer);

        settingsStackedWidget->addWidget(databasePage);
        aboutPage = new QWidget();
        aboutPage->setObjectName("aboutPage");
        aboutPageLayout = new QVBoxLayout(aboutPage);
        aboutPageLayout->setObjectName("aboutPageLayout");
        aboutCardWidget = new QWidget(aboutPage);
        aboutCardWidget->setObjectName("aboutCardWidget");
        aboutCardWidget->setStyleSheet(QString::fromUtf8("background-color: white; border-radius: 15px;"));
        aboutCardLayout = new QVBoxLayout(aboutCardWidget);
        aboutCardLayout->setSpacing(20);
        aboutCardLayout->setObjectName("aboutCardLayout");
        aboutCardLayout->setContentsMargins(30, 30, 30, 30);
        logoAboutLabel = new QLabel(aboutCardWidget);
        logoAboutLabel->setObjectName("logoAboutLabel");
        logoAboutLabel->setStyleSheet(QString::fromUtf8("font-size: 48px; font-weight: bold; color: #4A90E2;"));

        aboutCardLayout->addWidget(logoAboutLabel, 0, Qt::AlignHCenter);

        versionLabel = new QLabel(aboutCardWidget);
        versionLabel->setObjectName("versionLabel");
        versionLabel->setStyleSheet(QString::fromUtf8("font-size: 18px; color: #7F8C8D;"));

        aboutCardLayout->addWidget(versionLabel, 0, Qt::AlignHCenter);

        line5 = new QFrame(aboutCardWidget);
        line5->setObjectName("line5");
        line5->setFrameShape(QFrame::Shape::HLine);
        line5->setFrameShadow(QFrame::Shadow::Sunken);

        aboutCardLayout->addWidget(line5);

        descriptionLabel = new QLabel(aboutCardWidget);
        descriptionLabel->setObjectName("descriptionLabel");
        descriptionLabel->setWordWrap(true);

        aboutCardLayout->addWidget(descriptionLabel);

        line6 = new QFrame(aboutCardWidget);
        line6->setObjectName("line6");
        line6->setFrameShape(QFrame::Shape::HLine);
        line6->setFrameShadow(QFrame::Shadow::Sunken);

        aboutCardLayout->addWidget(line6);

        featuresLabel = new QLabel(aboutCardWidget);
        featuresLabel->setObjectName("featuresLabel");
        featuresLabel->setStyleSheet(QString::fromUtf8("font-size: 14px; color: #2C3E50; padding: 15px; background-color: #F9F9F9; border-radius: 8px;"));

        aboutCardLayout->addWidget(featuresLabel);

        line7 = new QFrame(aboutCardWidget);
        line7->setObjectName("line7");
        line7->setFrameShape(QFrame::Shape::HLine);
        line7->setFrameShadow(QFrame::Shadow::Sunken);

        aboutCardLayout->addWidget(line7);

        copyrightLabel = new QLabel(aboutCardWidget);
        copyrightLabel->setObjectName("copyrightLabel");
        copyrightLabel->setStyleSheet(QString::fromUtf8("font-size: 12px; color: #95A5A6;"));

        aboutCardLayout->addWidget(copyrightLabel, 0, Qt::AlignHCenter);


        aboutPageLayout->addWidget(aboutCardWidget);

        aboutPageSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        aboutPageLayout->addItem(aboutPageSpacer);

        settingsStackedWidget->addWidget(aboutPage);

        contentLayout->addWidget(settingsStackedWidget);


        mainLayout->addLayout(contentLayout);


        retranslateUi(SettingsPage);

        settingsStackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SettingsPage);
    } // setupUi

    void retranslateUi(QWidget *SettingsPage)
    {
        SettingsPage->setWindowTitle(QCoreApplication::translate("SettingsPage", "Param\303\250tres", nullptr));
        titleLabel->setText(QCoreApplication::translate("SettingsPage", "\342\232\231\357\270\217 Param\303\250tres", nullptr));
        menuTitle->setText(QCoreApplication::translate("SettingsPage", "Menu", nullptr));
        profileButton->setText(QCoreApplication::translate("SettingsPage", "\360\237\221\244 Profil", nullptr));
        themeButton->setText(QCoreApplication::translate("SettingsPage", "\360\237\216\250 Apparence", nullptr));
        notificationsButton->setText(QCoreApplication::translate("SettingsPage", "\360\237\224\224 Notifications", nullptr));
        securityButton->setText(QCoreApplication::translate("SettingsPage", "\360\237\224\220 S\303\251curit\303\251", nullptr));
        databaseButton->setText(QCoreApplication::translate("SettingsPage", "\360\237\223\212 Base de donn\303\251es", nullptr));
        aboutButton->setText(QCoreApplication::translate("SettingsPage", "\342\204\271\357\270\217 \303\200 propos", nullptr));
        profileTitle->setText(QCoreApplication::translate("SettingsPage", "\360\237\221\244 Informations du Profil", nullptr));
        usernameLabel->setText(QCoreApplication::translate("SettingsPage", "Nom d'utilisateur:", nullptr));
        usernameLineEdit->setText(QCoreApplication::translate("SettingsPage", "smart city", nullptr));
        roleLabel->setText(QCoreApplication::translate("SettingsPage", "R\303\264le:", nullptr));
        roleLineEdit->setText(QCoreApplication::translate("SettingsPage", "Administrateur", nullptr));
        emailLabel->setText(QCoreApplication::translate("SettingsPage", "Email:", nullptr));
        emailLineEdit->setText(QCoreApplication::translate("SettingsPage", "admin@nexora.tn", nullptr));
        phoneLabel->setText(QCoreApplication::translate("SettingsPage", "T\303\251l\303\251phone:", nullptr));
        phoneLineEdit->setText(QCoreApplication::translate("SettingsPage", "+216 XX XXX XXX", nullptr));
        saveProfileButton->setText(QCoreApplication::translate("SettingsPage", "\360\237\222\276 Enregistrer", nullptr));
        themeTitle->setText(QCoreApplication::translate("SettingsPage", "\360\237\216\250 Personnalisation de l'Apparence", nullptr));
        themeSubtitle->setText(QCoreApplication::translate("SettingsPage", "Choisissez votre th\303\250me pr\303\251f\303\251r\303\251", nullptr));
        lightThemeRadio->setText(QCoreApplication::translate("SettingsPage", "\342\230\200\357\270\217 Clair (Actuel)", nullptr));
        darkThemeRadio->setText(QCoreApplication::translate("SettingsPage", "\360\237\214\231 Sombre", nullptr));
        autoThemeRadio->setText(QCoreApplication::translate("SettingsPage", "\360\237\224\204 Automatique", nullptr));
        languageLabel->setText(QCoreApplication::translate("SettingsPage", "Langue:", nullptr));
        languageComboBox->setItemText(0, QCoreApplication::translate("SettingsPage", "\360\237\207\253\360\237\207\267 Fran\303\247ais", nullptr));
        languageComboBox->setItemText(1, QCoreApplication::translate("SettingsPage", "\360\237\207\254\360\237\207\247 English", nullptr));
        languageComboBox->setItemText(2, QCoreApplication::translate("SettingsPage", "\360\237\207\271\360\237\207\263 \330\247\331\204\330\271\330\261\330\250\331\212\330\251", nullptr));

        notifTitle->setText(QCoreApplication::translate("SettingsPage", "\360\237\224\224 Param\303\250tres de Notifications", nullptr));
        enableNotificationsCheckBox->setText(QCoreApplication::translate("SettingsPage", "Activer les notifications", nullptr));
        soundCheckBox->setText(QCoreApplication::translate("SettingsPage", "\360\237\224\212 Son des notifications", nullptr));
        emailNotifCheckBox->setText(QCoreApplication::translate("SettingsPage", "\360\237\223\247 Notifications par email", nullptr));
        notifTypesLabel->setText(QCoreApplication::translate("SettingsPage", "Types de notifications:", nullptr));
        residentsNotifCheckBox->setText(QCoreApplication::translate("SettingsPage", "\360\237\221\245 Nouveaux r\303\251sidents", nullptr));
        transactionsNotifCheckBox->setText(QCoreApplication::translate("SettingsPage", "\360\237\222\260 Transactions importantes", nullptr));
        incidentsNotifCheckBox->setText(QCoreApplication::translate("SettingsPage", "\360\237\232\250 Incidents critiques", nullptr));
        transportsNotifCheckBox->setText(QCoreApplication::translate("SettingsPage", "\360\237\232\227 Maintenance v\303\251hicules", nullptr));
        securityTitle->setText(QCoreApplication::translate("SettingsPage", "\360\237\224\220 S\303\251curit\303\251 et Confidentialit\303\251", nullptr));
        changePasswordButton->setText(QCoreApplication::translate("SettingsPage", "\360\237\224\221 Changer le mot de passe", nullptr));
        autoLogoutCheckBox->setText(QCoreApplication::translate("SettingsPage", "\342\217\261\357\270\217 D\303\251connexion automatique (30 min d'inactivit\303\251)", nullptr));
        twoFactorCheckBox->setText(QCoreApplication::translate("SettingsPage", "\360\237\233\241\357\270\217 Authentification \303\240 deux facteurs", nullptr));
        sessionLabel->setText(QCoreApplication::translate("SettingsPage", "Session actuelle:", nullptr));
        sessionInfoLabel->setText(QCoreApplication::translate("SettingsPage", "\342\200\242 Connect\303\251 depuis: Aujourd'hui \303\240 09:30\n"
"\342\200\242 Adresse IP: 192.168.1.100\n"
"\342\200\242 Appareil: Windows 10 Desktop", nullptr));
        dbTitle->setText(QCoreApplication::translate("SettingsPage", "\360\237\223\212 Gestion de la Base de Donn\303\251es", nullptr));
        dbInfoLabel->setText(QCoreApplication::translate("SettingsPage", "Informations sur la base de donn\303\251es:", nullptr));
        dbStatsLabel->setText(QCoreApplication::translate("SettingsPage", "\342\200\242 Type: SQLite\n"
"\342\200\242 Emplacement: nexora.db\n"
"\342\200\242 Taille: 2.5 MB\n"
"\342\200\242 Derni\303\250re sauvegarde: Jamais", nullptr));
        backupButton->setText(QCoreApplication::translate("SettingsPage", "\360\237\222\276 Sauvegarder", nullptr));
        restoreButton->setText(QCoreApplication::translate("SettingsPage", "\360\237\223\202 Restaurer", nullptr));
        clearDataButton->setText(QCoreApplication::translate("SettingsPage", "\360\237\227\221\357\270\217 Vider les donn\303\251es", nullptr));
        autoBackupCheckBox->setText(QCoreApplication::translate("SettingsPage", "\360\237\224\204 Sauvegarde automatique quotidienne", nullptr));
        logoAboutLabel->setText(QCoreApplication::translate("SettingsPage", "\360\237\217\231\357\270\217 NEXORA", nullptr));
        versionLabel->setText(QCoreApplication::translate("SettingsPage", "Version 1.0.0", nullptr));
        descriptionLabel->setText(QCoreApplication::translate("SettingsPage", "<html><head/><body><p align=\"center\"><span style=\" font-size:16px;\">Plateforme de gestion intelligente pour Smart Cities</span></p><p align=\"center\"><span style=\" font-size:14px; color:#7f8c8d;\">G\303\251rez efficacement vos r\303\251sidents, finances, incidents et transports</span></p></body></html>", nullptr));
        featuresLabel->setText(QCoreApplication::translate("SettingsPage", "Fonctionnalit\303\251s:\n"
"\n"
"\360\237\221\245 Gestion des r\303\251sidents avec recherche avanc\303\251e\n"
"\360\237\222\260 Suivi des transactions financi\303\250res\n"
"\360\237\232\250 D\303\251claration et suivi des incidents\n"
"\360\237\232\227 Gestion de flotte de v\303\251hicules\n"
"\360\237\223\212 Statistiques en temps r\303\251el\n"
"\360\237\223\204 Exports PDF et CSV", nullptr));
        copyrightLabel->setText(QCoreApplication::translate("SettingsPage", "\302\251 2025 NEXORA. Tous droits r\303\251serv\303\251s.", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SettingsPage: public Ui_SettingsPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSPAGE_H
