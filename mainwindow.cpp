#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "welcomedialog.h"
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QSequentialAnimationGroup>
#include <QPixmap>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , isLoggingIn(false)
{
    ui->setupUi(this);

    // Load the NEXORA logo from resources
    QPixmap logo(":/resources/nexorab.png");
    if (!logo.isNull() && ui->logoLabel) {
        ui->logoLabel->setPixmap(logo.scaled(220, 220, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        ui->logoLabel->setAlignment(Qt::AlignCenter);
        ui->logoLabel->setScaledContents(true);
    }

    setWindowTitle("Smart City - Connexion");
    showMaximized();

    if (ui->errorLabel) ui->errorLabel->hide();

    // Connect Enter key events if line edits exist
    if (ui->emailLineEdit) connect(ui->emailLineEdit, &QLineEdit::returnPressed, this, &MainWindow::performLogin);
    if (ui->passwordLineEdit) connect(ui->passwordLineEdit, &QLineEdit::returnPressed, this, &MainWindow::performLogin);

    // Fade-in illustration if present
    if (ui->illustrationLabel) {
        QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(ui->illustrationLabel);
        ui->illustrationLabel->setGraphicsEffect(opacityEffect);

        QPropertyAnimation *fadeIn = new QPropertyAnimation(opacityEffect, "opacity");
        fadeIn->setDuration(1500);
        fadeIn->setStartValue(0.0);
        fadeIn->setEndValue(1.0);
        fadeIn->setEasingCurve(QEasingCurve::InOutQuad);
        fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loginButton_clicked()
{
    performLogin();
}

void MainWindow::performLogin()
{
    if (isLoggingIn) return;
    isLoggingIn = true;

    QString email;
    QString password;
    if (ui->emailLineEdit) email = ui->emailLineEdit->text();
    if (ui->passwordLineEdit) password = ui->passwordLineEdit->text();

    if (ui->errorLabel) ui->errorLabel->hide();

    if (email.isEmpty() || password.isEmpty()) {
        showErrorMessage("Veuillez remplir tous les champs.");
        if (ui->emailLineEdit) shakeWidget(ui->emailLineEdit);
        if (ui->passwordLineEdit) shakeWidget(ui->passwordLineEdit);
        isLoggingIn = false;
        return;
    }

    // Temporary credential check: username = "smart city", password = "0000"
    if (email == "smart city" && password == "0000") {
        // Show welcome dialog like in Nexora
        WelcomeDialog *welcomeDialog = new WelcomeDialog(this);
        this->hide(); // Hide login window
        welcomeDialog->exec(); // Show welcome dialog modally
        delete welcomeDialog;
    } else {
        showErrorMessage("Incorrect ! Nom d'utilisateur ou mot de passe invalide.");
        if (ui->emailLineEdit) shakeWidget(ui->emailLineEdit);
        if (ui->passwordLineEdit) shakeWidget(ui->passwordLineEdit);
    }

    isLoggingIn = false;
}

void MainWindow::showErrorMessage(const QString &message)
{
    if (ui->errorLabel) {
        ui->errorLabel->setText(message);
        ui->errorLabel->show();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        bool focused = (ui->emailLineEdit && ui->emailLineEdit->hasFocus()) ||
                       (ui->passwordLineEdit && ui->passwordLineEdit->hasFocus()) ||
                       (ui->loginButton && ui->loginButton->hasFocus());
        if (focused) {
            performLogin();
            return;
        }
    }
    QMainWindow::keyPressEvent(event);
}

void MainWindow::shakeWidget(QWidget *widget)
{
    if (!widget) return;
    QRect originalGeometry = widget->geometry();

    QSequentialAnimationGroup *shakeGroup = new QSequentialAnimationGroup(this);
    int shakeDistance = 10;
    int shakeDuration = 50;

    for (int i = 0; i < 4; i++) {
        QPropertyAnimation *moveRight = new QPropertyAnimation(widget, "geometry");
        moveRight->setDuration(shakeDuration);
        QRect rightRect = originalGeometry;
        rightRect.moveLeft(originalGeometry.left() + shakeDistance);
        moveRight->setEndValue(rightRect);
        shakeGroup->addAnimation(moveRight);

        QPropertyAnimation *moveLeft = new QPropertyAnimation(widget, "geometry");
        moveLeft->setDuration(shakeDuration);
        QRect leftRect = originalGeometry;
        leftRect.moveLeft(originalGeometry.left() - shakeDistance);
        moveLeft->setEndValue(leftRect);
        shakeGroup->addAnimation(moveLeft);
    }

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
