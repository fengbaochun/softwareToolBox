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

//关闭相机
void usbcam::close()
{
    cam.release();
    qDebug()<<"usb cam close";
}

//获取图像
bool usbcam::getImg(Mat &img)
{
    if(cam.isOpened())cam >> img;
    return (!img.empty())? true:false;
}


void usbcam::saveImg()
{
    qDebug()<<"usb cam saveImg";
}

Mat usbcam::getImgData()
{

}

