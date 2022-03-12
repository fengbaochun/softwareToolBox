#include "motorctl.h"
#include <QTimer>
#include <QDebug>

motorCtl::motorCtl(){
    com = new serial();
    com->open("COM20",921600);

    timer = new QTimer();
//    connect(timer, SIGNAL(timeout()), this, SLOT(revDataSlot()));
    timer->start(1);
    connect(&com->port, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));
    com->port.clear();

    motorCurData[CUR_POS] = 0;
    motorCurData[CUR_SPEED] = 10;
    motorCurData[CUR_CURRENT] = 99;
}

void motorCtl::readyReadSlot(){
    QByteArray temp = com->read();
    if(!temp.isEmpty()){
        this->buf.append(temp);

        int headInx = 0;//头索引
        do{
            headInx = buf.toHex().indexOf("ffff");  //头
            if(headInx != 0){                       //去除冗余包头
                buf.remove(0,headInx);              //清除多余数据
            }
            if(buf.size()<2) return;                //确保有数据
            uint8_t len = buf.at(2);                //包长
            if(buf.size()<len+4) return;              //确保有数据

            uint8_t id = buf.at(3);                 //dev id
            uint8_t cmd = buf.at(4);                //cmd   命令
            uint8_t fun = buf.at(5);                //fun   功能
            int dataNum = (len-3-1)/4;              //4字节数据个数

            QByteArray funBuf = buf.mid(6,len-3-1); //去掉头、校验码的长度

            float answerVal[MAX_REV_DATA_LEN];      //最大值
            uint8_t *data2 = reinterpret_cast<uint8_t *>(funBuf.data());
            memcpy(answerVal,data2,sizeof(float)*dataNum);
            qDebug()<<"answer   "<<answerVal[0]<<answerVal[1]<<answerVal[2]<<answerVal[3]<<answerVal[4]<<answerVal[5];
            if(cmd == AUTO_REPORT_CMD){
                motorCurData[TAR_POS] = answerVal[TAR_POS];
                motorCurData[CUR_POS] = answerVal[CUR_POS];
                motorCurData[TAR_SPEED] = answerVal[TAR_SPEED];
                motorCurData[CUR_SPEED] = answerVal[CUR_SPEED];
                motorCurData[TAR_CURRENT] = answerVal[TAR_CURRENT];
                motorCurData[CUR_CURRENT] = answerVal[CUR_CURRENT];
            }

            uint8_t val = 0;
            for(int i = 2; i < len+2; i++){
                val += buf.at(i);                   //校检码
            }
//            qDebug()<<"buf      "<<buf.toHex().toUpper();
//            qDebug()<<"check val"<<QString::number(val,16).toUpper();
//            qDebug()<<"funBuf   "<<funBuf.toHex().toUpper();
//            qDebug()<<"index    "<<headInx;
//            qDebug()<<"buf size "<<buf.size();
//            qDebug()<<"len      "<<len;
//            qDebug()<<"answer   "<<answerVal;
//            qDebug()<<"dataNum  "<<dataNum;

            buf.remove(headInx,headInx+3+len);
        }while(headInx != -1);      //遍历出所有信息
    }
}

void motorCtl::senDataToBus(cmdTypedef cmd, funsionType fun, float val){
    #define BUF_LEN	50
    char buf[BUF_LEN];
    memset(buf,0,BUF_LEN);
    int cnt = 0;
    buf[cnt++] = 0xFF;
    buf[cnt++] = 0xFF;              	//头

    buf[cnt++] = 0x00;              	//长度
    buf[cnt++] = 0x01;              	//id

    buf[cnt++] = (uint8_t)cmd;			//属性
    buf[cnt++] = (uint8_t)fun;			//功能

    QByteArray outputdata;

    outputdata.resize(sizeof(val));    //重新调整QByteArray的大小
    memcpy(outputdata.data(),&val,sizeof(val));
    buf[cnt++] = outputdata[0];        	//数据
    buf[cnt++] = outputdata[1];
    buf[cnt++] = outputdata[2];
    buf[cnt++] = outputdata[3];

    buf[2] = cnt - 2;

    uint8_t sum = 0;
    for (int i = 2; i < cnt; i++)		//计算校验
        sum += buf[i];

    buf[cnt++] = sum;

    com->write(buf,cnt);
}


//设置目标位置
void motorCtl::setTarPos(float val){
    float pos = val*16384.0f/360.0f;
    qDebug()<<"tar pos is "<<pos;
    senDataToBus(WRITE_CMD,POS,pos);
}

//设置目标速度
void motorCtl::setTarSpeed(float val){
    senDataToBus(WRITE_CMD,SPEED,val);
}

//设置目标电流
void motorCtl::setTarCurrent(float val){
    senDataToBus(WRITE_CMD,CURRENT,val);
}

void motorCtl::run(){
    while(true){


    }
}
