#ifndef MAINCTL_H
#define MAINCTL_H

#include <QObject>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSemaphore>
#include <QCoreApplication>

#define CMD_BASE_ADDR       0x20    //命令基地址
#define FUN_BASE_ADDR       0x30	//参数表的基地址
#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

typedef enum{
    READ_CMD = CMD_BASE_ADDR,           //读
    WRITE_CMD,                          //写
    ANSWER_CMD,                         //应答
    AUTO_REPORT_CMD,                    //主动上报
    ERROR_CMD,                          //命令错误
    CMD_MAX,                            //最大值
}cmdTypedef;

typedef enum{
    KEY_STATUS     =   FUN_BASE_ADDR,   //开关按键
    E_CODE,                             //故障码（主动上报）
    RFTI_SW,	                        //射频开关（脚踏板、功率电源开关）
    RFTI_ENERY,                         //射频能量
    POWERIO,							//电源IO
    MAX_ADDR
}funsionType;

typedef struct{
    bool isAnswer;
    bool isReport;
    uint32_t data;          //应答数据
    uint32_t rData;         //上报数据
}answerType;

typedef union{
    uint32_t data;
    uint16_t da[2];
    uint16_t d[4];
}u32DataType;

class mainctl : public QObject
{
    Q_OBJECT
public:
    explicit mainctl(QObject *parent = nullptr);
    static mainctl* instance();
    bool open(QString name, qint32 bsp);
    void close();
    bool openRftiSw(bool isOpen);                   //打开/关闭射频开关（脚踏板）
    bool setRftiEnergy(uint8_t val);                //设置射频能量
    bool getRftiEnergy(uint8_t &val);               //获取射频能量
    bool powerOff();                                //关机
    bool setPowerIo(uint8_t id, bool isOpen);       //设置电源状态

private:
    static mainctl* INSTANCE;
    QSerialPort *port;
    QByteArray buf;
    answerType result[MAX_ADDR - FUN_BASE_ADDR];

    void packageToBus(cmdTypedef cmd, funsionType fun, uint32_t val);
    bool packageToBus(cmdTypedef cmd, funsionType fun, uint32_t val, int timoutVal, bool isBlock);
    uint32_t getResultData(funsionType fun);
    uint8_t checkValCode(QByteArray ary, uint8_t start, uint8_t end);

private slots:
    void readyReadSlot();


signals:
    void report(funsionType fun, uint32_t data);
};

#endif // MAINCTL_H
