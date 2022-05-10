#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("压力变送器V1.2");
    QIcon icon(":/img/favicon.ico");
    w.setWindowIcon(icon);
    w.show();
    return a.exec();
}
