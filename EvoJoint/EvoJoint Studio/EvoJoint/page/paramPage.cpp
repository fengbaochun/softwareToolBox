#include "paramPage.h"
#include "ui_paramPage.h"

paramPage::paramPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::paramPage)
{
    ui->setupUi(this);

    QStringList s = {"功能码" , "名称" , "参数类型" , "属性", "最大值", "最小值", "当前值"};
    ui->tableWidget->setColumnCount(s.size());
    ui->tableWidget->setFont(QFont("宋体", 9));
    ui->tableWidget->setHorizontalHeaderLabels(s);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

paramPage::~paramPage()
{
    delete ui;
}

//https://www.elecfans.com/d/2081372.html
