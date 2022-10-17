﻿#ifndef CAN_H
#define CAN_H

#include "ControlCAN.h"
#include <QDebug>
#include <QThread>
#include "commun.h"

class can : public commun
{
    Q_OBJECT
public:
    explicit can(QObject *parent = nullptr);
    static can* instance();

    bool open(int id);                              //打开
    void close();                                   //关闭
    comIdList scanPort();                           //扫描
    QByteArray getData();
    bool send(int len, quint32 id, uint8_t *data);  //发送

    void rest();
    void setId(uint8_t val);
    void sendData(uint8_t id,int len,uint8_t *data);
//    void run();

    VCI_INIT_CONFIG paramConfig();
    bool getHardwareInfo();

//信号
signals:
    void reveiced(VCI_CAN_OBJ buf);

//槽函数
public slots:
    void revDataSlot(VCI_CAN_OBJ buf);

private:
    int nCANInd;

};

#endif // CAN_H
