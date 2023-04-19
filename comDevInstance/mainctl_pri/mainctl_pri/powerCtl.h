#ifndef POWERCTL_H
#define POWERCTL_H

#include <QObject>
#include "mainctl_pri/uProtocol.h"

const QMap<QString, uint8_t> powerFun = {
    std::map<QString, uint8_t>::value_type("key", 0x30),
    std::map<QString, uint8_t>::value_type("rftisw", 0x32),
    std::map<QString, uint8_t>::value_type("powerIO", 0x34),
};

class powerCtl : public QObject
{
    Q_OBJECT
public:
    explicit powerCtl(QObject *parent = nullptr);
    static powerCtl* instance();
    bool open(QString name, qint32 bsp);
    void close();
    bool openRftiSw(bool isOpen);                   //打开/关闭射频开关（脚踏板）
    bool powerOff();                                //关机
    bool setPowerIo(uint8_t id, bool isOpen);       //设置电源状态

private slots:
    void reportCallBack(uint8_t fun, uint32_t data);

private:
    static powerCtl* INSTANCE;
    uProtocol *com;
};

#endif // POWERCTL_H
