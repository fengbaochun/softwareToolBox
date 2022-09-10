#include "camera/camera.h"
#include "camera/usbcam.h"
#include "camera/flircam.h"

//创建一个摄像头类型
camera *camera::create(camType type)
{
    if(type == USB_CAM){                    //普通的USB摄像头
        return usbcam::instance();
    }else if(type == FLIR_CAM){             //FLIR摄像头
        return flircam::instance();
    }
    return nullptr;
}

//构造函数
camera::camera(QObject *parent)
    : QThread()
{
    Q_UNUSED(parent);
    tim = new QTimer();
    connect(tim,SIGNAL(timeout()),this,SLOT(imgUpdateCallBack()));
    setFps(1.0f);
    tim->start();
}

//析构函数
camera::~camera()
{
    tim->stop();
}

//打开摄像头
bool camera::open(int id){
}

//获取图像
bool camera::getImg(){

}


//设置图像采集帧率
void camera::setFps(float fps){
    uint32_t time = 1000.0/fps;
    tim->setInterval(time);
}


//定时器图像更新
void camera::imgUpdateCallBack()
{
    getImg();
}





