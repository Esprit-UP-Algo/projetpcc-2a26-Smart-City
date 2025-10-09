QT += core gui widgets sql printsupport charts

CONFIG += c++17
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Residents
TEMPLATE = app

SOURCES += \
    main.cpp \
    residentspage.cpp \
    residentdialog.cpp \
    databasemanager.cpp

HEADERS += \
    residentspage.h \
    residentdialog.h \
    databasemanager.h

FORMS += \
    residentspage.ui \
    residentdialog.ui

QTPLUGIN += qjpeg qico
