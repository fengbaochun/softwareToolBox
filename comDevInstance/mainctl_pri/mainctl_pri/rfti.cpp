#include "rfti.h"

rfti* rfti::INSTANCE = nullptr;

rfti::rfti(QObject *parent)
    : QObject{parent}
{
    com = new uProtocol();
    connect(com, SIGNAL(report(uint8_t,uint32_t)), this, SLOT(reportCallBack(uint8_t, uint32_t)));
    com->registerFun(rftiFun["sw"]);
    com->registerFun(rftiFun["energy"]);
}

rfti *rfti::instance()
{
    return (INSTANCE == nullptr) ? (INSTANCE = new rfti()) : INSTANCE;
}

//打开
bool rfti::open(QString name, qint32 bsp)
{
    return com->open(name, bsp);
}

//关闭
void rfti::close()
{
    com->close();
}

//打开射频
bool rfti::openRftiSw(bool isOpen)
{
    qDebug()<<"设置射频开关 ： "<< isOpen;
    return com->packageToBus(cmd["write"], rftiFun["sw"], isOpen, 20,true);
}

//设置能量
bool rfti::setRftiEnergy(uint8_t val)
{
    qDebug()<<"设置射频能量 ： "<< val ;
    return com->packageToBus(cmd["write"], rftiFun["energy"], val, 20,true);
}

//获取射频能量
bool rfti::getRftiEnergy(uint8_t &val)
{
    bool ret = com->packageToBus(cmd["read"], rftiFun["energy"], val, 20,true);
    val = com->getResultData(rftiFun["energy"]);
    qDebug()<<"获取射频能量 ： "<< val ;
    return ret;
}

//主动上报回调
void rfti::reportCallBack(uint8_t fun, uint32_t data)
{
    qDebug()<<"report -> fun : "<<QString::number(fun, 16)<<"; data : "<<QString::number(data, 16);
}

