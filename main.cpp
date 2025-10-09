#include "financespage.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FinancesPage w;   // <-- your page is your main window
    w.setWindowTitle("Module Finances - NEXORA");
    w.setMinimumSize(1200, 700);            // ✅ ensures good base size
    w.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // ✅ expands smoothly
    w.show();          // show the window

    return a.exec();
}
