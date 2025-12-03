#include "welcomedialog.h"
#include "ui_welcomedialog.h"
#include <QScreen>
#include <QTimer>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

WelcomeDialog::WelcomeDialog(UserRole role,
                             const QString &email,
                             const QString &username,
                             QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::WelcomeDialog)
    , m_role(role)
    , m_email(email)
    , m_username(username)
{
    ui->setupUi(this);

    // 🔹 No fullscreen — keep premium compact window (700x420)
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setModal(true);

    // 🔹 Center the window
    move(QApplication::primaryScreen()->geometry().center() - rect().center());

    // 🔹 Smooth fade in
    fadeInAnimation();

    // 🔹 Start animated dots
    startDotsAnimation();

    // 🔹 Auto-close after 2.4s for premium smooth UX
    QTimer::singleShot(2400, this, &WelcomeDialog::accept);
}

WelcomeDialog::~WelcomeDialog()
{
    delete ui;
}

/*──────────────────────────────────────────────
 * ANIMATIONS
 *─────────────────────────────────────────────*/

void WelcomeDialog::fadeInAnimation()
{
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
    this->setGraphicsEffect(effect);

    QPropertyAnimation *fade = new QPropertyAnimation(effect, "opacity");
    fade->setDuration(550);
    fade->setStartValue(0.0);
    fade->setEndValue(1.0);
    fade->setEasingCurve(QEasingCurve::InOutCubic);
    fade->start(QAbstractAnimation::DeleteWhenStopped);
}

void WelcomeDialog::startDotsAnimation()
{
    QStringList frames = {"•  ", "• •", "• • •"};
    int *i = new int(0);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() mutable {
        ui->dotsLabel->setText(frames[*i]);
        *i = (*i + 1) % frames.size();
    });

    timer->start(320);
}

/*──────────────────────────────────────────────
 * LEGACY (unused but kept for compatibility)
 *─────────────────────────────────────────────*/
void WelcomeDialog::centerOnScreen() {}

/*──────────────────────────────────────────────
 * WELCOME SEQUENCE METHODS
 *─────────────────────────────────────────────*/
void WelcomeDialog::startWelcomeSequence()
{
    // Start the welcome animation sequence
    fadeInAnimation();
    startDotsAnimation();
    
    // Set a timer to automatically proceed after the welcome sequence
    QTimer::singleShot(3000, this, &WelcomeDialog::closeAndProceed);
}

void WelcomeDialog::closeAndProceed()
{
    // Accept the dialog (equivalent to clicking OK)
    // This will close the dialog and return QDialog::Accepted
    accept();
}
