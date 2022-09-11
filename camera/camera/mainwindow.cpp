#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

#include "visualAlg/stereoMatch.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug()<<"MainWindow thread id : "<<QThread::currentThreadId();
    this->camUi = new camPage();
    ui->tabWidget->insertTab(0, camUi, "相机调试");
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
//    emit sendString("123456789\r\n");                   //发射信号响应 线程中对应的槽函数
}


