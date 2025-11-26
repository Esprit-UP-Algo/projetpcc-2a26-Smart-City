QT += core gui widgets sql charts printsupport


CONFIG += c++17
QT += serialport


SOURCES += \
    MapDialog.cpp \
    MapWidget.cpp \
    arduino.cpp \
    main.cpp \
    mainwindow.cpp \
    welcomedialog.cpp \
    nexoradashboard.cpp \
    residentspage.cpp \
    incidentspage.cpp \
    locauxpage.cpp \
    settingspage.cpp \
    transportpage.cpp \
    databasemanager.cpp \
    connection.cpp \
    financespage.cpp
    arduino.cpp

HEADERS += \
    MapDialog.h \
    MapWidget.h \
    arduino.h \
    mainwindow.h \
    transportpage.h \
    welcomedialog.h \
    nexoradashboard.h \
    residentspage.h \
    incidentspage.h \
    locauxpage.h \
    settingspage.h \
    databasemanager.h \
    connection.h \
    financespage.h
    arduino.h

FORMS += \
    mainwindow.ui \
    transportpage.ui \
    welcomedialog.ui \
    nexoradashboard.ui \
    residentspage.ui \
    incidentspage.ui \
    locauxpage.ui \
    settingspage.ui \
    financespage.ui

RESOURCES += \
    resources.qrc

DISTFILES += \
    resources/incident.png \
    resources/map.png
