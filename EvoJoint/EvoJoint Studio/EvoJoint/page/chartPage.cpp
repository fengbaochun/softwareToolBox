#include "chartPage.h"
#include "ui_chartPage.h"
#include <QTimer>
#include "motor/ctl.h"

chartPage::chartPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chartPage)
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

    connect(ctl::instance(), SIGNAL(report(uint8_t,QByteArray)), this,
            SLOT(reportCallBack(uint8_t, QByteArray)));
}

chartPage::~chartPage()
{
    delete ui;
}

void chartPage::reportCallBack(uint8_t fun, QByteArray qb)
{
    QVector<float> gData = ctl::instance()->splitByteArrayToData(qb, sizeof(float));
//    qDebug()<<"ctl report -> fun"<<QString::number(fun, 16)<<"data"<<gData;
    qVdata.append(gData);
}

void chartPage::timer1Handler()
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
