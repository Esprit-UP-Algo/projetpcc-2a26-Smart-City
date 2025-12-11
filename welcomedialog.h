#ifndef WELCOMEDIALOG_H
#define WELCOMEDIALOG_H

#include <QDialog>
#include <QTimer>
#include "userrole.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class WelcomeDialog;
}
QT_END_NAMESPACE

class WelcomeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WelcomeDialog(UserRole role,
                           const QString &email,
                           const QString &username,
                           QWidget *parent = nullptr);
    ~WelcomeDialog();

private slots:
    void startWelcomeSequence();
    void closeAndProceed();

private:
    Ui::WelcomeDialog *ui;
    QTimer *welcomeTimer;

    UserRole m_role;
    QString m_email;
    QString m_username;

    // 🔹 NEW → needed for animation code
    void fadeInAnimation();
    void startDotsAnimation();

    // optional, kept for compatibility
    void centerOnScreen();
};

#endif // WELCOMEDIALOG_H
