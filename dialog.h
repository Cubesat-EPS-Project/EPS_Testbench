#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSerialPort>
#include <QByteArray>


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void readSerial();
    void updateTemperature(QString);

    void on_pushButton_clicked();

private:
    Ui::Dialog *ui;

    QSerialPort *arduino;
    static const quint16 arduino_uno_vendor_id = 9025;
    static const quint16 arduino_uno_product_id = 67;
    QByteArray serialData;
    QString serialBuffer;
    QString parsed_data;
    QString pd4;
    QString pd3;
    QString pd2;
    QString pd1;
    QString pd5;
    QString pd6;
    QString pd7;
    QString pd8;
    QString pd9;
    QString pd10;
    QString pd11;
    //float e1=0.0,e2=0.0,e4,e5;
    double mppteff;
    double temperature_value;
};

#endif // DIALOG_H
