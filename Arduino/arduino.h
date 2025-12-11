#ifndef ARDUINO_H
#define ARDUINO_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QDebug>
#include <QMap>

class Arduino : public QObject
{
    Q_OBJECT

public:
    explicit Arduino(QObject *parent = nullptr);
    ~Arduino();

    bool connect_arduino();                // auto-detection
    void disconnect_arduino();
    bool isConnected() const;
    QString portName() const;
    QString getarduino_port_name() const; // Méthode manquante

    qint64 writeData(const QByteArray &data);

signals:
    void dataReceived(const QByteArray &data);   // NOUVEAU
    void messageReceived(const QString &message); // Méthode manquante
    void arduinoConnected();
    void arduinoDisconnected();

private slots:
    void handleReadyRead();
    void handleError(QSerialPort::SerialPortError error);
    void checkConnection();

private:
    QSerialPort *m_serial;
    QTimer *m_checkTimer;
    QString m_portName;
    bool m_isConnected;
};

#endif // ARDUINO_H
