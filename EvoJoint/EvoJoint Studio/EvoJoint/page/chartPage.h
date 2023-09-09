#ifndef CHARTPAGE_H
#define CHARTPAGE_H

#include <QWidget>

namespace Ui {
class chartPage;
}

class chartPage : public QWidget
{
    Q_OBJECT

public:
    explicit chartPage(QWidget *parent = nullptr);
    ~chartPage();


private slots:
    void reportCallBack(uint8_t fun, QByteArray qb);
    void timer1Handler();

private:
    Ui::chartPage *ui;

    QTimer *waveT;
    QVector<QVector<float>> qVdata;

};

#endif // CHARTPAGE_H
