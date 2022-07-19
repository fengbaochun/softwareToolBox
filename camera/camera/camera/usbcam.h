#ifndef USBCAM_H
#define USBCAM_H

#include "camera.h"

class usbcam : public camera
{
public:
    explicit usbcam(QObject *parent = nullptr);
    ~usbcam();

    static usbcam* instance();

    bool open(int id);                      //打开摄像头
    void close(int id);                     //关闭摄像头
    void getImg(int id);                    //获取图像
    void saveImg();                         //保存图像

private:
    static usbcam* INSTANCE;


};

#endif // USBCAM_H
