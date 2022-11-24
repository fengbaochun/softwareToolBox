#include "usbHid.h"

usbHid::usbHid(QObject *parent)
    : QObject{parent}
{
}

void usbHid::viewDevInfo()
{
    hid_device_info *hid_info;//usb链表
    /*打开指定VID PID设备*/
    hid_info = hid_enumerate(1155,22352);
    //hid_info = hid_enumerate(0, 0);
    /*遍历所有信息并打印*/
    for(;hid_info != nullptr;hid_info = hid_info->next){
        qDebug("设备接口号:%d",hid_info->interface_number);
        qDebug("厂商字符串:%ls",hid_info->manufacturer_string);
        qDebug("设备字符串:%ls",hid_info->product_string);
        qDebug("版本号:%d",hid_info->release_number);
        qDebug("地址:%s",hid_info->path);
    }
    /*释放链表*/
    hid_free_enumeration(hid_info);
}

bool usbHid::write()
{
    #define	TX_MAXSIZE   64
    int res;
    uint8_t buf[TX_MAXSIZE + 1];
    hid_device *handle;//usb句柄
    hid_device_info *hid_info;//usb链表

    /*打开指定VID PID设备*/
    hid_info = hid_enumerate(1155,22352);
    for(;hid_info != nullptr;hid_info = hid_info->next){
        if(hid_info->interface_number == -1)//接口匹配
        {
            qDebug("interface_number:%s",hid_info->path);//打印地址
            break;
        }
    }
    /*打开指定地址的设备*/
    handle = hid_open_path(hid_info->path);
    /*释放链表*/
    hid_free_enumeration(hid_info);
    buf[0] = 0x09;//Report ID（自定义）
    buf[1] = 0x11;//随便一点东西
    buf[2] = 0x11;
    buf[3] = 0x11;
    buf[4] = 0x11;
    buf[5] = 0x11;
    for(int i=1;i<65;i++){
        buf[i] = i*2;
    }
    res = hid_write(handle, buf, TX_MAXSIZE + 1);
    if(res < 0){
        /*返回值查看*/
        qDebug("err_string = %ls\n",hid_error(handle));
    }

    return 0;
}

bool usbHid::read()
{
    int res;
    uint8_t buf[64+1];
    hid_device *handle;//usb句柄
    hid_device_info *hid_info;//usb链表

    /*打开指定VID PID设备*/
    hid_info = hid_enumerate(1155,22352);
    for(;hid_info != nullptr;hid_info = hid_info->next){
        if(hid_info->interface_number == -1)//接口匹配
        {
            qDebug("interface_number:%s",hid_info->path);//打印地址
            break;
        }
    }
    /*打开指定地址的设备*/
    handle = hid_open_path(hid_info->path);
    /*释放链表*/
    hid_free_enumeration(hid_info);

    res = hid_read(handle, buf, 64+1);
    if(res < 0){
        /*返回值查看*/
        qDebug("err_string = %ls\n",hid_error(handle));
    }

    /*阻塞*/
    hid_set_nonblocking(handle, 0);
    for(int i = 0;i<64;i++){
        qDebug("buf[%d]:%02x",i,buf[i]);
    }
    return 0;
}

