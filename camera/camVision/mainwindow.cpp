#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //图像采集线程
    cam = new camera();
    cam->start();

//    接收子线程发来的数据
    connect(cam, &camera::sendImg, this, &MainWindow::imgShowCallback,Qt::DirectConnection);

}

MainWindow::~MainWindow()
{
    delete ui;
}

//显示图像
void MainWindow::imgShowCallback(QImage img)
{
    ui->imgLabel->setScaledContents(true);
    ui->imgLabel->setPixmap(QPixmap::fromImage(img));
}

//摄像头打开关闭
void MainWindow::on_camOpenButton_toggled(bool checked)
{
    QString name = (checked == true) ? "关闭相机" : "打开相机";
    ui->camOpenButton->setText(name);
    if(checked){
        cam->open(0);                       //打开摄像头
    }else{
        cam->close();                       //关闭摄像头
        ui->imgLabel->clear();              //清除控件图像
    }
}

//保存图像
void MainWindow::on_pushButtonSave_clicked()
{
    cam->saveImg();
}

