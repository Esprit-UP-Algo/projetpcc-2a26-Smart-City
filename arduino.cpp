#include "arduino.h"
#include <QDebug>

Arduino::Arduino(QObject *parent)
    : QObject(parent),
    serial(new QSerialPort(this))
{
    // Lorsque de nouvelles données arrivent via USB
    connect(serial, &QSerialPort::readyRead,
            this, &Arduino::onDataAvailable);
}

Arduino::~Arduino()
{
    disconnectArduino();
}

bool Arduino::connectArduino(const QString &portName)
{
    if (serial->isOpen())
        serial->close();

    serial->setPortName(portName);       // ex: "COM3"
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadWrite)) {
        emit connectionStatusChanged(true);
        qDebug() << "Arduino connecté sur" << portName;
        return true;
    }

    qDebug() << "Erreur connexion Arduino :" << serial->errorString();
    emit connectionStatusChanged(false);
    return false;
}

void Arduino::disconnectArduino()
{
    if (serial->isOpen()) {
        serial->close();
        emit connectionStatusChanged(false);
        qDebug() << "Arduino déconnecté.";
    }
}

bool Arduino::isConnected() const
{
    return serial->isOpen();
}

void Arduino::sendToArduino(const QString &data)
{
    if (!serial->isOpen()) {
        qDebug() << "Arduino non connecté — impossible d'envoyer.";
        return;
    }

    serial->write(data.toUtf8());
}

void Arduino::simulateReceived(const QString &data)
{
    emit dataReceived(data);
}

void Arduino::onDataAvailable()
{
    if (!serial->isOpen())
        return;

    QString data = QString::fromUtf8(serial->readAll()).trimmed();

    if (!data.isEmpty()) {
        qDebug() << "[Arduino] Reçu :" << data;
        emit dataReceived(data);
    }
}
