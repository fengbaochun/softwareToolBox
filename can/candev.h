#ifndef CANDEV_H
#define CANDEV_H

#include <QThread>
#include "ControlCAN.h"
#include <QDebug>

class canDev:public QThread
{
    Q_OBJECT
public:
    canDev();
    ~canDev();
    void run();
    bool getHardwareInfo();
    bool open();

    void rest();
    void close();

    VCI_INIT_CONFIG paramConfig();

    void sendData(int id,int len,uint8_t *data);
private:
    int nCANInd;

//信号
signals:
    void reveiced(VCI_CAN_OBJ buf);

//槽函数
public slots:
    void revDataSlot(VCI_CAN_OBJ buf);

};

#endif // CANDEV_H
