#include "camPage.h"
#include "ui_camPage.h"

camPage::camPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::camPage)
{
    ui->setupUi(this);
    qDebug()<<"camPage thread id : "<<QThread::currentThreadId();

    camera::camType type[CAM_NUM] = {camera::USB_CAM, camera::USB_CAM} ;     //USB摄像头
    cam[0] = camera::create(type[0]);               //创建相机
    cam[1] = camera::create(type[1]);               //创建相机


    QThread *camThd0 = new QThread();                //创建线程
    cam[0]->moveToThread(camThd0);                   //把这个对象移到线程里面使用

    QThread *camThd1 = new QThread();                //创建线程
    cam[1]->moveToThread(camThd1);                   //把这个对象移到线程里面使用

    connect(cam[0], &camera::sendImg, this, &camPage::imgShowCallback,Qt::AutoConnection);   //图片刷新
    camThd0->start();

    connect(cam[1], &camera::sendImg, this, &camPage::imgShowCallback2,Qt::AutoConnection);   //图片刷新
    camThd1->start();
}

camPage::~camPage()
{
    delete ui;
}

//图片显示
void camPage::imgShowCallback(int id, QImage img)
{
    Q_UNUSED(id);
    ui->imgLabel->setScaledContents(true);              //图片自适应空间大小
    ui->imgLabel->setPixmap(QPixmap::fromImage(img));
    this->curImg[0] = img;
}

void camPage::imgShowCallback2(int id, QImage img)
{
    Q_UNUSED(id);
    ui->imgLabel_2->setScaledContents(true);              //图片自适应空间大小
    ui->imgLabel_2->setPixmap(QPixmap::fromImage(img));
    this->curImg[1] = img;
}

//打开相机
void camPage::on_openBut_toggled(bool checked)
{
    QString name = (checked == true) ? "关闭相机" : "打开相机";
    ui->openBut->setText(name);
    if(checked){
        QMetaObject::invokeMethod(cam[0], [this]{
            cam[0]->start(this->port[0]);                   //打开摄像头
            qDebug()<<"cam->start(0) thread id : "<<QThread::currentThreadId();
        });

        QMetaObject::invokeMethod(cam[1], [this]{
            cam[1]->start(this->port[1]);                   //打开摄像头
            qDebug()<<"cam->start(1) thread id : "<<QThread::currentThreadId();
        });

    }else{
        QMetaObject::invokeMethod(cam[0], [this]{
            cam[0]->stop();                     //关闭摄像头
            qDebug()<<"cam->stop(0) thread id : "<<QThread::currentThreadId();
        });

        QMetaObject::invokeMethod(cam[1], [this]{
            cam[1]->stop();                     //关闭摄像头
            qDebug()<<"cam->stop(1) thread id : "<<QThread::currentThreadId();
        });

        ui->imgLabel->clear();
        ui->imgLabel_2->clear();
    }
}

//相机帧率设置
void camPage::on_spinBox_valueChanged(int arg1)
{
    QMetaObject::invokeMethod(cam[0], [=]{ cam[0]->setFps(arg1);                   //打开摄像头
        qDebug()<<"cam 0 ->setFps(arg1): "<<QThread::currentThreadId();
    });
    QMetaObject::invokeMethod(cam[1], [=]{ cam[1]->setFps(arg1);                   //打开摄像头
        qDebug()<<"cam 1 ->setFps(arg1): "<<QThread::currentThreadId();
    });
}

//图像采集
void camPage::on_getBut_clicked()
{
    QMetaObject::invokeMethod(cam[0], [=]{ cam[0]->saveImg(".\\", curImg[0], "-L");        //保存图像
        qDebug()<<"cam 0 ->saveImg: "<<QThread::currentThreadId();
    });
    QMetaObject::invokeMethod(cam[1], [=]{ cam[1]->saveImg(".\\", curImg[1], "-R");        //保存图像
        qDebug()<<"cam 1 ->saveImg: "<<QThread::currentThreadId();
    });
}

//端口扫描
void camPage::on_scanBut_clicked()
{
    QMetaObject::invokeMethod(cam[0], [=]{ camIdList list = cam[0]->scanPort();                  //扫描端口
        ui->camComboBox->clear();
        qDebug()<<"cam Port list : "<<list;
        if(!list.isEmpty()) {
            ui->camComboBox->addItems(list);
            ui->camLComboBox->addItems(list);
            ui->camRComboBox->addItems(list);       //list 同步
        }
        qDebug()<<"cam 0 ->scanPort: "<<QThread::currentThreadId();
    });
}


//相机端口选择
void camPage::on_camLComboBox_activated(int index)
{
    this->port[0] = index;
    qDebug()<<"camLComboBox_ "<<index;
}


void camPage::on_camRComboBox_activated(int index)
{
    this->port[1] = index;
    qDebug()<<"camRComboBox_ "<<index;
}

