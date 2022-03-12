#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->treeViewInit();

    this->trunNum=0;
    this->encVal=0;

    mc = new motorCtl();

    wave = new MWaveView(ui->widget);
    wave->resize(ui->widget->size());

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateData()));
    timer->start(1);
}


void MainWindow::updateData(){
    wave->resize(ui->widget->size());

    //没选择就不添加数据
    if(!ui->currentChosecheckBox->checkState()&&
       !ui->speedChosecheckBox->checkState()&&
       !ui->posChosecheckBox->checkState())     return;

    static int index=0;
    QPointF point;

    for(int i=0;i<6;i++){
        point.setX(index);
        point.setY(mc->motorCurData[i]);

        this->wave_data[i].append(point);
        if (this->wave_data[i].size() > 20000){
            this->wave_data[i].removeFirst();
        }
        wave->addSeriesData((WAVE_CH)i,this->wave_data[i]);
    }

    index = index + 1;
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::treeViewInit()
{
    ui->treeWidget->setColumnWidth(0, 200);  //设置列宽
    ui->treeWidget->setWindowTitle("Displays");
    ui->treeWidget->setWindowIcon(QIcon(":/new/prefix1/img/config.png"));
    //header 设置
    ui->treeWidget->setHeaderHidden(true);
    ui->treeWidget->setHeaderLabels(QStringList()<<"key"<<"value");

    //board param
    QTreeWidgetItem *Global=new QTreeWidgetItem(QStringList()<<"board");
    Global->setIcon(0,QIcon(":/new/prefix1/img/config.png"));

    QTreeWidgetItem* FixedFrame=new QTreeWidgetItem(QStringList()<<"commun mode");
    Global->addChild(FixedFrame);

    ui->treeWidget->addTopLevelItem(Global);

    //添加combox控件
    QComboBox *combo=new QComboBox();
    combo->addItem("usart");
    combo->addItem("can");
    combo->setMaximumWidth(150);
    ui->treeWidget->setItemWidget(FixedFrame,1,combo);
    connect(combo, SIGNAL(currentIndexChanged(int)), this, SLOT(choseCommunMode(int)));         //槽函数连接

    QTreeWidgetItem* bcolor=new QTreeWidgetItem(QStringList()<<"Background Color");
    Global->addChild(bcolor);

    //添加lineedit控件
    QLineEdit *colorval=new QLineEdit("255;255;255");
    colorval->setMaximumWidth(150);
    ui->treeWidget->setItemWidget(bcolor,1,colorval);
    connect(colorval, SIGNAL(textChanged(const QString &)), this, SLOT(updateColor(const QString &)));         //槽函数连接




    QTreeWidgetItem *motorParam=new QTreeWidgetItem(QStringList()<<"motor");
    motorParam->setIcon(0,QIcon(":/new/prefix1/img/config.png"));

    QTreeWidgetItem* pos_p=new QTreeWidgetItem(QStringList()<<"pos P");
    motorParam->addChild(pos_p);
    ui->treeWidget->addTopLevelItem(motorParam);

    QSpinBox *pos_p_spinBox=new QSpinBox();
    pos_p_spinBox->setMaximumWidth(150);
    pos_p_spinBox->setRange(10,50);
    ui->treeWidget->setItemWidget(pos_p,1,pos_p_spinBox);

    QTreeWidgetItem* pos_d=new QTreeWidgetItem(QStringList()<<"pos D");
    motorParam->addChild(pos_d);
    ui->treeWidget->addTopLevelItem(motorParam);

    QSpinBox *pos_d_spinBox=new QSpinBox();
    pos_p_spinBox->setMaximumWidth(150);
    pos_p_spinBox->setRange(10,50);
    ui->treeWidget->setItemWidget(pos_d,1,pos_d_spinBox);

    QTreeWidgetItem* max_speed=new QTreeWidgetItem(QStringList()<<"max speed");
    motorParam->addChild(max_speed);
    ui->treeWidget->addTopLevelItem(motorParam);

    QSpinBox *max_speed_spinBox=new QSpinBox();
    pos_p_spinBox->setMaximumWidth(150);
    pos_p_spinBox->setRange(10,50);
    ui->treeWidget->setItemWidget(max_speed,1,max_speed_spinBox);

    QTreeWidgetItem* speed_p=new QTreeWidgetItem(QStringList()<<"speed P");
    motorParam->addChild(speed_p);
    ui->treeWidget->addTopLevelItem(motorParam);

    QSpinBox *speed_p_spinBox=new QSpinBox();
    pos_p_spinBox->setMaximumWidth(150);
    pos_p_spinBox->setRange(10,50);
    ui->treeWidget->setItemWidget(speed_p,1,speed_p_spinBox);

    QTreeWidgetItem* speed_i=new QTreeWidgetItem(QStringList()<<"speed I");
    motorParam->addChild(speed_i);
    ui->treeWidget->addTopLevelItem(motorParam);

    QSpinBox *speed_i_spinBox=new QSpinBox();
    pos_p_spinBox->setMaximumWidth(150);
    pos_p_spinBox->setRange(10,50);
    ui->treeWidget->setItemWidget(speed_i,1,speed_i_spinBox);




//    /*----------------------------------------------------*/
//    //grid
//    QTreeWidgetItem *grid=new QTreeWidgetItem(QStringList()<<"motor ctl");
//    grid->setIcon(0,QIcon(":/new/prefix1/img/config.png"));

//    ui->treeWidget->addTopLevelItem(grid);
////    QCheckBox* gridcheck=new QCheckBox;
////    ui->treeWidget->setItemWidget(grid,1,gridcheck);

//    QTreeWidgetItem *gridstatus=new QTreeWidgetItem(QStringList()<<"Statue:");
//    gridstatus->setIcon(0,QIcon(":/new/prefix1/img/config.png"));
//    grid->addChild(gridstatus);
//    QLabel *gridstatusvalue=new QLabel("ok");
//    gridstatusvalue->setMaximumWidth(150);
//    ui->treeWidget->setItemWidget(gridstatus,1,gridstatusvalue);

}


//void MainWindow::on_dial_valueChanged(int value)
//{
//    static int lastVal = 0;
//    if(lastVal - value > 180){
//        this->trunNum++;
//    }else if(lastVal - value < -180){
//        trunNum--;
//    }
//    lastVal = value;
//    this->encVal = (value + 360*trunNum)*(-1);

//    mc->setTarPos(this->encVal);
//}

void MainWindow::on_currentChosecheckBox_clicked(bool checked)
{
    qDebug()<<"checked  "<<checked;
    wave->choseOpenChannel((WAVE_CH)TAR_CURRENT,checked);
    wave->choseOpenChannel((WAVE_CH)CUR_CURRENT,checked);
}

void MainWindow::on_speedChosecheckBox_clicked(bool checked)
{
    qDebug()<<"checked  "<<checked;
    wave->choseOpenChannel((WAVE_CH)TAR_SPEED,checked);
    wave->choseOpenChannel((WAVE_CH)CUR_SPEED,checked);
}

void MainWindow::on_posChosecheckBox_clicked(bool checked)
{
    qDebug()<<"checked  "<<checked;
    wave->choseOpenChannel((WAVE_CH)TAR_POS,checked);
    wave->choseOpenChannel((WAVE_CH)CUR_POS,checked);
}

void MainWindow::choseCommunMode(int index)
{
    qDebug()<<"index  "<<index;
}

void MainWindow::updateColor(const QString &text)
{
    qDebug()<<"index  "<<text;
}
