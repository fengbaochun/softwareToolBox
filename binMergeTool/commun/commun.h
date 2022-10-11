#ifndef COMMUN_H
#define COMMUN_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QTimer>
#include <QString>
#include <QDateTime>
#include <QQueue>
#include <QMap>
#include <QList>

typedef QList<QString> comIdList;
typedef void (*pFun)(QByteArray);

class commun : public QObject
{
    Q_OBJECT
public:
    explicit commun(QObject *parent = nullptr);
    virtual ~commun();              //析构函数

    enum communType{                //通讯类型
        SERIAL_COMMUN,              //串口通讯
        CAN_COMMUN,                 //can总线通讯
    };

    static commun* create(communType type);
//    virtual comIdList scanPort() = 0;                   //扫描
    virtual bool open(int id) = 0;                      //打开
    virtual void close() = 0;                           //关闭
    virtual QByteArray getData() = 0;                   //获取数据

    void start();                                       //通信开始
    void stop();                                        //通信停止
    void registerRevFun(pFun fun);                      //注册接收函数
    QQueue <QByteArray> datQue;                         //数据队列
private:
    QTimer *tim = nullptr;                              //定时器

signals:
    void reveiced(QByteArray);

private slots:
    void dataUpdateCallBack();                           //图像更新回调
};

#endif // COMMUN_H
