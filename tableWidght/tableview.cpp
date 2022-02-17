#include "tableview.h"

tableView::tableView(QWidget *parent)
{
    QStringList header;
    header<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7";
    this->tableWidget = new QTableWidget(2000,header.length(),parent);
    this->tableWidget->setWindowTitle("QTableWidget & Item");
    this->tableWidget->resize(800, 400);                      //设置表格
    this->tableWidget->verticalHeader()->setVisible(false);   //隐藏列表头
    this->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);    //设置不可编辑
}

tableView::~tableView()
{

}
