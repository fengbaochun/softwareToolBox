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
    ui->tabWidget->setCurrentIndex(0);
}


MainWindow::~MainWindow()
{
    delete ui;
}


