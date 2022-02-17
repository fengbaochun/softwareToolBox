#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QObject>
#include <QWidget>

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QCombobox>
#include <QHeaderView>

#include <QBoxLayout>

class tableView
{
//    Q_OBJECT
public:
    tableView(QWidget *parent);
    ~tableView();
private:
    QTableWidget *tableWidget;
    QBoxLayout *layout;
};

#endif // TABLEVIEW_H
