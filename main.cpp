#include <QApplication>
#include <QDebug>
#include <QCoreApplication>

#include "connection.h"
#include "mainwindow.h"
#include "thememanager.h"
#include "languagemanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Important: required for QSettings used by ThemeManager + LanguageManager
    QCoreApplication::setOrganizationName("Nexora");
    QCoreApplication::setApplicationName("SmartCity");

    // Load global theme + language BEFORE UI loads
    ThemeManager::instance().initialize();
    LanguageManager::instance().initialize();

    // Apply theme to whole application
    ThemeManager::instance().applyTheme(a.activeWindow());

    // Database connection
    Connection& c = Connection::getInstance();
    if (!c.createConnection()) {
        qDebug() << "❌ Oracle connection failed";
        return -1;
    }
    qDebug() << "✅ Connected to Oracle (NEXORA / XE)";

    // Start login window
    MainWindow loginWindow;

    // Apply theme on login window
    ThemeManager::instance().applyTheme(&loginWindow);

    loginWindow.show();
    return a.exec();
}
