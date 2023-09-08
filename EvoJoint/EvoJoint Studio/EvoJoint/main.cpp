#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
/*
 * 参考：
 * https://blog.csdn.net/m0_60259116/article/details/127616278
*/
