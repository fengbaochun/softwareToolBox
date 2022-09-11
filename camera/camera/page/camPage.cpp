#include "camPage.h"
#include "ui_camPage.h"

camPage::camPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::camPage)
{
    ui->setupUi(this);
    qDebug()<<"camPage thread id : "<<QThread::currentThreadId();

    camera::camType type = camera::USB_CAM;     //USB摄像头
    cam = camera::create(type);                 //创建一个相机

    QThread *camThd= new QThread();             //创建一个线程
    cam->moveToThread(camThd);                  //把这个对象移到线程里面使用

    connect(cam, &camera::sendImg, this, &camPage::imgShowCallback,Qt::DirectConnection);   //图片刷新
    camThd->start();
}

camPage::~camPage()
{
    delete ui;
}

//图片显示
void camPage::imgShowCallback(QImage img)
{
    ui->imgLabel->setScaledContents(true);              //图片自适应空间大小
    ui->imgLabel->setPixmap(QPixmap::fromImage(img));
}

//打开相机
void camPage::on_openBut_toggled(bool checked)
{
    QString name = (checked == true) ? "关闭相机" : "打开相机";
    ui->openBut->setText(name);
    if(checked){
        QMetaObject::invokeMethod(cam, [this]{
                                            cam->start(0);                   //打开摄像头
                                            qDebug()<<"cam->start(0) thread id : "<<QThread::currentThreadId();
        });
    }else{
        QMetaObject::invokeMethod(cam, [this]{
                                            cam->stop();                     //关闭摄像头
                                            qDebug()<<"cam->stop(0) thread id : "<<QThread::currentThreadId();
        });
        ui->imgLabel->clear();
    }
}

//相机帧率设置
void camPage::on_spinBox_valueChanged(int arg1)
{
    QMetaObject::invokeMethod(cam, [=]{ cam->setFps(arg1);                   //打开摄像头
        qDebug()<<"cam->setFps(arg1): "<<QThread::currentThreadId();
    });
}

