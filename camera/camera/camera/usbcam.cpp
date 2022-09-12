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
//    if(INSTANCE == nullptr){          //单例模式
//        INSTANCE = new usbcam();
//    }
//    return INSTANCE;

    return new usbcam();
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

//扫描端口
camIdList usbcam::scanPort()
{
    camIdList list;
    for(int i = 0;i < 10; i++){
        if(this->open(i)){                          //尝试打开
            list.append(QString::number(i,10));     //添加可以打开的相机到list
            this->close();                          //关闭相机
            msleep(1);
        }
    }
    return list;
}


