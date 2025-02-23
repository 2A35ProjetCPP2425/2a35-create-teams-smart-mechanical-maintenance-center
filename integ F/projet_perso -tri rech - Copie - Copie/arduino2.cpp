#include "arduino2.h"

#include "QSerialPortInfo"
#include "qDebug"
arduino2::arduino2()
{
    data="";
    arduino_port_name="";
    arduino_is_available=false;
    serial=new QSerialPort;
}


QString arduino2::getarduino_port_name()
{
    return arduino_port_name;
}

QSerialPort *arduino2::getserial()
{
    return serial;
}
int arduino2::connect_arduino()
{   // recherche du port sur lequel la carte arduino identifée par  arduino_uno_vendor_id
    // est connectée
    foreach (const QSerialPortInfo &serial_port_info, QSerialPortInfo::availablePorts()){
        if(serial_port_info.hasVendorIdentifier() && serial_port_info.hasProductIdentifier()){
            if(serial_port_info.vendorIdentifier() == arduino_uno_vendor_id && serial_port_info.productIdentifier()
                                                                                    == arduino_uno_producy_id) {
                arduino_is_available = true;
                arduino_port_name=serial_port_info.portName();
            } } }
    qDebug() << "arduino_port_name is :" << arduino_port_name;
    if(arduino_is_available){ // configuration de la communication ( débit...)
        serial->setPortName(arduino_port_name);
        if(serial->open(QSerialPort::ReadWrite)){
            serial->setBaudRate(QSerialPort::Baud9600); // débit : 9600 bits/s
            serial->setDataBits(QSerialPort::Data8); //Longueur des données : 8 bits,
            serial->setParity(QSerialPort::NoParity); //1 bit de parité optionnel
            serial->setStopBits(QSerialPort::OneStop); //Nombre de bits de stop : 1
            serial->setFlowControl(QSerialPort::NoFlowControl);
            return 0;
        }
        return 1;
    }
    return -1;
}

int arduino2::close_arduino()

{

    if(serial->isOpen()){
        serial->close();
        return 0;
    }
    return 1;


}


QByteArray arduino2::read_from_arduino()
{
    if(serial->isReadable()){
        data=serial->readAll(); //récupérer les données reçues

        return data;
    }
}



int arduino2::write_to_arduino( QByteArray d)

{

    if(serial->isWritable()){
        serial->write(d);  // envoyer des donnés vers Arduino
    }else{
        qDebug() << "Couldn't write to serial!";
    }


}

