/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QPushButton *btnAjouterTransaction;
    QPushButton *btnListe;
    QPushButton *btnStatistique;
    QLabel *label_2;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QLabel *label;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_4;
    QComboBox *comboBox;
    QLineEdit *lineEdit_5;
    QLineEdit *lineEdit_6;
    QDateEdit *dateEdit;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QPushButton *pushButton_4;
    QWidget *page_2;
    QLabel *label_4;
    QLineEdit *lineEdit_3;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_10;
    QPushButton *pushButton_11;
    QTableWidget *tableWidget;
    QComboBox *comboBox_3;
    QWidget *page_3;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1178, 693);
        MainWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 170, 255)"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(-10, 0, 211, 701));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        frame = new QFrame(verticalLayoutWidget);
        frame->setObjectName("frame");
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
        btnAjouterTransaction = new QPushButton(frame);
        btnAjouterTransaction->setObjectName("btnAjouterTransaction");
        btnAjouterTransaction->setGeometry(QRect(30, 240, 171, 51));
        btnAjouterTransaction->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnAjouterTransaction->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: transparent;\n"
"    color: #ffffff;\n"
"    font-family: \"Segoe UI\", Arial, sans-serif;\n"
"    font-size: 15px;\n"
"    font-weight: 500;\n"
"    padding: 12px 18px;\n"
"    border-radius: 10px;\n"
"    text-align: left;\n"
"    border: 1px solid #ffffff; /* encadrement blanc fin */\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: rgba(255,255,255,0.15);\n"
"}\n"
"\n"
"QPushButton:checked {\n"
"    background-color: #123f8f; /* bleu nuit pas trop sombre */\n"
"    border: 1px solid #ffffff; /* conserver le contour lors de la s\303\251lection */\n"
"}\n"
""));
        btnAjouterTransaction->setCheckable(true);
        btnListe = new QPushButton(frame);
        btnListe->setObjectName("btnListe");
        btnListe->setGeometry(QRect(30, 310, 171, 51));
        btnListe->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnListe->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: transparent;\n"
"    color: #ffffff;\n"
"    font-family: \"Segoe UI\", Arial, sans-serif;\n"
"    font-size: 15px;\n"
"    font-weight: 500;\n"
"    padding: 12px 18px;\n"
"    border-radius: 10px;\n"
"    text-align: left;\n"
"    border: 1px solid #ffffff; /* encadrement blanc fin */\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: rgba(255,255,255,0.15);\n"
"}\n"
"\n"
"QPushButton:checked {\n"
"    background-color: #123f8f; /* bleu nuit pas trop sombre */\n"
"    border: 1px solid #ffffff; /* conserver le contour lors de la s\303\251lection */\n"
"}\n"
""));
        btnListe->setCheckable(true);
        btnStatistique = new QPushButton(frame);
        btnStatistique->setObjectName("btnStatistique");
        btnStatistique->setGeometry(QRect(30, 390, 171, 51));
        btnStatistique->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnStatistique->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: transparent;\n"
"    color: #ffffff;\n"
"    font-family: \"Segoe UI\", Arial, sans-serif;\n"
"    font-size: 15px;\n"
"    font-weight: 500;\n"
"    padding: 12px 18px;\n"
"    border-radius: 10px;\n"
"    text-align: left;\n"
"    border: 1px solid #ffffff; /* encadrement blanc fin */\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: rgba(255,255,255,0.15);\n"
"}\n"
"\n"
"QPushButton:checked {\n"
"    background-color: #123f8f; /* bleu nuit pas trop sombre */\n"
"    border: 1px solid #ffffff; /* conserver le contour lors de la s\303\251lection */\n"
"}\n"
""));
        btnStatistique->setCheckable(true);
        label_2 = new QLabel(frame);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(0, 0, 211, 151));
        label_2->setStyleSheet(QString::fromUtf8("ui->label_2->setStyleSheet(\"background: transparent;\");\n"
"ui->label_2->setAutoFillBackground(false);\n"
""));
        label_2->setPixmap(QPixmap(QString::fromUtf8(":/icons/icons/logo.png")));

        verticalLayout->addWidget(frame);

        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(209, 19, 951, 651));
        stackedWidget->setStyleSheet(QString::fromUtf8("QWidget {\n"
"    background-color: rgb(255, 255, 255); /* fond blanc */\n"
"    border-radius: 15px; /* coins arrondis, ajuste la valeur selon le besoin */\n"
"}"));
        page = new QWidget();
        page->setObjectName("page");
        label = new QLabel(page);
        label->setObjectName("label");
        label->setGeometry(QRect(20, 30, 911, 181));
        label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: rgb(245, 245, 245);   /* gris tr\303\250s clair */\n"
"    border: 2px solid rgb(148, 98, 255);    /* contour mauve */\n"
"    border-radius: 12px;                    /* coins arrondis */\n"
"    color: black;                           /* texte noir */\n"
"    padding: 6px;                           /* espace int\303\251rieur */\n"
"    font-size: 32px;                        /* police plus grande */\n"
"    font-weight: bold;                      /* texte en gras */\n"
"    qproperty-alignment: 'AlignCenter';     /* centre le texte dans le QLabel */\n"
"}"));
        label->setPixmap(QPixmap(QString::fromUtf8(":/icons/icons/bg.png")));
        lineEdit = new QLineEdit(page);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(40, 250, 401, 41));
        lineEdit->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    background-color: rgb(245, 245, 245);   /* gris clair */\n"
"    border: 1px rgb(200, 200, 200);   /* contour gris un peu plus fonc\303\251 */\n"
"    border-radius: 12px;                    /* coins arrondis */\n"
"    padding: 6px;                           /* espace int\303\251rieur pour le texte */\n"
"    font-size: 16px;                        /* taille du texte */\n"
"    color: black;                           /* couleur du texte */\n"
"}"));
        lineEdit_4 = new QLineEdit(page);
        lineEdit_4->setObjectName("lineEdit_4");
        lineEdit_4->setGeometry(QRect(510, 250, 401, 41));
        lineEdit_4->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    background-color: rgb(245, 245, 245);   /* gris clair */\n"
"    border: 1px rgb(200, 200, 200);   /* contour gris un peu plus fonc\303\251 */\n"
"    border-radius: 12px;                    /* coins arrondis */\n"
"    padding: 6px;                           /* espace int\303\251rieur pour le texte */\n"
"    font-size: 16px;                        /* taille du texte */\n"
"    color: black;                           /* couleur du texte */\n"
"}"));
        comboBox = new QComboBox(page);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(40, 320, 401, 41));
        comboBox->setStyleSheet(QString::fromUtf8("QComboBox {\n"
"    background-color: rgb(245, 245, 245);   /* fond gris clair */\n"
"    border: 1px  rgb(200, 200, 200);   /* contour gris */\n"
"    border-radius: 12px;                    /* coins arrondis */\n"
"    padding: 6px;                           /* espace int\303\251rieur */\n"
"    font-size: 14px;                        /* taille du texte */\n"
"    color: black;                           /* couleur du texte */\n"
"}\n"
"\n"
"/* Pour que le menu d\303\251roulant ait le m\303\252me style */\n"
"QComboBox QAbstractItemView {\n"
"    background-color: rgb(245, 245, 245);   /* fond gris clair du menu */\n"
"    border: 1px solid rgb(200, 200, 200);   /* contour du menu */\n"
"    border-radius: 12px;                    /* coins arrondis du menu */\n"
"    selection-background-color: rgb(200, 200, 255); /* surlignage d'une option */\n"
"}"));
        lineEdit_5 = new QLineEdit(page);
        lineEdit_5->setObjectName("lineEdit_5");
        lineEdit_5->setGeometry(QRect(40, 390, 401, 41));
        lineEdit_5->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    background-color: rgb(245, 245, 245);   /* gris clair */\n"
"    border: 1px rgb(200, 200, 200);   /* contour gris un peu plus fonc\303\251 */\n"
"    border-radius: 12px;                    /* coins arrondis */\n"
"    padding: 6px;                           /* espace int\303\251rieur pour le texte */\n"
"    font-size: 16px;                        /* taille du texte */\n"
"    color: black;                           /* couleur du texte */\n"
"}"));
        lineEdit_6 = new QLineEdit(page);
        lineEdit_6->setObjectName("lineEdit_6");
        lineEdit_6->setGeometry(QRect(510, 390, 401, 41));
        lineEdit_6->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    background-color: rgb(245, 245, 245);   /* gris clair */\n"
"    border: 1px rgb(200, 200, 200);   /* contour gris un peu plus fonc\303\251 */\n"
"    border-radius: 12px;                    /* coins arrondis */\n"
"    padding: 6px;                           /* espace int\303\251rieur pour le texte */\n"
"    font-size: 16px;                        /* taille du texte */\n"
"    color: black;                           /* couleur du texte */\n"
"}"));
        dateEdit = new QDateEdit(page);
        dateEdit->setObjectName("dateEdit");
        dateEdit->setGeometry(QRect(510, 320, 391, 41));
        dateEdit->setStyleSheet(QString::fromUtf8("QDateEdit {\n"
"    background-color: rgb(245, 245, 245);   /* fond gris clair */\n"
"    border: 1px solid rgb(200, 200, 200);   /* contour gris */\n"
"    border-radius: 12px;                    /* coins arrondis */\n"
"    padding: 6px;                           /* espace int\303\251rieur pour le texte */\n"
"    font-size: 14px;                        /* taille du texte */\n"
"    color: black;                           /* couleur du texte */\n"
"}\n"
"\n"
"/* Pour le calendrier qui s\342\200\231ouvre */\n"
"QDateEdit::drop-down {\n"
"    subcontrol-origin: padding;\n"
"    subcontrol-position: top right;\n"
"    width: 20px;\n"
"    border-left: 1px solid rgb(200, 200, 200);\n"
"    border-radius: 0 12px 12px 0;\n"
"    background-color: rgb(245, 245, 245);\n"
"}\n"
"\n"
"QDateEdit QCalendarWidget {\n"
"    background-color: rgb(245, 245, 245);   /* fond gris clair du calendrier */\n"
"    border: 1px solid rgb(200, 200, 200);   /* contour du calendrier */\n"
"    border-radius: 12px;\n"
"}"));
        pushButton_5 = new QPushButton(page);
        pushButton_5->setObjectName("pushButton_5");
        pushButton_5->setGeometry(QRect(332, 498, 121, 41));
        pushButton_5->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        pushButton_5->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background: qlineargradient(\n"
"        x1:0, y1:0, x2:1, y2:1,\n"
"        stop:0 rgb(245, 245, 245),   /* gris clair */\n"
"        stop:1 rgb(220, 220, 220)    /* gris l\303\251g\303\250rement plus fonc\303\251 */\n"
"    );\n"
"    border: 1px solid rgb(150, 100, 200);   /* contour mauve fin */\n"
"    border-radius: 12px;                    /* coins arrondis */\n"
"    padding: 6px;                           /* espace int\303\251rieur */\n"
"}\n"
""));
        pushButton_5->setCheckable(true);
        pushButton_6 = new QPushButton(page);
        pushButton_6->setObjectName("pushButton_6");
        pushButton_6->setGeometry(QRect(490, 498, 121, 41));
        pushButton_6->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        pushButton_6->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background: qlineargradient(\n"
"        x1:0, y1:0, x2:1, y2:1,\n"
"        stop:0 rgb(245, 245, 245),   /* gris clair */\n"
"        stop:1 rgb(220, 220, 220)    /* gris l\303\251g\303\250rement plus fonc\303\251 */\n"
"    );\n"
"    border: 1px solid rgb(150, 100, 200);   /* contour mauve fin */\n"
"    border-radius: 12px;                    /* coins arrondis */\n"
"    padding: 6px;                           /* espace int\303\251rieur */\n"
"}\n"
""));
        pushButton_6->setCheckable(true);
        pushButton_4 = new QPushButton(page);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setGeometry(QRect(170, 500, 121, 41));
        pushButton_4->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        pushButton_4->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background: qlineargradient(\n"
"        x1:0, y1:0, x2:1, y2:1,\n"
"        stop:0 rgb(245, 245, 245),   /* gris clair */\n"
"        stop:1 rgb(220, 220, 220)    /* gris l\303\251g\303\250rement plus fonc\303\251 */\n"
"    );\n"
"    border: 1px solid rgb(150, 100, 200);   /* contour mauve fin */\n"
"    border-radius: 12px;                    /* coins arrondis */\n"
"    padding: 6px;                           /* espace int\303\251rieur */\n"
"}\n"
""));
        pushButton_4->setCheckable(true);
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        label_4 = new QLabel(page_2);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(10, 20, 911, 181));
        label_4->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: rgb(245, 245, 245);   /* gris tr\303\250s clair */\n"
"    border: 2px solid rgb(148, 98, 255);    /* contour mauve */\n"
"    border-radius: 12px;                    /* coins arrondis */\n"
"    color: black;                           /* texte noir */\n"
"    padding: 6px;                           /* espace int\303\251rieur */\n"
"    font-size: 32px;                        /* police plus grande */\n"
"    font-weight: bold;                      /* texte en gras */\n"
"    qproperty-alignment: 'AlignCenter';     /* centre le texte dans le QLabel */\n"
"}"));
        label_4->setPixmap(QPixmap(QString::fromUtf8(":/icons/icons/bg.png")));
        lineEdit_3 = new QLineEdit(page_2);
        lineEdit_3->setObjectName("lineEdit_3");
        lineEdit_3->setGeometry(QRect(20, 230, 441, 51));
        lineEdit_3->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    background-color: rgb(245, 245, 245);   /* gris clair */\n"
"    border: 1px rgb(200, 200, 200);   /* contour gris un peu plus fonc\303\251 */\n"
"    border-radius: 12px;                    /* coins arrondis */\n"
"    padding: 6px;                           /* espace int\303\251rieur pour le texte */\n"
"    font-size: 16px;                        /* taille du texte */\n"
"    color: black;                           /* couleur du texte */\n"
"}"));
        pushButton = new QPushButton(page_2);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(470, 240, 41, 31));
        pushButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        pushButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #6a0dad;   /* bleu nuit */\n"
"    color: white;\n"
"    border-radius: 6px;\n"
"    padding: 6px 12px;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #8e44ad;   /* bleu nuit plus clair au survol */\n"
"}\n"
""));
        pushButton->setCheckable(true);
        pushButton_2 = new QPushButton(page_2);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(640, 240, 141, 31));
        pushButton_2->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        pushButton_2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #6a0dad;   /* bleu nuit */\n"
"    color: white;\n"
"    border-radius: 6px;\n"
"    padding: 6px 12px;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #8e44ad;   /* bleu nuit plus clair au survol */\n"
"}\n"
""));
        pushButton_2->setCheckable(true);
        pushButton_3 = new QPushButton(page_2);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(790, 240, 41, 31));
        pushButton_3->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        pushButton_3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #6a0dad;   /* bleu nuit */\n"
"    color: white;\n"
"    border-radius: 6px;\n"
"    padding: 6px 12px;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #8e44ad;   /* bleu nuit plus clair au survol */\n"
"}\n"
""));
        pushButton_3->setCheckable(true);
        pushButton_10 = new QPushButton(page_2);
        pushButton_10->setObjectName("pushButton_10");
        pushButton_10->setGeometry(QRect(840, 240, 41, 31));
        pushButton_10->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        pushButton_10->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #6a0dad;   /* bleu nuit */\n"
"    color: white;\n"
"    border-radius: 6px;\n"
"    padding: 6px 12px;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #8e44ad;   /* bleu nuit plus clair au survol */\n"
"}\n"
""));
        pushButton_10->setCheckable(true);
        pushButton_11 = new QPushButton(page_2);
        pushButton_11->setObjectName("pushButton_11");
        pushButton_11->setGeometry(QRect(890, 240, 41, 31));
        pushButton_11->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        pushButton_11->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #6a0dad;   /* bleu nuit */\n"
"    color: white;\n"
"    border-radius: 6px;\n"
"    padding: 6px 12px;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #8e44ad;   /* bleu nuit plus clair au survol */\n"
"}\n"
""));
        pushButton_11->setCheckable(true);
        tableWidget = new QTableWidget(page_2);
        if (tableWidget->columnCount() < 6)
            tableWidget->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        if (tableWidget->rowCount() < 6)
            tableWidget->setRowCount(6);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setGeometry(QRect(30, 310, 861, 321));
        tableWidget->setStyleSheet(QString::fromUtf8("QTableWidget, QTableView {\n"
"    background-color: #f0f0f0;       /* gris clair */\n"
"    border: 1px solid #8a3ebd;       /* encadrement mauve tr\303\250s fin */\n"
"    gridline-color: #cccccc;         /* lignes internes gris clair */\n"
"    selection-background-color: #8a3ebd;  /* fond s\303\251lection mauve */\n"
"    selection-color: white;          /* texte blanc sur s\303\251lection */\n"
"}\n"
""));
        comboBox_3 = new QComboBox(page_2);
        comboBox_3->addItem(QString());
        comboBox_3->addItem(QString());
        comboBox_3->addItem(QString());
        comboBox_3->setObjectName("comboBox_3");
        comboBox_3->setGeometry(QRect(520, 240, 111, 31));
        comboBox_3->setStyleSheet(QString::fromUtf8("QComboBox {\n"
"    background-color: #6a0dad;   /* mauve sombre */\n"
"    color: white;\n"
"    border-radius: 6px;\n"
"    padding: 6px 12px;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"    border: none;                /* pas de bordure grise par d\303\251faut */\n"
"}\n"
"\n"
"/* Survol */\n"
"QComboBox:hover {\n"
"    background-color: #8e44ad;   /* mauve plus clair au survol */\n"
"}\n"
"\n"
"/* Liste d\303\251roulante */\n"
"QComboBox QAbstractItemView {\n"
"    background-color: #6a0dad;   /* m\303\252me couleur que le bouton */\n"
"    color: white;\n"
"    border: 1px solid #8e44ad;   /* encadrement discret */\n"
"    selection-background-color: #8e44ad; /* item s\303\251lectionn\303\251 */\n"
"    selection-color: white;\n"
"}\n"
""));
        stackedWidget->addWidget(page_2);
        page_3 = new QWidget();
        page_3->setObjectName("page_3");
        stackedWidget->addWidget(page_3);
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        btnAjouterTransaction->setText(QCoreApplication::translate("MainWindow", "Ajouter transaction ", nullptr));
        btnListe->setText(QCoreApplication::translate("MainWindow", "Liste de transaction", nullptr));
        btnStatistique->setText(QCoreApplication::translate("MainWindow", "Statistiques", nullptr));
        label_2->setText(QString());
        label->setText(QString());
        lineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\360\237\224\242Code unique ", nullptr));
        lineEdit_4->setPlaceholderText(QCoreApplication::translate("MainWindow", "\360\237\222\266Montant", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("MainWindow", "paiement", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("MainWindow", "facture", nullptr));

        lineEdit_5->setPlaceholderText(QCoreApplication::translate("MainWindow", "\360\237\227\203\357\270\217Cat\303\251gorie", nullptr));
        lineEdit_6->setText(QString());
        lineEdit_6->setPlaceholderText(QCoreApplication::translate("MainWindow", "Mode de paiement ", nullptr));
        pushButton_5->setText(QCoreApplication::translate("MainWindow", "R\303\251initialiser", nullptr));
        pushButton_6->setText(QCoreApplication::translate("MainWindow", "Annuler ", nullptr));
        pushButton_4->setText(QCoreApplication::translate("MainWindow", "enregistrer", nullptr));
        label_4->setText(QString());
        lineEdit_3->setPlaceholderText(QCoreApplication::translate("MainWindow", "\360\237\224\215 rechercher ", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "\360\237\224\215", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "exporter en pdf ", nullptr));
        pushButton_3->setText(QCoreApplication::translate("MainWindow", "\360\237\227\221\357\270\217", nullptr));
        pushButton_10->setText(QCoreApplication::translate("MainWindow", "\342\234\217\357\270\217", nullptr));
        pushButton_11->setText(QCoreApplication::translate("MainWindow", "\360\237\224\204", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Code unique", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Montant", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Type", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Date", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "Cat\303\251gorie", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "Mode de paiement", nullptr));
        comboBox_3->setItemText(0, QCoreApplication::translate("MainWindow", "Trier par", nullptr));
        comboBox_3->setItemText(1, QCoreApplication::translate("MainWindow", "code unique", nullptr));
        comboBox_3->setItemText(2, QCoreApplication::translate("MainWindow", "date", nullptr));

        comboBox_3->setPlaceholderText(QCoreApplication::translate("MainWindow", "Trier par", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
