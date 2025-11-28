QT += core gui widgets sql charts printsupport

CONFIG += c++17

TARGET = SmartCity
TEMPLATE = app

# Fichiers sources
SOURCES += \
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

# Fichiers d'en-tête
HEADERS += \
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

# Fichiers d'interface
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

# Ressources
RESOURCES += resources.qrc

# Pour Qt Charts
greaterThan(QT_MAJOR_VERSION, 4): QT += charts

# Configuration release/debug
CONFIG(release, debug|release) {
    DEFINES += QT_NO_DEBUG_OUTPUT
}
CONFIG(debug, debug|release) {
    DEFINES += QT_DEBUG
}

# Paramètres de compilation
QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS_WARN_ON += -Wall -Wextra
