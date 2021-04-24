#include "serialdev.h"

serialDev::serialDev()
{
    qDebug()<<"start... ...";

    baudList << u8"4800" << u8"9600" << u8"38400" << u8"115200" << u8"230400" << u8"460800";

    serPort = new QSerialPort;

    serPort->setPortName("COM5");
    if(serPort->open(QIODevice::ReadWrite))
    {
        qDebug()<<"串口已打开";
        serPort->setBaudRate(115200);
        serPort->setDataBits(QSerialPort::Data8); // 设置数据位(数据位为8位)
        serPort->setParity(QSerialPort::NoParity); // 设置校验位(无校验位)
        serPort->setStopBits(QSerialPort::OneStop); // 设置停止位(停止位为1)
        serPort->setFlowControl(QSerialPort::NoFlowControl); // 设置流控制(无数据流控制)
    }
    else
    {
        qDebug()<<"串口打开";
    }

    connect(serPort, SIGNAL(readyRead()), this, SLOT(revDataSlot()),Qt::QueuedConnection);
}

serialDev::~serialDev()
{

}

void serialDev::readData()
{

}

void serialDev::revDataSlot()
{
    if(serPort->bytesAvailable()>= 0)
    {
        QByteArray temp;
        temp = serPort->readAll();
        qDebug()<<temp;
    }
}

void serialDev::run()
{
    while (1)
    {
        msleep(100);
    }
}
