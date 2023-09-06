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
#include <QProcess>
#include <QDataStream>
#include <QVector>

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

//应用报文结构
enum{
    CM_H1,
    CM_H2,
    CM_LEN,
    CM_CMD,
    CM_FUN,
    CM_DATA
};

class uProtocol : public QObject
{
    Q_OBJECT
public:
    explicit uProtocol(QObject *parent = nullptr);
    virtual ~uProtocol();
    typedef union{
        uint32_t data;
        uint16_t da[2];
        uint16_t d[4];
    }u32DataType;

    virtual bool open(QString name, qint32 bsp);
    virtual void close();
    virtual void showFunCode();

    void registerFun(uint8_t code);
    bool packageToBus(uint8_t cmd, uint8_t fun, QByteArray qb, int timoutVal, bool isBlock);
    bool packageToBus(uint8_t cmd, uint8_t fun, QByteArray qb, int timoutVal, int cnt, bool isBlock);

    bool packageToBus(uint8_t cmd, uint8_t fun, uint32_t val, int timoutVal, bool isBlock);             //为了向下兼容之前的 代码
    bool packageToBus(uint8_t cmd, uint8_t fun, uint32_t val, int timoutVal, int cnt, bool isBlock);

    QByteArray getResultData(uint8_t fun);
    uint32_t  getResultU32Data(uint8_t fun);

    //不同数据类型转换
    template <typename T>
    static T byteArrayToValue(const QByteArray &byteArray, QDataStream::ByteOrder byteOrder = QDataStream::LittleEndian) {
        T value;
        QDataStream stream(byteArray);
        stream.setByteOrder(byteOrder);
        stream.setFloatingPointPrecision(QDataStream::SinglePrecision); // 设置为单精度浮点数
        stream >> value;
        return value;
    }

    //拆组
    QVector<QByteArray> splitByteArray(const QByteArray &byteArray, int dataSize);
    QVector<float> splitByteArrayToData(const QByteArray &byteArray, int dataSize);

    float fps = 0.0f;

protected:
    QObject* worker_;
    QThread workerThread_;

private:
    typedef struct{
        bool isAnswer;
        bool isReport;
        QByteArray data;          //应答数据
        QByteArray rData;         //上报数据
    }answerType;

    QSerialPort *port;
    QByteArray buf;
    QVector<QByteArray> message;  //消息列表
    QMap<uint8_t, answerType> mFun;

    QTimer *fpsT;
    QTimer *revT;

    void packageToBus(uint8_t cmd, uint8_t fun, uint8_t *data, uint8_t dLen);
    uint8_t checkValCode(QByteArray ary, uint8_t start, uint8_t end);
    bool isExist(uint8_t code);
    void process();
    int pkgNum = 0, lastPkgNum = 0;

private slots:
    void readyReadSlot();


signals:
    void report(uint8_t fun, QByteArray qb);

};

#endif // UPROTOCOL_H
