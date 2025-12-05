QT += core gui widgets sql charts printsupport serialport network

CONFIG += c++17

SOURCES += \
    arduino.cpp \
    main.cpp \
    mainwindow.cpp \
    welcomedialog.cpp \
    nexoradashboard.cpp \
    residentspage.cpp \
    incidentspage.cpp \
    MapDialog.cpp \
    mapwidget.cpp \
    locauxpage.cpp \
    settingspage.cpp \
    transportpage.cpp \
    databasemanager.cpp \
    connection.cpp \
    financespage.cpp \
    financialassistant.cpp \
    openaichat.cpp \
    thememanager.cpp \
    languagemanager.cpp \
    settingsmanager.cpp \
    admindatabasepage.cpp \
    admindbinfopage.cpp \
    adminfilespage.cpp

HEADERS += \
    arduino.h \
    mainwindow.h \
    transportpage.h \
    welcomedialog.h \
    nexoradashboard.h \
    residentspage.h \
    incidentspage.h \
    MapDialog.h \
    mapwidget.h \
    locauxpage.h \
    settingspage.h \
    databasemanager.h \
    connection.h \
    financespage.h \
    financialassistant.h \
    openaichat.h \
    config.h \
    userrole.h \
    thememanager.h \
    languagemanager.h \
    settingsmanager.h \
    admindatabasepage.h \
    admindbinfopage.h \
    adminfilespage.h

FORMS += \
    mainwindow.ui \
    transportpage.ui \
    welcomedialog.ui \
    nexoradashboard.ui \
    residentspage.ui \
    incidentspage.ui \
    locauxpage.ui \
    settingspage.ui \
    financespage.ui \
    admindatabasepage.ui \
    admindbinfopage.ui \
    adminfilespage.ui

RESOURCES += \
    resources.qrc

TRANSLATIONS += \
    translations/nexora_en.ts

# Build translations
qtPrepareTool(LRELEASE, lrelease)
for(tsfile, TRANSLATIONS) {
    qmfile = $$shadowed($$tsfile)
    qmfile ~= s,.ts$,.qm,
    qmdir = $$dirname(qmfile)
    !exists($$qmdir) {
        mkpath($$qmdir)
    }
    command = $$LRELEASE -silent $$tsfile -qm $$qmfile
    system($$command)
}
