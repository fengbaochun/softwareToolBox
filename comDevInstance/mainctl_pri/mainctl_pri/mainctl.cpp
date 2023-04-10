#include "mainctl.h"

#include <QElapsedTimer>
#include <QTimer>
#include <QThread>

mainctl* mainctl::INSTANCE = nullptr;

mainctl::mainctl(QObject *parent)
    : QObject{parent}
{
    port = new QSerialPort();
    connect(port, SIGNAL(readyRead()),this, SLOT(readyReadSlot()));
}

mainctl *mainctl::instance()
{
    return (INSTANCE == nullptr) ? (INSTANCE = new mainctl()) : INSTANCE;
}

bool mainctl::open(QString name, qint32 bsp)
{
    port->setPortName(name);

    if(port->open(QIODevice::ReadWrite)){
        port->setBaudRate(bsp);
        port->clearError();
        port->clear();
        qDebug()<< "打开串口成功";
        return true;
    }else{
        qDebug()<< "串口打开失败";
        return false;
    }
}

void mainctl::close()
{
    port->clearError();
    port->clear();
    port->close();
}

//打开射频（脚踏板）
bool mainctl::openRftiSw(bool isOpen)
{
    qDebug()<<((isOpen) ? "打开":"关闭")<<"射频";
    return packageToBus(WRITE_CMD, RFTI_SW, isOpen, 20, true);
}

//设置射频能量
bool mainctl::setRftiEnergy(uint8_t val)
{
    qDebug()<<"设置射频能量 ： "<< val ;
    return packageToBus(WRITE_CMD, RFTI_ENERY, val, 20, true);
}

//获取射频能量
bool mainctl::getRftiEnergy(uint8_t &val)
{
    bool ret = packageToBus(READ_CMD, RFTI_ENERY, 0x00, 20, true);
    val = getResultData(RFTI_ENERY);
    qDebug()<<"获取射频能量 ： "<< val ;
    return ret;
}

//关机
bool mainctl::powerOff()
{
    qDebug()<<"关机！";
    return  packageToBus(WRITE_CMD, KEY_STATUS, 0x01, 20, true);
}

//设置IO(包括 电源、继电器、脚踏板等)
bool mainctl::setPowerIo(uint8_t id, bool isOpen)
{
    if(id>17) return false;
    u32DataType pkg;
    pkg.da[0] = id;
    pkg.da[1] = isOpen;
    qDebug()<<"设置电源IO ："<<id<<" status: "<<isOpen<<" data: "<<QString::number(pkg.data,16);
    return packageToBus(WRITE_CMD, POWERIO, pkg.data, 20, true);;
}

void mainctl::packageToBus(cmdTypedef cmd, funsionType fun, uint32_t val)
{
    uint8_t buf[255];
    memset(buf,0,255);
    int cnt = 0;
    buf[cnt++] = 0xFF;          //头1
    buf[cnt++] = 0xFF;			//头2
    buf[cnt++] = 0x00;			//长度

    buf[cnt++] = cmd;			//属性  读 写 应答 主动上报 命令错误
    buf[cnt++] = fun;			//功能码

    buf[cnt++] = BYTE0(val);	//数据 固定4字节
    buf[cnt++] = BYTE1(val);
    buf[cnt++] = BYTE2(val);
    buf[cnt++] = BYTE3(val);

    buf[2] = cnt-3+1;		    //除去 头(2B),长度(1B);包含校验(1B)

    uint8_t sum = 0;
    for(uint8_t i = 2; i < cnt; i++){		//计算校验
        sum += buf[i];
    }

    buf[cnt++] = sum;
    port->write((char *)buf, cnt);
}

bool mainctl::packageToBus(cmdTypedef cmd, funsionType fun, uint32_t val, int timoutVal, bool isBlock)
{
    result[fun - FUN_BASE_ADDR].isAnswer = false;
    packageToBus(cmd, fun, val);
    if(!isBlock) return true;
    QElapsedTimer time;
    time.start();
    do{
        QCoreApplication::processEvents(QEventLoop::EventLoopExec, 20);
        QThread::msleep(1);
        if(result[fun - FUN_BASE_ADDR].isAnswer){
            qDebug()<<"cmd "<< QString::number(cmd,16)<<" fun "<<QString::number(fun,16)<<" answer ok!";
            return true;
        }
    }while(time.elapsed() < timoutVal);
    qDebug()<<"answer cmd time out "<<timoutVal;
    return false;
}

uint32_t mainctl::getResultData(funsionType fun)
{
    return result[fun - FUN_BASE_ADDR].data;
}

uint8_t mainctl::checkValCode(QByteArray ary, uint8_t start, uint8_t end){
    uint8_t val = 0;
    for(int i = start; i < end; i++){
        val += ary.at((i));                   //校检码
    }
    return val;
}

void mainctl::readyReadSlot()
{
    QByteArray temp = port->readAll();
    if(temp.isEmpty()) return;
    buf.append(temp);
    int headIndex = buf.toHex().indexOf("ffff",0);  //找头
    if(headIndex == -1){
        qDebug()<<"找不到包头";
        return;
    }
    if(headIndex != 0){                             //去除头及冗余部分
        buf.remove(0,headIndex);
        qDebug()<<"去除冗余部分 ： "<<headIndex;
    }
    if(buf.size()<buf.at(2)+2+1)    return;
    uint8_t code = checkValCode(buf, headIndex+2, buf.at(2)+2);
    uint8_t len = buf.at(2);

    qDebug()<<"rev data : "<<temp.toHex().toUpper();
    qDebug()<<"code : "<<QString::number(code,16)<< " " <<QString::number((uint8_t)buf.at(len+2),16);

    if(code != (uint8_t)buf.at(len+2)){             //校验失败
        buf.clear();
        qDebug()<<"校验失败 rev data : "<<temp.toHex().toUpper();
        return;
    }

    uint32_t data = 0;
    memcpy(&data, buf.data()+5,4);
    qDebug()<<"data size:"<<buf.size()
                        <<" len:"<<buf.mid(2,1).toHex()
                        <<" cmd: "<<buf.mid(3,1).toHex()
                        <<" fun:"<<buf.mid(4,1).toHex()
                        <<" data: "<<QString::number(data,16);

    if(buf.at(3) == ANSWER_CMD){                    //应答
        result[(funsionType)buf.at(4) - FUN_BASE_ADDR].isAnswer = true;
        result[(funsionType)buf.at(4) - FUN_BASE_ADDR].data = data;
    }else if(buf.at(3) == AUTO_REPORT_CMD){         //主动上报
        emit report((funsionType)buf.at(4), data);
    }else if(buf.at(3) == ERROR_CMD){               //帧错误

    }
    buf.clear();
}


