#include "camera.h"
#include <QDebug>

//构造函数
camera::camera()
{
    tim = new QTimer();
    connect(tim,SIGNAL(timeout()),this,SLOT(imgUpdateSync()));
}

//装载图像数据(带时间戳)
imgType camera::fillImgData(Mat img)
{
    imgType temp;
    temp.frame = img;
    temp.time = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss.zzz");
    return temp;
}

//获取图像
Mat camera::getImg(int id)
{
    Q_UNUSED(id);
    Mat img;
    cam >> img;
    return img;
}

//关闭
void camera::close(){
    tim->stop();
    imgQue.clear();
}

//打开
bool camera::open(int id)
{
    Q_UNUSED(id);
    tim->setInterval(20);
    tim->start();
    return true;
}

//保存图像(https://blog.csdn.net/bigData1994pb/article/details/119541300)
void camera::saveImg(){
    if(!curImg.frame.empty()){                      //图像不为空
        QImage img(curImg.frame.data,
                   curImg.frame.cols,
                   curImg.frame.rows,
                   curImg.frame.step,
                   QImage::Format_BGR888);
        QString name = ".\\"+ curImg.time + ".jpg";
        bool ret = img.save(name);
        qDebug()<<"img : "<<name<<"写入"<<(ret == true ? "成功" : "失败");
    }else{
        qDebug()<<"请打开摄像头！！！";
    }
}

//线程运行
void camera::run()
{
    //初始化摄像机
    cam = VideoCapture(CAMERA_ID_NUM);

    cam.set(CAP_PROP_FRAME_WIDTH, CAM_W/ IMAGE_SIZE_NUM);
    cam.set(CAP_PROP_FRAME_HEIGHT, CAM_H/ IMAGE_SIZE_NUM);

    if (cam.isOpened()){
        qDebug()<<"Camera open succeed!!!\r\n";
    }else{
        qDebug()<<"Camera open failed!!!\r\n";
    }

    while(1)
    {
        if(cam.isOpened() && tim->isActive()){
            Mat img = getImg(0);                //获取图像
            if(!img.empty()){                   //图像及相机状态检测
                imgQue.append(fillImgData(getImg(0)));
            }else{
                qDebug()<<"Camera img is empty!!!\r\n";
            }
        }else{
            qDebug()<<"Camera disconnect !!!\r\n";
        }

        msleep(1);
    }
}

QImage camera::Mat2QImage(Mat cvImg, int format)
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


//图像更新 & 同步
void camera::imgUpdateSync()
{
    if(!imgQue.empty()){
        curImg = imgQue.dequeue();
        emit sendImg(Mat2QImage(curImg.frame, 1));
    }else{

    }
}
