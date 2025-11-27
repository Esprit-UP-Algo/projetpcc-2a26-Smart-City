#ifndef NEXORADASHBOARD_H
#define NEXORADASHBOARD_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

QT_BEGIN_NAMESPACE
namespace Ui { class NexoraDashboard; }
QT_END_NAMESPACE

class ResidentsPage;
class FinancesPage;
class IncidentsPage;
class TransportPage;
class LocauxPage;
class SettingsPage;

class NexoraDashboard : public QMainWindow
{
    Q_OBJECT

public:
    NexoraDashboard(QWidget *parent = nullptr);
    ~NexoraDashboard();

private slots:
    void onResidentsClicked();
    void onFinancesClicked();
    void onIncidentsClicked();
    void onTransportsClicked();
    void onLocauxClicked();
    void onSettingsClicked();
    void onLogoutClicked();

private:
    Ui::NexoraDashboard *ui;
    
    // Page management
    QStackedWidget *pageStack;
    ResidentsPage *residentsPage;
    FinancesPage *financesPage;
    IncidentsPage *incidentsPage;
    TransportPage *transportsPage;  // ✅ Correct
    LocauxPage *locauxPage;
    SettingsPage *settingsPage;
    
    // Navigation
    
    // Animations
    QPropertyAnimation *pageTransition;
    QGraphicsOpacityEffect *pageOpacity;
    
    // Methods
    void setupUI();
    void setupNavigation();
    void setupPages();
    void setupAnimations();
    void showPage(int pageIndex, const QString &pageTitle);
};

#endif // NEXORADASHBOARD_H
