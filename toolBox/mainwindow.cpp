#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "serial_tool/serial_tool.h"
#include "chart/chart.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //设置上位机标题
    this->setWindowTitle("ToolBOX");

    //串口助手
    this->ss_ui = new serial_tool(this);
    ui->tabWidget->insertTab(0,ss_ui,QIcon(":/new/prefix1/img/serialport.png"),u8"串口助手");
//    //PID调试助手
//    this->chart_ui = new Chart(this);
//    ui->tabWidget->insertTab(1,chart_ui,QIcon(":/new/prefix1/img/serialport.png"),u8"PID调试助手");

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_tabWidget_currentChanged(int index)
{
    qDebug()<< index;
}
