#include "arduino.h"

Arduino::Arduino()
{
    arduino_is_available = false;
    arduino_port_name = "";
}

int Arduino::connect_arduino()
{
    // Recherche du port sur lequel Arduino est connecté
    foreach (const QSerialPortInfo &serial_port_info, QSerialPortInfo::availablePorts()) {
        if (serial_port_info.hasVendorIdentifier() && serial_port_info.hasProductIdentifier()) {
            if (serial_port_info.vendorIdentifier() == arduino_uno_vendor_id &&
                serial_port_info.productIdentifier() == arduino_uno_product_id) {
                arduino_is_available = true;
                arduino_port_name = serial_port_info.portName();
                break;
            }
        }
    }

    qDebug() << "arduino_port_name is:" << arduino_port_name;

    if (arduino_is_available) {
        serial.setPortName(arduino_port_name);
        if (serial.open(QSerialPort::ReadWrite)) {
            serial.setBaudRate(QSerialPort::Baud9600);
            serial.setDataBits(QSerialPort::Data8);
            serial.setParity(QSerialPort::NoParity);
            serial.setStopBits(QSerialPort::OneStop);
            serial.setFlowControl(QSerialPort::NoFlowControl);
            return 0; // connexion OK
        }
        return 1; // échec ouverture port
    }

    return -1; // Arduino non trouvé
}

int Arduino::close_arduino()
{
    if (serial.isOpen())
        serial.close();
    return 0;
}

void Arduino::write_to_arduino(QByteArray data)
{
    if (serial.isOpen()) {
        serial.write(data);
    }
}

QByteArray Arduino::read_from_arduino()
{
    if (serial.isOpen() && serial.waitForReadyRead(100)) {
        data = serial.readAll();
        while (serial.waitForReadyRead(10))
            data += serial.readAll();
    }
    return data;
}

QSerialPort* Arduino::getSerial()
{
    return &serial;
}

QString Arduino::getArduinoPortName()
{
    return arduino_port_name;
}
