#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug()<<"this is test demo!!!";

//    camera::camType type = camera::USB_CAM;     //USB摄像头
    camera::camType type = camera::FLIR_CAM;     //USB摄像头
    camera* cam = camera::create(type);

    cam->open(1);
    cam->close(2);
    cam->getImg(3);
    cam->saveImg();

    cam->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

