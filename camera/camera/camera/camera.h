#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QTimer>
#include <QString>
#include <QDateTime>
#include <QQueue>
#include <QMap>
#include <QImage>

#include <opencv2/opencv.hpp>
using namespace cv;

class camera : public QThread
{
    Q_OBJECT
public:
    enum camType{               //摄像头类型
        USB_CAM,                //USB摄像头
        FLIR_CAM,               //FLIR摄像头
        HK_CAM,                 //海康摄像头
    };

    static camera* create(camType type);
    explicit camera(QObject *parent = nullptr);
    virtual ~camera();                                  //析构函数（有啥好处？）

    virtual bool open(int id) = 0;                      //打开摄像头
    virtual void close() = 0;                           //关闭摄像头
    virtual bool getImg(Mat &img) = 0;                  //获取图像

    void start(int id, float fps = 20.0f);              //启动
    void stop();                                        //停止
    void setFps(float fps);                             //设置帧率
    void saveImg(QString path, QImage img, QString mark = "-");                           //保存图像

    QQueue <cv::Mat> imgQue;                            //图像队列

private:
    int id;                                             //相机ID
    QTimer *tim = nullptr;                              //定时器
    QImage mat2QImage(Mat cvImg, int format);           //图像格式化

signals:
    void sendImg(int, QImage);                          //发送图片

private slots:
    void imgUpdateCallBack();                           //图像更新回调


};

#endif // CAMERA_H

