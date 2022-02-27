#ifndef CHART_H
#define CHART_H

#include <QWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTime>
#include <QDebug>
#include "chart/qcustomplot.h"
#include "chart/serial.h"

namespace Ui {
class Chart;
}
#define CHANNEL_COUNT 5 // 通道数量
class Chart : public QWidget
{
    Q_OBJECT

public:
    explicit Chart(QWidget *parent = nullptr);
    ~Chart();
    void initChart(QCustomPlot *ChartWidget); // 初始化图表

    void clearChannel(int channel); // 清除通道对应曲线
    void clearChart(); // 清除图表
    void replotChart(); // 刷新图表
    void clearPauseTime();//清除暂停时间
    void startTimer();// 启动串口计时器
    void clearChannelData(); // 清空本地各通道的数据

    void updataChart(char channel, QVector<int> vecPidData);//更新图标



private slots:

    void readFromSerial(); // 读取来自串口类的数据
    void handleSerialError(); // 处理串口错误
    void onNewPortList(QStringList portName); // 用于响应SerialPortList信号，周期获取串口列表

    void on_openSerialButton_clicked(); // 打开串口按钮-点击槽函数

//    void on_readPidButton_clicked(); // 读取PID按钮-点击槽函数
//    void on_writePidButton_clicked(); // 写入PID按钮-点击槽函数

    void get_target_val();//获取新的目标值

private:
    QCustomPlot *m_customPlot; // 图表实例指针
    QVector<int> m_vecPidLastData; // 存放5个通道的PID上一次结果值
    QMap<double, int> m_mapChannel[5]; // 本地保存各通道的数据
    bool m_ismoving; // 鼠标左键是否移动和滚轮是否使用的标志位

    QTime m_pauseTimer; // 暂停时间计时器
    int m_pauseTime; // 暂停时间
    QTime m_serialTime; // 串口打开后的计时器
    Serial *m_serial; // 自定义串口类实例指针

    Ui::Chart *ui;
    bool m_isPause = true; // 是否暂停更新的标志位

    QVector<int> m_vecPidData; // 存放5个通道的PID结果值
};

#endif // CHART_H
