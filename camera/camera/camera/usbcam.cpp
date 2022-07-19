#include "usbcam.h"

usbcam* usbcam::INSTANCE = nullptr;         //实例指针

usbcam::usbcam(QObject *parent)
    : camera{parent}
{

}

usbcam::~usbcam()
{

}

//创建USB摄像头实例
usbcam *usbcam::instance()
{
    if(INSTANCE == nullptr){
        INSTANCE = new usbcam();
    }
    return INSTANCE;
}

bool usbcam::open(int id)
{
    Q_UNUSED(id);
    qDebug()<<"usb cam open"<<id;

    return true;
}

void usbcam::close(int id)
{
    Q_UNUSED(id);
    qDebug()<<"usb cam close"<<id;
}

void usbcam::getImg(int id)
{
    Q_UNUSED(id);
    qDebug()<<"usb cam getImg"<<id;
}

void usbcam::saveImg()
{
    qDebug()<<"usb cam saveImg";
}
