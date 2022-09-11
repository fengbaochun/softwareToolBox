#include "camPage.h"
#include "ui_camPage.h"

camPage::camPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::camPage)
{
    ui->setupUi(this);
    qDebug()<<"camPage thread id : "<<QThread::currentThreadId();

    camera::camType type = camera::USB_CAM;     //USB摄像头
    cam = camera::create(type);                 //工厂模式

//    QThread *camThd= new QThread();             //创建一个线程
//    cam->moveToThread(camThd);                  //把这个对象移到线程里面使用

    connect(cam, &camera::sendImg, this, &camPage::imgShowCallback,Qt::DirectConnection);   //图片刷新

//    stereoMatch* stere = new stereoMatch();                                     //创建一个对象
//    QThread *alg = new QThread();                                               //创建一个线程
//    stere->moveToThread(alg);                                                   //把这个对象移到线程里面 使用
//    connect(this, SIGNAL(sendString(QString)), stere, SLOT(loadImg(QString)));
//    camThd->start();
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
    qDebug()<<"camera "<<QThread::currentThreadId();
}

//打开相机
void camPage::on_openBut_toggled(bool checked)
{
    QString name = (checked == true) ? "关闭相机" : "打开相机";
    ui->openBut->setText(name);
    if(checked){
        cam->start(0);                                  //打开摄像头
    }else{
        cam->stop();                                    //关闭摄像头
        ui->imgLabel->clear();                          //清除控件图像
    }
}

//相机帧率设置
void camPage::on_spinBox_valueChanged(int arg1)
{
    cam->setFps(arg1);
}

