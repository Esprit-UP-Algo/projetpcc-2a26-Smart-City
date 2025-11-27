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

    bool connectArduino(const QString &portName);
    void disconnectArduino();
    bool isConnected() const;

    void sendToArduino(const QString &data);    // envoyer une commande
    void simulateReceived(const QString &data); // pour tests sans matériel

signals:
    void dataReceived(QString data);
    void connectionStatusChanged(bool connected);

private slots:
    void onDataAvailable();

private:
    QSerialPort *serial;
};

#endif // ARDUINO_H
