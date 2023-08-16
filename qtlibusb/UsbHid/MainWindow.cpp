#include "MainWindow.h"
#include "ui_MainWindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    u = new UsbHid();

    tim = new QTimer();
    connect(tim, &QTimer::timeout, this,[=](){
        static int num = 222;
        QString numStr = QString::number(num++);
        QByteArray qbStr = numStr.toLocal8Bit();
        QByteArray a = qbStr + "qsx" +qbStr + "fgh" +qbStr+ "fgh" +qbStr+ "fgh" +qbStr ;
        u->send(0x01, a);
        u->revice(0x01, 100);

        u->send(0x02, a);
        u->revice(0x02, 100);

    });
    u->start();
//    tim->start(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    static int num = 222;
    QString numStr = QString::number(num++);
    QByteArray qbStr = numStr.toLocal8Bit();
    QByteArray a = qbStr + "qsx" +qbStr + "fgh" +qbStr+ "fgh" +qbStr+ "fgh" +qbStr ;
//    u->send(0x01, a);
    u->send(0x02, a);
}


void MainWindow::on_pushButton_2_clicked()
{
    static int num = 222;
    QString numStr = QString::number(num++);
    QByteArray qbStr = numStr.toLocal8Bit();
    QByteArray a = qbStr + "qsx" +qbStr + "fgh" +qbStr+ "fgh" +qbStr+ "fgh" +qbStr ;
    u->send(0x01, a);
//    u->send(0x02, a);
}

