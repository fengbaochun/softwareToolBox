#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);        //开启高分辨率支持
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);           //处理图像模糊问题
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    qputenv("QT_SCALE_FACTOR", "0.95");                                 //qt界面缩放倍率

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
