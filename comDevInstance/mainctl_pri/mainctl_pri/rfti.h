#ifndef RFTI_H
#define RFTI_H

#include <QObject>
#include "mainctl_pri/uProtocol.h"

const QMap<QString, uint8_t> rftiFun = {
    std::map<QString, uint8_t>::value_type("sw", 0x32),
    std::map<QString, uint8_t>::value_type("energy", 0x33),
};

class rfti : public QObject
{
    Q_OBJECT
public:
    explicit rfti(QObject *parent = nullptr);
    static rfti* instance();
    bool open(QString name, qint32 bsp);
    void close();
    bool openRftiSw(bool isOpen);                   //打开/关闭射频开关（脚踏板）
    bool setRftiEnergy(uint8_t val);                //设置射频能量
    bool getRftiEnergy(uint8_t &val);               //获取射频能量

private slots:
    void reportCallBack(uint8_t fun, uint32_t data);

private:
    static rfti* INSTANCE;
    uProtocol *com;

};

#endif // RFTI_H
