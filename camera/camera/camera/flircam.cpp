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
    if(INSTANCE == nullptr){
        INSTANCE = new flircam();
    }
    return INSTANCE;
}

bool flircam::open(int id)
{
    Q_UNUSED(id);
    qDebug()<<"flircam open"<<id;
    return true;
}

void flircam::close(int id)
{
    Q_UNUSED(id);
    qDebug()<<"flircam close"<<id;
}

bool flircam::getImg()
{
    qDebug()<<"flircam getImg";
}

void flircam::saveImg()
{
    qDebug()<<"flircam saveImg";
}
