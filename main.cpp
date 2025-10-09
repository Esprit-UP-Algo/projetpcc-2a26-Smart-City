#include "mainwindow.h"       // Your login page
#include "dashboardwindow.h"  // Main app after login
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Create and show the login window first
    MainWindow loginWindow;
    loginWindow.show();

    // Run the event loop (login happens here)
    int result = a.exec();

    // If you plan to switch to dashboard after login, handle it in MainWindow
    // Example: emit a signal when login is successful, then open DashboardWindow

    return result;
}
