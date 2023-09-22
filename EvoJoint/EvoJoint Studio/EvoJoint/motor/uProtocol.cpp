#include "uProtocol.h"

uProtocol::uProtocol(QObject *parent)
    : QObject{parent}
{
    worker_ = new QObject;
    worker_->moveToThread(&workerThread_);
    workerThread_.start();

    qDebug()<<"uProtocol in thread id"<<QThread::currentThreadId();
    QMetaObject::invokeMethod(worker_, [this]{
        qDebug()<<"thread id"<<QThread::currentThreadId();
        port = new QSerialPort();
        fpsT = new QTimer();
        revT = new QTimer();
        //帧率计算
        connect(fpsT, &QTimer::timeout,this,[=](){
            fps = pkgNum - lastPkgNum;
            pkgNum = lastPkgNum;
        });
    });
}

uProtocol::~uProtocol()
{
    QMetaObject::invokeMethod(worker_, [&]{
        port->close();
        fpsT->stop();
        revT->stop();
    },Qt::BlockingQueuedConnection);
    workerThread_.quit();
    workerThread_.wait();
    worker_->deleteLater();
    workerThread_.deleteLater();
}

//打开串口
bool uProtocol::open(QString name, qint32 bsp)
{
    QMetaObject::invokeMethod(worker_, [&, name, bsp]{
        port->setPortName(name);        //设置串口名字
        if(port->isOpen()){             //如果串口已经打开了 先给他关闭了
            port->clear();
            port->close();
        }
        if(!port->open(QIODevice::ReadWrite)){//用ReadWrite 的模式尝试打开串口
            qCritical() << QStringLiteral("%1打开失败!").arg(name) << port->errorString();
//            emit onErrorOccurred(QStringLiteral("%1打开失败! 请检查串口设备").arg(name) + port->errorString());
//            return ;
        }
        //打开成功
        port->setBaudRate(bsp,QSerialPort::AllDirections);  //设置波特率和读写方向
        port->setDataBits(QSerialPort::Data8);              //数据位为8位
        port->setFlowControl(QSerialPort::NoFlowControl);   //无流控制
        port->setParity(QSerialPort::NoParity);             //无校验位
        port->setStopBits(QSerialPort::OneStop);            //一位停止位
        port->setReadBufferSize(1024 * 1024);
        fpsT->start(1000);
        qInfo() << "open serial sucess:" << name;
        connect(port, &QIODevice::readyRead, worker_, [&]{readyReadSlot();});           //信号槽链接（适用于<5000 fps）
//        connect(revT, &QTimer::timeout, worker_, [&]{readyReadSlot();});           //定时器信号(适用于>5000fps)
//        revT->start(1);
    });
}

//关闭串口
void uProtocol::close()
{
    buf.clear();
    QMetaObject::invokeMethod(worker_,[&]{
        fps = 0;
        port->close();
        revT->stop();
        fpsT->stop();
    });
}

void uProtocol::showFunCode()
{
    for(auto it = mFun.begin(); it!=mFun.end();it++){
        qDebug()<<it.key();
    }
}

void uProtocol::registerFun(uint8_t code)
{
    answerType answer;
    mFun.insert(code, answer);
    qDebug()<<"添加功能码 ： "<< QString::number(code,16);
}

void uProtocol::packageToBus(uint8_t cmd, uint8_t fun, uint8_t *data, uint8_t dLen)
{
    uint8_t buf[255];
    memset(buf,0,255);
    int cnt = 0;
    buf[cnt++] = 0xFF;          //头1
    buf[cnt++] = 0xFF;			//头2
    buf[cnt++] = 0x00;			//长度

    buf[cnt++] = cmd;			//属性  读 写 应答 主动上报 命令错误
    buf[cnt++] = fun;			//功能码

    for(int i=0; i<dLen; i++){  //数据打包
        buf[cnt++] = data[i];
    }

    buf[2] = cnt-3+1;		    //除去 头(2B),长度(1B);包含校验(1B)

    QByteArray qb = QByteArray::fromRawData(reinterpret_cast<const char*>(&buf), cnt);

    buf[cnt++] = crc8(qb, 2, cnt);        //计算校验

    QByteArray byte;
    byte = QByteArray((char *)buf,cnt);
    qDebug()<<"senddata : "<<byte.toHex(' ').toUpper();

    QMetaObject::invokeMethod(worker_, [&, buf, cnt]{
        port->write((char *)buf, cnt);
    });
}

bool uProtocol::packageToBus(uint8_t cmd, uint8_t fun, QByteArray qb, int timoutVal, bool isBlock)
{
    if(!isExist(fun)) {
        qDebug()<<"fun "<<QString::number(fun,16)<<" is not exist";
        return false;
    }
    mFun[fun].isAnswer = false;
    packageToBus(cmd, fun, (uint8_t *)qb.data(), qb.length());
    if(!isBlock) return true;
    QElapsedTimer time;
    time.start();
    do{
        QCoreApplication::processEvents(QEventLoop::EventLoopExec, timoutVal);
        QThread::msleep(1);

        if(mFun[fun].isAnswer){
            qDebug()<<"cmd "<< QString::number(cmd,16)<<" fun "<<QString::number(fun,16)<<" answer ok!";
            return true;
        }
    }while(time.elapsed() < timoutVal);
    qDebug()<<"answer cmd time out "<<timoutVal;
    return false;
}

bool uProtocol::packageToBus(uint8_t cmd, uint8_t fun, QByteArray qb, int timoutVal, int cnt, bool isBlock)
{
    bool ret = false;
    int i=0;
    do{
        ret = packageToBus(cmd, fun, qb, timoutVal, isBlock);
        i++;
    }while((!ret)&&(i<cnt));
    return ret;
}

bool uProtocol::packageToBus(uint8_t cmd, uint8_t fun, uint32_t val, int timoutVal, bool isBlock)
{
    QByteArray qb = QByteArray::fromRawData(reinterpret_cast<const char*>(&val), 4);
    return this->packageToBus(cmd, fun, qb, timoutVal, isBlock);
}

bool uProtocol::packageToBus(uint8_t cmd, uint8_t fun, uint32_t val, int timoutVal, int cnt, bool isBlock)
{
    QByteArray qb = QByteArray::fromRawData(reinterpret_cast<const char*>(&val), 4);
    return this->packageToBus(cmd, fun, qb, timoutVal, cnt, isBlock);
}

QByteArray uProtocol::getResultData(uint8_t fun)
{
    return mFun[fun].data;
}

uint32_t uProtocol::getResultU32Data(uint8_t fun)
{
    return byteArrayToValue<uint32_t>(mFun[fun].data);
}

QVector<QByteArray> uProtocol::splitByteArray(const QByteArray &byteArray, int dataSize) {
    QVector<QByteArray> result;
    int byteArraySize = byteArray.size();
    int numGroups = byteArraySize / dataSize;

    for (int i = 0; i < numGroups; ++i) {
        int startPos = i * dataSize;
        QByteArray group = byteArray.mid(startPos, dataSize);
        result.append(group);
    }
    return result;
}

QVector<float> uProtocol::splitByteArrayToData(const QByteArray &byteArray, int dataSize)
{
    QVector<QByteArray> gs = splitByteArray(byteArray, dataSize);
    QVector<float> gData;
    for (const QByteArray& g : gs) {
        float converFloat = byteArrayToValue<float>(g);
        gData.append(converFloat);
    }
    return gData;
}

uint8_t uProtocol::checkValCode(QByteArray ary, uint8_t start, uint8_t end)
{
    uint8_t val = 0;
    for(int i = start; i < end; i++){
        val += ary.at((i));
    }
    return val;
}

uint8_t uProtocol::crc8(QByteArray ary, uint8_t start, uint8_t end)
{
    uint8_t crc = 0;
    for ( uint32_t i = start; i < end; ++i ) {
        crc = crc8_table[(ary.at(i) ^ crc) & 0xFF];
    }
    return crc;
}

bool uProtocol::isExist(uint8_t code)
{
    return mFun.contains(code);
}

//报文处理
void uProtocol::process()
{
    foreach (QByteArray qb, message) {
        if(qb.size() < CM_DATA){
            qCritical()<<"bufsize error"<<qb.toHex(' ');
            break;
        }
        uint8_t code = crc8(qb, 2, qb.size()-1);    //计算校验
        if((uint8_t)qb.at(qb.size()-1) != code){
            qCritical()<<"check error"<<QString::number(code,16)<<QString::number((uint8_t)qb.at(qb.size()-1),16);
            break;
        }
        uint8_t c = qb.at(CM_CMD);
        uint8_t f = qb.at(CM_FUN);
        int dataLen = qb.at(CM_LEN) - 2 - 1 ;               //计算有效数据长度
        QByteArray data(qb.data()+CM_DATA, dataLen);        //装载数据
//        qDebug()<<"cmd"<<QString::number(c, 16) <<"fun"<<QString::number(f, 16)<<"data"<<data.toHex(' ');

        if(c == cmd["answer"]){                             //应答
            mFun[f].isAnswer = true;
            mFun[f].data.clear();
            mFun[f].data.append(data);
        }else if(c == cmd["autoReport"]){                   //主动上报
            mFun[f].rData.clear();
            mFun[f].rData.append(data);
            emit report(f, mFun[f].rData);
        }else if(c == cmd["error"]){                        //帧错误

        }
        pkgNum++;                                           //报文帧率计算
    }
//    qDebug()<<"message size"<<message.size();
    message.clear();
}

void uProtocol::readyReadSlot()
{
    QByteArray temp = port->readAll();
    if(temp.size() > 512){                  //一次性收 太多数据丢掉
        temp.clear();
        return;
    }

    static QByteArray tailRemain = 0;       //尾剩余
    int tailIndex = 0;                      //尾索引
    temp.prepend(tailRemain);               //将剩余的合并到容器前面解析

    if(temp.isEmpty()) return;
//    qDebug()<<"size"<<temp.size()<<"data"<<temp.toHex(' ').toUpper();
    int headIndex = 0;
    for(int i=1; i<temp.size(); i++){
        if((temp.at(i-1) == (char)0xff) && (temp.at(i) == (char)0xff)){     //找头（indexOf("ffff",0) 这个函数用不了）
            headIndex = i;
            int remain = temp.size() - headIndex;                           //找到头后的剩余字节数
            if(remain < 2+2){                                               //len cmd fun
                break ;
            }
            int len = temp.at(headIndex+1);                                 //报文长度
//            int cmd = temp.at(headIndex+2);                                 //命令
//            int fun = temp.at(headIndex+3);                                 //功能
            if(remain < (2+len)){   break;  }
            if(len < 0){    break;  }
            message.append(temp.mid(headIndex-1, 3+len));                   //添加消息列表
//            qDebug()<<"message size"<<message.size()                        //报文信息打印
//                    <<"head"<<headIndex<<"len"<<len
////                    <<"cmd"<<QString::number(cmd,16)<<"fun"<<QString::number(fun,16)
//                    <<temp.mid(headIndex-1, 3+len).toHex(' ');
            tailIndex = 2+headIndex+len;                                    //计算尾索引

        }
    }
    process();                                                              //报文处理
    tailRemain.resize(temp.size()-tailIndex);                               //构建剩余 bytearrary
    memcpy(tailRemain.data(), temp.data()+tailIndex, temp.size()-tailIndex);
//    qDebug()<<"tailRemain"<<tailRemain.toHex(' ');
    return ;
}

