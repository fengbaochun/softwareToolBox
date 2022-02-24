#ifndef CANDEV_H
#define CANDEV_H

#include <QThread>
#include "ControlCAN.h"
#include <QDebug>

#define CMD_BASE_ADDR    0x40	        //命令基地址
typedef enum{
    CMD_BASE = CMD_BASE_ADDR,
    READ_CMD,                           //读
    WRITE_CMD,                          //写
    ANSWER_CMD,                         //应答
    AUTO_REPORT_CMD,                    //主动上报
    CMD_MAX,                            //最大值
}cmdType;

typedef struct{
    uint16_t idVal;         //获取ID应答
    bool isCalZero;         //标零应答
    bool isCalVal;          //标值应答
    uint16_t tareVal;       //去皮值应答
    bool isCanclePeel;      //取消去皮应答
    uint16_t fps;           //实时帧率(fps/s) 应答
    int16_t val;            //实时压力值（单位：g）
    bool isReport;          //上报使能
    bool isEnable;			//是否使能
    uint16_t version;       //版本信息
    uint8_t buf[CMD_MAX][8];
}forceTypedef;


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
    void sendData(uint8_t id,int len,uint8_t *data);

    forceTypedef force;
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
