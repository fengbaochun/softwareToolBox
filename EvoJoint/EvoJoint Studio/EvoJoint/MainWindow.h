#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include "page/chartPage.h"
#include "page/devCfgPage.h"
#include "page/operatePage.h"
#include "page/paramPage.h"
#include "page/debugPage.h"

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
    void mousePressEvent(QMouseEvent *event) override;

private slots:

private:
    Ui::MainWindow *ui;
    devCfgPage *devUi;
    chartPage *chartUi;
    operatePage *operateUi;
    paramPage *paramUi;
    debugPage *debugUi;

};
#endif // MAINWINDOW_H
