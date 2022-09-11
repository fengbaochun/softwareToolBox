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
    camera* cam;

private slots:
    void imgShowCallback(QImage img);
    void on_openBut_toggled(bool checked);

    void on_spinBox_valueChanged(int arg1);

private:
    Ui::camPage *ui;
};

#endif // CAMPAGE_H
