#include "operatePage.h"
#include "ui_operatePage.h"

operatePage::operatePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::operatePage)
{
    ui->setupUi(this);

    QStringList  f;
    f<<"0fps"<<"50fps"<<"100fps"<<"200fps"<<"500fps";//<<"1000fps";
    ui->comboBoxFps->addItems(f);

    QStringList  m;
    m<<"力矩模式"<<"速度模式"<<"位置模式"<<"梯形位置模式";
    ui->modeComboBox->addItems(m);

    ui->horizontalSliderSpeed->setRange(-1000, 1000);
    ui->horizontalSliderSpeed->setValue(0);

    ui->horizontalSliderDq->setRange(-1000, 1000);
    ui->horizontalSliderDq->setValue(0);

    ui->spinBoxTarPos->setRange(-16384*500, 16384*500);

    QTimer *t = new QTimer();
    connect(t,&QTimer::timeout,this,[=](){
        double f = ctl::instance()->fps;
        QString s = "实时帧率：" + QString::number(f,'f',1) + "fps";
        ui->lableFps->setText(s);
    });
    t->start(500);

}

operatePage::~operatePage()
{
    delete ui;
}

void operatePage::on_modeComboBox_activated(int index)
{
    ui->ctlSstackedWidget->setCurrentIndex(index);
}

void operatePage::on_comboBoxFps_currentIndexChanged(int index)
{
    const int timeOut[6] = {S0FPS, S50FPS, S100FPS, S200FPS, S500FPS, S1000FPS};
    ctl::instance()->setFps(timeOut[index]);
}

void operatePage::on_spinBoxMaxSpeed_valueChanged(int arg1)
{
    ctl::instance()->setSpeedRange(ctl::instance()->motor.minSpeed, arg1);
}

void operatePage::on_spinBoxMinSpeed_valueChanged(int arg1)
{
    ctl::instance()->setSpeedRange(arg1, ctl::instance()->motor.maxSpeed);
}

void operatePage::on_doubleSpinBoxMaxCur_valueChanged(double arg1)
{
    ctl::instance()->setCurrentRange(ctl::instance()->motor.minIq, (float)arg1);
}

void operatePage::on_doubleSpinBoxMinCur_valueChanged(double arg1)
{
    ctl::instance()->setCurrentRange((float)arg1, ctl::instance()->motor.maxIq);
}

void operatePage::on_doubleSpinBoxPosP_valueChanged(double arg1)
{
    ctl::instance()->setPosP(arg1);
}

void operatePage::on_doubleSpinBoxPosD_valueChanged(double arg1)
{
    ctl::instance()->setPosD(arg1);
}

void operatePage::on_doubleSpinBoxSpeedP_valueChanged(double arg1)
{
    ctl::instance()->setSpeedP(arg1);
}

void operatePage::on_doubleSpinBoxSpeedI_valueChanged(double arg1)
{
    ctl::instance()->setSpeedI(arg1);
}

void operatePage::on_doubleSpinBoxDqP_valueChanged(double arg1)
{
    ctl::instance()->setCurrentP(arg1);
}

void operatePage::on_doubleSpinBoxDqI_valueChanged(double arg1)
{
    ctl::instance()->setCurrentI(arg1);
}

void operatePage::on_horizontalSliderSpeed_valueChanged(int value)
{
    ctl::instance()->setTarSpeed(value);
}

void operatePage::on_horizontalSliderDq_valueChanged(int value)
{
    float dq = (float)value * 0.01f;
    ctl::instance()->setTarCurrent(dq);
}

void operatePage::on_spinBoxTarPos_editingFinished()
{
    ctl::instance()->setTarPos(ui->spinBoxTarPos->value());
}

void operatePage::on_pushButtonCal_clicked()
{
    ctl::instance()->setMode(CAL_MODE);
}

void operatePage::on_pushButtonSave_clicked()
{
    ctl::instance()->paramSave();
}

void operatePage::on_pushButtonSync_clicked()
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

