#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    can = new canDev();
    can->run();
}

MainWindow::~MainWindow()
{
    delete ui;
}

