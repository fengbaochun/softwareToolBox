#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    firware = new firmwareUpdate();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,"打开","./","files(*)");
    qDebug()<<"chose path : " << path;
    ui->lineEdit->setText(path);
    if(path != nullptr){
        firware->getBinInfo(path);

        qint64 tick = QDateTime::currentDateTime().toMSecsSinceEpoch();     //毫秒时间戳

        if(!firware->enterBootloader()){
            qDebug()<<"enterBootloader failed!!!";
        }
        if(!firware->sendBinInfo(firware->bin.size)){
            qDebug()<<"sendBinInfo failed!!!";
        }
        firware->startUpdate();

        qint64 time = abs(QDateTime::currentDateTime().toMSecsSinceEpoch() - tick);
        qDebug()<<"time : "<<time<<"ms";
    }

}



void MainWindow::on_pushButton_2_clicked()
{
//    if(!firware->enterBootloader()){
//        qDebug()<<"enterBootloader failed!!!";
//    }
//    if(!firware->sendBinInfo(firware->bin.size)){
//        qDebug()<<"sendBinInfo failed!!!";
//    }
}


void MainWindow::on_pushButton_3_clicked()
{
//    firware->startUpdate();
}

