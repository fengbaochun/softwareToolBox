#include "usbHid.h"

usbHid::usbHid(QObject *parent)
    : QObject{parent}
{
    hid_init();
    tim = new QTimer();
    connect(tim,SIGNAL(timeout()),this,SLOT(revCallBack()));
    tim->setInterval(1);
    tim->start(1);
}

//扫描USB设备
void usbHid::scan(void)
{
    hid_device_info *hid_info;          //usb链表
    hid_info = hid_enumerate(0, 0);
    /*遍历所有信息并打印*/
    for(;hid_info != nullptr;hid_info = hid_info->next){
        qDebug("----------------------------------------------------------------");
        qDebug("设备接口号:%d",hid_info->interface_number);
        qDebug("厂商字符串:%ls",hid_info->manufacturer_string);
        qDebug("设备字符串:%ls",hid_info->product_string);
        qDebug("版本号:%d",hid_info->release_number);
        qDebug("地址:%s",hid_info->path);
    }
    /*释放链表*/
    hid_free_enumeration(hid_info);
}


//打开设备
bool usbHid::open(uint32_t vid, uint32_t pid)
{
    this->handle = hid_open(vid, pid, nullptr);

    if (!this->handle){
        hid_exit();
        qDebug("无法打开USB设备");
        return false;
    }
    if (hid_set_nonblocking(this->handle, 1) != 0){// 将hid_read()函数设置为非阻塞 1启用非阻塞  0禁用非阻塞。
        hid_close(this->handle);
        hid_exit();
//        qDebug("设置非阻塞失败,错误原因:%s", hid_error(this->handle));
        return false;
    }
    return true;
}

//关闭
void usbHid::close()
{
    hid_close(this->handle);
    hid_exit();
}

//写入
bool usbHid::write(uint8_t reportId, uint8_t data[])
{
    uint8_t buf[64 + 1];
    buf[0] = reportId;
    memcpy(buf +1, data, 64);
    int res = hid_write(handle, buf, 64 + 1);
    if(res != 65){
        qDebug("err_string = %ls\n",hid_error(handle));
        return false;
    }
    return true;
}

//读取
bool usbHid::read(uint8_t *data)
{
    int res = hid_read(handle, data, 64);
    if(res != 64){
//        qDebug("err_string = %ls\n",hid_error(handle));
        return false;
    }
    return true;
}

//定时器测试接收回调
void usbHid::revCallBack()
{
    uint8_t data[64];
    if(!read(data)) return;
    qDebug("--------------------------------------------");
    for(int j=0;j<8;j++){
        int i=j*8;
        qDebug("data:0x%2X 0x%2X 0x%2X 0x%2X 0x%2X 0x%2X 0x%2X 0x%2X ",data[0+i],data[1+i],data[2+i],data[3+i],data[4+i],data[5+i],data[6+i],data[7+i]);
    }
}


