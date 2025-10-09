QT += core gui widgets sql charts printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# Uncomment this to disable deprecated APIs before Qt 6.0.0
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
    dashboardwindow.cpp \
    databasemanager.cpp \
    main.cpp \
    mainwindow.cpp \
    residentdialog.cpp \
    residentspage.cpp \
    settingspage.cpp \
    welcomedialog.cpp

HEADERS += \
    dashboardwindow.h \
    databasemanager.h \
    mainwindow.h \
    residentdialog.h \
    residentspage.h \
    settingspage.h \
    welcomedialog.h

FORMS += \
    dashboardwindow.ui \
    mainwindow.ui \
    residentdialog.ui \
    residentspage.ui \
    settingspage.ui \
    welcomedialog.ui

RESOURCES += \
    resources.qrc

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
