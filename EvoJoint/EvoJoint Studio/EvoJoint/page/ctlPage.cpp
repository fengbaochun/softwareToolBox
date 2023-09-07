#include "ctlPage.h"
#include "ui_ctlPage.h"
#include <QTimer>
#include "motor/ctl.h"


ctlPage::ctlPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ctlPage)
{
    ui->setupUi(this);

    // widgetPlot2D为通过窗口提升得到的控件
    ui->widgetPlot2D->initGraphName(QStringList()
                                    << "tarPos" << "curPos"
                                    << "tarSpeed" << "curSpeed"
                                    << "tarIq" << "curIq"
                                    << "adcA" << "adcB"<< "adcC"
                                    );
    /* 使用WidgetPlot2D绘制实时波形只需两步：
     * 1.初始化波形名称：函数initGraphName(QStringList)
     * 2.给对应的波形添加数据：函数addData(QString, double)
     * 波形名称和数据要一一对应。
     * 函数addData为公有槽函数，也可使用信号槽传递波形数据。*/

    waveT = new QTimer();
    connect(waveT, SIGNAL(timeout()), this, SLOT(timer1Handler()));
    waveT->start(1);


    ui->horizontalSliderSpeed->setRange(-1000, 1000);
    ui->horizontalSliderSpeed->setValue(0);

    ui->horizontalSliderDq->setRange(-1000, 1000);
    ui->horizontalSliderDq->setValue(0);

    ui->spinBoxTarPos->setRange(-16384*500, 16384*500);

    connect(ctl::instance(), SIGNAL(report(uint8_t,QByteArray)), this,
            SLOT(reportCallBack(uint8_t, QByteArray)));

    QStringList  str;
    str<<"0fps"<<"50fps"<<"100fps"<<"200fps"<<"500fps";//<<"1000fps";

    ui->comboBoxFps->addItems(str);
    QTimer *t = new QTimer();
    connect(t,&QTimer::timeout,this,[=](){
        double f = ctl::instance()->fps;
        QString s = "实时帧率：" + QString::number(f,'f',1) + "fps";
        ui->lableFps->setText(s);
    });
    t->start(500);
}

ctlPage::~ctlPage()
{
    delete ui;
}

void ctlPage::on_comboBoxFps_currentIndexChanged(int index)
{
    const int timeOut[6] = {S0FPS, S50FPS, S100FPS, S200FPS, S500FPS, S1000FPS};
    ctl::instance()->setFps(timeOut[index]);
}


void ctlPage::on_spinBoxMaxSpeed_valueChanged(int arg1)
{
    ctl::instance()->setSpeedRange(ctl::instance()->motor.minSpeed, arg1);
}


void ctlPage::on_spinBoxMinSpeed_valueChanged(int arg1)
{
    ctl::instance()->setSpeedRange(arg1, ctl::instance()->motor.maxSpeed);
}


void ctlPage::on_doubleSpinBoxMaxCur_valueChanged(double arg1)
{
    ctl::instance()->setCurrentRange(ctl::instance()->motor.minIq, (float)arg1);
}


void ctlPage::on_doubleSpinBoxMinCur_valueChanged(double arg1)
{
    ctl::instance()->setCurrentRange((float)arg1, ctl::instance()->motor.maxIq);
}


void ctlPage::on_pushButton_clicked()
{
    float max, min;
    ctl::instance()->getCurrentRange(&max, &min);
    ctl::instance()->motor.maxIq = max;
    ctl::instance()->motor.minIq = min;
}


void ctlPage::on_pushButtonSync_clicked()
{
    bool ret = ctl::instance()->paramSync();
    if(ret){
        ui->spinBoxMaxSpeed->setValue(ctl::instance()->motor.maxSpeed);
        ui->spinBoxMinSpeed->setValue(ctl::instance()->motor.minSpeed);

        ui->doubleSpinBoxMaxCur->setValue(ctl::instance()->motor.maxIq);
        ui->doubleSpinBoxMinCur->setValue(ctl::instance()->motor.minIq);

        ui->doubleSpinBoxPosP->setValue(ctl::instance()->motor.posP);
        ui->doubleSpinBoxPosD->setValue(ctl::instance()->motor.posD);

        ui->doubleSpinBoxSpeedP->setValue(ctl::instance()->motor.speedP);
        ui->doubleSpinBoxSpeedI->setValue(ctl::instance()->motor.speedI);

        ui->doubleSpinBoxDqP->setValue(ctl::instance()->motor.currentP);
        ui->doubleSpinBoxDqI->setValue(ctl::instance()->motor.currentI);
    }
}


void ctlPage::on_doubleSpinBoxPosP_valueChanged(double arg1)
{
    ctl::instance()->setPosP(arg1);
}


void ctlPage::on_doubleSpinBoxPosD_valueChanged(double arg1)
{
    ctl::instance()->setPosD(arg1);
}


void ctlPage::on_doubleSpinBoxSpeedP_valueChanged(double arg1)
{
    ctl::instance()->setSpeedP(arg1);
}


void ctlPage::on_doubleSpinBoxSpeedI_valueChanged(double arg1)
{
    ctl::instance()->setSpeedI(arg1);
}


void ctlPage::on_doubleSpinBoxDqP_valueChanged(double arg1)
{
    ctl::instance()->setCurrentP(arg1);
}


void ctlPage::on_doubleSpinBoxDqI_valueChanged(double arg1)
{
    ctl::instance()->setCurrentI(arg1);
}


void ctlPage::on_horizontalSliderSpeed_valueChanged(int value)
{
    ctl::instance()->setTarSpeed(value);
}

void ctlPage::on_horizontalSliderDq_valueChanged(int value)
{
    float dq = (float)value * 0.01f;
    ctl::instance()->setTarCurrent(dq);
}


void ctlPage::on_pushButtonSave_clicked()
{
    ctl::instance()->paramSave();
}

void ctlPage::on_spinBoxTarPos_editingFinished()
{
    ctl::instance()->setTarPos(ui->spinBoxTarPos->value());
}

void ctlPage::on_pushButtonCal_clicked()
{
    ctl::instance()->setMode(CAL_MODE);
}

void ctlPage::reportCallBack(uint8_t fun, QByteArray qb)
{
    QVector<float> gData = ctl::instance()->splitByteArrayToData(qb, sizeof(float));
//    qDebug()<<"ctl report -> fun"<<QString::number(fun, 16)<<"data"<<gData;
    qVdata.append(gData);
}

void ctlPage::timer1Handler()
{
    if(!qVdata.isEmpty()){
        for(int i=0; i<qVdata.size(); i++){
            ui->widgetPlot2D->addData("tarPos"  , qVdata.at(i).at(0));
            ui->widgetPlot2D->addData("curPos"  , qVdata.at(i).at(1));
            ui->widgetPlot2D->addData("tarSpeed", qVdata.at(i).at(2));
            ui->widgetPlot2D->addData("curSpeed", qVdata.at(i).at(3));
            ui->widgetPlot2D->addData("tarIq"   , qVdata.at(i).at(4));
            ui->widgetPlot2D->addData("curIq"   , qVdata.at(i).at(5));
            ui->widgetPlot2D->addData("adcA"    , qVdata.at(i).at(6));
            ui->widgetPlot2D->addData("adcB"    , qVdata.at(i).at(7));
            ui->widgetPlot2D->addData("adcC"    , qVdata.at(i).at(8));
        }
        qVdata.clear();
    }
}

