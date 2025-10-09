#include "welcomedialog.h"
#include "ui_welcomedialog.h"
#include "dashboardwindow.h"
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QEasingCurve>

WelcomeDialog::WelcomeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WelcomeDialog)
{
    ui->setupUi(this);

    // --- Frameless full-screen dialog ---
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setWindowState(Qt::WindowFullScreen);

    // ❌ Remove translucency (it causes dark background)
    // setAttribute(Qt::WA_TranslucentBackground);

    // --- Modern light gradient background (luminous bleu clair) ---
    this->setStyleSheet(R"(
        QDialog {
            background: qlineargradient(
                x1:0, y1:0, x2:1, y2:1,
                stop:0 #E9F2FF,
                stop:1 #BFD8FF
            );
        }
        QLabel#welcomeLabel {
            font-family: "Segoe UI Semibold";
            font-size: 72px;
            font-weight: bold;
            color: #1E3A8A;
            background-color: rgba(255, 255, 255, 0.85);
            border: 4px solid #1E3A8A;
            border-radius: 30px;
            padding: 40px;
            qproperty-alignment: AlignCenter;
            box-shadow: 0px 0px 20px rgba(0, 0, 0, 0.1);
        }
    )");

    // Center label (just in case)
    ui->welcomeLabel->setAlignment(Qt::AlignCenter);
}

WelcomeDialog::~WelcomeDialog()
{
    delete ui;
}

void WelcomeDialog::showWithAnimation()
{
    show();
    setupAnimations();
    QTimer::singleShot(3000, this, &WelcomeDialog::closeWithAnimation);
}

void WelcomeDialog::setupAnimations()
{
    QGraphicsOpacityEffect *textOpacity = new QGraphicsOpacityEffect(ui->welcomeLabel);
    ui->welcomeLabel->setGraphicsEffect(textOpacity);

    QPropertyAnimation *textFade = new QPropertyAnimation(textOpacity, "opacity");
    textFade->setDuration(1200);
    textFade->setStartValue(0.0);
    textFade->setEndValue(1.0);
    textFade->setEasingCurve(QEasingCurve::InOutCubic);
    textFade->start(QAbstractAnimation::DeleteWhenStopped);
}

void WelcomeDialog::closeWithAnimation()
{
    QGraphicsOpacityEffect *textOpacity = new QGraphicsOpacityEffect(ui->welcomeLabel);
    ui->welcomeLabel->setGraphicsEffect(textOpacity);

    QPropertyAnimation *fadeOut = new QPropertyAnimation(textOpacity, "opacity");
    fadeOut->setDuration(1000);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.0);
    fadeOut->setEasingCurve(QEasingCurve::InOutCubic);
    connect(fadeOut, &QPropertyAnimation::finished, this, &WelcomeDialog::accept);
    fadeOut->start(QAbstractAnimation::DeleteWhenStopped);
}
