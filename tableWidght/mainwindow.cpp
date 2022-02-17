#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tableview.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    table = new tableView(ui->widget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

