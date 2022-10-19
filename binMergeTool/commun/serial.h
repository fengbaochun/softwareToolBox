#ifndef SERIAL_H
#define SERIAL_H

#include "commun.h"
#include <QSerialPort>
#include <QSerialPortInfo>

class Serial : public commun
{
public:
    explicit Serial(QObject *parent = nullptr);
    bool open(QString name, int bps) = 0;                   //打开
    void close() = 0;                                       //关闭
    bool write(quint32 id, int len, uint8_t *data) = 0;     //发送
    bool write(QByteArray data) = 0;
    QByteArray getData() = 0;                               //获取数据

    QSerialPort port;                       //串口对象

private:
    QString name;                           //端口名字
    int bps;                                //波特率
};

#endif // SERIAL_H
