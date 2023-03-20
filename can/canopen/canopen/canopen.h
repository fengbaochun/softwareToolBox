#ifndef CANOPEN_H
#define CANOPEN_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QString>
#include <QDateTime>
#include "canfestival.h"

class canopen : public QThread
{
    Q_OBJECT
public:
    explicit canopen(QObject *parent = nullptr);
    void run();

    void init();
//    void start();
//    void stop();
    void readDevType();             //读取设备类型
    void readDevVision();           //读取设备版本号
    void restNode();                //复位节点

signals:

};

#endif // CANOPEN_H
