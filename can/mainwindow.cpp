#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    can = new canDev();
    can->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_peelButton_clicked()
{
    qDebug()<<"------------------------------------------";
}

void MainWindow::on_calButton_clicked()
{

}

void MainWindow::on_zeroButton_clicked()
{

}

void MainWindow::on_canclePeelButton_clicked()
{

}
