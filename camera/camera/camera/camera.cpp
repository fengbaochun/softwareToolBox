﻿#include "camera/camera.h"
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

}

//析构函数
camera::~camera()
{
    stop();
}

//设置图像采集帧率
void camera::setFps(float fps){
    uint32_t time = 1000.0/fps;
    if(tim != nullptr){
        tim->setInterval(time);
    }
}

//保存图像
void camera::saveImg(QString path, QImage img, QString mark)
{
    if(!img.isNull()){
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss.zzz");
//        QString name = ".\\"+ time + ".jpg";
        QString name = path+ time + mark +".jpg";
        bool ret = img.save(name);
        qDebug()<<"img : "<<name<<"写入"<<(ret == true ? "成功" : "失败");
    }else{
        qDebug()<<"请打开摄像头！！！";
    }
}


//启动相机
void camera::start(int id, float fps)
{
    if(tim == nullptr){
        tim = new QTimer();
        connect(tim,SIGNAL(timeout()),this,SLOT(imgUpdateCallBack()));
        qDebug()<<id<<"time start--------";
    }
    this->id = id;
    open(id);
    setFps(fps);
    tim->start();
    qDebug()<<"start thread id : "<<QThread::currentThreadId();
}

//停止相机
void camera::stop()
{
    tim->stop();
    this->close();
    imgQue.clear();
    qDebug()<<"stop thread id : "<<QThread::currentThreadId();
}

QImage camera::mat2QImage(Mat cvImg, int format)
{
    Mat cvRgbImg;
    if (format == 1)//RGB
    {
        cvtColor(cvImg, cvRgbImg, COLOR_BGRA2RGB);
        QImage dstImage((const uchar *)cvRgbImg.data, cvRgbImg.cols, cvRgbImg.rows, cvRgbImg.step, QImage::Format_RGB888);
        dstImage.bits();
        return dstImage;
    }
    else//二值化
    {
        cvRgbImg=cvImg;
        QImage dstImage((const uchar *)cvRgbImg.data, cvRgbImg.cols, cvRgbImg.rows, cvRgbImg.step, QImage::Format_Grayscale8);
        dstImage.bits();
        return dstImage;
    }
}


//定时器图像更新
void camera::imgUpdateCallBack()
{
    if(imgQue.size() > 0){
        emit sendImg(this->id, mat2QImage(imgQue.dequeue(), 1));    //图像输出
    }

    Mat img;
    bool ret = getImg(img);                                         //图像采集
    if(ret){
        imgQue.append(img);
        qDebug()<<"img size info : "<< img.size <<imgQue.size()<<QThread::currentThreadId();
    }else{
//        qDebug()<<"camera "<<"get img "<< ((ret)? "succeed!!!": " failed!!!");
    }
}





