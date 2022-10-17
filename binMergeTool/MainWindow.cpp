#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    com = commun::create(commun::CAN_COMMUN);
    com->open(0);
    com->start();

    firware = new firmwareUpdate();
    connect(com, &commun::reveiced, firware, &firmwareUpdate::messageCallBack);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,"打开","./","files(*)");
    ui->lineEdit->setText(path);
    firware->getBinInfo(path);
}

