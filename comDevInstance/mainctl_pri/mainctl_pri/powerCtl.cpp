#include "powerCtl.h"

powerCtl* powerCtl::INSTANCE = nullptr;

powerCtl::powerCtl(QObject *parent)
    : QObject{parent}
{
    com = new uProtocol();
    connect(com, SIGNAL(report(uint8_t,uint32_t)), this, SLOT(reportCallBack(uint8_t, uint32_t)));

    com->registerFun(powerFun["key"]);
    com->registerFun(powerFun["rftisw"]);
    com->registerFun(powerFun["powerIO"]);
}

powerCtl *powerCtl::instance()
{
    return (INSTANCE == nullptr) ? (INSTANCE = new powerCtl()) : INSTANCE;
}

//打开
bool powerCtl::open(QString name, qint32 bsp)
{
    return com->open(name, bsp);
}

//关闭
void powerCtl::close()
{
    com->close();
}

//打开脚踏板
bool powerCtl::openRftiSw(bool isOpen)
{
    return com->packageToBus(cmd["write"], powerFun["rftisw"], isOpen, 20,true);
}

//关机
bool powerCtl::powerOff()
{
    return com->packageToBus(cmd["write"], powerFun["key"], 0x01, 20,true);
}

//设置电源IO
bool powerCtl::setPowerIo(uint8_t id, bool isOpen)
{
    if(id>17) return false;
    uProtocol::u32DataType pkg;
    pkg.da[0] = id;
    pkg.da[1] = isOpen;
    qDebug()<<"设置IO ："<<id<<" status: "<<isOpen<<" data: "<<QString::number(pkg.data,16);
    return com->packageToBus(cmd["write"], powerFun["powerIO"], pkg.data, 20, true);;
}

//主动上报回调
void powerCtl::reportCallBack(uint8_t fun, uint32_t data)
{
    qDebug()<<"report -> fun : "<<QString::number(fun, 16)<<"; data : "<<QString::number(data, 16);
    if(fun == powerFun["key"]){
        qDebug()<<"按键板触发关机 ... ...";
    }
}
