#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "camera.h"


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
    void imgShowCallback(QImage img);          //图像显示槽函数
    void on_camOpenButton_toggled(bool checked);
    void on_pushButtonSave_clicked();

private:
    Ui::MainWindow *ui;
    camera *cam;
};
#endif // MAINWINDOW_H
