#ifndef FLIRCAM_H
#define FLIRCAM_H

#include "camera.h"

class flircam : public camera
{
public:
    explicit flircam(QObject *parent = nullptr);
    ~flircam();

    static flircam* instance();

    bool open(int id);                      //打开摄像头
    void close();                           //关闭摄像头
    bool getImg(Mat &img);                  //获取图像

private:
    static flircam* INSTANCE;
};

#endif // FLIRCAM_H
