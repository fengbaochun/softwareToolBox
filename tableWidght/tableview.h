#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QObject>
#include <QWidget>

#include <QDebug>
#include <QTimer>

#include <QTableWidget>
#include <QTableWidgetItem>
//#include <QCombobox>
#include <QHeaderView>
#include <QBoxLayout>

class tableView : public QObject
{
    Q_OBJECT
public:
    tableView(QWidget *parent);
    ~tableView();
    void apindData();
    int index;

private slots:
    void onMsGetTimeOut();

private:
    QTableWidget *tableWidget;
    QBoxLayout *layout;
};

#endif // TABLEVIEW_H
