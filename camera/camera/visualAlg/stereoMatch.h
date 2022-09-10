#ifndef STEREOMATCH_H
#define STEREOMATCH_H

#include <QObject>
#include <QDebug>

class stereoMatch : public QObject
{
    Q_OBJECT
public:
    explicit stereoMatch(QObject *parent = nullptr);

public slots:
    void loadImg(QString s);              //加载参数
    void camClicked();

};

#endif // STEREOMATCH_H
