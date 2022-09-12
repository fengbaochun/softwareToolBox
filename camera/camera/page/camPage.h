#ifndef CAMPAGE_H
#define CAMPAGE_H

#include <QWidget>
#include "camera/camera.h"

namespace Ui {
class camPage;
}


class camPage : public QWidget
{
    Q_OBJECT

public:
    explicit camPage(QWidget *parent = nullptr);
    ~camPage();
    #define CAM_NUM 2
    camera* cam[CAM_NUM];

private slots:
    void imgShowCallback(int id, QImage img);
    void imgShowCallback2(int id, QImage img);
    void on_openBut_toggled(bool checked);
    void on_spinBox_valueChanged(int arg1);
private:
    Ui::camPage *ui;
};

#endif // CAMPAGE_H
