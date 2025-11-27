#include "arduino.h"
#include <QDebug>

Arduino::Arduino(QObject *parent)
    : QObject(parent), serial(new QSerialPort(this))
{
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

    serial->setPortName(portName);
    serial->setBaudRate(QSerialPort::Baud9600);

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
    }
}

bool Arduino::isConnected() const
{
    return serial->isOpen();
}

void Arduino::sendToArduino(const QString &data)
{
    if (serial->isOpen())
        serial->write(data.toUtf8());
}

void Arduino::simulateReceived(const QString &data)
{
    // Permet de tester Qt SANS Arduino physique
    emit dataReceived(data);
}

void Arduino::onDataAvailable()
{
    QString data = QString::fromUtf8(serial->readAll());
    emit dataReceived(data);
}
