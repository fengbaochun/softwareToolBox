#ifndef STEREOMATCH_H
#define STEREOMATCH_H

#include <QObject>

class stereoMatch : public QObject
{
    Q_OBJECT
public:
    explicit stereoMatch(QObject *parent = nullptr);

signals:

public slots:
    void loadImg(QString);              //加载参数
//    void


};

#endif // STEREOMATCH_H
