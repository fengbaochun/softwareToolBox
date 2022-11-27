#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "./hidapi-win/include/hidapi.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    usb = new usbHid();

//    usb->scan();

    usb->open(0x0483, 0x5750);
    uint8_t data[64];
    for(int i=0;i<64;i++){
        data[i] = i;
    }
//    memset(data,0x13,64);
    usb->write(0x09, data);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//https://blog.csdn.net/weixin_45381001/article/details/125802184
//https://blog.csdn.net/qq_36347513/article/details/127694257
//https://www.freesion.com/article/60011508420/



void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    qDebug("position %d",position);
    uint8_t data[64];
    memset(data,position,64);
    usb->write(0x09, data);
}

