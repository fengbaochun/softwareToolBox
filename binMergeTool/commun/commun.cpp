#include "commun.h"
#include "can.h"

commun::commun(QObject *parent)
    : QObject{parent}
{

}

commun::~commun()
{

}

commun *commun::create(communType type)
{
    if(type == CAN_COMMUN){
        return can::instance();
    }else{
        return NULL;
    }
}

void commun::start()
{
    if(tim == nullptr){
        tim = new QTimer();
        tim->setInterval(1);
        connect(tim, SIGNAL(timeout()), this, SLOT(dataUpdateCallBack()));
        qDebug()<<"time start--------";
    }
    tim->start();
}

void commun::stop()
{
    tim->stop();
}

void commun::dataUpdateCallBack()
{
    QByteArray buf = getData();
    if(!buf.isEmpty()){
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss.zzz");
        qDebug() << time << buf.length() << buf;
    }
}

//https://blog.csdn.net/weixin_38931060/article/details/117994816?spm=1001.2101.3001.6661.1&utm_medium=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-1-117994816-blog-113613555.pc_relevant_aa_2&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-1-117994816-blog-113613555.pc_relevant_aa_2&utm_relevant_index=1


