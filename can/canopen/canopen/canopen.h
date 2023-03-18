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
signals:

};

#endif // CANOPEN_H
