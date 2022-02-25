#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "candev.h"
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
    void appindLog(QString temp);
    void updateShow();


private slots:
    void on_peelButton_clicked();

    void on_calButton_clicked();

    void on_zeroButton_clicked();

    void on_canclePeelButton_clicked();
    void updateData();

private:
    Ui::MainWindow *ui;
    MWaveView *wave;
    QList<QPointF> wave_data[16];
    canDev *can;
//    QTimer*  mvGetTimer;
};
#endif // MAINWINDOW_H
