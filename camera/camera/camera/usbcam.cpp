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

//打开摄像头
bool usbcam::open(int id)
{
    cam = VideoCapture(id);
    qDebug()<<"camera "<< id << " open"<< ((cam.isOpened())? " succeed!!!": " failed!!!");
    return cam.isOpened();
}

void usbcam::close(int id)
{
    Q_UNUSED(id);
    qDebug()<<"usb cam close"<<id;
}

bool usbcam::getImg()
{
    qDebug()<<"usb cam getImg";
}

void usbcam::saveImg()
{
    qDebug()<<"usb cam saveImg";
}

