<<<<<<< HEAD
#include "mainwindow.h"

=======
#include "residentspage.h"
>>>>>>> b9d5983 (Ajout du module Gestion des Résidents (Qt))
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
<<<<<<< HEAD
    MainWindow w;
    w.show();
=======
    ResidentsPage w;   // <-- your page is your main window
    w.setWindowTitle("Module Finances - NEXORA");
    w.setMinimumSize(1200, 700);            // ✅ ensures good base size
    w.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // ✅ expands smoothly
    w.show();          // show the window

>>>>>>> b9d5983 (Ajout du module Gestion des Résidents (Qt))
    return a.exec();
}
