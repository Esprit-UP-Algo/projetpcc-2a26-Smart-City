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

    buffer += QString::fromUtf8(serial->readAll());

    // Tant qu'il n'y a pas de fin de ligne, on attend
    if (!buffer.contains("\n"))
        return;

    // Séparation des messages (si plusieurs)
    QStringList messages = buffer.split("\n");

    // Le dernier fragment peut être incomplet → on le garde en buffer
    buffer = messages.takeLast();

    // Traiter chaque message complet
    for (QString msg : messages) {
        msg = msg.trimmed();
        if (msg.isEmpty()) continue;

        qDebug() << "[Arduino] Message complet:" << msg;
        emit dataReceived(msg);
    }
}

void Arduino::sendResidentInfo(const QString &nom, const QString &prenom)
{
    if (!serial->isOpen()) {
        qDebug() << "Arduino non connecté — impossible d'envoyer les informations du résident.";
        return;
    }

    // Format: APPROVED:NOM:PRENOM
    QString message = QString("APPROVED:%1:%2\n").arg(nom, prenom);
    serial->write(message.toUtf8());
    
    qDebug() << "[Arduino] Informations du résident envoyées:" << nom << prenom;
}

