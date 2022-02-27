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
private slots:
    void on_dial_valueChanged(int value);


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
