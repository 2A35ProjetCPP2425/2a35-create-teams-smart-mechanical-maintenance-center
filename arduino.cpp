#include "arduino.h"
#include <QDebug>

Arduino::Arduino() {
    serial = new QSerialPort();  // Initialiser le pointeur serial
    arduino_is_available = false; // Statut par défaut de la disponibilité
    arduino_port_name = "";       // Nom de port par défaut
}

int Arduino::connect_arduino() {
    foreach (const QSerialPortInfo &serial_port_info, QSerialPortInfo::availablePorts()) {
        if (serial_port_info.hasVendorIdentifier() && serial_port_info.hasProductIdentifier()) {
            if (serial_port_info.vendorIdentifier() == arduino_uno_vendor_id &&
                serial_port_info.productIdentifier() == arduino_uno_product_id) {
                arduino_is_available = true;
                arduino_port_name = serial_port_info.portName();
                break;  // Exit loop once Arduino is found
            }
        }
    }

    qDebug() << "arduino_port_name is:" << arduino_port_name;
    if (arduino_is_available) {
        serial->setPortName(arduino_port_name);
        if (serial->open(QSerialPort::ReadWrite)) {
            serial->setBaudRate(QSerialPort::Baud9600);
            serial->setDataBits(QSerialPort::Data8);
            serial->setParity(QSerialPort::NoParity);
            serial->setStopBits(QSerialPort::OneStop);
            serial->setFlowControl(QSerialPort::NoFlowControl);
            return 0;  // Success
        } else {
            qDebug() << "Failed to open serial port:" << serial->errorString();
        }
    } else {
        qDebug() << "Arduino not found.";
    }

    return 1;  // Failure
}

QByteArray Arduino::read_from_arduino() {
    if (serial->isReadable()) {
        data = serial->readAll();
        return data;
    } else {
        qDebug() << "Port série non lisible.";
        return QByteArray();  // Retourne un QByteArray vide
    }
}
void Arduino::write_to_arduino(QByteArray d) {
    if (serial->isWritable()) {
        serial->write(d);
    } else {
        qDebug() << "Impossible d'écrire sur le port série!";
    }
}

int Arduino::close_arduino() {
    if (serial->isOpen()) {
        serial->close();
        return 0;  // Fermé avec succès
    }
    return 1;  // Échec de la fermeture
}

QString Arduino::getarduino_port_name() {
    return arduino_port_name;
}

QSerialPort* Arduino::getserial() {
    return serial;
}
void Arduino::sendMessage(const QString &message) {
    if (serial->isOpen() && serial->isWritable()) {
        serial->write(message.toUtf8() + '\n'); // Envoi du message avec un saut de ligne
    }
}
