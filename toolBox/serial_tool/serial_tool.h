#ifndef SERIAL_TOOL_H
#define SERIAL_TOOL_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QTimer>

namespace Ui {
class serial_tool;
}

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

#define CMD_BASE_ADDR    	0x20	        //命令基地址
typedef enum{
    CMD_BASE = CMD_BASE_ADDR,
    READ_CMD,                           //读
    WRITE_CMD,                          //写
    ANSWER_CMD,                         //应答
    AUTO_REPORT_CMD,                    //主动上报
    CMD_MAX,                            //最大值
}cmdTypedef;


#define FUN_BASE_ADDR		0x30

typedef enum{
    FUNSION_BASE = FUN_BASE_ADDR,

    SPEED,			//速度
    POS,			//位置
    CURRENT,		//力矩

    MAX_ADDR,
}funsionType;

class serial_tool : public QWidget
{
    Q_OBJECT

public:
    explicit serial_tool(QWidget *parent = nullptr);
    ~serial_tool();

    void open_port();           //打开串口
    void show_rev_data();       //显示接收数据
    void clear_send_buf();      //清空发送区
    QByteArray read_data();     //读取数据
    void send_data();           //清空发送区
    void clear_rev_buf();       //清空接收区
    QString ByteArrayToHexString(QByteArray data);
    QByteArray HexStringToByteArray(QString HexString);
    void timed_callback();
    void loop_send_callback();

    void sendDataToBus(cmdTypedef cmd, funsionType fun, float val);       //发送数据（带有协议）
    float encVal;                                                         //旋钮的值（多圈）
    int trunNum;                                                          //圈数
private slots:
    void on_currentHorizontalSlider_valueChanged(int value);

    void on_dial_valueChanged(int value);

    void on_speedHorizontalSlider_valueChanged(int value);

    void on_goZeroPushButton_clicked();

private:
    Ui::serial_tool *ui;
    const QObject *btn[9];    //按键对象缓存

    QSerialPort serial;     //串口对象

    int serial_num = 0;     //串口数量
    QTimer *update_data = new QTimer;    //定时器
    QTimer *timed_sending = new QTimer;    //定时器

    void but_manage();      //按键管理
    void find_portinfo();
};

#endif // SERIAL_TOOL_H
