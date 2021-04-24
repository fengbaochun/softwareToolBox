#include "serialtool.h"
#include "ui_serialtool.h"

serialTool::serialTool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::serialTool)
{
    ui->setupUi(this);
}

serialTool::~serialTool()
{
    delete ui;
}
