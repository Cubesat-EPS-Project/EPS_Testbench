#include "dialog.h"
#include "ui_dialog.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <QString>
#include <unistd.h>
#include <ios>
#include <fstream>
#include <string>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <string>
#include <QDebug>
#include <QMessageBox>
using namespace std;
float e1=0.0;
float e2=0.0;
float e4=0.0;
float e5=0.0;


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->label_2->setFrameStyle(2);
    ui->temp_lcdNumber->display("-------");
    arduino = new QSerialPort(this);
    serialBuffer = "";
    parsed_data = "";
    pd2= "";
    pd8="";
    //pd9="";
    //pd10="";
    //pd11="";
    temperature_value = 0.0;

    /*
     *  Testing code, prints the description, vendor id, and product id of all ports.
     *  Used it to determine the values for the arduino uno.
     *
     *
    qDebug() << "Number of ports: " << QSerialPortInfo::availablePorts().length() << "\n";
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        qDebug() << "Description: " << serialPortInfo.description() << "\n";
        qDebug() << "Has vendor id?: " << serialPortInfo.hasVendorIdentifier() << "\n";
        qDebug() << "Vendor ID: " << serialPortInfo.vendorIdentifier() << "\n";
        qDebug() << "Has product id?: " << serialPortInfo.hasProductIdentifier() << "\n";
        qDebug() << "Product ID: " << serialPortInfo.productIdentifier() << "\n";
    }
    */


    /*
     *   Identify the port the arduino uno is on.
     */
    bool arduino_is_available = false;
    QString arduino_uno_port_name;
    //
    //  For each available serial port
//    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
//        //  check if the serialport has both a product identifier and a vendor identifier
//        if(serialPortInfo.hasProductIdentifier() && serialPortInfo.hasVendorIdentifier()){
//            //  check if the product ID and the vendor ID match those of the arduino uno
//            if((serialPortInfo.productIdentifier() == arduino_uno_product_id)
//                    && (serialPortInfo.vendorIdentifier() == arduino_uno_vendor_id)){
//                arduino_is_available = true; //    arduino uno is available on this port
//                arduino_uno_port_name = serialPortInfo.portName();
//            }
//        }
//    }

    /*
     *  Open and configure the arduino port if available
     */
    if(1){
        qDebug() << "Found the arduino port...\n";

        arduino->setPortName("ttyUSB0");
        arduino->open(QSerialPort::ReadWrite);
        arduino->setBaudRate(QSerialPort::Baud115200);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setFlowControl(QSerialPort::NoFlowControl);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);

        QObject::connect(arduino, SIGNAL(readyRead()), this, SLOT(readSerial()));
    }else{
        qDebug() << "Couldn't find the correct port for the arduino.\n";
        QMessageBox::information(this, "Serial Port Error", "Couldn't open serial port to arduino.");
    }
}

Dialog::~Dialog()
{
    if(arduino->isOpen()){
        arduino->close(); //    Close the serial port if it's open.
    }
    delete ui;
}

void Dialog::readSerial()
{
    /*
     * readyRead() doesn't guarantee that the entire message will be received all at once.
     * The message can arrive split into parts.  Need to buffer the serial data and then parse for the temperature value.
     *
     */
    QStringList buffer_split = serialBuffer.split(","); //  split the serialBuffer string, parsing with ',' as the separator

    //  Check to see if there less than 3 tokens in buffer_split.
    //  If there are at least 3 then this means there were 2 commas,
    //  means there is a parsed temperature value as the second token (between 2 commas)
    if(buffer_split.length() < 3){
        // no parsed value yet so continue accumulating bytes from serial in the buffer.
        serialData = arduino->readAll();
        qDebug() << serialData << "\n";
        serialBuffer = serialBuffer + QString::fromStdString(serialData.toStdString());
        serialData.clear();
    }else{
        // the second element of buffer_split is parsed correctly, update the temperature value on temp_lcdNumber
        serialBuffer = "";

        //qDebug() << buffer_split << "\n";

        parsed_data = buffer_split[0];
        pd1=buffer_split[1];
        pd4=buffer_split[4];
        pd3=buffer_split[3];
        pd2=buffer_split[2];
        pd5=buffer_split[5];
        pd6=buffer_split[6];
        pd7=buffer_split[7];
        pd8=buffer_split[8];
        pd9=buffer_split[9];
        //pd10=buffer_split[10];
        //pd11=buffer_split[11];


        //qDebug() << "Temperature: " << temperature_value << "\n";
        //parsed_data = QString::number(temperature_value, 'g', 4); // format precision of temperature_value to 4 digits or fewer


        Dialog::updateTemperature(parsed_data);
    }

}

void Dialog::updateTemperature(QString sensor_reading)
{

    ui->temp_lcdNumber->display(parsed_data);//time
    float p2=e2+pd2.toFloat();
    float p1=e1+pd1.toFloat();
    float p4=e4+pd4.toFloat();
    float p5=e5+pd5.toFloat();
    ui->lcdNumber->display(p2);//batt curr
    ui->lcdNumber_2->display(p1);//pow path curr
    ui->lcdNumber_3->display(pd3);ui->lcdNumber_20->display(pd3);//batt volt
    ui->lcdNumber_4->display(pd7);//mppt op volt
    ui->lcdNumber_5->display(pd6);ui->lcdNumber_32->display(pd6);//ip volt
    ui->lcdNumber_6->display(p4);//ip curr
    ui->lcdNumber_7->display(pd7);ui->lcdNumber_19->display(pd7);ui->lcdNumber_33->display(pd7);//mppt op volt, batt ip volt
    ui->lcdNumber_8->display(p5);//mppt op curr


    mppteff= (pd5.toDouble()*pd7.toDouble()*100)/(pd4.toDouble()*pd6.toDouble());

    ui->lcdNumber_9->display(mppteff);

    ui->lcdNumber_10->display(pd8);
    ui->lcdNumber_11->display(pd9);
}

void Dialog::on_pushButton_clicked()
{
    e1=0-pd1.toFloat();
    e2=0-pd2.toFloat();
    e4=0-pd4.toFloat();
    e5=0-pd5.toFloat();
}

