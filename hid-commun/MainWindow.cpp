#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "./hidapi-win/include/hidapi.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    usb = new usbHid();

    usb->viewDevInfo();

    usb->read();

    usb->write();

    usb->read();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//https://blog.csdn.net/weixin_45381001/article/details/125802184
//https://blog.csdn.net/qq_36347513/article/details/127694257
