#include "ctl.h"

ctl* ctl::INSTANCE = nullptr;

ctl::ctl(QObject *parent)
    : uProtocol{parent}
{
    qRegisterMetaType<uint8_t>("uint8_t");
    qRegisterMetaType<uint32_t>("uint32_t");

    connect(this, SIGNAL(report(uint8_t,QByteArray)), this,
            SLOT(reportCallBack(uint8_t, QByteArray)));

    this->registerFun(ctlFun["mode"]);
    this->registerFun(ctlFun["fps"]);
    this->registerFun(ctlFun["saveParam"]);

    this->registerFun(ctlFun["tarPos"]);
    this->registerFun(ctlFun["curPos"]);
    this->registerFun(ctlFun["posRange"]);

    this->registerFun(ctlFun["posP"]);
    this->registerFun(ctlFun["posI"]);
    this->registerFun(ctlFun["posD"]);

    this->registerFun(ctlFun["tarSpeed"]);
    this->registerFun(ctlFun["curSpeed"]);
    this->registerFun(ctlFun["speedRange"]);

    this->registerFun(ctlFun["speedP"]);
    this->registerFun(ctlFun["speedI"]);
    this->registerFun(ctlFun["speedD"]);

    this->registerFun(ctlFun["tarCurrent"]);
    this->registerFun(ctlFun["curCurrent"]);
    this->registerFun(ctlFun["currentRange"]);

    this->registerFun(ctlFun["currentP"]);
    this->registerFun(ctlFun["currentI"]);
    this->registerFun(ctlFun["currentD"]);

}

ctl *ctl::instance()
{
    return (INSTANCE == nullptr) ? (INSTANCE = new ctl()) : INSTANCE;
}

bool ctl::setFps(int timeout)
{
    QLOG_INFO()<<"设置帧率"<<1000/timeout<<"fps";
    QByteArray qb = QByteArray::fromRawData(reinterpret_cast<const char*>(&timeout), 4);
    return this->packageToBus(cmd["write"], ctlFun["fps"], qb, 30, true);
}

void ctl::setMode(ctlModeTypedef m)
{
    QLOG_INFO()<<"设置模式"<<m;
    QByteArray qb = QByteArray::fromRawData(reinterpret_cast<const char*>(&m), 1);
    bool ret = this->packageToBus(cmd["write"], ctlFun["mode"], qb, 30, true);
}

bool ctl::setTarPos(float val)
{
    QLOG_INFO()<<"设置目标位置"<<val;
    QByteArray qb = QByteArray::fromRawData(reinterpret_cast<const char*>(&val), 4);
    return this->packageToBus(cmd["write"], ctlFun["tarPos"], qb, 30, true);
}

bool ctl::setTarSpeed(float val)
{
    QLOG_INFO()<<"设置目标速度"<<val;
    QByteArray qb = QByteArray::fromRawData(reinterpret_cast<const char*>(&val), 4);
    return this->packageToBus(cmd["write"], ctlFun["tarSpeed"], qb, 30, true);
}

bool ctl::setTarCurrent(float val)
{
    QLOG_INFO()<<"设置目标Iq"<<val;
    QByteArray qb = QByteArray::fromRawData(reinterpret_cast<const char*>(&val), 4);
    return this->packageToBus(cmd["write"], ctlFun["tarCurrent"], qb, 30, true);
}

bool ctl::setPosRange(float minVal, float maxVal)
{

}

bool ctl::setSpeedRange(float minVal, float maxVal)
{
    QLOG_INFO()<<"设置速度范围"<<minVal<<maxVal;

    float val[2] = { minVal, maxVal};
    QByteArray qb = QByteArray::fromRawData(reinterpret_cast<const char*>(&val), 4*2);
    return this->packageToBus(cmd["write"], ctlFun["speedRange"], qb, 30, true);
}

bool ctl::setCurrentRange(float minVal, float maxVal)
{
    QLOG_INFO()<<"设置电流范围"<<minVal<<maxVal;
    float val[2] = { minVal, maxVal};
    QByteArray qb = QByteArray::fromRawData(reinterpret_cast<const char*>(&val), 4*2);
    return this->packageToBus(cmd["write"], ctlFun["currentRange"], qb, 30, true);
}

bool ctl::getPosRange(float *minVal, float *maxVal)
{
    (void)minVal;
    (void)maxVal;
    return true;
}

//获取速度范围
bool ctl::getSpeedRange(float *minVal, float *maxVal)
{
    QByteArray qb;
    bool ret = this->packageToBus(cmd["read"], ctlFun["speedRange"], qb, 30, true);
    if(ret){
        QByteArray qb = getResultData(ctlFun["speedRange"]);
        QVector<float> gs = splitByteArrayToData(qb, sizeof(float));
        if(gs.size() != 2){
            return false;
        }
        *minVal = gs.at(0);
        *maxVal = gs.at(1);
        QLOG_INFO()<<"速度 范围："<<gs;
    }
    return ret;
}

//获取iq范围
bool ctl::getCurrentRange(float *minVal, float *maxVal)
{
    QByteArray qb;
    bool ret = this->packageToBus(cmd["read"], ctlFun["currentRange"], qb, 30, true);
    if(ret){
        QByteArray qb = getResultData(ctlFun["currentRange"]);
        QVector<float> gs = splitByteArrayToData(qb, sizeof(float));
        if(gs.size() != 2){
            return false;
        }
        *minVal = gs.at(0);
        *maxVal = gs.at(1);
        QLOG_INFO()<<"IQ 范围："<<gs;
    }
    return ret;
}

bool ctl::setPosP(float val)
{
    QLOG_INFO()<<"设置位置环P"<<val;
    QByteArray qb = QByteArray::fromRawData(reinterpret_cast<const char*>(&val), 4);
    return this->packageToBus(cmd["write"], ctlFun["posP"], qb, 30, true);
}

bool ctl::setPosD(float val)
{
    QLOG_INFO()<<"设置位置环D"<<val;
    QByteArray qb = QByteArray::fromRawData(reinterpret_cast<const char*>(&val), 4);
    return this->packageToBus(cmd["write"], ctlFun["posD"], qb, 30, true);
}

bool ctl::setSpeedP(float val)
{
    QLOG_INFO()<<"设置速度环P"<<val;
    QByteArray qb = QByteArray::fromRawData(reinterpret_cast<const char*>(&val), 4);
    return this->packageToBus(cmd["write"], ctlFun["speedP"], qb, 30, true);
}

bool ctl::setSpeedI(float val)
{
    QLOG_INFO()<<"设置速度环I"<<val;
    QByteArray qb = QByteArray::fromRawData(reinterpret_cast<const char*>(&val), 4);
    return this->packageToBus(cmd["write"], ctlFun["speedI"], qb, 30, true);
}

bool ctl::setCurrentP(float val)
{
    QLOG_INFO()<<"设置力矩换环P"<<val;
    QByteArray qb = QByteArray::fromRawData(reinterpret_cast<const char*>(&val), 4);
    return this->packageToBus(cmd["write"], ctlFun["currentP"], qb, 30, true);
}

bool ctl::setCurrentI(float val)
{
    QLOG_INFO()<<"设置力矩换环I"<<val;
    QByteArray qb = QByteArray::fromRawData(reinterpret_cast<const char*>(&val), 4);
    return this->packageToBus(cmd["write"], ctlFun["currentI"], qb, 30, true);
}

//参数同步
bool ctl::paramSync()
{
    QLOG_INFO()<<"******************************** 获取参数 ********************************";
    QByteArray qb;
    bool ret = this->packageToBus(cmd["read"], ctlFun["currentRange"], qb, 30, true);
    if(ret){
        QByteArray qb = getResultData(ctlFun["currentRange"]);
        QVector<float> gs = splitByteArrayToData(qb, sizeof(float));
        motor.minIq = gs.at(0);
        motor.maxIq = gs.at(1);
        QLOG_INFO()<<"IQ 范围："<<gs;
    }
    ret = this->packageToBus(cmd["read"], ctlFun["speedRange"], qb, 30, true);
    if(ret){
        QByteArray qb = getResultData(ctlFun["speedRange"]);
        QVector<float> gs = splitByteArrayToData(qb, sizeof(float));
        motor.minSpeed = gs.at(0);
        motor.maxSpeed = gs.at(1);
        QLOG_INFO()<<"速度 范围："<<gs;
    }
    ret = this->packageToBus(cmd["read"], ctlFun["posP"], qb, 30, true);
    if(ret){
        QByteArray qb = getResultData(ctlFun["posP"]);
        QVector<float> gs = splitByteArrayToData(qb, sizeof(float));
        motor.posP = gs.at(0);
        QLOG_INFO()<<"位置环P："<<gs;
    }
    ret = this->packageToBus(cmd["read"], ctlFun["posD"], qb, 30, true);
    if(ret){
        QByteArray qb = getResultData(ctlFun["posD"]);
        QVector<float> gs = splitByteArrayToData(qb, sizeof(float));
        motor.posD = gs.at(0);
        QLOG_INFO()<<"位置环D："<<gs;
    }
    ret = this->packageToBus(cmd["read"], ctlFun["speedP"], qb, 30, true);
    if(ret){
        QByteArray qb = getResultData(ctlFun["speedP"]);
        QVector<float> gs = splitByteArrayToData(qb, sizeof(float));
        motor.speedP = gs.at(0);
        QLOG_INFO()<<"速度环P："<<gs;
    }
    ret = this->packageToBus(cmd["read"], ctlFun["speedI"], qb, 30, true);
    if(ret){
        QByteArray qb = getResultData(ctlFun["speedI"]);
        QVector<float> gs = splitByteArrayToData(qb, sizeof(float));
        motor.speedI = gs.at(0);
        QLOG_INFO()<<"速度环I："<<gs;
    }
    ret = this->packageToBus(cmd["read"], ctlFun["currentP"], qb, 30, true);
    if(ret){
        QByteArray qb = getResultData(ctlFun["currentP"]);
        QVector<float> gs = splitByteArrayToData(qb, sizeof(float));
        motor.currentP = gs.at(0);
        QLOG_INFO()<<"力矩环P："<<gs;
    }
    ret = this->packageToBus(cmd["read"], ctlFun["currentI"], qb, 30, true);
    if(ret){
        QByteArray qb = getResultData(ctlFun["currentI"]);
        QVector<float> gs = splitByteArrayToData(qb, sizeof(float));
        motor.currentI = gs.at(0);
        QLOG_INFO()<<"力矩环I："<<gs;
    }
    QLOG_INFO()<<"********************************   end   ********************************";
    return true;
}

//参数保存
bool ctl::paramSave()
{
    QLOG_INFO()<<"参数保存";
    QByteArray qb;
    return this->packageToBus(cmd["write"], ctlFun["saveParam"], qb, 30, true);
}

void ctl::reportCallBack(uint8_t fun, QByteArray qb)
{
    QVector<float> gData = splitByteArrayToData(qb, sizeof(float));
//    QLOG_INFO()<<"ctl report -> fun"<<QString::number(fun, 16)<<"data"<<gData;
}
