#ifndef SERIALDEV_H
#define SERIALDEV_H

#include <QThread>
#include <QDebug>
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

class serialDev : public QThread
{
     Q_OBJECT
public:
    serialDev();
    ~serialDev();
    void readData();

    QStringList baudList;

private:
    QSerialPort *serPort;
    QByteArray m_tempAll;

protected:
    void run(); //线程执行函数

//信号
signals:
    void reveiced(QString);

//槽函数
public slots:
    void revDataSlot();

};

#endif // SERIALDEV_H
