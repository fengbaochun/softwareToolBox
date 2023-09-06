#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    customPlotWidget = new QCustomPlot(ui->widget);
    customPlotWidget->resize(ui->widget->size());


    //可移动缩放
    customPlotWidget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom );

    //设置背景颜色
    customPlotWidget->setBackground(QColor(25,35,45));

    //轴刻度文字
    customPlotWidget->xAxis->setTickLabelColor(Qt::white);
    customPlotWidget->yAxis->setTickLabelColor(Qt::white);

    //设定右上角图形标注可见
    customPlotWidget->legend->setVisible(true);
    customPlotWidget->legend->setBrush(QColor(25,35,45));
    customPlotWidget->legend->setTextColor(Qt::white);
    customPlotWidget->legend->setFont(QFont("Helvetica", 9));

    //设置X轴坐标范围
    customPlotWidget->xAxis->setRange(-10, 100);
    //设置Y轴坐标范围
    customPlotWidget->yAxis->setRange(-150, 150);

    customPlotWidget->xAxis->setLabel("x 轴");                   // 设置 x 轴的标签
    customPlotWidget->yAxis->setLabel("y 轴");                   // 设置 y 轴的标签

    customPlotWidget->addGraph();                               // 创建一条线段
    customPlotWidget->graph(0)->setName("通道1");
    customPlotWidget->graph(0)->setPen(QPen(QColor(178,34,34)));//设置颜色

    //传入数据 QVector<double>类型
    QVector<double> xData;
    QVector<double> yData;

    for(int i = 0; i < 100; i++)
    {
        xData.append(i);
        yData.append(150 * sin(i));
    }
    customPlotWidget->graph(0)->setData(xData, yData);          // 设置线段的数据

    customPlotWidget->rescaleAxes();                            //自动缩放
    customPlotWidget->replot();                                 //图标重绘


}

MainWindow::~MainWindow()
{
    delete ui;
}


/*
 * 参考：
 * https://blog.csdn.net/hw5230/article/details/130139356
 * https://blog.csdn.net/weixin_47488212/article/details/129299987
 *
 *
 */

