#include "serial.h"

Serial::Serial(QObject *parent)
    : commun{parent}
{

}

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

void Serial::close()
{
    port.close();
}

bool Serial::send(quint32 id, int len, uint8_t *data)
{

}

bool Serial::send(QByteArray data)
{

}

QByteArray Serial::getData()
{

}

/*

//写入数据
void serial::write(char buf[],int len){
    port.write(buf,len);
}

//读取数据
QByteArray serial::read(){
    QByteArray buf = port.readAll();
    return buf;
}

*/
