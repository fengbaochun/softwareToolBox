#include "firmwareUpdate.h"
#include <QFile>
#include <QDebug>

firmwareUpdate::firmwareUpdate(QObject *parent)
    : QObject{parent}
{

}

void firmwareUpdate::getBinInfo(QString path)
{
    QFile firmware(path);                       //要读取的bin文件
    if (!firmware.open (QFile::ReadOnly)){
       qDebug()<<"open failed!!!";              //打开文件
       return;
    }
    data.path = path;
    data.perPkgSize = 16;
    data.size = firmware.size();
    data.pkgNum = data.size / data.perPkgSize;
    data.lastPkgSize = data.size % data.perPkgSize;

    qDebug()<<"path             : "<<data.path ;
    qDebug()<<"size             : "<<data.size<<"B";
    qDebug()<<"perPkgSize       : "<<data.perPkgSize<<"B";
    qDebug()<<"packNum          : "<<data.pkgNum;
    qDebug()<<"lastPackSize     : "<<data.lastPkgSize;

    QByteArray ary;
    for(int i = 0; i < data.pkgNum ; i++){
        ary = firmware.read(data.perPkgSize);
        qDebug()<< i <<" : " << ary.toHex();
    }
    ary = firmware.read(data.lastPkgSize);
    qDebug()<< data.pkgNum << " : " <<ary.toHex();

    firmware.seek(0);
    firmware.close();
}

//进入boot
bool firmwareUpdate::enterBootloader()
{
    return true;
}

//发送固件信息
bool firmwareUpdate::sendBinInfo()
{
    return true;
}

//开始更新
bool firmwareUpdate::startUpdate()
{
    return true;
}

//报文回调
void firmwareUpdate::messageCallBack(QByteArray buf)
{

}
