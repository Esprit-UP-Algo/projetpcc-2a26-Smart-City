#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "welcomedialog.h"
#include "dashboardwindow.h"
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QSequentialAnimationGroup>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 🖼️ Set the NEXORA logo (transparent PNG)
    QPixmap logo(":/nexorab.png");
    ui->logoLabel->setPixmap(logo.scaled(220, 220, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->logoLabel->setAlignment(Qt::AlignCenter);
    ui->logoLabel->setScaledContents(true);


    setWindowTitle("REERUI - Connexion");
    showMaximized();

    // Hide error label initially
    ui->errorLabel->hide();

    // Add fade-in animation to the illustration (plays once)
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(ui->illustrationLabel);
    ui->illustrationLabel->setGraphicsEffect(opacityEffect);

    QPropertyAnimation *fadeIn = new QPropertyAnimation(opacityEffect, "opacity");
    fadeIn->setDuration(1500);
    fadeIn->setStartValue(0.0);
    fadeIn->setEndValue(1.0);
    fadeIn->setEasingCurve(QEasingCurve::InOutQuad);
    fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loginButton_clicked()
{
    QString email = ui->emailLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    // Hide error label initially
    ui->errorLabel->hide();

    if (email.isEmpty() || password.isEmpty()) {
        ui->errorLabel->setText("Veuillez remplir tous les champs.");
        ui->errorLabel->show();
        shakeWidget(ui->emailLineEdit);
        shakeWidget(ui->passwordLineEdit);
        return;
    }

    // Check credentials: username = "smart city", password = "0000"
    if (email == "smart city" && password == "0000") {
        // Hide the login window
        this->hide();

        // Show welcome dialog with animation
        WelcomeDialog *welcomeDialog = new WelcomeDialog(this);
        welcomeDialog->showWithAnimation();
        int result = welcomeDialog->exec();

        if (result == QDialog::Accepted) {
            // Close login window
            this->close();

            // Open dashboard window
            DashboardWindow *dashboard = new DashboardWindow();
            dashboard->showFullScreen();
            dashboard->show();
        }

        delete welcomeDialog;
    } else {
        // Show red error text and shake animation
        ui->errorLabel->setText("Incorrect ! Nom d'utilisateur ou mot de passe invalide.");
        ui->errorLabel->show();
        shakeWidget(ui->emailLineEdit);
        shakeWidget(ui->passwordLineEdit);
    }
}

void MainWindow::shakeWidget(QWidget *widget)
{
    // Get the original geometry
    QRect originalGeometry = widget->geometry();

    // Create a sequential animation group for the shake effect
    QSequentialAnimationGroup *shakeGroup = new QSequentialAnimationGroup(this);

    // Shake left and right multiple times
    int shakeDistance = 10;
    int shakeDuration = 50;

    for (int i = 0; i < 4; i++) {
        // Move right
        QPropertyAnimation *moveRight = new QPropertyAnimation(widget, "geometry");
        moveRight->setDuration(shakeDuration);
        QRect rightRect = originalGeometry;
        rightRect.moveLeft(originalGeometry.left() + shakeDistance);
        moveRight->setEndValue(rightRect);
        shakeGroup->addAnimation(moveRight);

        // Move left
        QPropertyAnimation *moveLeft = new QPropertyAnimation(widget, "geometry");
        moveLeft->setDuration(shakeDuration);
        QRect leftRect = originalGeometry;
        leftRect.moveLeft(originalGeometry.left() - shakeDistance);
        moveLeft->setEndValue(leftRect);
        shakeGroup->addAnimation(moveLeft);
    }

    // Return to original position
    QPropertyAnimation *returnToOriginal = new QPropertyAnimation(widget, "geometry");
    returnToOriginal->setDuration(shakeDuration);
    returnToOriginal->setEndValue(originalGeometry);
    shakeGroup->addAnimation(returnToOriginal);

    shakeGroup->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::on_fullscreenButton_clicked()
{
    if (isFullScreen()) {
        showMaximized();
    } else {
        showFullScreen();
    }
}
