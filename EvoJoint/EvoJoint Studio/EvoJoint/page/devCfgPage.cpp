#include "devCfgPage.h"
#include "qdebug.h"
#include "ui_devCfgPage.h"
#include <qserialportinfo.h>
#include "motor/ctl.h"

devCfgPage::devCfgPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::devCfgPage)
{
    ui->setupUi(this);
    ui->comboBox->installEventFilter(this);     //安装QComboBox的事件过滤器：
    //设备信息
    dev.append({    .index=0,
                    .name="aimapro massage",
                    .port=""}   );


    dev.append({    .index=1,
                    .name="vima massage",
                    .port=""}   );

    dev.append({    .index=2,
                    .name="ctl ",
                    .port=""}   );

    QStringList str;
    foreach (devInfoType d, dev) {
        str.append(d.name);
    }

    ui->devList->addItems(str);
}

devCfgPage::~devCfgPage()
{
    delete ui;
}

//事件过滤器
bool devCfgPage::eventFilter(QObject *watched, QEvent *event)
{
    if( (watched == ui->comboBox) &&
        (event->type() == QEvent::MouseButtonPress))
    {
        if(watched == ui->comboBox){
            QComboBox* comboBox = qobject_cast<QComboBox *>(watched);
            comboBox->clear();
            QList<QSerialPortInfo> serials = QSerialPortInfo::availablePorts();
            foreach (QSerialPortInfo info, serials){
                comboBox->addItem(info.portName());
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}

void devCfgPage::on_pushButton_clicked(bool checked)
{
    QString str = (!checked) ? "打开":"关闭";
    ui->pushButton->setText(str);
    if(ui->comboBox->currentText() != NULL){
        qDebug()<<ui->devList->currentText()
                <<ui->comboBox->currentText()<<str;

        //区分设备
        if(ui->devList->currentText() == dev.at(0).name){                           //aimapro massage
//            if(checked){
//                ledTempture::instance()->open(ui->comboBox->currentText(), 921600);
//            }else{
//                ledTempture::instance()->close();
//            }
        }else if(ui->devList->currentText() == dev.at(1).name){                     //vima massage
//            if(checked){
//                Massage::instance()->open(ui->comboBox->currentText(), 921600);
//            }else{
//                Massage::instance()->close();
//            }
        }else if(ui->devList->currentText() == dev.at(2).name){
            if(checked){
                ctl::instance()->open(ui->comboBox->currentText(), 921600);
                this->hide();
            }else{
                ctl::instance()->close();
            }
        }
    }
}

//设备列表
void devCfgPage::on_devList_activated(int index)
{
    static int lastIndex = -1;
    if(lastIndex != index){
        qDebug()<<index;
//        ui->stackedWidget->setCurrentIndex(index);
        lastIndex = index;
    }
}

//串口列表
void devCfgPage::on_comboBox_activated(int index)
{
    (void)index;
    qDebug()<<ui->comboBox->currentText();
}

