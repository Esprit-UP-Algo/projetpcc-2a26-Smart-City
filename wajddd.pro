QT += core gui widgets sql printsupport charts
CONFIG += c++17
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Incident
TEMPLATE = app

SOURCES += \
    main.cpp \
    incidentspage.cpp \
    incidentdialog.cpp \
    databasemanager.cpp \
    connection.cpp \


HEADERS += \
    incidentspage.h \
    incidentdialog.h \
    databasemanager.h \
    connection.h \


FORMS += \
    incidentspage.ui \
    incidentdialog.ui

QTPLUGIN += qjpeg qico
