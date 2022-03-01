#include "serial_tool/serial_tool.h"
#include "ui_serial_tool.h"
#include <QtCore>
#include <QAbstractItemView>
#include  <QSplitter>

serial_tool::serial_tool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::serial_tool)
{
    ui->setupUi(this);

    //按钮填充
    btn[0]=ui->openButton;
    btn[1]=ui->sendButton;
    btn[2]=ui->clearButton;
    btn[3]=ui->rev_clear_Button;

    //连接信号槽
    connect(ui->openButton,&QPushButton::clicked,this,&serial_tool::but_manage);
    connect(ui->sendButton,&QPushButton::clicked,this,&serial_tool::but_manage);
    connect(ui->clearButton,&QPushButton::clicked,this,&serial_tool::but_manage);
    connect(ui->rev_clear_Button,&QPushButton::clicked,this,&serial_tool::but_manage);

    connect(ui->chb_timer_send,&QCheckBox::clicked,this,&serial_tool::timed_callback);
//    connect(ui->send_time,&QSpinBox::valueChanged(),this,&serial_tool::timed_callback);


    //波特率下拉列表设置
    QStringList baudList;
    baudList << u8"4800" << u8"9600" << u8"38400" << u8"115200" << u8"230400" << u8"460800";
    ui->baudBox->addItems(baudList);        // 添加列表到控件中
    ui->baudBox->setCurrentText(baudList[3]);//设置当前选项

    //校检下拉列表设置
    QStringList xcheckList;
    xcheckList << u8"无校检" << u8"奇校检" << u8"偶校检" << u8"1校检" << u8"0校检" ;
    ui->xcheckBox->addItems(xcheckList);        // 添加列表到控件中
    ui->xcheckBox->setCurrentText(xcheckList[0]);//设置当前选项

    //停止位下拉列表设置
    QStringList stopList;
    stopList << u8"0" << u8"1" << u8"2" ;
    ui->stopBox->addItems(stopList);        // 添加列表到控件中
    ui->stopBox->setCurrentText(stopList[0]);//设置当前选项

    //停止位下拉列表设置
    QStringList dataList;
    dataList << u8"5" << u8"6" << u8"7" << u8"8" ;
    ui->dataBox->addItems(dataList);        // 添加列表到控件中
    ui->dataBox->setCurrentText(dataList[3]);//设置当前选项

    //设置当前发送和接收的模式为文本模式
    ui->radio_rev_text->setChecked(true);
    ui->radio_send_text->setChecked(true);

    //获取当前下拉列表的内容
    QString current_dataBox=ui->dataBox->currentText();//获取当前combox的文本
    int dataBox_index=ui->dataBox->currentIndex();//获取当前combox的文本

    qDebug()<<dataBox_index;  //打印数据位下拉列表的索引
    qDebug()<<dataList[dataBox_index];  //打印数据位下拉列表的内容
    qDebug()<<current_dataBox;  //打印数据位下拉列表的内容

    ui->Rev_textBrowser->setText("Welcome to use Serial Assistant!!!");

    find_portinfo();

}

serial_tool::~serial_tool()
{
    delete ui;
}


void serial_tool::but_manage()
{
    //发送信号者的对象
    QPushButton *optBtn = qobject_cast<QPushButton *>(sender());
    //发送信号者的对象名
    QString name = sender()->objectName();

    if(btn[0] == optBtn)
    {
        open_port();
    }
    else if(btn[1] == optBtn)
    {
        send_data();
    }
    else if(btn[2] == optBtn)
    {
        clear_send_buf();
    }
    else if(btn[3] == optBtn)
    {
        clear_rev_buf();
    }

}
//寻找串口设备
void serial_tool::find_portinfo()
{
    //查找可用的串口
    foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))//依次打开可读可写的所有设备
        {
            //ui->comBox->addItem(serial.portName()+" "+info.description());
            ui->comBox->addItem(serial.portName());

            serial_num++;

            serial.close();
        }
    }
    qDebug() << u8"串口个数" << serial_num;
}

//打开串口
void serial_tool::open_port()
{
    //判断按键状态
   if(ui->openButton->text() == tr(u8"打开串口"))
   {
       //设置串口名字
       serial.setPortName(ui->comBox->currentText());
       //打开串口(读写模式)
       //serial.open(QIODevice::ReadWrite);
       if(serial.open(QIODevice::ReadWrite))
       {
           qDebug()<< ui->comBox->currentText() <<u8"空闲";
       }
       else
       {
           qDebug()<< ui->comBox->currentText() <<u8"忙碌，请先关闭再打开";
           return ;
       }
       //设置波特率
       serial.setBaudRate(ui->baudBox->currentText().toInt());
       //设置数据位
       switch(ui->dataBox->currentText().toInt())
       {
           case 5: serial.setDataBits(QSerialPort::Data5); break;
           case 6: serial.setDataBits(QSerialPort::Data6); break;
           case 7: serial.setDataBits(QSerialPort::Data7); break;
           case 8: serial.setDataBits(QSerialPort::Data8); break;

       }
       //设置校检位
       serial.setParity(QSerialPort::NoParity);
       //设置停止位
       switch (ui->stopBox->currentIndex())
       {
           case 1: serial.setStopBits(QSerialPort::OneStop); break;
           case 2: serial.setStopBits(QSerialPort::TwoStop); break;
       }
       //设置流控制
       serial.setFlowControl(QSerialPort::NoFlowControl);
       //设置打开按钮的文本
       ui->openButton->setText(tr(u8"关闭串口"));
       qDebug()<< ui->comBox->currentText() <<u8"已打开";
       qDebug()<< u8"串口状态"<<serial.isOpen();

      //开启定时器
       connect(update_data,&QTimer::timeout,this,&serial_tool::show_rev_data);
       update_data->start(5); //5ms定时器
       qDebug() << u8"定时器 ID:"<<update_data->timerId();
   }
   else
   {
       //判断串口确实打开了
       if(serial.isOpen())
       {
           serial.clear();
           serial.close();
           ui->openButton->setText(tr(u8"打开串口"));
           qDebug()<< ui->comBox->currentText() <<u8"已关闭";
           update_data->stop();//停止定时器
           qDebug()<<u8"定时器已停止";
       }
   }
}

//清空发送区
void serial_tool::clear_send_buf()
{
    ui->Send_textEdit->clear();
    qDebug()<< u8"已清空";
}
//清空接收区
void serial_tool::clear_rev_buf()
{
    ui->Rev_textBrowser->clear();
    qDebug()<< u8"已清空";
}

//读取数据
QByteArray serial_tool::read_data()
{
    QByteArray buf;
    //一次性读取所有数据
    buf=serial.readAll();
    return buf;
}

//显示接收数据
void serial_tool::show_rev_data()
{
    //是否接收开关
    if(ui->stop_show_checkBox->isChecked() == false)
    {
        QString str_temp;
        QByteArray buf=read_data();

        if(!buf.isEmpty())//判断是否有数据
        {
            //hex 显示
            if(ui->radio_rev_hex->isChecked())
            {
                str_temp = ByteArrayToHexString(buf);
            }
            else//文本模式
            {
                str_temp = tr(buf);
            }

            //更新到控件中
            ui->Rev_textBrowser->append(str_temp);
        }
        buf.clear();
    }
}

//发送数据
void serial_tool::send_data()
{
    QString temp_str;
    temp_str = ui->Send_textEdit->toPlainText();

    qDebug()<< u8"字符串长度："<<temp_str.length();

    if(temp_str.length()&& serial.isOpen())
    {
        if(ui->newline_qcheckBox->isChecked())
        {
            temp_str = temp_str + "\r\n";
        }

        //hex 模式
        if(ui->radio_send_hex->isChecked())
        {
            QByteArray data = HexStringToByteArray(temp_str);
            qDebug()<<data;
            serial.write(data);
        }
        else //文本模式
        {
            serial.write(temp_str.toUtf8());//发送数据
        }
    }
}


//定时发送回调函数
void serial_tool::timed_callback()
{
    //串口已被打开
    if(serial.isOpen())
    {
        int time_val = ui->send_time->value();
        if(ui->chb_timer_send->isChecked())
        {
            connect(timed_sending,&QTimer::timeout,this,&serial_tool::loop_send_callback);
            timed_sending->start(time_val);
        }
        else
        {
            timed_sending->stop();
        }
    }

    qDebug()<<ui->chb_timer_send->checkState();
    qDebug()<<ui->send_time->value();
}

//定时发送回调函数
void serial_tool::loop_send_callback()
{
    qDebug()<<ui->send_time->value();
    send_data();
}



QString serial_tool::ByteArrayToHexString(QByteArray data)
{
    QString ret(data.toHex().toUpper());
    int len = ret.length()/2;
    for(int i=1;i<=len;i++)
    {
        ret.insert(2*i+i-1," ");
    }
    return ret;
}

QByteArray serial_tool::HexStringToByteArray(QString HexString)
{
    bool ok;
    QByteArray ret;
    HexString = HexString.trimmed();
    HexString = HexString.simplified();
    QStringList sl = HexString.split(" ");

    foreach (QString s, sl) {
        if(!s.isEmpty())
        {
            char c = s.toInt(&ok,16)&0xFF;
            if(ok){
                ret.append(c);
            }else{
                ret.clear();
                //ret = NULL;
                qDebug()<<"非法的16进制字符："<<s;
                return ret;

               // QMessageBox::warning(0,tr("错误："),QString("非法的16进制字符: \"%1\"").arg(s));
            }
        }
    }
   // qDebug()<<ret;
    return ret;
}

void serial_tool::sendDataToBus(cmdTypedef cmd, funsionType fun, float val){
    #define BUF_LEN	50
    char buf[BUF_LEN];
    memset(buf,0,BUF_LEN);
    int cnt = 0;
    buf[cnt++] = 0xFF;
    buf[cnt++] = 0xFF;              	//头

    buf[cnt++] = 0x01;              	//ID
    buf[cnt++] = 0x00;              	//长度

    buf[cnt++] = (uint8_t)cmd;			//属性
    buf[cnt++] = (uint8_t)fun;			//功能

    QByteArray outputdata;

    outputdata.resize(sizeof(val));    //重新调整QByteArray的大小
    memcpy(outputdata.data(),&val,sizeof(val));
    buf[cnt++] = outputdata[0];        	//数据
    buf[cnt++] = outputdata[1];
    buf[cnt++] = outputdata[2];
    buf[cnt++] = outputdata[3];

    buf[3] = cnt - 3;

    uint8_t sum = 0;
    for (int i = 0; i < cnt; i++)		//计算校验
        sum += buf[i];

    buf[cnt++] = sum;

    serial.write(buf,cnt);
//    QString s;
//    s.sprintf("data 0x%X ,0x%X ,0x%X ,0x%X ,0x%X ,0x%X ,0x%X ,0x%X ,0x%X ,0x%X ,0x%X,0x%X \r\n",\
//              buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7],buf[8],buf[9],buf[10]);
//    qDebug()<<s;
}


void serial_tool::on_dial_valueChanged(int value)
{
    static int lastVal = 0;
    static int encVal = 0;

    static int trunNum = 0;
    if(lastVal - value > 180){
        trunNum++;
    }else if(lastVal - value < -180){
        trunNum--;
    }
    lastVal = value;
    encVal = (value + 360*trunNum)*(-1);

    sendDataToBus(WRITE_CMD,POS,encVal);

    ui->posLineEdit->setText(QString::number(encVal));
}

void serial_tool::on_speedHorizontalSlider_valueChanged(int value)
{
    qDebug()<<value;
}
