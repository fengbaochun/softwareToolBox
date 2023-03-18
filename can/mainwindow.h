﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include "candev.h"
#include "canopen.h"

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
    void on_peelButton_clicked();

    void on_calButton_clicked();

    void on_zeroButton_clicked();

    void on_canclePeelButton_clicked();

private:
    Ui::MainWindow *ui;
//    canDev *can;
    canopen *canfestival;
};
#endif // MAINWINDOW_H
