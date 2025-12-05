#ifndef ARDUINO_H
#define ARDUINO_H

#include <QObject>
#include <QtSerialPort/QSerialPort>

class Arduino : public QObject
{
    Q_OBJECT

public:
    explicit Arduino(QObject *parent = nullptr);
    ~Arduino();

    bool connectArduino(const QString &portName); // COM3
    void disconnectArduino();
    bool isConnected() const;

    void sendToArduino(const QString &data);      // Envoyer une commande
    void simulateReceived(const QString &data);   // Pour tests sans Arduino

signals:
    void dataReceived(QString data);              // UID reçu
    void connectionStatusChanged(bool connected); // true / false

private slots:
    void onDataAvailable();                       // Lecture QSerialPort

private:
    QSerialPort *serial;
    QString buffer;

};

#endif // ARDUINO_H
