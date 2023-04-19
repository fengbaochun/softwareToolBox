#ifndef UPROTOCOL_H
#define UPROTOCOL_H

#include <QObject>
#include <QDebug>
#include <QSerialPort>
#include <QtSerialPort/QSerialPort>
#include <QCoreApplication>
#include <QElapsedTimer>
#include <QTimer>
#include <QThread>
#include <QMap>

#define CMD_BASE_ADDR       0x20    //命令基地址
#define FUN_BASE_ADDR       0x30	//参数表的基地址
#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

const QMap<QString, uint8_t> cmd = {
    std::map<QString, uint8_t>::value_type("read", 0x20),
    std::map<QString, uint8_t>::value_type("write", 0x21),
    std::map<QString, uint8_t>::value_type("answer", 0x22),
    std::map<QString, uint8_t>::value_type("autoReport", 0x23),
    std::map<QString, uint8_t>::value_type("error", 0x24),
};

class uProtocol : public QObject
{
    Q_OBJECT
public:
    explicit uProtocol(QObject *parent = nullptr);

    typedef union{
        uint32_t data;
        uint16_t da[2];
        uint16_t d[4];
    }u32DataType;

    uProtocol* create();
    bool open(QString name, qint32 bsp);
    void close();
    void showFunCode();
    void registerFun(uint8_t code);
    bool packageToBus(uint8_t cmd, uint8_t fun, uint32_t val, int timoutVal, bool isBlock);
    uint32_t getResultData(uint8_t fun);

private:
    typedef struct{
        bool isAnswer;
        bool isReport;
        uint32_t data;          //应答数据
        uint32_t rData;         //上报数据
    }answerType;

    QSerialPort *port;
    QByteArray buf;
    QMap<uint8_t, answerType> mFun;

    void packageToBus(uint8_t cmd, uint8_t fun, uint32_t val);
    uint8_t checkValCode(QByteArray ary, uint8_t start, uint8_t end);
    bool isExist(uint8_t code);

private slots:
    void readyReadSlot();


signals:
    void report(uint8_t fun, uint32_t data);

};

#endif // UPROTOCOL_H
