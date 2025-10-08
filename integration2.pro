QT += core gui widgets sql charts printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# Uncomment below to disable deprecated APIs before Qt 6.0
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
    main.cpp \
    databasemanager.cpp \
    dashboardwindow.cpp \
    mainwindow.cpp \
    welcomedialog.cpp \
    settingspage.cpp \
    incidentspage.cpp \
    incidentdialog.cpp \
    transportspage.cpp \
    vehiculedialog.cpp

HEADERS += \
    databasemanager.h \
    dashboardwindow.h \
    mainwindow.h \
    welcomedialog.h \
    settingspage.h \
    incidentspage.h \
    incidentdialog.h \
    transportspage.h \
    vehiculedialog.h

FORMS += \
    dashboardwindow.ui \
    mainwindow.ui \
    welcomedialog.ui \
    settingspage.ui \
    incidentspage.ui \
    incidentdialog.ui \
    transportspage.ui \
    vehiculedialog.ui

RESOURCES += \
    resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
