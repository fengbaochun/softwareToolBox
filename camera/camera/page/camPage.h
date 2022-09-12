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
    QImage curImg[CAM_NUM];
    QList<int> port = {0,1};
private slots:
    void imgShowCallback(int id, QImage img);
    void imgShowCallback2(int id, QImage img);
    void on_openBut_toggled(bool checked);
    void on_spinBox_valueChanged(int arg1);
    void on_getBut_clicked();

    void on_scanBut_clicked();

    void on_camLComboBox_activated(int index);

    void on_camRComboBox_activated(int index);

private:
    Ui::camPage *ui;
};

#endif // CAMPAGE_H
