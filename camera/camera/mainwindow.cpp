#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

#include "visualAlg/stereoMatch.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug()<<QThread::currentThreadId()<<"this is test demo!!!";

    stereoMatch* stere = new stereoMatch();                                     //创建一个对象
    QThread *alg = new QThread();                                               //创建一个线程
    stere->moveToThread(alg);                                                   //把这个对象移到线程里面 使用
    connect(this, SIGNAL(sendString(QString)), stere, SLOT(loadImg(QString)));
    alg->start();                                                               //开启线程


////    camera::camType type = camera::USB_CAM;     //USB摄像头
//    camera::camType type = camera::FLIR_CAM;     //USB摄像头
//    camera* cam = camera::create(type);

//    cam->open(1);
//    cam->close(2);
//    cam->getImg(3);
//    cam->saveImg();
//    cam->start();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    emit sendString("123456789\r\n");                                           //发射信号响应 线程中对应的槽函数
}

