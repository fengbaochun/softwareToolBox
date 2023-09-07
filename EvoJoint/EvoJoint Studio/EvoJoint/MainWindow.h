#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "page/ctlPage.h"
#include "motor/ctl.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    typedef struct{
        int index;
        QString name;
        QString port;
    }devInfoType;

    QList<devInfoType> dev;

private slots:

    bool eventFilter(QObject *watched, QEvent *event);
    void on_pushButton_clicked(bool checked);

    void on_devList_activated(int index);

    void on_comboBox_activated(int index);

    void on_stackedWidget_currentChanged(int arg1);

private:
    Ui::MainWindow *ui;
    ctlPage *ctlPageUi;
};
#endif // MAINWINDOW_H
