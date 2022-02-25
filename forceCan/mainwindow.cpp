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

    devList << 0xA8<<0xB8;
    this->devId = 0xA8;

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

    static int index=0;
    QPointF point;

    point.setY(val);
    point.setX(index);

    this->wave_data[WAVE_CH0].append(point);

    if (this->wave_data[WAVE_CH0].size() > 20000){
        this->wave_data[WAVE_CH0].removeFirst();
    }

    wave->addSeriesData(WAVE_CH0,this->wave_data[WAVE_CH0]);

//    qDebug()<< point<<" \r\n";
    index = index + 1;
}

void MainWindow::appindLog(QString temp){
    ui->logTextBrowser->append(temp);
    qDebug()<<temp;
}

void MainWindow::on_peelButton_clicked(){
    uint8_t data[8]={0x42,0x54,0x00,0x00,0x00,0x00,0x00,0x00};
    can->sendData(devId,8,data);
    appindLog("去皮");
}

void MainWindow::on_calButton_clicked(){
    int16_t val = ui->calValtextEdit->toPlainText().toInt();
    if(val){
        uint8_t h = val>>8;
        uint8_t l = val&0x00FF;

        uint8_t data[8]={0x42,0x53,h,l,0x00,0x00,0x00,0x00};
        can->sendData(devId,8,data);
        appindLog("标定值ok");
    }else{
        appindLog("标定值无效");
    }
}

void MainWindow::on_zeroButton_clicked(){
    uint8_t data[8]={0x42,0x52,0x00,0x00,0x00,0x00,0x00,0x00};
    can->sendData(devId,8,data);
    appindLog("标零");
}

void MainWindow::on_canclePeelButton_clicked(){
    uint8_t data[8]={0x42,0x55,0x00,0x00,0x00,0x00,0x00,0x00};
    can->sendData(devId,8,data);
    appindLog("取消去皮");
}

void MainWindow::on_savePushButton_clicked(){
    uint8_t data[8]={0x42,0x5D,0x00,0x00,0x00,0x00,0x00,0x00};
    can->sendData(devId,8,data);
    appindLog("参数保存");
}

void MainWindow::on_devComboBox_currentIndexChanged(int index)
{
    this->devId = devList[index];
    qDebug()<< devList[index]<<" \r\n";
}