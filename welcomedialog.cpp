#include "welcomedialog.h"
#include "ui_welcomedialog.h"

#include <QApplication>
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
    , welcomeTimer(new QTimer(this))
    , m_role(role)
    , m_email(email)
    , m_username(username)
{
    ui->setupUi(this);

    setWindowTitle("Bienvenue - Smart City");
    setModal(true);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setWindowState(Qt::WindowFullScreen);

    setupAnimations();

    connect(welcomeTimer, &QTimer::timeout, this, &WelcomeDialog::closeAndProceed);

    QTimer::singleShot(100, this, &WelcomeDialog::startWelcomeSequence);
}

WelcomeDialog::~WelcomeDialog()
{
    delete ui;
}

void WelcomeDialog::startWelcomeSequence()
{
    if (ui->loadingText) {
        QStringList messages = {
            "Chargement de l'interface...",
            "Connexion à la base de données...",
            "Initialisation des modules...",
            "Ouverture du tableau de bord..."
        };

        QTimer *textTimer = new QTimer(this);
        int messageIndex = 0;

        connect(textTimer, &QTimer::timeout, [this, textTimer, messages, &messageIndex]() {
            if (messageIndex < messages.size()) {
                ui->loadingText->setText(messages[messageIndex]);
                messageIndex++;
            } else {
                textTimer->stop();
                textTimer->deleteLater();
            }
        });

        textTimer->start(750);
    }

    welcomeTimer->setSingleShot(true);
    welcomeTimer->start(3000);
}

void WelcomeDialog::setupAnimations()
{
    QGraphicsOpacityEffect *dialogEffect = new QGraphicsOpacityEffect(this);
    this->setGraphicsEffect(dialogEffect);

    QPropertyAnimation *fadeIn = new QPropertyAnimation(dialogEffect, "opacity");
    fadeIn->setDuration(800);
    fadeIn->setStartValue(0.0);
    fadeIn->setEndValue(1.0);
    fadeIn->setEasingCurve(QEasingCurve::InOutQuad);
    fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
}

void WelcomeDialog::closeAndProceed()
{
    // 🟦 DO NOT create dashboard here
    // 🟦 DO NOT close the parent here
    // 🟦 Just close the dialog and let MainWindow handle everything

    accept();
}

void WelcomeDialog::centerOnScreen() {}
