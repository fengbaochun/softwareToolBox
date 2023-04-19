#include "uProtocol.h"

uProtocol::uProtocol(QObject *parent)
    : QObject{parent}
{
    port = new QSerialPort();
    connect(port, SIGNAL(readyRead()),this, SLOT(readyReadSlot()));
}

uProtocol *uProtocol::create()
{
    return new uProtocol();
}

bool uProtocol::open(QString name, qint32 bsp)
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

void uProtocol::close()
{
    port->close();
    buf.clear();
}

void uProtocol::registerFun(uint8_t code)
{
    answerType answer;
    mFun.insert(code, answer);
    qDebug()<<"添加功能码 ： "<< QString::number(code,16);
}

void uProtocol::packageToBus(uint8_t cmd, uint8_t fun, uint32_t val)
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

bool uProtocol::packageToBus(uint8_t cmd, uint8_t fun, uint32_t val, int timoutVal, bool isBlock)
{
    if(!isExist(fun)) return false;
    mFun[fun].isAnswer = false;
    packageToBus(cmd, fun, val);
    if(!isBlock) return true;
    QElapsedTimer time;
    time.start();
    do{
        QCoreApplication::processEvents(QEventLoop::EventLoopExec, 20);
        QThread::msleep(1);

        if(mFun[fun].isAnswer){
            qDebug()<<"cmd "<< QString::number(cmd,16)<<" fun "<<QString::number(fun,16)<<" answer ok!";
            return true;
        }
    }while(time.elapsed() < timoutVal);
    qDebug()<<"answer cmd time out "<<timoutVal;
    return false;
}

uint32_t uProtocol::getResultData(uint8_t fun)
{
    return mFun[fun].data;
}

uint8_t uProtocol::checkValCode(QByteArray ary, uint8_t start, uint8_t end)
{
    uint8_t val = 0;
    for(int i = start; i < end; i++){
        val += ary.at((i));
    }
    return val;
}

bool uProtocol::isExist(uint8_t code)
{
    return mFun.contains(code);
}

void uProtocol::readyReadSlot()
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

    if(buf.at(3) == cmd["answer"]){                    //应答
        mFun[buf.at(4)].isAnswer = true;
        mFun[buf.at(4)].data = data;
    }else if(buf.at(3) == cmd["autoReport"]){         //主动上报
        emit report(buf.at(4), data);
    }else if(buf.at(3) == cmd["error"]){               //帧错误

    }
    buf.clear();
}
