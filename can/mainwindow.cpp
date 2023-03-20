#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    can = new canDev();
//    can->start();

    canfestival = new canopen();
    canfestival->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_peelButton_clicked()
{
    qDebug()<<"获取设备类型";
    canfestival->readDevType();
}

void MainWindow::on_calButton_clicked()
{
    qDebug()<<"复位节点";
    canfestival->restNode();
}

void MainWindow::on_zeroButton_clicked()
{

}

void MainWindow::on_canclePeelButton_clicked()
{

}
