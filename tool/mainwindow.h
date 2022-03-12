#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serial.h"
#include "motorctl.h"
#include "mwaveview.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void treeViewInit();

private:
    void timeCallback();
    Ui::MainWindow *ui;
    motorCtl *mc;
    MWaveView *wave;
    QList<QPointF> wave_data[16];

    float encVal;                 //旋钮的值（多圈）
    int trunNum;                  //圈数

private slots:
    void updateData();

    void on_currentChosecheckBox_clicked(bool checked);
    void on_speedChosecheckBox_clicked(bool checked);
    void on_posChosecheckBox_clicked(bool checked);

    void choseCommunMode(int index);    //选择  通信模式
    void  updateColor(const QString & text);
};
#endif // MAINWINDOW_H
