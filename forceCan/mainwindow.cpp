#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    can = new canDev();
    can->start();

    wave = new MWaveView(ui->widget);
    wave->resize(ui->widget->size());

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateData()));
    timer->start(50);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateData(){
    int val = can->force.val;
    ui->weightLineEdit->setText(QString::number(val,10));

//    static QPointF point;
//    static int index = 0;
//    point.setY((double)val);
//    point.setX(index++);
//    this->wave_data[1].append(point);
//    wave->addSeriesData((WAVE_CH)1,this->wave_data[1]);
//    qDebug()<<index<<"  "<<val<<"  \r\n";

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
        Ydata[i] = val;
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

void MainWindow::appindLog(QString temp){
    ui->logTextBrowser->append(temp);
    qDebug()<<temp;
}

void MainWindow::on_peelButton_clicked(){
    uint8_t data[8]={0x42,0x54,0x00,0x00,0x00,0x00,0x00,0x00};
    can->sendData(0xA8,8,data);
    appindLog("去皮");
}

void MainWindow::on_calButton_clicked(){
    int16_t val = ui->calValtextEdit->toPlainText().toInt();
    if(val){
        uint8_t h = val>>8;
        uint8_t l = val&0x00FF;

        uint8_t data[8]={0x42,0x53,h,l,0x00,0x00,0x00,0x00};
        can->sendData(0xA8,8,data);
        appindLog("标定值ok");
    }else{
        appindLog("标定值无效");
    }
}

void MainWindow::on_zeroButton_clicked(){
    uint8_t data[8]={0x42,0x52,0x00,0x00,0x00,0x00,0x00,0x00};
    can->sendData(0xA8,8,data);
    appindLog("标零");
}

void MainWindow::on_canclePeelButton_clicked(){
    uint8_t data[8]={0x42,0x55,0x00,0x00,0x00,0x00,0x00,0x00};
    can->sendData(0xA8,8,data);
    appindLog("取消去皮");
}
