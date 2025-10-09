QT += core gui widgets sql printsupport charts

CONFIG += c++17
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Finance
TEMPLATE = app

SOURCES += \
    main.cpp \
    financespage.cpp \
    transactiondialog.cpp \
    databasemanager.cpp

HEADERS += \
    financespage.h \
    transactiondialog.h \
    databasemanager.h

FORMS += \
    financespage.ui \
    transactiondialog.ui

QTPLUGIN += qjpeg qico
