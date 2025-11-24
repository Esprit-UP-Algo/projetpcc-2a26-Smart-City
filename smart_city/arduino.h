#ifndef ARDUINO_H
#define ARDUINO_H

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QByteArray>

class Arduino
{
public:
    Arduino();
    int connect_arduino();            // connecter le PC à Arduino
    int close_arduino();              // fermer la connexion
    void write_to_arduino(QByteArray data); // envoyer des données
    QByteArray read_from_arduino();   // lire les données depuis Arduino
    QSerialPort* getSerial();         // accesseur
    QString getArduinoPortName();

private:
    QSerialPort serial;               // objet pour la communication série
    static const quint16 arduino_uno_vendor_id = 0x1A86;  // 6790
    static const quint16 arduino_uno_product_id = 0x7523; // 29987

    QString arduino_port_name;
    bool arduino_is_available;
    QByteArray data;
};

#endif // ARDUINO_H
