#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <qDebug>
#include "ThreadPool.h"
#include <QCoreApplication>
#include <QElapsedTimer>
#include <QTimer>
#include <QThread>

//耗时计算
void timeConsumCalc(volatile int num, int* data){
    std::this_thread::sleep_for(std::chrono::milliseconds(num*100));
    data = (int *)num;
    std::cout << "data : " << (int *)data << std::endl;
}

//https://blog.csdn.net/fuhanghang/article/details/114818215

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    int data1 = 0;
    int data2 = 0;
    int data3 = 0;

    /// 队列可以自己实现
    ThreadPool<std::queue<Task *>> tp(6);

    char test1[24] = "qqq";
    char test2[24] = "www";
    char test3[24] = "eee";

    tp.addOneTask(new Task(timeConsumCalc, 1, &data1));
    tp.addOneTask(new Task(timeConsumCalc, 2, &data2));
    tp.addOneTask(new Task(timeConsumCalc, 30, &data3));
    tp.addOneTask(new Task(timeConsumCalc, 4, &data3));
    tp.addOneTask(new Task(timeConsumCalc, 5, &data3));
    tp.addOneTask(new Task(timeConsumCalc, 6, &data3));

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    tp.waitRunFinash();

//    QElapsedTimer time;
//    time.start();
//    while(!tp.is_running){
//        QCoreApplication::processEvents(QEventLoop::EventLoopExec, 10000);
//        QThread::msleep(100);
//        qDebug()<<data1<<data2<<data3;
//    }

    qDebug()<<data1<<data2<<data3;
    qDebug()<<"==================================";

}

MainWindow::~MainWindow()
{
    delete ui;
}

