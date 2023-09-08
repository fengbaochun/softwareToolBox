#ifndef WIDGETPLOT2D_H
#define WIDGETPLOT2D_H

#include <QWidget>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include "qcustomplot.h"

namespace Ui {
class WidgetPlot2D;
}

class WidgetPlot2D : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetPlot2D(QWidget *parent = 0);
    ~WidgetPlot2D();

    void initGraphName(QStringList name);

private:
    Ui::WidgetPlot2D *ui;

    QVector<QCheckBox*> isShowCheckBoxVector;              // 是否显示勾选框
    QVector<QLabel*> valueLabelVector;                     // label数值显示
    QVector<QPushButton*>   GraphColorPushButtonVector;    // 曲线颜色选择按钮
    QMap<QString, int> nameToGraphMap;                     // 曲线名称对应曲线序号
    QVector<double> valueVector;                           // 存储曲线的当前值
    QTime time;

    void initQCP();
    void initWidget();
    void setTheme(QColor axis, QColor background);
    bool isDirExist(QString fullPath);
    void savePlotPng();

    bool isPause = false;
    bool isShowCfg = false;

private slots:
    void changeGraphVisible();
    void changeGraphColor();
    void plotOperation();
    void horzScrollBarChanged(int value);
    void changePlotTheme();

    void slot_show_region_context_menu(QMouseEvent *event);

public slots:
    void addData(QString name, double data);
};

#endif // WIDGETPLOT2D_H
