#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}


//#include <QApplication>
//#include <QTableWidget>
//#include <QTableWidgetItem>
//#include <QCombobox>
//#include <QHeaderView>

//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    QStringList header;
//    header<<"序号"<<"系统时间"<<"CAN通道"<<"传输方向"<<"ID"<<"长度"<<"数据";
//    // 构造了一个QTableWidget的对象，并且设置为10行，5列
//    QTableWidget *tableWidget = new QTableWidget(2000,header.length());
//    // 也可用下面的方法构造QTableWidget对象
//    // QTableWidget *tableWidget = new QTableWidget;
//    // tableWidget->setRowCount(10); //设置行数为10
//    // tableWidget->setColumnCount(5); //设置列数为5
//    tableWidget->setWindowTitle("QTableWidget & Item");
//    tableWidget->resize(800, 400);                      //设置表格
//    tableWidget->verticalHeader()->setVisible(false);   //隐藏列表头
//    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);    //设置不可编辑
//    //设置表头颜色
//    tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:#DCDCDC;}");//#D1EEEE
////    tableWidget->setStyleSheet("QTableView{selection-background-color:#D1EEEE;background-color:rgb(50,50,50);border:1px solid rgb(70,70,70);color:rgb(200,200,200)}");

//    tableWidget->setHorizontalHeaderLabels(header);
//    tableWidget->setItem(0,0,new QTableWidgetItem("1"));
//    tableWidget->setItem(1,0,new QTableWidgetItem("2"));
//    tableWidget->setItem(2,0,new QTableWidgetItem("3"));

//    tableWidget->setItem(0,1,new QTableWidgetItem(QIcon(":/Image/IED.png"), "Jan's month"));
//    tableWidget->setItem(1,1,new QTableWidgetItem(QIcon(":/Image/IED.png"), "Feb's month"));
//    tableWidget->setItem(2,1,new QTableWidgetItem(QIcon(":/Image/IED.png"), "Mar's month"));

//    tableWidget->setColumnWidth(0,60);          //设置列宽度
//    tableWidget->setColumnWidth(2,80);
//    tableWidget->setColumnWidth(3,80);
//    tableWidget->setColumnWidth(4,40);
//    tableWidget->setColumnWidth(5,40);
//    tableWidget->setColumnWidth(6,280);

////    QComboBox *comBox = new QComboBox();
////    comBox->addItem("Y");
////    comBox->addItem("N");
////    tableWidget->setCellWidget(0,2,comBox);

//    tableWidget->show();

//    return a.exec();
//}
