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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QFrame *frame;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QLineEdit *lineEdit;
    QStackedWidget *stackedWidget;
    QWidget *page_3;
    QWidget *page_7;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QLabel *label;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QComboBox *comboBox;
    QComboBox *comboBox_2;
    QLabel *label_9;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QLineEdit *lineEdit_5;
    QComboBox *comboBox_4;
    QLabel *label_2;
    QTextEdit *textEdit;
    QWidget *page_8;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1201, 569);
        MainWindow->setStyleSheet(QString::fromUtf8("QMainWindow {\n"
"    background: qlineargradient(\n"
"        x1:0, y1:0, x2:1, y2:1,\n"
"        stop:0   #030b1a,   /* bleu nuit tr\303\250s profond */\n"
"        stop:0.5 #1e3a8a,   /* bleu marine soutenu */\n"
"        stop:1   #6366f1    /* bleu \303\251lectrique/violet bleut\303\251 */\n"
"    );\n"
"}\n"
""));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        frame = new QFrame(centralwidget);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(-11, -1, 201, 721));
        frame->setStyleSheet(QString::fromUtf8("QFrame {\n"
"    background: qlineargradient(\n"
"        x1:0, y1:0, x2:1, y2:1,\n"
"        stop:0   #061a3a,   /* bleu nuit tr\303\250s fonc\303\251 */\n"
"        stop:0.5 #0a2f6f,   /* bleu marine sombre */\n"
"        stop:1   #8a3ebd    /* mauve intense en bas */\n"
"    );\n"
"    padding: 12px;   /* espace int\303\251rieur */\n"
"    border: none;    /* pas de bordure */\n"
"}\n"
""));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        pushButton = new QPushButton(frame);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(10, 110, 171, 51));
        pushButton->setStyleSheet(QString::fromUtf8("\n"
"QPushButton {\n"
"    background-color: transparent;\n"
"    color: #ffffff;\n"
"    font-family: \"Segoe UI\", Arial, sans-serif;\n"
"    font-size: 15px;\n"
"    font-weight: 500;\n"
"    padding: 12px 18px;\n"
"    border-radius: 10px;\n"
"    text-align: left;\n"
"    margin: 4px 12px;\n"
"    transition: all 0.2s;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: rgba(255,255,255,0.1);\n"
"    transform: translateY(-2px);\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: rgba(255,255,255,0.2);\n"
"}\n"
"\n"
"tab"));
        pushButton_2 = new QPushButton(frame);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(10, 160, 181, 51));
        pushButton_2->setStyleSheet(QString::fromUtf8("\n"
"QPushButton {\n"
"    background-color: transparent;\n"
"    color: #ffffff;\n"
"    font-family: \"Segoe UI\", Arial, sans-serif;\n"
"    font-size: 15px;\n"
"    font-weight: 500;\n"
"    padding: 12px 18px;\n"
"    border-radius: 10px;\n"
"    text-align: left;\n"
"    margin: 4px 12px;\n"
"    transition: all 0.2s;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: rgba(255,255,255,0.1);\n"
"    transform: translateY(-2px);\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: rgba(255,255,255,0.2);\n"
"}\n"
"\n"
"tab"));
        pushButton_3 = new QPushButton(frame);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(20, 210, 171, 51));
        pushButton_3->setStyleSheet(QString::fromUtf8("\n"
"QPushButton {\n"
"    background-color: transparent;\n"
"    color: #ffffff;\n"
"    font-family: \"Segoe UI\", Arial, sans-serif;\n"
"    font-size: 15px;\n"
"    font-weight: 500;\n"
"    padding: 12px 18px;\n"
"    border-radius: 10px;\n"
"    text-align: left;\n"
"    margin: 4px 12px;\n"
"    transition: all 0.2s;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: rgba(255,255,255,0.1);\n"
"    transform: translateY(-2px);\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: rgba(255,255,255,0.2);\n"
"}\n"
"\n"
"tab"));
        lineEdit = new QLineEdit(frame);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(40, 40, 113, 22));
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(190, 0, 1011, 571));
        stackedWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        page_3 = new QWidget();
        page_3->setObjectName("page_3");
        stackedWidget->addWidget(page_3);
        page_7 = new QWidget();
        page_7->setObjectName("page_7");
        pushButton_4 = new QPushButton(page_7);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setGeometry(QRect(70, 440, 191, 41));
        pushButton_4->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 170, 127);"));
        pushButton_5 = new QPushButton(page_7);
        pushButton_5->setObjectName("pushButton_5");
        pushButton_5->setGeometry(QRect(370, 440, 191, 41));
        pushButton_5->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 170, 0);"));
        pushButton_6 = new QPushButton(page_7);
        pushButton_6->setObjectName("pushButton_6");
        pushButton_6->setGeometry(QRect(670, 440, 201, 41));
        pushButton_6->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0);"));
        label = new QLabel(page_7);
        label->setObjectName("label");
        label->setGeometry(QRect(10, 130, 111, 31));
        label_5 = new QLabel(page_7);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(170, 320, 81, 16));
        label_6 = new QLabel(page_7);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(360, 260, 81, 16));
        label_7 = new QLabel(page_7);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(20, 250, 81, 16));
        label_8 = new QLabel(page_7);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(390, 190, 81, 16));
        comboBox = new QComboBox(page_7);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(110, 190, 181, 20));
        comboBox_2 = new QComboBox(page_7);
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->setObjectName("comboBox_2");
        comboBox_2->setGeometry(QRect(470, 190, 331, 22));
        label_9 = new QLabel(page_7);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(380, 140, 81, 16));
        lineEdit_2 = new QLineEdit(page_7);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(230, 320, 321, 22));
        lineEdit_3 = new QLineEdit(page_7);
        lineEdit_3->setObjectName("lineEdit_3");
        lineEdit_3->setGeometry(QRect(110, 139, 181, 21));
        lineEdit_4 = new QLineEdit(page_7);
        lineEdit_4->setObjectName("lineEdit_4");
        lineEdit_4->setGeometry(QRect(110, 251, 181, 21));
        lineEdit_5 = new QLineEdit(page_7);
        lineEdit_5->setObjectName("lineEdit_5");
        lineEdit_5->setGeometry(QRect(460, 251, 341, 31));
        comboBox_4 = new QComboBox(page_7);
        comboBox_4->addItem(QString());
        comboBox_4->addItem(QString());
        comboBox_4->addItem(QString());
        comboBox_4->setObjectName("comboBox_4");
        comboBox_4->setGeometry(QRect(470, 140, 331, 22));
        label_2 = new QLabel(page_7);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(20, 180, 51, 31));
        textEdit = new QTextEdit(page_7);
        textEdit->setObjectName("textEdit");
        textEdit->setGeometry(QRect(280, 20, 531, 71));
        stackedWidget->addWidget(page_7);
        page_8 = new QWidget();
        page_8->setObjectName("page_8");
        stackedWidget->addWidget(page_8);
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Ajouter v\303\251hicule", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "Liste de v\303\251hicules", nullptr));
        pushButton_3->setText(QCoreApplication::translate("MainWindow", "Statistiques", nullptr));
        lineEdit->setText(QCoreApplication::translate("MainWindow", "Gestion transport ", nullptr));
        pushButton_4->setText(QCoreApplication::translate("MainWindow", "Enregistrer ", nullptr));
        pushButton_5->setText(QCoreApplication::translate("MainWindow", "R\303\251nitialiser ", nullptr));
        pushButton_6->setText(QCoreApplication::translate("MainWindow", "R\303\251nitialiser ", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "ID v\303\251hicule ", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Capacit\303\251 ", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Horaire pr\303\251vu ", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Zone desservi", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Etat", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("MainWindow", "choisir entre", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Bus", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("MainWindow", "Taxi", nullptr));
        comboBox->setItemText(3, QCoreApplication::translate("MainWindow", "Voiture", nullptr));

        comboBox_2->setItemText(0, QCoreApplication::translate("MainWindow", "choisir entre", nullptr));
        comboBox_2->setItemText(1, QCoreApplication::translate("MainWindow", "reserv\303\251", nullptr));
        comboBox_2->setItemText(2, QCoreApplication::translate("MainWindow", "Disponible", nullptr));

        label_9->setText(QCoreApplication::translate("MainWindow", "Fonction", nullptr));
        comboBox_4->setItemText(0, QCoreApplication::translate("MainWindow", "choisir entre", nullptr));
        comboBox_4->setItemText(1, QCoreApplication::translate("MainWindow", "Livraison", nullptr));
        comboBox_4->setItemText(2, QCoreApplication::translate("MainWindow", "Deplacement", nullptr));

        label_2->setText(QCoreApplication::translate("MainWindow", "Type", nullptr));
        textEdit->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:28pt;\">Ajouter un nouveau v\303\251hicule </span></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
