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

camera::camera(QObject *parent)
    : QThread()
{

}

camera::~camera()
{

}

void camera::run()
{
    while(true){
        getImg(1);
        msleep(500);
    }
}
