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
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QFrame *frame;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *label_6;
    QComboBox *comboBox;
    QLineEdit *lineEdit;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QLabel *label_3;
    QLabel *label_5;
    QPushButton *pushButton_6;
    QLabel *label_8;
    QLabel *label_9;
    QComboBox *comboBox_2;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QLineEdit *lineEdit_5;
    QLineEdit *lineEdit_6;
    QFrame *frame_2;
    QLabel *label_7;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QLineEdit *lineEdit_7;
    QPushButton *pushButton_7;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1314, 669);
        MainWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"background-color: rgb(144, 216, 216);"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        frame = new QFrame(centralwidget);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(390, 80, 871, 521));
        frame->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border-radius:15px;\n"
""));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        label = new QLabel(frame);
        label->setObjectName("label");
        label->setGeometry(QRect(40, 40, 49, 16));
        label_2 = new QLabel(frame);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(40, 90, 61, 16));
        label_4 = new QLabel(frame);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(30, 180, 101, 16));
        label_6 = new QLabel(frame);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(50, 140, 101, 16));
        comboBox = new QComboBox(frame);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(170, 180, 121, 22));
        lineEdit = new QLineEdit(frame);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(150, 30, 151, 31));
        lineEdit->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    font-size: 12px;\n"
"    font-weight: bold;\n"
"    color: #b5b5b5;\n"
"    background-color: white;\n"
"    border: 2px solid #1e4fa3;\n"
"    border-radius: 10px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QLineEdit:focus {\n"
"    border: 2px solid #4a6eff;\n"
"}\n"
"\n"
"QLineEdit:placeholder {\n"
"    color: #a0a0a0;\n"
"    font-style: italic;\n"
"    font-size: 7px;        /* Taille plus petite */\n"
"    font-weight: normal;    /* Enlever le gras */\n"
"}"));
        pushButton_4 = new QPushButton(frame);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setGeometry(QRect(60, 440, 101, 24));
        pushButton_4->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    font-size: 15px;\n"
"    font-weight: bold;\n"
"    color: white;                 /* texte blanc */\n"
"    background-color: orange;     /* fond orange */\n"
"    border: none;                 /* pas de bordure */\n"
"    border-radius: 10px;          /* coins arrondis */\n"
"    padding: 5px 15px;            /* espace autour du texte */\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #ff9900;    /* orange plus clair au survol */\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #e68a00;    /* orange plus fonc\303\251 au clic */\n"
"}\n"
""));
        pushButton_5 = new QPushButton(frame);
        pushButton_5->setObjectName("pushButton_5");
        pushButton_5->setGeometry(QRect(230, 440, 101, 24));
        pushButton_5->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    font-size: 15px;\n"
"    font-weight: bold;\n"
"    color: white;                 /* texte blanc */\n"
"    background-color: orange;     /* fond orange */\n"
"    border: none;                 /* pas de bordure */\n"
"    border-radius: 10px;          /* coins arrondis */\n"
"    padding: 5px 15px;            /* espace autour du texte */\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #ff9900;    /* orange plus clair au survol */\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #e68a00;    /* orange plus fonc\303\251 au clic */\n"
"}\n"
""));
        label_3 = new QLabel(frame);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(40, 230, 49, 16));
        label_5 = new QLabel(frame);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(30, 280, 91, 16));
        pushButton_6 = new QPushButton(frame);
        pushButton_6->setObjectName("pushButton_6");
        pushButton_6->setGeometry(QRect(450, 440, 121, 24));
        pushButton_6->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    font-size: 15px;\n"
"    font-weight: bold;\n"
"    color: white;                 /* texte blanc */\n"
"    background-color: orange;     /* fond orange */\n"
"    border: none;                 /* pas de bordure */\n"
"    border-radius: 10px;          /* coins arrondis */\n"
"    padding: 5px 15px;            /* espace autour du texte */\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #ff9900;    /* orange plus clair au survol */\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #e68a00;    /* orange plus fonc\303\251 au clic */\n"
"}\n"
""));
        label_8 = new QLabel(frame);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(40, 330, 91, 16));
        label_9 = new QLabel(frame);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(40, 380, 91, 20));
        comboBox_2 = new QComboBox(frame);
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->setObjectName("comboBox_2");
        comboBox_2->setGeometry(QRect(180, 380, 121, 22));
        lineEdit_2 = new QLineEdit(frame);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(150, 80, 151, 31));
        lineEdit_2->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    font-size: 12px;\n"
"    font-weight: bold;\n"
"    color: #b5b5b5;\n"
"    background-color: white;\n"
"    border: 2px solid #1e4fa3;\n"
"    border-radius: 10px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QLineEdit:focus {\n"
"    border: 2px solid #4a6eff;\n"
"}\n"
"\n"
"QLineEdit:placeholder {\n"
"    color: #a0a0a0;\n"
"    font-style: italic;\n"
"    font-size: 7px;        /* Taille plus petite */\n"
"    font-weight: normal;    /* Enlever le gras */\n"
"}"));
        lineEdit_3 = new QLineEdit(frame);
        lineEdit_3->setObjectName("lineEdit_3");
        lineEdit_3->setGeometry(QRect(160, 330, 151, 31));
        lineEdit_3->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    font-size: 12px;\n"
"    font-weight: bold;\n"
"    color: #b5b5b5;\n"
"    background-color: white;\n"
"    border: 2px solid #1e4fa3;\n"
"    border-radius: 10px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QLineEdit:focus {\n"
"    border: 2px solid #4a6eff;\n"
"}\n"
"\n"
"QLineEdit:placeholder {\n"
"    color: #a0a0a0;\n"
"    font-style: italic;\n"
"    font-size: 7px;        /* Taille plus petite */\n"
"    font-weight: normal;    /* Enlever le gras */\n"
"}"));
        lineEdit_4 = new QLineEdit(frame);
        lineEdit_4->setObjectName("lineEdit_4");
        lineEdit_4->setGeometry(QRect(150, 280, 151, 31));
        lineEdit_4->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    font-size: 12px;\n"
"    font-weight: bold;\n"
"    color: #b5b5b5;\n"
"    background-color: white;\n"
"    border: 2px solid #1e4fa3;\n"
"    border-radius: 10px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QLineEdit:focus {\n"
"    border: 2px solid #4a6eff;\n"
"}\n"
"\n"
"QLineEdit:placeholder {\n"
"    color: #a0a0a0;\n"
"    font-style: italic;\n"
"    font-size: 7px;        /* Taille plus petite */\n"
"    font-weight: normal;    /* Enlever le gras */\n"
"}"));
        lineEdit_5 = new QLineEdit(frame);
        lineEdit_5->setObjectName("lineEdit_5");
        lineEdit_5->setGeometry(QRect(160, 220, 151, 31));
        lineEdit_5->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    font-size: 12px;\n"
"    font-weight: bold;\n"
"    color: #b5b5b5;\n"
"    background-color: white;\n"
"    border: 2px solid #1e4fa3;\n"
"    border-radius: 10px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QLineEdit:focus {\n"
"    border: 2px solid #4a6eff;\n"
"}\n"
"\n"
"QLineEdit:placeholder {\n"
"    color: #a0a0a0;\n"
"    font-style: italic;\n"
"    font-size: 7px;        /* Taille plus petite */\n"
"    font-weight: normal;    /* Enlever le gras */\n"
"}"));
        lineEdit_6 = new QLineEdit(frame);
        lineEdit_6->setObjectName("lineEdit_6");
        lineEdit_6->setGeometry(QRect(150, 130, 151, 31));
        lineEdit_6->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    font-size: 12px;\n"
"    font-weight: bold;\n"
"    color: #b5b5b5;\n"
"    background-color: white;\n"
"    border: 2px solid #1e4fa3;\n"
"    border-radius: 10px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QLineEdit:focus {\n"
"    border: 2px solid #4a6eff;\n"
"}\n"
"\n"
"QLineEdit:placeholder {\n"
"    color: #a0a0a0;\n"
"    font-style: italic;\n"
"    font-size: 7px;        /* Taille plus petite */\n"
"    font-weight: normal;    /* Enlever le gras */\n"
"}"));
        frame_2 = new QFrame(centralwidget);
        frame_2->setObjectName("frame_2");
        frame_2->setGeometry(QRect(20, 80, 331, 521));
        frame_2->setStyleSheet(QString::fromUtf8("background-color: rgb(6, 15, 143);\n"
"border-radius: 15px;\n"
"background: qlineargradient(\n"
"    x1:0, y1:0, x2:1, y2:0,\n"
"    stop:0 #060f8f,    /* bleu sombre */\n"
"    stop:1 #6996ff     /* bleu clair un peu plus lumineux */\n"
");\n"
""));
        frame_2->setFrameShape(QFrame::Shape::StyledPanel);
        frame_2->setFrameShadow(QFrame::Shadow::Raised);
        label_7 = new QLabel(frame_2);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(90, 20, 181, 31));
        label_7->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 15px;\n"
"    font-weight: bold;\n"
"    background-color: white;   /* fond blanc */\n"
"    color: black;              /* texte en noir pour contraste */\n"
"    border-radius: 10px;       /* coins arrondis si tu veux */\n"
"    padding: 3px;              /* un peu d'espace autour du texte */\n"
"}\n"
""));
        pushButton = new QPushButton(frame_2);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(90, 80, 161, 24));
        pushButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    font-size: 15px;\n"
"    font-weight: bold;\n"
"    background-color: white;   /* fond blanc */\n"
"    color: black;              /* texte noir */\n"
"    border-radius: 10px;       /* coins arrondis */\n"
"    padding: 3px;              /* espace autour du texte */\n"
"    border: none;              /* optionnel : pas de bordure par d\303\251faut */\n"
"}\n"
"\n"
"/* Optionnel : effet au survol */\n"
"QPushButton:hover {\n"
"    background-color: #f0f0f0; /* fond un peu plus clair quand la souris passe dessus */\n"
"}\n"
""));
        pushButton_2 = new QPushButton(frame_2);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(90, 140, 161, 24));
        pushButton_2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    font-size: 15px;\n"
"    font-weight: bold;\n"
"    background-color: white;   /* fond blanc */\n"
"    color: black;              /* texte noir */\n"
"    border-radius: 10px;       /* coins arrondis */\n"
"    padding: 3px;              /* espace autour du texte */\n"
"    border: none;              /* optionnel : pas de bordure par d\303\251faut */\n"
"}\n"
"\n"
"/* Optionnel : effet au survol */\n"
"QPushButton:hover {\n"
"    background-color: #f0f0f0; /* fond un peu plus clair quand la souris passe dessus */\n"
"}\n"
""));
        pushButton_3 = new QPushButton(frame_2);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(90, 200, 191, 24));
        pushButton_3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    font-size: 15px;\n"
"    font-weight: bold;\n"
"    background-color: white;   /* fond blanc */\n"
"    color: black;              /* texte noir */\n"
"    border-radius: 10px;       /* coins arrondis */\n"
"    padding: 3px;              /* espace autour du texte */\n"
"    border: none;              /* optionnel : pas de bordure par d\303\251faut */\n"
"}\n"
"\n"
"/* Optionnel : effet au survol */\n"
"QPushButton:hover {\n"
"    background-color: #f0f0f0; /* fond un peu plus clair quand la souris passe dessus */\n"
"}\n"
""));
        lineEdit_7 = new QLineEdit(centralwidget);
        lineEdit_7->setObjectName("lineEdit_7");
        lineEdit_7->setGeometry(QRect(960, 0, 211, 42));
        lineEdit_7->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    color: black;                  /* couleur du texte */\n"
"    background-color: white;       /* fond blanc */\n"
"    border: 2px solid white;       /* bordure blanche */\n"
"    border-radius: 10px;           /* coins arrondis */\n"
"    padding: 4px;                  /* espace entre texte et bord */\n"
"    font-size: 14px;               /* taille du texte */\n"
"    min-width: 150px;              /* largeur minimale (optionnel) */\n"
"    min-height: 30px;              /* hauteur minimale (optionnel) */\n"
"}\n"
""));
        pushButton_7 = new QPushButton(centralwidget);
        pushButton_7->setObjectName("pushButton_7");
        pushButton_7->setGeometry(QRect(1190, 10, 111, 24));
        pushButton_7->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    font-size: 15px;\n"
"    font-weight: bold;\n"
"    color: white;                 /* texte blanc */\n"
"    background-color: orange;     /* fond orange */\n"
"    border: none;                 /* pas de bordure */\n"
"    border-radius: 10px;          /* coins arrondis */\n"
"    padding: 5px 15px;            /* espace autour du texte */\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #ff9900;    /* orange plus clair au survol */\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #e68a00;    /* orange plus fonc\303\251 au clic */\n"
"}\n"
""));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1314, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "name", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "first name ", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Vehicle type ", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "ID  ", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("MainWindow", "choose a type ", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Taxi", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("MainWindow", "Bus", nullptr));
        comboBox->setItemText(3, QCoreApplication::translate("MainWindow", "car", nullptr));

        lineEdit->setText(QString());
        lineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "enter your name ", nullptr));
        pushButton_4->setText(QCoreApplication::translate("MainWindow", "confirm", nullptr));
        pushButton_5->setText(QCoreApplication::translate("MainWindow", "cancle", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Ability", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Area served ", nullptr));
        pushButton_6->setText(QCoreApplication::translate("MainWindow", "Reset ", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Time", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "Action ", nullptr));
        comboBox_2->setItemText(0, QCoreApplication::translate("MainWindow", "choose the action ", nullptr));
        comboBox_2->setItemText(1, QCoreApplication::translate("MainWindow", "delivery", nullptr));
        comboBox_2->setItemText(2, QCoreApplication::translate("MainWindow", "shift", nullptr));

        lineEdit_2->setText(QString());
        lineEdit_2->setPlaceholderText(QCoreApplication::translate("MainWindow", "enter your first name ", nullptr));
        lineEdit_3->setText(QString());
        lineEdit_3->setPlaceholderText(QCoreApplication::translate("MainWindow", "enter the time ", nullptr));
        lineEdit_4->setText(QString());
        lineEdit_4->setPlaceholderText(QCoreApplication::translate("MainWindow", "enter your area ", nullptr));
        lineEdit_5->setText(QString());
        lineEdit_5->setPlaceholderText(QCoreApplication::translate("MainWindow", "enter the number ", nullptr));
        lineEdit_6->setText(QString());
        lineEdit_6->setPlaceholderText(QCoreApplication::translate("MainWindow", "enter your ID", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Transport management ", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "add a request ", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "list of requests ", nullptr));
        pushButton_3->setText(QCoreApplication::translate("MainWindow", "delete a request ", nullptr));
        lineEdit_7->setText(QCoreApplication::translate("MainWindow", "search the site ", nullptr));
        lineEdit_7->setPlaceholderText(QCoreApplication::translate("MainWindow", "search the site ", nullptr));
        pushButton_7->setText(QCoreApplication::translate("MainWindow", "search ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
