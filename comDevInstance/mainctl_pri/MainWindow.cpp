#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "./mainctl_pri/mainctl.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mainctl::instance()->open("COM10", 115200);
    connect(mainctl::instance(),SIGNAL(report(funsionType, uint32_t )),this,SLOT(reportCallBack(funsionType , uint32_t)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked(bool checked)
{
    mainctl::instance()->openRftiSw(checked);
}


void MainWindow::on_pushButton_2_clicked()
{
    static uint8_t val = 0;
    mainctl::instance()->setRftiEnergy(val++);
}

void MainWindow::on_pushButton_3_clicked()
{
    uint8_t data;
    mainctl::instance()->getRftiEnergy(data);
}

void MainWindow::on_pushButton_4_clicked()
{
    mainctl::instance()->powerOff();
}

void MainWindow::on_pushButton_5_clicked()
{
    static bool ret = false;
    mainctl::instance()->setPowerIo(13, ret);
    mainctl::instance()->setPowerIo(14, ret);

    mainctl::instance()->setPowerIo(15, ret);
    mainctl::instance()->setPowerIo(16, ret);
    mainctl::instance()->setPowerIo(17, ret);
    ret = !ret;
}


void MainWindow::reportCallBack(funsionType fun, uint32_t data)
{
    qDebug()<<"按键板触发关机，，，，";
}



