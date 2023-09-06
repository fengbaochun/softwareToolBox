#ifndef CTLPAGE_H
#define CTLPAGE_H

#include <QWidget>

namespace Ui {
class ctlPage;
}

class ctlPage : public QWidget
{
    Q_OBJECT

public:
    explicit ctlPage(QWidget *parent = nullptr);
    ~ctlPage();

private slots:
    void on_comboBoxFps_currentIndexChanged(int index);

    void on_spinBoxMaxSpeed_valueChanged(int arg1);

    void on_spinBoxMinSpeed_valueChanged(int arg1);

    void on_doubleSpinBoxMaxCur_valueChanged(double arg1);

    void on_doubleSpinBoxMinCur_valueChanged(double arg1);

    void on_pushButton_clicked();

    void on_pushButtonSync_clicked();

    void on_doubleSpinBoxPosP_valueChanged(double arg1);

    void on_doubleSpinBoxPosD_valueChanged(double arg1);

    void on_doubleSpinBoxSpeedP_valueChanged(double arg1);

    void on_doubleSpinBoxSpeedI_valueChanged(double arg1);

    void on_doubleSpinBoxDqP_valueChanged(double arg1);

    void on_doubleSpinBoxDqI_valueChanged(double arg1);

    void on_horizontalSliderSpeed_valueChanged(int value);

    void on_horizontalSliderDq_valueChanged(int value);

    void on_pushButtonSave_clicked();

    void on_spinBoxTarPos_editingFinished();

    void on_pushButtonCal_clicked();

private slots:
    void reportCallBack(uint8_t fun, QByteArray qb);

private:
    Ui::ctlPage *ui;
    QVector<QPointF> waveData[16];     //最多16条曲线

    typedef enum{                       //上报速度
        S0FPS       = 1000000,
        S50FPS      = 1000/50,
        S100FPS     = 1000/100,
        S200FPS     = 1000/200,
        S500FPS     = 1000/500,
        S1000FPS    = 1000/1000,
    }reportSpeedType;
    QTimer *waveT;
    QVector<QVector<float>> qVdata;
};

#endif // CTLPAGE_H
