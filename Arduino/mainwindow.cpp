#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "welcomedialog.h"
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QSequentialAnimationGroup>
#include <QPixmap>
#include <QMessageBox>
#include <QStatusBar>
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>

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
    
    // 🔥 FORCER L'AFFICHAGE COMPLET
    setMinimumSize(1200, 800);  // Taille minimum
    resize(1400, 900);          // Taille par défaut  
    showMaximized();            // Maximiser
    setWindowState(Qt::WindowMaximized); // Forcer maximisé

    // ===== INDICATEUR DE STATUT ARDUINO =====
    setupArduinoStatusIndicator();

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

// ===== MÉTHODES DE GESTION DU STATUT ARDUINO =====

void MainWindow::setupArduinoStatusIndicator()
{
    // Initialiser Arduino
    arduino = new Arduino(this);
    
    // Connecter les signaux - PROPRE SANS SPAM
    connect(arduino, &Arduino::messageReceived, this, &MainWindow::onArduinoMessage);
    
    // Créer une barre de statut si elle n'existe pas
    if (!statusBar()) {
        setStatusBar(new QStatusBar(this));
    }
    
    // Créer le label de statut Arduino
    arduinoStatusLabel = new QLabel(this);
    arduinoStatusLabel->setMinimumWidth(250);
    arduinoStatusLabel->setStyleSheet(
        "QLabel {"
        "    padding: 5px 10px;"
        "    border-radius: 5px;"
        "    font-weight: bold;"
        "    font-size: 12px;"
        "}"
    );
    
    // Ajouter à la barre de statut (à droite)
    statusBar()->addPermanentWidget(arduinoStatusLabel);
    
    // Configuration initiale
    updateArduinoStatus();
    
    // Timer pour vérifier périodiquement la connexion Arduino
    arduinoCheckTimer = new QTimer(this);
    connect(arduinoCheckTimer, &QTimer::timeout, this, &MainWindow::checkArduinoConnection);
    arduinoCheckTimer->start(10000); // Vérifier toutes les 10 secondes
    
    qDebug() << "🔍 [UI] Indicateur de statut Arduino configuré";
}

void MainWindow::updateArduinoStatus()
{
    if (arduino && arduino->isConnected()) {
        // Arduino connecté
        arduinoStatusLabel->setText(QString("🟢 Arduino: Connecté (%1)").arg(arduino->getarduino_port_name()));
        arduinoStatusLabel->setStyleSheet(
            "QLabel {"
            "    background-color: #d4edda;"
            "    color: #155724;"
            "    border: 1px solid #c3e6cb;"
            "    padding: 5px 10px;"
            "    border-radius: 5px;"
            "    font-weight: bold;"
            "    font-size: 12px;"
            "}"
        );
        qDebug() << "✅ [ARDUINO] Statut: Connecté sur" << arduino->getarduino_port_name();
    } else {
        // Arduino déconnecté
        arduinoStatusLabel->setText("🔴 Arduino: Déconnecté");
        arduinoStatusLabel->setStyleSheet(
            "QLabel {"
            "    background-color: #f8d7da;"
            "    color: #721c24;"
            "    border: 1px solid #f5c6cb;"
            "    padding: 5px 10px;"
            "    border-radius: 5px;"
            "    font-weight: bold;"
            "    font-size: 12px;"
            "}"
        );
        qDebug() << "❌ [ARDUINO] Statut: Déconnecté";
    }
}

void MainWindow::checkArduinoConnection()
{
    qDebug() << "🔍 [ARDUINO] Vérification de la connexion";
    
    // Si pas connecté, essayer de se reconnecter
    if (!arduino->isConnected()) {
        if (arduino->connect_arduino() == 1) {
            qDebug() << "🔄 [ARDUINO] Reconnexion réussie";
            onArduinoConnected();
        }
    }
    
    updateArduinoStatus();
}

void MainWindow::onArduinoConnected()
{
    qDebug() << "🔌 [ARDUINO] Connexion établie";
    updateArduinoStatus();
}

void MainWindow::onArduinoDisconnected()
{
    qDebug() << "🔌 [ARDUINO] Connexion perdue";
    updateArduinoStatus();
}

void MainWindow::onArduinoDataReceived(const QByteArray &data)
{
    Q_UNUSED(data)  // Supprime l'avertissement de paramètre non utilisé
    
    // Garde juste pour le statut visuel - pas de spam
    if (arduino && arduino->isConnected()) {
        arduinoStatusLabel->setText(QString("🟢 Arduino: Actif (%1)").arg(arduino->getarduino_port_name()));
        QTimer::singleShot(2000, this, &MainWindow::updateArduinoStatus);
    }
}

// 🔥 NOUVELLE FONCTION - TRANSFERT VERS TRANSPORTPAGE POUR VALIDATION D'ABORD
void MainWindow::onArduinoMessage(const QString &msg)
{
    // 🔍 DEBUG: Afficher tous les messages reçus sans spam
    if (msg.startsWith("DEBUG:")) {
        return;
    }
    
    qDebug() << "📡 [MAINWINDOW] Message Arduino reçu:" << msg;
    
    // 🔥 REDIRECTION VERS TRANSPORTPAGE POUR VALIDATION VÉHICULE
    // TransportPage vérifie si le véhicule existe et crée la transaction
    if (msg.startsWith("VEHICLE_CODE:")) {
        // Format: VEHICLE_CODE:A123:5.00
        QStringList parts = msg.split(":");
        if (parts.size() >= 3) {
            QString vehicleCode = parts[1].trimmed();
            QString amount = parts[2].trimmed();
            
            qDebug() << "🚗 [MAINWINDOW] Code véhicule à valider:" << vehicleCode << "Montant:" << amount;
            
            // Signaler à TransportPage pour validation et traitement
            emit vehicleCodeForValidation(vehicleCode);
        }
        return;
    }
    
    // Autres messages...
}

// 🔥 NOUVELLE MÉTHODE - ENVOYER RÉPONSE À ARDUINO
void MainWindow::sendArduinoResponse(const QString &response)
{
    if (arduino && arduino->isConnected()) {
        arduino->writeData(response.toUtf8());
    }
}
