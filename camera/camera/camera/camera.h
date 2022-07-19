#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QDebug>
#include <QThread>

class camera : public QThread
{
    Q_OBJECT
public:
    enum camType{               //摄像头类型
        USB_CAM,                //USB摄像头
        FLIR_CAM,               //FLIR摄像头
    };

    static camera* create(camType type);

    explicit camera(QObject *parent = nullptr);
    virtual ~camera();                                  //析构函数（有啥好处？）

    virtual bool open(int id) = 0;                      //打开摄像头
    virtual void close(int id) = 0;                     //关闭摄像头
    virtual void getImg(int id) = 0;                    //获取图像
    virtual void saveImg() = 0;                         //保存图像

    void run();

signals:

};

#endif // CAMERA_H
