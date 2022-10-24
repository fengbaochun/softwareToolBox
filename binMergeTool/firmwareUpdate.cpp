#include "firmwareUpdate.h"
#include <QFile>
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <QThread>

firmwareUpdate::firmwareUpdate(QObject *parent)
    : QObject{parent}
{
    this->init(commun::CAN_COMMUN);
}

//初始化
void firmwareUpdate::init(int type)
{
    this->iap.answerPkgNum = 0;
    this->iap.binDataStatus = false;
    this->iap.binSizeStatus = false;
    this->iap.enterBootStatus = false;
    this->id = 0xA8;
    if(com == nullptr){
        com = commun::create((commun::communType)type);
        com->open("CAN2",1000000);
        com->start();

        connect(com, &commun::reveiced, this, &firmwareUpdate::messageCallBack);
    }
}

//获取固件信息
void firmwareUpdate::getBinInfo(QString path)
{
    QFile firmware(path);                       //要读取的bin文件
    if (!firmware.open (QFile::ReadOnly)){
        qDebug()<<"open failed!!!";              //打开文件
        return;
    }
    bin.path = path;
    bin.perPkgSize = 8;
    bin.size = firmware.size();
    bin.pkgNum = bin.size / bin.perPkgSize;
    bin.lastPkgSize = bin.size % bin.perPkgSize;

    qDebug()<<"path             : "<<bin.path ;
    qDebug()<<"size             : "<<bin.size<<"B";
    qDebug()<<"perPkgSize       : "<<bin.perPkgSize<<"B";
    qDebug()<<"packNum          : "<<bin.pkgNum;
    qDebug()<<"lastPackSize     : "<<bin.lastPkgSize;

    QByteArray ary;
    for(int i = 0; i < bin.pkgNum ; i++){
        ary = firmware.read(bin.perPkgSize);
        qDebug()<< i <<" : " << ary.toHex();
    }
    ary = firmware.read(bin.lastPkgSize);
    qDebug()<< bin.pkgNum << " : " <<ary.toHex();

    firmware.seek(0);
    firmware.close();
}

//进入boot
bool firmwareUpdate::enterBootloader()
{
    uint8_t data[8]={WRITE_CMD,UPDATE_BIN_ADDR,0xFF,0xFF, 0x00,0x00,0x00,0x00};
    com->write(this->id, 8, data);
    QThread::msleep(1);
    qint64 tick = QDateTime::currentDateTime().toMSecsSinceEpoch();     //毫秒时间戳
    do{
        if(abs(QDateTime::currentDateTime().toMSecsSinceEpoch() - tick) > 2*1000){
            qDebug()<<"wait answer time out !";
            return false;
        }else{
            QThread::msleep(1);
        }
    }while(!iap.enterBootStatus);
    return true;
}

//发送固件信息
bool firmwareUpdate::sendBinInfo(uint16_t size)
{
    uint8_t data[8]={WRITE_CMD,BIN_SIZE_ADDR,(uint8_t)((size>>8)&0xff), (uint8_t)(size&0xff),0x00,0x00,0x00,0x00};
    com->write(this->id, 8, data);
    qint64 tick = QDateTime::currentDateTime().toMSecsSinceEpoch();     //毫秒时间戳
    do{
        if(abs(QDateTime::currentDateTime().toMSecsSinceEpoch() - tick) > 5*1000){
            qDebug()<<"wait answer time out !";
            return false;
        }else{
            QThread::msleep(1);
        }
    }while(!iap.binSizeStatus);
    return true;
}

//开始更新
bool firmwareUpdate::startUpdate()
{
#if 1
    QFile firmware(bin.path);                       //要读取的bin文件
    if (!firmware.open (QFile::ReadOnly)){
        qDebug()<<"open failed!!!";
        return false;
    }

    int packNum = bin.size / 8;
    int lastPackSize = bin.size % 8;
    uint32_t addr = 0x00;

    qDebug()<<"ID           : "<<	this->id;
    qDebug()<<"BinSize      : "<<	bin.size;
    qDebug()<<"PackNum      : "<<	packNum;
    qDebug()<<"LastPackSize : "<<	lastPackSize;

//    uint32_t tempData[2] = {0};
    uint8_t driveData[8] = {0};

    uint8_t index = 0;			//每KB完成索引
    QThread::msleep(100);
    for(int i=0;i<packNum;i++){
//        STMFLASH_Read (addr, tempData , 2);       //固件读取 更换为 读取文件;
        memcpy(driveData, firmware.read(bin.perPkgSize).data(), 8);
        com->write(this->id, 8, driveData);
        QThread::msleep(5);
        addr = addr + 8;
        if( ((i+1)*8) % 1024 == 0){
            index++;
            qDebug()<<"Index : %d KB\r\n"<<index;
            qint64 tick = QDateTime::currentDateTime().toMSecsSinceEpoch();     //毫秒时间戳
            do{
                if(abs(QDateTime::currentDateTime().toMSecsSinceEpoch() - tick) > 5*1000){
                    qDebug()<<"wait answer time out !\r\n";
                    return false;
                }
            }while(index != iap.answerPkgNum);  //等待回包
        }
        emit progress(packNum, i);              //进度信号
    }

    if(lastPackSize>0){
//        STMFLASH_Read (addr, tempData , 1);       //固件读取 更换为 读取文件
        memcpy(driveData, firmware.read(bin.perPkgSize).data(), 4);         //固件读取
        com->write(this->id, 8, driveData);
        index++;

        qDebug()<<"Index : %d KB "<<index;
        qint64 tick = QDateTime::currentDateTime().toMSecsSinceEpoch();     //毫秒时间戳
        do{
            if(abs(QDateTime::currentDateTime().toMSecsSinceEpoch() - tick) > 5*1000){
                qDebug()<<"wait answer time out !\r\n";
                return false;
            }
        }while(index != iap.answerPkgNum);          //等待回包
        emit progress(packNum, packNum);            //进度信号
    }
    qDebug()<<"update bin finash";
#endif
    return true;
}

//报文回调
void firmwareUpdate::messageCallBack(QByteArray buf)
{
    qDebug() << buf.length() << buf.toHex().toUpper();
    uint8_t data[10];
    memcpy(data,buf.data(),10);

    uint8_t diff = 2;

    if(data[0+diff]==ANSWER_CMD && data[1+diff]==UPDATE_BIN_ADDR && data[2+diff]==0xFF && data[3+diff]==0xFF){//bootloader 应答成功
        iap.enterBootStatus = true;
        qDebug()<<"iap.enterBootStatus ok ";
    }else if((data[0+diff] == ANSWER_CMD) && (data[1+diff] == BIN_SIZE_ADDR)){		//bin size 应答成功
        iap.binSizeStatus = true;
        qDebug()<<"iap.binSizeStatus ok ";
    }else if((data[0+diff] == ANSWER_CMD) && (data[1+diff] == BIN_DATA_ADDR)){		//发送1KB数据 应答成功
        iap.binDataStatus = true;
        iap.answerPkgNum = data[3];                                 //应答包 号
    }else{
    }
}
