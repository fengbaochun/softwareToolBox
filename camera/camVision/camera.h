#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QThread>
#include <opencv2/opencv.hpp>
#include <QTimer>
#include <QString>
#include <QDateTime>
#include <QQueue>
#include <QMap>
#include <QSemaphore>
#include <QImage>

using namespace cv;

typedef struct {
    Mat frame;              //帧
    QString time;
}imgType;

class camera:public QThread
{
    Q_OBJECT

    //摄像头ID
    //笔记本的是0，USB的从1依次递增
#define CAMERA_ID_NUM	0

    //图像大小
#define CAM_W	640
#define CAM_H	480
    //图像缩放
#define IMAGE_SIZE_NUM 1
public:
    camera();
    camera(QTimer *update, QSemaphore *sync, const VideoCapture &cam_dev, const QQueue<imgType> &imgQue);

    void close();                       //关闭摄像头
    bool open(int id);                  //打开摄像头（并开始采集）
    void suspend();                     //暂停摄像头
    void saveImg();                     //保存图像

    imgType curImg;                     //当前图像帧，用于图像保存

    const Mat &getImageFrame() const;
    void setImageFrame(const Mat &newImageFrame);

private:
    QTimer *tim;                                //定时器
    QSemaphore *sync;                           //信号量
    VideoCapture cam;                           //
    QQueue <imgType> imgQue;                    //图像队列

    imgType fillImgData(Mat img);               //装载数据到结构体
    Mat getImg(int id);                         //获取图像
    QImage Mat2QImage(Mat cvImg, int format);

protected:
    void run();

signals:
    void sendImg(QImage);              //发送图片到UI

    void imageFrameChanged();

private slots:
    void imgUpdateSync();

};

#endif // CAMERA_H
