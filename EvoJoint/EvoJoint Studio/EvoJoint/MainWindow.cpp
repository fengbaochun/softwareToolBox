#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QSerialPortInfo>
#include <QTimer>
#include <QtEndian>

#include <QPainter>
#include <QProxyStyle>
#include <QStyleOptionTab>

class CustomTabStyle : public QProxyStyle
{
public:
    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
        const QSize &size, const QWidget *widget) const
    {
        QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
        if (type == QStyle::CT_TabBarTab) {
            s.transpose();
            s.rwidth() = 90; // 设置每个tabBar中item的大小
            s.rheight() = 60;
        }
        return s;
    }

    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        if (element == CE_TabBarTabLabel) {
            if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option)) {
                QRect allRect = tab->rect;

                if (tab->state & QStyle::State_Selected) {
                    painter->save();
                    painter->setPen(0x89cfff);
                    painter->setBrush(QBrush(0x89cfff));
                    painter->drawRect(allRect.adjusted(6, 6, -6, -6));
                    painter->restore();
                }
                QTextOption option;
                option.setAlignment(Qt::AlignCenter);
                if (tab->state & QStyle::State_Selected) {
                    painter->setPen(0xf8fcff);
                }
                else {
                    painter->setPen(0x5d5d5d);
                }

                painter->drawText(allRect, tab->text, option);
                return;
            }
        }

        if (element == CE_TabBarTab) {
            QProxyStyle::drawControl(element, option, painter, widget);
        }
    }
};


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("ToolBOX");
    ui->comboBox->installEventFilter(this);     //安装QComboBox的事件过滤器：

    //设备信息
    dev.append({    .index=0,
                    .name="aimapro massage",
                    .port=""}   );


    dev.append({    .index=1,
                    .name="vima massage",
                    .port=""}   );

    dev.append({    .index=2,
                    .name="ctl ",
                    .port=""}   );

    QStringList str;
    foreach (devInfoType d, dev) {
        str.append(d.name);
    }

    ui->devList->addItems(str);
    this->ctlPageUi = new ctlPage(this);                    //控制页（功能 demo 测试使用）
    ui->stackedWidget->insertWidget(2, ctlPageUi);

    ui->stackedWidget->setCurrentIndex(0);


    // widgetPlot2D为通过窗口提升得到的控件
    ui->widgetPlot2D->initGraphName(QStringList() << "正弦波" << "矩形波" << "随机数");

    /* 使用WidgetPlot2D绘制实时波形只需两步：
     * 1.初始化波形名称：函数initGraphName(QStringList)
     * 2.给对应的波形添加数据：函数addData(QString, double)
     * 波形名称和数据要一一对应。
     * 函数addData为公有槽函数，也可使用信号槽传递波形数据。*/

    timer1 = new QTimer();
    connect(timer1, SIGNAL(timeout()), this, SLOT(timer1Handler()));
    timer1->start(1);

    timer2 = new QTimer();
    connect(timer2, SIGNAL(timeout()), this, SLOT(timer2Handler()));
    timer2->start(1);

    timer3 = new QTimer();
    connect(timer3, SIGNAL(timeout()), this, SLOT(timer3Handler()));
    timer3->start(1);
}


/* 定时器1溢出事件 */
void MainWindow::timer1Handler()
{
    // 模拟正弦波
    static double degrees = 0;
    double radians = qDegreesToRadians(degrees);
    double s = 15.0 * qSin(radians);
    ui->widgetPlot2D->addData("正弦波", s);
    ui->widgetPlot2D->addData("正弦波", s);
    degrees += 1.0;
    if (degrees >= 360.0) degrees = 0;

    // 模拟矩形波
    static int rect = 0;
    static uint16_t cnt = 0;
    cnt++;
    if (cnt >= 100) {
        cnt = 0;
        if (rect == 0) rect = 10;
        else rect = 0;
    }
    ui->widgetPlot2D->addData("矩形波", rect);
    ui->widgetPlot2D->addData("矩形波", rect);

    // 模拟随机数，随机数在-20到20内
    static QRandomGenerator rg;
    double rand = rg.generateDouble() * 40 - 20;
    ui->widgetPlot2D->addData("随机数", rand);
    ui->widgetPlot2D->addData("随机数", rand);
}

/* 定时器2溢出事件 */
void MainWindow::timer2Handler()
{

}

/* 定时器3溢出事件 */
void MainWindow::timer3Handler()
{

}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        if(watched == ui->comboBox){
            QComboBox* comboBox = qobject_cast<QComboBox *>(watched);
            comboBox->clear();
            QList<QSerialPortInfo> serials = QSerialPortInfo::availablePorts();
            foreach (QSerialPortInfo info, serials){
                comboBox->addItem(info.portName());
            }
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::on_pushButton_clicked(bool checked)
{
    QString str = (!checked) ? "打开":"关闭";
    ui->pushButton->setText(str);
    if(ui->comboBox->currentText() != NULL){
        qDebug()<<ui->stackedWidget->currentIndex()
                <<ui->devList->currentText()
                <<ui->comboBox->currentText()<<str;

        //区分设备
        if(ui->devList->currentText() == dev.at(0).name){                           //aimapro massage
//            if(checked){
//                ledTempture::instance()->open(ui->comboBox->currentText(), 921600);
//            }else{
//                ledTempture::instance()->close();
//            }
        }else if(ui->devList->currentText() == dev.at(1).name){                     //vima massage
//            if(checked){
//                Massage::instance()->open(ui->comboBox->currentText(), 921600);
//            }else{
//                Massage::instance()->close();
//            }
        }else if(ui->devList->currentText() == dev.at(2).name){
            if(checked){
                ctl::instance()->open(ui->comboBox->currentText(), 921600);
            }else{
                ctl::instance()->close();
            }
        }
    }
}

//设备列表
void MainWindow::on_devList_activated(int index)
{
    static int lastIndex = -1;
    if(lastIndex != index){
        qDebug()<<index;
        ui->stackedWidget->setCurrentIndex(index);
        lastIndex = index;
    }
}

//串口列表
void MainWindow::on_comboBox_activated(int index)
{
    (void)index;
    qDebug()<<ui->comboBox->currentText();
}

void MainWindow::on_stackedWidget_currentChanged(int arg1)
{
    static int lastIndex = 0;
    if(lastIndex != arg1){
        qDebug()<<dev.at(lastIndex).name<<"->"<<dev.at(arg1).name;
        lastIndex = arg1;
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}

