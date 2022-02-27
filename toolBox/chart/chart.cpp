#include "chart.h"
#include "serial.h"
#include "ui_chart.h"


Chart::Chart(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Chart)
{
    ui->setupUi(this);
    // 初始化成员变量
    m_pauseTime = 0;
    m_vecPidLastData.resize(5); // 存放5个通道的PID上一次结果值
    m_ismoving = false;

    // 初始化图表
    initChart(ui->chartWidget);

    //设置刻度可见
    ui->dial->setNotchesVisible(true);
    //设置范围
    ui->dial->setRange(0,1024);
    //设置小刻度
    ui->dial->setNotchTarget(4);
    //设置大刻度
    ui->dial->setPageStep(1024/4);
    //设置当前值
    ui->dial->setValue(1024/2);
    //设置空间下放是否留白
    ui->dial->setWrapping(false);
    //槽函数回调
    connect(ui->dial,&QDial::valueChanged, this, &Chart::get_target_val);

    // 初始化串口
    m_serial = new Serial;
    // 寻找可用串口
    QStringList serialStrList;
    serialStrList = m_serial->scanSerial();
    for (int i=0; i<serialStrList.size(); i++)
    {
        ui->portComboBox->addItem(serialStrList[i]); // 在comboBox那添加串口号
    }
    // 默认设置波特率为115200（第5项）
    ui->baudComboBox->setCurrentIndex(5);
    // 当下位机中有数据发送过来时就会响应这个槽函数
    connect(m_serial, SIGNAL(readOneFrame()), this, SLOT(readFromSerial()));
    // 处理串口错误
    connect(m_serial, SIGNAL(errorSignal()), this, SLOT(handleSerialError()));
    // 连接onNewSerialPort信号槽，定时获取串口列表
    connect(m_serial, SIGNAL(onNewSerialPort(QStringList)),this, SLOT(onNewPortList(QStringList)));
}

//新的目标值
void Chart::get_target_val()
{
    int value = 0;
    value = ui->dial->value();
    qDebug()<<"cur_tar_val : "<<value<<"\r\n";

}

void Chart::initChart(QCustomPlot *ChartWidget)
{
    // 初始化图表实例指针
    m_customPlot = ChartWidget;
    // 将customPlot的mouseMove信号连接到mouseCoordConver函数
//    connect(m_customPlot, &QCustomPlot::mouseMove, this, &Chart::mouseCoordConver);
    // 安装事件过滤器
    m_customPlot->installEventFilter(this);
    // 设置背景色
    m_customPlot->setBackground(QBrush(QColor(0, 0, 0)));
    // 设置x/y轴刻度标签颜色
    m_customPlot->yAxis->setTickLabelColor(QColor(255, 255, 255));
    m_customPlot->xAxis->setTickLabelColor(QColor(255, 255, 255));
    // 支持鼠标拖拽轴的范围、滚动缩放轴的范围，左键点选图层（每条曲线独占一个图层）
    m_customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    // 添加通道1曲线
    m_customPlot->addGraph(); // 向绘图区域QCustomPlot添加一条曲线
    m_customPlot->graph(0)->setPen(QPen(Qt::red, 3)); // 设置曲线颜色
    // 添加通道2曲线
    m_customPlot->addGraph();
    m_customPlot->graph(1)->setPen(QPen(Qt::magenta, 3));
    // 添加通道3曲线
    m_customPlot->addGraph();
    m_customPlot->graph(2)->setPen(QPen(Qt::green, 3));
    // 添加通道4曲线
    m_customPlot->addGraph();
    m_customPlot->graph(3)->setPen(QPen(Qt::blue, 3));
    // 添加通道5曲线
    m_customPlot->addGraph();//添加graph等价于添加新曲线
    m_customPlot->graph(4)->setPen(QPen(Qt::cyan, 3));

    // 最开始设置Y轴的范围为0-1000
    m_customPlot->yAxis->setRange(0, 1000);

    // 创建时间坐标轴
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%s");
    timeTicker->setTickCount(10); // 设置越大，同屏显示的时间栅格越多
    m_customPlot->xAxis->setTicker(timeTicker);

    for(int i=0; i<CHANNEL_COUNT; i++)
    {
        // 自动设定graph曲线x和y轴的范围
        m_customPlot->graph(i)->rescaleAxes(true);
        // 解决点击曲线，曲线变蓝色的问题
        m_customPlot->graph(i)->setSelectable(QCP::stNone);
    }

    // 立即刷新图表
    m_customPlot->replot();
}

// 读取来自串口类的数据
void Chart::readFromSerial()
{
    //test
    static int i=0;

    qDebug()<<u8"接收信号";

    QByteArray readBuf = m_serial->getReadBuf();
    qDebug()<<readBuf;
    // 清除读取数据缓冲区
    m_serial->clearReadBuf();

    // 初始化成员变量
    m_vecPidData.resize(5); // 存放5个通道的PID结果值

    // 更新图表数据
    for(int i=0; i<CHANNEL_COUNT; i++)
//        m_vecPidData[i] = 100*(i+1);//test
        m_vecPidData[i] = 100*(5);//test


    for(int i = 0;i < CHANNEL_COUNT;i++)
        updataChart(i, m_vecPidData);
}


// 处理串口错误
void Chart::handleSerialError()
{
//    MyMessageBox *msgBox = new MyMessageBox();
//    msgBox->showMyMessageBox(nullptr, tr("错误提示"), tr("串口连接中断，请检查是否正确连接!"), MESSAGE_WARNNING,
//                                        BUTTON_OK, true);

    // 再次触发"打开串口"按钮槽函数，以关闭串口
    ui->openSerialButton->setText(tr("关闭串口"));
    emit ui->openSerialButton->clicked();
}

// 用于响应SerialPortList信号，周期获取串口列表
void Chart::onNewPortList(QStringList portName)
{
    ui->portComboBox->clear();
    ui->portComboBox->addItems(portName);
}


// 打开串口按钮-点击槽函数
void Chart::on_openSerialButton_clicked()
{
    if(ui->openSerialButton->text() == tr(u8"打开串口"))
    {
        // 打开串口
        if(m_serial->open(ui->portComboBox->currentText(), ui->baudComboBox->currentText().toInt()))
        {
            // 打开串口前清除图表，避免加载数据产生曲线和串口接受曲线混杂
            clearChart();
            // 清空本地各通道的数据(避免加载数据中本地保存通道数据和打开串口后接受通道数据混淆)
            clearChannelData();
            // 暂停时间清零
            clearPauseTime();
            // 启动串口计时器，以获得串口打开后的经过时间
            startTimer();

            // 关闭下拉列表使能
            ui->portComboBox->setEnabled(false);
            ui->baudComboBox->setEnabled(false);
            // 使能暂停更新按钮
            ui->pauseUpdateButton->setEnabled(true);
            // 修改按钮名称
            ui->openSerialButton->setText(tr(u8"关闭串口"));
            // 修改串口提示标签的文本，且颜色修改为蓝色
            ui->serialTipLabel->setText(tr(u8"串口已开启"));
            ui->serialTipLabel->setStyleSheet("color:blue");
        }
    }
    else
    {
        // 避免当暂停更新(会disconnect)后关闭串口，再打开串口不接受数据的情况
        m_isPause = false;
        emit ui->pauseUpdateButton->clicked();

        // 关闭串口
        m_serial->close();
        // 重新开启下拉列表使能
        ui->portComboBox->setEnabled(true);
        ui->baudComboBox->setEnabled(true);
        // 设置不使能暂停更新按钮
        ui->pauseUpdateButton->setEnabled(false);
        // 恢复按钮名称
        ui->openSerialButton->setText(tr(u8"打开串口"));
        // 恢复串口提示标签的文本及颜色
        ui->serialTipLabel->setText(tr(u8"串口未开启"));
        ui->serialTipLabel->setStyleSheet("color:red");
    }
}

// 更新图表
void Chart::updataChart(char channel, QVector<int> vecPidData)
{

    // 自串口打开以来经过的时间-暂停时间，即x轴的时间值，以s为单位
    double XAxisTime = (m_serialTime.elapsed() - this->m_pauseTime)/1000.0;

    // 关闭通道后停止向graph里面添加数据
    if(channel == 0)
    {
//        if(m_vecPidLastData[0] != vecPidData[0]) //值改变才添加数据到graph中
        {
            // 将数据添加到曲线
            m_customPlot->graph(0)->addData(XAxisTime, vecPidData[0]);
            // 添加到本地数据vector，方便保存和重新加载
            m_mapChannel[0].insert(XAxisTime, vecPidData[0]);
            // 获得上一次的数据(曲线不光滑的原因是好几次的Y值都一样，导致横线[时间过长]过长)
            m_vecPidLastData[0] = vecPidData[0];
        }
    }
    if(channel == 1)
    {
//        if(m_vecPidLastData[1] != vecPidData[1])
        {
            m_customPlot->graph(1)->addData(XAxisTime, vecPidData[1]);
            m_mapChannel[1].insert(XAxisTime, vecPidData[1]);
            m_vecPidLastData[1] = vecPidData[1];
        }
    }
    if(channel == 2)
    {
//        if(m_vecPidLastData[2] != vecPidData[2])
        {
            m_customPlot->graph(2)->addData(XAxisTime, vecPidData[2]);
            m_mapChannel[2].insert(XAxisTime, vecPidData[2]);
            m_vecPidLastData[2] = vecPidData[2];
        }
    }
    if(channel == 3)
    {
//        if(m_vecPidLastData[3] != vecPidData[3])
        {
            m_customPlot->graph(3)->addData(XAxisTime, vecPidData[3]);
            m_mapChannel[3].insert(XAxisTime, vecPidData[3]);
            m_vecPidLastData[3] = vecPidData[3];
        }
    }
    if(channel == 4)
    {
//        if(m_vecPidLastData[4] != vecPidData[4])
        {
            m_customPlot->graph(4)->addData(XAxisTime, vecPidData[4]);
            m_mapChannel[4].insert(XAxisTime, vecPidData[4]);
            m_vecPidLastData[4] = vecPidData[4];
        }
    }
    /*
        可支持多组数据保存
        暂时删掉
    */

    // 自适应设置X和Y轴的范围
    if(!m_ismoving)
    {
        // 自定义x轴值的显示范围(起始值为elapsedTime，终止值为TIMER_COUNT)
        m_customPlot->xAxis->setRange(XAxisTime, 8, Qt::AlignRight);

        for(int i=0; i<CHANNEL_COUNT; i++)
            m_customPlot->graph(i)->rescaleValueAxis(true);
    }

    // 立即刷新图像
    m_customPlot->replot();
}

// 清除通道对应曲线
void Chart::clearChannel(int channel)
{
    // 写入空的vector即可清除曲线
    m_customPlot->graph(channel)->setData({}, {});
}

// 清除图表
void Chart::clearChart()
{
    for(int i=0; i<CHANNEL_COUNT; i++)
        m_customPlot->graph(i)->setData({}, {});
}

// 刷新图表
void Chart::replotChart()
{
    m_customPlot->replot();
}

// 清空本地各通道的数据
void Chart::clearChannelData()
{
    for(int i=0; i<CHANNEL_COUNT; i++)
        m_mapChannel[0].clear();
}

// 清零暂停时间
void Chart::clearPauseTime()
{
    m_pauseTime = 0;
}

// 启动串口计时器
void Chart::startTimer()
{
    m_serialTime.start();
}

Chart::~Chart()
{
    delete ui;
}
