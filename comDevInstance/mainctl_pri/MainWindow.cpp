#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "mainctl_pri/rfti.h"
#include "mainctl_pri/powerCtl.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    rfti::instance()->open("COM10", 115200);            //射频
    powerCtl::instance()->open("COM9", 115200);         //主控板
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked(bool checked)
{
    powerCtl::instance()->openRftiSw(checked);
}

void MainWindow::on_pushButton_4_clicked()
{
    powerCtl::instance()->powerOff();
}

void MainWindow::on_pushButton_5_clicked()
{
    static bool ret = false;

    powerCtl::instance()->setPowerIo(13, ret);
    powerCtl::instance()->setPowerIo(14, ret);
    powerCtl::instance()->setPowerIo(15, ret);
    powerCtl::instance()->setPowerIo(16, ret);
    powerCtl::instance()->setPowerIo(17, ret);

    ret = !ret;
}

void MainWindow::on_pushButton_6_clicked()
{
    static bool ret = true;
    rfti::instance()->openRftiSw(ret);
    ret =!ret;
}

void MainWindow::on_pushButton_7_clicked()
{
    uint8_t data;
    rfti::instance()->getRftiEnergy(data);
}


void MainWindow::on_pushButton_8_clicked()
{
    static uint8_t val = 0;
    rfti::instance()->setRftiEnergy(val++);
}

