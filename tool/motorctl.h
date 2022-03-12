#ifndef MOTORCTL_H
#define MOTORCTL_H

#include "serial.h"
#include <QThread>
#include <qdatetime.h>
#include <QObject>

#define CMD_BASE_ADDR    	0x20	        //命令基地址
typedef enum{
    CMD_BASE = CMD_BASE_ADDR,
    READ_CMD,                           //读
    WRITE_CMD,                          //写
    ANSWER_CMD,                         //应答
    AUTO_REPORT_CMD,                    //主动上报
    CMD_MAX,                            //最大值
}cmdTypedef;


#define FUN_BASE_ADDR		0x30

typedef enum{
    FUNSION_BASE = FUN_BASE_ADDR,

    SPEED,			//速度
    POS,			//位置
    CURRENT,		//力矩

    MAX_ADDR,
}funsionType;

typedef struct{
    uint8_t len;
    uint8_t id;
    uint8_t cmd;
    uint8_t fun;
    float data;
    uint8_t checkVal;
}devFunTypedef;

#define MAX_REV_DATA_LEN    20

typedef enum{
    TAR_POS,
    CUR_POS,
    TAR_SPEED,
    CUR_SPEED,
    TAR_CURRENT,
    CUR_CURRENT,
}dataIndex;

class motorCtl: public QThread
{
    Q_OBJECT
public:
    motorCtl();
    void openDev();
    void setTarPos(float val);
    void setTarSpeed(float val);
    void setTarCurrent(float val);
    float curPos;
    float curSpeed;
    float curCurrent;

    float motorCurData[MAX_REV_DATA_LEN];

private:
    serial *com;
    QDateTime curTime;
    QTimer *timer;
    QByteArray buf;     //接收buf
    void run();
    void senDataToBus(cmdTypedef cmd, funsionType fun, float val);

private slots:
    void readyReadSlot();
};

#endif // MOTORCTL_H
