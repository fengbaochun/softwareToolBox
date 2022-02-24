/***************************************************************
 * @Copyright(C)    wangchongwei (版权申明请勿删除)
 * @FileName:       mainwindow.cpp
 * @Author:         wangchongwei
 * @Version:        1.0.0
 * @Date:           2021.7.11
 *
 * 个人微信号: wang_chongwei
 * 个人公众号: 王崇卫
 * CSDN: https://blog.csdn.net/qq_38190041
 * 个人网站: https://wang_chong_wei.gitee.io/blog/
 ***************************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    wave = new MWaveView(ui->widget);
    wave->resize(ui->widget->size());

    // 定时器
    mvGetTimer = new QTimer(this);
    connect(mvGetTimer, SIGNAL(timeout()), this, SLOT(onMsGetTimeOut()));
    mvGetTimer->start(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 定时器槽函数，正弦波生成
void MainWindow::onMsGetTimeOut()
{
    static double Ydata[16];
    static double  param,result;
    #define PI 3.14159265
    // 测试
    param++;

    result = sin(param*PI/180);
    static QPointF point;

    //for (int i = 0; i< 16; i++)
    int i = 0;
    {
//        Ydata[i] = result* 50*(i+1) ;
        Ydata[i] = 10;
        point.setY(Ydata[i]);
        point.setX(param);
        this->wave_data[i].append(point);

        if (this->wave_data[i].size() > 20000)
        {
            this->wave_data[i].removeFirst();
        }

        wave->addSeriesData((WAVE_CH)i,this->wave_data[i]);
    }
}


void MainWindow::on_checkBox_0_stateChanged(int arg1)
{
    if(arg1){
        wave->openChannel(WAVE_CH0);
    }
    else{
        wave->closeChannel(WAVE_CH0);
    }
}


// 处理旋钮函数d
int limit(int value, int last_value, int size)
{
    int dir =0;
    int tmp = value - last_value;
    if ((tmp>0&& tmp< (size/2)) ||(tmp< -(size/2)))
    {
        dir = 1;
    }
    else if ((tmp<0 && tmp>-(size/2)) || (tmp > (size/2)))
    {
        dir = -1;
    }
    return dir;
}

void MainWindow::on_dial_X_sliderMoved(int position)
{
    static int last_value = 5;
    int now_ret = 0;

    now_ret = limit(position,last_value,10);
    last_value = position;

    if(now_ret>0){
        wave->ZoomX();
    }
    else {
        wave->ZoomOutX();
    }
}

void MainWindow::on_dial_Y_sliderMoved(int position)
{
    static int last_value = 5;
    int now_ret = 0;

    now_ret = limit(position,last_value,10);
    last_value = position;

    if(now_ret>0){
        wave->ZoomY();
    }
    else {
        wave->ZoomOutY();
    }
}

void MainWindow::on_checkBox_1_stateChanged(int arg1)
{
    if(arg1){
        wave->openChannel(WAVE_CH1);
    }
    else{
        wave->closeChannel(WAVE_CH1);
    }
}

void MainWindow::on_checkBox_2_stateChanged(int arg1)
{
    if(arg1){
        wave->openChannel(WAVE_CH2);
    }
    else{
        wave->closeChannel(WAVE_CH2);
    }
}

void MainWindow::on_checkBox_3_stateChanged(int arg1)
{
    if(arg1){
        wave->openChannel(WAVE_CH3);
    }
    else{
        wave->closeChannel(WAVE_CH3);
    }
}

void MainWindow::on_checkBox_4_stateChanged(int arg1)
{
    if(arg1){
        wave->openChannel(WAVE_CH4);
    }
    else{
        wave->closeChannel(WAVE_CH4);
    }
}

void MainWindow::on_checkBox_5_stateChanged(int arg1)
{
    if(arg1){
        wave->openChannel(WAVE_CH5);
    }
    else{
        wave->closeChannel(WAVE_CH5);
    }
}

void MainWindow::on_checkBox_6_stateChanged(int arg1)
{
    if(arg1){
        wave->openChannel(WAVE_CH6);
    }
    else{
        wave->closeChannel(WAVE_CH6);
    }
}

void MainWindow::on_checkBox_7_stateChanged(int arg1)
{
    if(arg1){
        wave->openChannel(WAVE_CH7);
    }
    else{
        wave->closeChannel(WAVE_CH7);
    }
}

void MainWindow::on_checkBox_8_stateChanged(int arg1)
{
    if(arg1){
        wave->openChannel(WAVE_CH8);
    }
    else{
        wave->closeChannel(WAVE_CH8);
    }
}

void MainWindow::on_checkBox_9_stateChanged(int arg1)
{
    if(arg1){
        wave->openChannel(WAVE_CH9);
    }
    else{
        wave->closeChannel(WAVE_CH9);
    }
}

void MainWindow::on_checkBox_10_stateChanged(int arg1)
{
    if(arg1){
        wave->openChannel(WAVE_CH10);
    }
    else{
        wave->closeChannel(WAVE_CH10);
    }
}

void MainWindow::on_checkBox_11_stateChanged(int arg1)
{
    if(arg1){
        wave->openChannel(WAVE_CH11);
    }
    else{
        wave->closeChannel(WAVE_CH11);
    }
}

void MainWindow::on_checkBox_12_stateChanged(int arg1)
{
    if(arg1){
        wave->openChannel(WAVE_CH12);
    }
    else{
        wave->closeChannel(WAVE_CH12);
    }
}

void MainWindow::on_checkBox_13_stateChanged(int arg1)
{
    if(arg1){
        wave->openChannel(WAVE_CH13);
    }
    else{
        wave->closeChannel(WAVE_CH13);
    }
}

void MainWindow::on_checkBox_14_stateChanged(int arg1)
{
    if(arg1){
        wave->openChannel(WAVE_CH14);
    }
    else{
        wave->closeChannel(WAVE_CH14);
    }
}

void MainWindow::on_checkBox_15_stateChanged(int arg1)
{
    if(arg1){
        wave->openChannel(WAVE_CH15);
    }
    else{
        wave->closeChannel(WAVE_CH15);
    }
}

void MainWindow::on_pushButton_pause_clicked()
{
    wave->pauseGraph();
}

void MainWindow::on_pushButton_start_clicked()
{
    wave->startGraph();
}
