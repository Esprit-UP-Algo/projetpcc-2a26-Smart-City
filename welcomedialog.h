#ifndef WELCOMEDIALOG_H
#define WELCOMEDIALOG_H

#include <QDialog>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

namespace Ui {
class WelcomeDialog;
}

class WelcomeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WelcomeDialog(QWidget *parent = nullptr);
    ~WelcomeDialog();

    void showWithAnimation();

private:
    Ui::WelcomeDialog *ui;
    void setupAnimations();
    void closeWithAnimation();
};

#endif // WELCOMEDIALOG_H
