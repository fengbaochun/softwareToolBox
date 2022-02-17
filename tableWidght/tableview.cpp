#include "tableview.h"

tableView::tableView(QWidget *parent)
{
    this->layout = new QBoxLayout(QBoxLayout::LeftToRight,parent);
    QStringList header;
    header<<"序号"<<"系统时间"<<"CAN通道"<<"传输方向"<<"ID"<<"长度"<<"数据";
    this->tableWidget = new QTableWidget(2000,header.length(),parent);
    this->tableWidget->setWindowTitle("QTableWidget & Item");
    this->tableWidget->resize(800, 400);                      //设置表格
    this->tableWidget->verticalHeader()->setVisible(false);   //隐藏列表头
    this->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);    //设置不可编辑

    //设置表头颜色
    this->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:#DCDCDC;}");//#D1EEEE
//    tableWidget->setStyleSheet("QTableView{selection-background-color:#D1EEEE;background-color:rgb(50,50,50);border:1px solid rgb(70,70,70);color:rgb(200,200,200)}");

    this->tableWidget->setHorizontalHeaderLabels(header);
    this->tableWidget->setItem(0,0,new QTableWidgetItem("1"));
    this->tableWidget->setItem(1,0,new QTableWidgetItem("2"));
    this->tableWidget->setItem(2,0,new QTableWidgetItem("3"));

    this->tableWidget->setItem(0,1,new QTableWidgetItem(QIcon(":/Image/IED.png"), "Jan's month"));
    this->tableWidget->setItem(1,1,new QTableWidgetItem(QIcon(":/Image/IED.png"), "Feb's month"));
    this->tableWidget->setItem(2,1,new QTableWidgetItem(QIcon(":/Image/IED.png"), "Mar's month"));

    this->tableWidget->setColumnWidth(0,60);          //设置列宽度
    this->tableWidget->setColumnWidth(2,80);
    this->tableWidget->setColumnWidth(3,80);
    this->tableWidget->setColumnWidth(4,40);
    this->tableWidget->setColumnWidth(5,40);
    this->tableWidget->setColumnWidth(6,280);

    ////    QComboBox *comBox = new QComboBox();
    ////    comBox->addItem("Y");
    ////    comBox->addItem("N");
    ////    tableWidget->setCellWidget(0,2,comBox);

}

tableView::~tableView()
{

}
