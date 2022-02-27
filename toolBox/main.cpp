 #include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}


//#include "mainwindow.h"

//#include <QApplication>

//#include "opencv2/core.hpp"
//#include <opencv2/core/utility.hpp>
//#include "opencv2/imgproc.hpp"
//#include "opencv2/calib3d.hpp"
//#include "opencv2/imgcodecs.hpp"
//#include "opencv2/videoio.hpp"
//#include "opencv2/highgui.hpp"
//#include <iostream>

//using namespace cv;

//using namespace std;
////摄像头ID
////笔记本的是0，USB的从1依次递增
//#define CAMERA_ID_NUM	1

////图像大小
////#define CAM_W	640
////#define CAM_H	480
//#define CAM_W	1280
//#define CAM_H	720

////图像缩放
//#define IMAGE_SIZE_NUM 1

//int main()
//{
//    //初始化摄像机
//    VideoCapture cam_dev(CAMERA_ID_NUM);
//    cam_dev.set(CAP_PROP_FRAME_WIDTH, CAM_W / IMAGE_SIZE_NUM);
//    cam_dev.set(CAP_PROP_FRAME_HEIGHT, CAM_H / IMAGE_SIZE_NUM);
//    //校检连接
//    if (cam_dev.isOpened())
//        printf("camera %d open succeed!!!\r\n", CAMERA_ID_NUM);
//    else
//    {
//        printf("camera %d open failed!!!\r\n plase check!!!\r\n", CAMERA_ID_NUM);
//        while (1);
//    }
//    Mat3b image_frame;
//    int img_num = 1000;
//    while (img_num--) {
//        cam_dev >> image_frame;
//        imshow("Video", image_frame);
//        waitKey(10);
//    }


//    qDebug()<<"123456789";
//    return 0;

//}



//https://www.cnblogs.com/jeffkuang/articles/6477790.html





