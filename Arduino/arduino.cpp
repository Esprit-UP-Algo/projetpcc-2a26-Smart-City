#include "arduino.h"
#include <QDebug>

Arduino::Arduino(QObject *parent)
    : QObject(parent),
    m_serial(new QSerialPort(this)),
    m_checkTimer(new QTimer(this)),
    m_isConnected(false)
{
    m_serial->setBaudRate(QSerialPort::Baud9600);

    connect(m_serial, &QSerialPort::readyRead,
            this, &Arduino::handleReadyRead);

    connect(m_serial, &QSerialPort::errorOccurred,
            this, &Arduino::handleError);

    connect(m_checkTimer, &QTimer::timeout,
            this, &Arduino::checkConnection);

    m_checkTimer->start(3000);
}

Arduino::~Arduino()
{
    if (m_serial->isOpen())
        m_serial->close();
}

bool Arduino::connect_arduino()
{
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

    for (const QSerialPortInfo &p : ports)
    {
        if (!p.portName().startsWith("COM"))
            continue;

        m_serial->setPortName(p.portName());

        if (m_serial->open(QIODevice::ReadWrite))
        {
            m_isConnected = true;
            m_portName = p.portName();
            // qInfo() << "[Arduino] Connecté sur" << m_portName;

            emit arduinoConnected();
            return true;
        }
    }

    // qWarning() << "[Arduino] Aucun port COM détecté !";
    return false;
}

void Arduino::disconnect_arduino()
{
    if (m_serial->isOpen())
    {
        // qInfo() << "[Arduino] Déconnexion du port" << m_serial->portName();
        m_serial->close();
    }
    m_isConnected = false;
    emit arduinoDisconnected();
}

bool Arduino::isConnected() const
{
    return m_serial->isOpen();
}

QString Arduino::portName() const
{
    return m_portName;
}

QString Arduino::getarduino_port_name() const
{
    return m_portName;
}

qint64 Arduino::writeData(const QByteArray &data)
{
    if (!m_serial || !m_serial->isOpen())
    {
        // qWarning() << "[Arduino] writeData() appelé alors que le port n'est pas ouvert";
        return -1;
    }

    // qDebug() << "[Arduino] >>" << data.trimmed();
    return m_serial->write(data);
}

void Arduino::handleReadyRead()
{
    QByteArray data = m_serial->readAll();
    // qDebug() << "[Arduino] <<" << data;
    emit dataReceived(data);
    
    // Parser le protocole structuré VEHICLE:xxx AMOUNT:xxx ACTION:xxx END
    static QString buffer;
    buffer += QString::fromUtf8(data);
    
    QStringList lines = buffer.split('\n', Qt::KeepEmptyParts);
    buffer = lines.takeLast(); // Garder la dernière ligne partielle
    
    static QMap<QString, QString> transaction;
    static bool inTransaction = false;
    
    for (const QString &line : lines) {
        QString cleanLine = line.trimmed();
        if (cleanLine.isEmpty()) continue;
        
        if (cleanLine.startsWith("VEHICLE:")) {
            inTransaction = true;
            transaction.clear();
            transaction["vehicle"] = cleanLine.mid(8); // Après "VEHICLE:"
        }
        else if (cleanLine.startsWith("AMOUNT:")) {
            transaction["amount"] = cleanLine.mid(7); // Après "AMOUNT:"
        }
        else if (cleanLine.startsWith("ACTION:")) {
            transaction["action"] = cleanLine.mid(7); // Après "ACTION:"
        }
        else if (cleanLine == "END" && inTransaction) {
            // Transaction complète reçue - émettre le signal
            QString vehicleCode = transaction.value("vehicle");
            QString amount = transaction.value("amount");
            QString action = transaction.value("action");
            
            if (!vehicleCode.isEmpty() && action == "PAYMENT") {
                QString fullMessage = QString("PAYMENT_REQUEST:VEHICLE:%1:AMOUNT:%2")
                                      .arg(vehicleCode).arg(amount);
                emit messageReceived(fullMessage);
            }
            
            inTransaction = false;
            transaction.clear();
        }
        else {
            // Messages simples (DEBUG, etc.)
            emit messageReceived(cleanLine);
        }
    }
}

void Arduino::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError)
    {
        // qWarning() << "[Arduino] Ressource perdue, déconnexion...";
        disconnect_arduino();
    }
}

void Arduino::checkConnection()
{
    if (!m_serial->isOpen() && m_isConnected)
    {
        // qWarning() << "[Arduino] Connexion perdue";
        m_isConnected = false;
        emit arduinoDisconnected();
    }
}
