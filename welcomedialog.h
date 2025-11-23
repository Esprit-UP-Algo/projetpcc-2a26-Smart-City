#ifndef WELCOMEDIALOG_H
#define WELCOMEDIALOG_H

#include <QDialog>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class WelcomeDialog;
}
QT_END_NAMESPACE

class WelcomeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WelcomeDialog(QWidget *parent = nullptr);
    ~WelcomeDialog();

private slots:
    void startWelcomeSequence();
    void closeAndProceed();

private:
    Ui::WelcomeDialog *ui;
    QTimer *welcomeTimer;
    
    void centerOnScreen();
    void setupAnimations();
};

#endif // WELCOMEDIALOG_H
