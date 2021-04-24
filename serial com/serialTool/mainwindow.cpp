#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QObject>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->serialUI = new serialTool();
    ui->tabWidget->insertTab(0,serialUI,"串口助手");


//    devThr = new serialDev();
//    devThr->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//https://blog.csdn.net/qq_29344757/article/details/78136829
