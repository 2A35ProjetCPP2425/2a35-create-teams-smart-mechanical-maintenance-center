#ifndef ARDUINO_H
#define ARDUINO_H

#include <QCoreApplication>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>

class Arduino
{
public:
    Arduino();                               // Constructeur
    int connect_arduino();                    // Connexion à Arduino
    int close_arduino();                      // Fermeture de la connexion
    void write_to_arduino(QByteArray);        // Écriture vers Arduino
    QByteArray read_from_arduino();           // Lecture depuis Arduino
    QSerialPort* getserial();                 // Obtenir l'objet QSerialPort
    QString getarduino_port_name();           // Obtenir le nom du port Arduino
    void sendMessage(const QString &message);
private:
    QSerialPort *serial;                      // Objet QSerialPort
    static const quint16 arduino_uno_vendor_id = 9025;  // Vendor ID de l'Arduino Uno
    static const quint16 arduino_uno_product_id = 67;   // Product ID de l'Arduino Uno
    QString arduino_port_name;                // Nom du port Arduino
    bool arduino_is_available;                // Indicateur de disponibilité
    QByteArray data;                          // Tampon de données
};

#endif // ARDUINO_H
