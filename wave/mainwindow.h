#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "module/wave_view/mwaveview.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void onMsGetTimeOut();
    void on_checkBox_0_stateChanged(int arg1);

    void on_dial_X_sliderMoved(int position);

    void on_dial_Y_sliderMoved(int position);

    void on_checkBox_1_stateChanged(int arg1);

    void on_checkBox_2_stateChanged(int arg1);

    void on_checkBox_3_stateChanged(int arg1);

    void on_checkBox_4_stateChanged(int arg1);

    void on_checkBox_5_stateChanged(int arg1);

    void on_checkBox_6_stateChanged(int arg1);

    void on_checkBox_7_stateChanged(int arg1);

    void on_checkBox_8_stateChanged(int arg1);

    void on_checkBox_9_stateChanged(int arg1);

    void on_checkBox_10_stateChanged(int arg1);

    void on_checkBox_11_stateChanged(int arg1);

    void on_checkBox_12_stateChanged(int arg1);

    void on_checkBox_13_stateChanged(int arg1);

    void on_checkBox_14_stateChanged(int arg1);

    void on_checkBox_15_stateChanged(int arg1);

    void on_pushButton_pause_clicked();

    void on_pushButton_start_clicked();

private:
    Ui::MainWindow *ui;
    MWaveView *wave;
    QTimer*  mvGetTimer;
    QList<QPointF> wave_data[16];

};
#endif // MAINWINDOW_H
