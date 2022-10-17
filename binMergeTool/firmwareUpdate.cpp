#include "firmwareUpdate.h"
#include <QFile>
#include <QDebug>
#include <QTimer>
#include <QDateTime>

firmwareUpdate::firmwareUpdate(QObject *parent)
    : QObject{parent}
{
}

void firmwareUpdate::init(int type)
{
    this->iap.answerPkgNum = 0;
    this->iap.binDataStatus = false;
    this->iap.binSizeStatus = false;
    this->iap.enterBootStatus = false;

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
    uint8_t data[8]={WRITE_CMD,UPDATE_BIN_ADDR,0xFF,0xFF, 0x00,0x00,0x00,0x00};
//    forceSendData(armID, data);

    qint64 tick = QDateTime::currentDateTime().toMSecsSinceEpoch();     //毫秒时间戳
    do{
        if(abs(QDateTime::currentDateTime().toMSecsSinceEpoch() - tick) > 5*1000){
            qDebug()<<"wait answer time out !";
            return false;
        }
    }while(!iap.enterBootStatus);
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
    qDebug() << buf.length() << buf.toHex().toUpper();
    uint8_t data[10];
    memcpy(data,buf.data(),10);

//    qDebug()<<data[0]<<data[1]<<data[2]<<data[3]<<data[4]<<data[5]<<data[6]<<data[7]<<data[8]<<data[9];

    if(data[0]==ANSWER_CMD && data[1]==UPDATE_BIN_ADDR && data[2]==0xFF && data[3]==0xFF){//bootloader 应答成功
        iap.enterBootStatus = true;
    }else if(data[0]==ANSWER_CMD && data[1]==BIN_SIZE_ADDR){		//bin size 应答成功
        iap.binSizeStatus = true;
    }else if(data[0]==ANSWER_CMD && data[1]==BIN_DATA_ADDR){		//发送1KB数据 应答成功
        iap.binDataStatus = true;
        iap.answerPkgNum = data[3];                                 //应答包 号
    }else{
    }
}
