#include "arduinotransport.h"
#include <QDebug>

ArduinoTransport::ArduinoTransport(QObject *parent)
    : QObject(parent)
    , serial(nullptr)
{
}

ArduinoTransport::~ArduinoTransport()
{
    disconnectArduino();
}

bool ArduinoTransport::connectArduino(const QString &portName)
{
    if (serial && serial->isOpen()) {
        qDebug() << "[ArduinoTransport] Déjà connecté sur" << serial->portName();
        return true;
    }

    serial = new QSerialPort(this);
    serial->setPortName(portName);
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (!serial->open(QIODevice::ReadWrite)) {
        qWarning() << "[ArduinoTransport] Échec connexion sur" << portName << ":" << serial->errorString();
        delete serial;
        serial = nullptr;
        emit connectionStatusChanged(false);
        return false;
    }

    connect(serial, &QSerialPort::readyRead, this, &ArduinoTransport::onDataAvailable);
    
    qDebug() << "[ArduinoTransport] ✅ Connecté sur" << portName;
    emit connectionStatusChanged(true);
    
    // Envoyer confirmation de connexion
    sendToArduino("QT_CONNECTED\n");
    
    return true;
}

void ArduinoTransport::disconnectArduino()
{
    if (serial) {
        if (serial->isOpen()) {
            serial->close();
            qDebug() << "[ArduinoTransport] Déconnecté";
        }
        delete serial;
        serial = nullptr;
        emit connectionStatusChanged(false);
    }
}

bool ArduinoTransport::isConnected() const
{
    return serial && serial->isOpen();
}

void ArduinoTransport::sendToArduino(const QString &data)
{
    if (!serial || !serial->isOpen()) {
        qWarning() << "[ArduinoTransport] Pas de connexion. Message non envoyé:" << data;
        return;
    }
    
    QByteArray bytes = data.toUtf8();
    serial->write(bytes);
    serial->flush();
    
    qDebug() << "[ArduinoTransport] ➤ Envoyé:" << data.trimmed();
}

void ArduinoTransport::sendSuccess(const QString &vehicleID, const QString &montantTotal)
{
    // Format: SUCCESS:ID:B888 TOTAL:15.00DT
    QString message = QString("SUCCESS:ID:%1 TOTAL:%2\n").arg(vehicleID, montantTotal);
    sendToArduino(message);
}

void ArduinoTransport::sendError(const QString &errorMsg)
{
    QString message = QString("ERROR:%1\n").arg(errorMsg);
    sendToArduino(message);
}

void ArduinoTransport::sendVehicleAdded(const QString &vehicleID)
{
    QString message = QString("VEHICLE_ADDED:%1\n").arg(vehicleID);
    sendToArduino(message);
}

void ArduinoTransport::onDataAvailable()
{
    if (!serial) return;
    
    QByteArray data = serial->readAll();
    buffer.append(QString::fromUtf8(data));
    
    // Traiter les lignes complètes
    while (buffer.contains('\n')) {
        int pos = buffer.indexOf('\n');
        QString line = buffer.left(pos).trimmed();
        buffer.remove(0, pos + 1);
        
        if (line.isEmpty()) continue;
        
        qDebug() << "[ArduinoTransport] ◄ Reçu:" << line;
        
        // Parser les messages de la borne
        if (line.startsWith("VEHICLE_CODE:")) {
            // Format: VEHICLE_CODE:B888:5.00
            QStringList parts = line.split(':');
            if (parts.size() >= 3) {
                QString code = parts[1].trimmed();
                QString montant = parts[2].trimmed();
                
                qDebug() << "[ArduinoTransport] 🚗 Code véhicule:" << code << "Montant:" << montant;
                emit vehicleCodeReceived(code, montant);
            }
        }
        else if (line.startsWith("DEBUG:") || line.startsWith("STATUS:")) {
            // Messages de debug/status - juste logger
            qDebug() << "[ArduinoTransport]" << line;
        }
    }
}
