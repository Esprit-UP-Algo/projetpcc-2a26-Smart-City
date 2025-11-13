#include "welcomedialog.h"
#include "ui_welcomedialog.h"
#include "nexoradashboard.h"
#include <QApplication>
#include <QScreen>
#include <QTimer>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QSequentialAnimationGroup>

WelcomeDialog::WelcomeDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::WelcomeDialog)
    , welcomeTimer(new QTimer(this))
{
    ui->setupUi(this);
    
    // Set window properties for full screen
    setWindowTitle("Bienvenue - Smart City");
    setModal(true);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setWindowState(Qt::WindowFullScreen);
    
    // Setup animations
    setupAnimations();
    
    // Connect timer to close dialog after welcome sequence
    connect(welcomeTimer, &QTimer::timeout, this, &WelcomeDialog::closeAndProceed);
    
    // Start welcome sequence
    QTimer::singleShot(100, this, &WelcomeDialog::startWelcomeSequence);
}

WelcomeDialog::~WelcomeDialog()
{
    delete ui;
}

void WelcomeDialog::startWelcomeSequence()
{
    // Simple loading text animation
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
            if (messageIndex < messages.size() && ui->loadingText) {
                ui->loadingText->setText(messages[messageIndex]);
                messageIndex++;
            } else {
                textTimer->stop();
                textTimer->deleteLater();
            }
        });
        
        textTimer->start(750); // Change message every 750ms
    }
    
    // Show welcome dialog for 3 seconds
    welcomeTimer->setSingleShot(true);
    welcomeTimer->start(3000);
}

void WelcomeDialog::setupAnimations()
{
    // Simple fade in effect for the entire dialog
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
    // Accept dialog and close
    accept();
    
    // Create and show dashboard
    NexoraDashboard *dashboard = new NexoraDashboard();
    dashboard->show();
    
    // Close the parent window (login window)
    if (parent()) {
        QWidget *parentWidget = qobject_cast<QWidget*>(parent());
        if (parentWidget) {
            parentWidget->close();
        }
    }
}

void WelcomeDialog::centerOnScreen()
{
    // Get primary screen
    QScreen *screen = QApplication::primaryScreen();
    if (!screen) return;
    
    // Get screen geometry
    QRect screenGeometry = screen->geometry();
    
    // Calculate center position
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    
    // Move to center
    move(x, y);
}
