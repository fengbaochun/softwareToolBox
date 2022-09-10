#ifndef USBCAM_H
#define USBCAM_H

#include "camera.h"
#include <QObject>
#include <QThread>
#include <opencv2/opencv.hpp>
#include <QString>
#include <QDateTime>
#include <QQueue>
#include <QMap>
#include <QSemaphore>
#include <QImage>

using namespace cv;

class usbcam : public camera
{
public:
    explicit usbcam(QObject *parent = nullptr);
    ~usbcam();

    static usbcam* instance();

    bool open(int id);                      //打开摄像头
    void close(int id);                     //关闭摄像头
    bool getImg();                           //获取图像
    void saveImg();                         //保存图像

private:
    static usbcam* INSTANCE;
    VideoCapture cam;

};

#endif // USBCAM_H
