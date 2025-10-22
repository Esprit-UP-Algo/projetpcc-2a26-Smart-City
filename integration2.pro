QT += core gui widgets sql charts printsupport
QT += core gui sql


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    connection.cpp \
    dashboardwindow.cpp \
    databasemanager.cpp \
    financespage.cpp \
    incidentdialog.cpp \
    incidentspage.cpp \
    locauxpage.cpp \
    main.cpp \
    mainwindow.cpp \
    residentdialog.cpp \
    residentspage.cpp \
    settingspage.cpp \
    vehiculedialog.cpp \
    transactiondialog.cpp \
    transportspage.cpp \
    welcomedialog.cpp
    connection.cpp

HEADERS += \
    connection.h \
    dashboardwindow.h \
    databasemanager.h \
    financespage.h \
    incidentdialog.h \
    incidentspage.h \
    locauxpage.h \
    mainwindow.h \
    residentdialog.h \
    residentspage.h \
    settingspage.h \
    vehiculedialog.h \
    transactiondialog.h \
    transportspage.h \
    welcomedialog.h
    connection.h

FORMS += \
    dashboardwindow.ui \
    financespage.ui \
    incidentdialog.ui \
    incidentspage.ui \
    locauxpage.ui \
    mainwindow.ui \
    residentdialog.ui \
    residentspage.ui \
    settingspage.ui \
    transactiondialog.ui \
    transportspage.ui \
    vehiculedialog.ui \
    welcomedialog.ui

RESOURCES += \
    resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
