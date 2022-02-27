#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serial_tool/serial_tool.h"
#include "chart/chart.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:


private slots:
    void on_tabWidget_currentChanged(int index);

private:
    Ui::MainWindow *ui;
    serial_tool *ss_ui;
    Chart *chart_ui;
};
#endif // MAINWINDOW_H
