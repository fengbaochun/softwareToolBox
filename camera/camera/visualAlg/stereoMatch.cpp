#include "stereoMatch.h"
#include <QThread>

// 参考 ： https://zhuanlan.zhihu.com/p/349896858

stereoMatch::stereoMatch(QObject *parent)
    : QObject{parent}
{

}

void stereoMatch::loadImg(QString s)
{
    qDebug()<<QThread::currentThreadId()<<s ;
}

void stereoMatch::camClicked()
{
    qDebug()<<QThread::currentThreadId()<<" + -- - - !!! 123456789";
}
