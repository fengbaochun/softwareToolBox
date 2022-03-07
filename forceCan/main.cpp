#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("压力变送器V1.0");
    QIcon icon(":/new/prefix1/img/check.pngg");     //:/new/prefix1/img/check.png
    w.setWindowIcon(icon);
    w.show();
    return a.exec();
}
