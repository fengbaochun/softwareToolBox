#include "serial.h"

Serial::Serial(QObject *parent)
    : commun{parent}
{

}

//打开串口
bool Serial::open(QString name, int bps)
{
    port.setPortName(name);                                 //设置串口名字

    if(port.open(QIODevice::ReadWrite)){                    //打开串口(读写模式)
        port.setBaudRate(bps);                              //设置波特率
        port.clearError();
        port.clear();
        qDebug()<< "打开串口成功";
        return true;
    }else{
        qDebug()<< "串口打开失败";
        return false;
    }
}

//关闭串口
void Serial::close()
{
    port.close();
}

//串口写
bool Serial::write(quint32 id, int len, uint8_t *data)
{
    Q_UNUSED(id);
    if(!port.isOpen()) return false;
    port.write((char*)data,len);
    return true;
}

//串口写
bool Serial::write(QByteArray data)
{
    if(!port.isOpen()) return false;
    port.write(data);
    return true;
}

QByteArray Serial::getData()
{
}

/*
//读取数据
QByteArray serial::read(){
    QByteArray buf = port.readAll();
    return buf;
}
*/
