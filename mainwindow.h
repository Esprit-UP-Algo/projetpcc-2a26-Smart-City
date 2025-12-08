#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QLabel>
#include <QTimer>
#include "arduino.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void on_loginButton_clicked();
    void on_fullscreenButton_clicked();
    void onArduinoMessage(const QString &msg);
    void onArduinoDataReceived(const QByteArray &data);
    void onArduinoConnected();
    void onArduinoDisconnected();

public slots:
    void sendArduinoResponse(const QString &response);  // 🔥 NOUVEAU

signals:
    void arduinoDataForFinance(const QByteArray &data);  // 🔥 SIGNAL POUR FINANCEPAGE
    void vehicleCodeForValidation(const QString &code);  // 🔥 NOUVEAU SIGNAL POUR TRANSPORTPAGE

private:
    Ui::MainWindow *ui;
    void shakeWidget(QWidget *widget);
    void performLogin();
    void showErrorMessage(const QString &message);
    bool isLoggingIn;
    
    // ===== GESTION STATUT ARDUINO =====
    Arduino *arduino;
    QLabel *arduinoStatusLabel;
    QTimer *arduinoCheckTimer;
    QString currentVehicleCode;  // 🔥 NOUVEAU
    
    void setupArduinoStatusIndicator();
    void updateArduinoStatus();
    void checkArduinoConnection();

signals:
    void vehiclePaymentRequested(const QString &codeVehicule);  // 🔥 NOUVEAU

};
#endif // MAINWINDOW_H
