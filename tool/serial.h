#ifndef SERIAL_H
#define SERIAL_H

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QThread>
#include <qdatetime.h>
#include <QObject>

class serial: public QThread
{
    Q_OBJECT
public:
    serial();
    QSerialPort port;                       //串口对象
    bool isOpened;                          //是否被打开
    bool open(QString name,int bps);        //打开
    void close(QString name);               //关闭
    void write(char buf[],int len);         //写入
    QByteArray read();                      //读取

private:

    QString name;                           //端口名字
    int bps;                                //波特率

private slots:
//   void readyReadSlot();

};

#endif // SERIAL_H
