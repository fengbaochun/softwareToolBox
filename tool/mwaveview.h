/***************************************************************
 * @Copyright(C)    wangchongwei (版权申明请勿删除)
 * @FileName:       MWaveview.h
 * @Author:         wangchongwei
 * @Version:        1.0.0
 * @Date:           2021.7.11
 *
 * @Description: 波形显示的继承类
    1. 加载数据波形
        - 1.追加点方式
        - 2.传入波形数据
    2. 支持16个通道
    3. 鼠标混轮可以缩小放大
    4. 鼠标右键波形设置暂停开始
    5. 鼠标移动显示坐标
    6. 鼠标左键按住可以拖动波形
    7. 设置显示关闭某个通道
    8. 清楚某个通道数据
    9. 设置X轴Y轴范围
    10. 设置缩放比例
 * 个人微信号: wang_chongwei
 * 个人公众号: 王崇卫(欢迎关注)
 * CSDN: https://blog.csdn.net/qq_38190041
 * 个人网站: https://wang_chong_wei.gitee.io/blog/
 *
 ***************************************************************/

#ifndef MWAVEVIEW_H
#define MWAVEVIEW_H

#include <QObject>
#include <QWidget>
#include <QChartView>
#include <QtCharts>



// 定义最大通道数量
#define WAVE_CHANNEL_MAX  16
enum WAVE_CH
{
    WAVE_CH0,
    WAVE_CH1,
    WAVE_CH2,
    WAVE_CH3,
    WAVE_CH4,
    WAVE_CH5,
    WAVE_CH6,
    WAVE_CH7,
    WAVE_CH8,
    WAVE_CH9,
    WAVE_CH10,
    WAVE_CH11,
    WAVE_CH12,
    WAVE_CH13,
    WAVE_CH14,
    WAVE_CH15,
};

// 直线:  QLineSeries
// 平滑线  QSplineSeries
typedef QLineSeries SeriesType;

struct Wave
{
    QMap<WAVE_CH, SeriesType *> map_series;  //key: ch, value SeriesType *
    QChart *chart;  // 图纸
    QValueAxis *axisX, *axisY;   // xy轴
    double rangeX, rangeY;      // 控制XY的可视范围

    double multipleX, multipleY;   // 放大倍数，对应缩小倍数为1/multiple.
    double last_point_x = 0;
};

struct Event
{
    bool rightButtonPressed = false;
    bool leftButtonPressed = false;
    QPoint PressedPos;
    double moveX,moveY;  // 记录移动
    bool pauseWave;
    QMenu *menu;
    QAction* startAction;
    QAction* pauseAction;
};



class MWaveView:public QChartView
{
    Q_OBJECT
public:
    MWaveView(QWidget *parent);
    ~MWaveView();
    #define SET_GLOBLE_CHANNEL  16  // 设置的通道数量
    void openChannel(WAVE_CH ch);   // 打开某个通道显示
    void closeChannel(WAVE_CH ch);  // 关闭某个通道显示
    void choseOpenChannel(WAVE_CH ch,bool isOpen);//选择是否打开通道
    void clearChannel(WAVE_CH ch);  // 清除某个通道波形数据
    void addSeriesData(WAVE_CH ch, const QPointF& point);   // 追加某个通道的坐标点
    void addSeriesData(WAVE_CH ch, const QList<QPointF>& point_list);   // 传入某个通道的波形
    void setRangeX(int rangeX);     // 设置X轴范围
    void setRangeY(int rangeY);     // 设置Y轴范围
    void setZoomX(double multiple); // 设置X轴范围放大倍数,默认1.2，不可为0，对应缩小为1/multiple
    void setZoomY(double multiple); // 设置Y轴范围放大倍数，默认1.2，不可为0，对应缩小为1/multiple
    void ZoomOutX(void);            // X轴范围按比例缩小
    void ZoomX(void);       // X轴范围按比例放大
    void ZoomOutY(void);    // Y轴范围按比例缩小
    void ZoomY(void);       // Y轴范围按比例放大
    void startGraph(void);  // 波形开始
    void pauseGraph(void);  // 波形暂停

public slots:
    void slots_startGraph(void);// 波形开始槽函数
    void slots_pauseGraph(void); // 波形暂停槽函数
protected:
    // 事件重写
    virtual void mouseMoveEvent(QMouseEvent *pEvent) override;
    virtual void mousePressEvent(QMouseEvent *pEvent) override;
    virtual void mouseReleaseEvent(QMouseEvent *pEvent) override;
    virtual void wheelEvent(QWheelEvent *pEvent) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;

private:
    void addChannel(WAVE_CH ch);
    void updateRange(void);

    QBoxLayout *wave_layout;
    Wave m_wave;
    Event m_event;
};

#endif // MWAVEVIEW_H
