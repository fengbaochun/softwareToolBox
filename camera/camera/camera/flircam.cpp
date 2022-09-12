#include "flircam.h"

flircam* flircam::INSTANCE = nullptr;         //实例指针

flircam::flircam(QObject *parent)
    : camera{parent}
{

}

flircam::~flircam()
{

}

flircam *flircam::instance()
{
//    if(INSTANCE == nullptr){                  //单例
//        INSTANCE = new flircam();
//    }
//    return INSTANCE;
    return new flircam();
}

bool flircam::open(int id)
{
    Q_UNUSED(id);
    qDebug()<<"flircam open"<<id;
    return true;
}

void flircam::close()
{
    qDebug()<<"flircam close";
}

bool flircam::getImg(Mat &img)
{
    Q_UNUSED(img);
    qDebug()<<"flircam getImg";
    return false;
}

void flircam::saveImg()
{
    qDebug()<<"flircam saveImg";
}
