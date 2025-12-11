#ifndef ARDUINOTRANSPORT_H
#define ARDUINOTRANSPORT_H

#include <QObject>
#include <QtSerialPort/QSerialPort>

class ArduinoTransport : public QObject
{
    Q_OBJECT

public:
    explicit ArduinoTransport(QObject *parent = nullptr);
    ~ArduinoTransport();

    bool connectArduino(const QString &portName); // COM5 pour borne transport
    void disconnectArduino();
    bool isConnected() const;

    void sendToArduino(const QString &data);      // Envoyer une réponse à la borne
    void sendSuccess(const QString &vehicleID, const QString &montantTotal);  // Succès paiement
    void sendError(const QString &errorMsg);      // Erreur
    void sendVehicleAdded(const QString &vehicleID); // Nouveau véhicule ajouté

signals:
    void vehicleCodeReceived(QString code, QString montant);  // Code véhicule reçu depuis borne
    void connectionStatusChanged(bool connected);             // État connexion

private slots:
    void onDataAvailable();  // Lecture données série

private:
    QSerialPort *serial;
    QString buffer;
};

#endif // ARDUINOTRANSPORT_H
