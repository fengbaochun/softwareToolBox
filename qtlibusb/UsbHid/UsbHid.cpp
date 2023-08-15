#include "UsbHid.h"

#define VID 0x1111
#define PID 0x2222

#define edp1in 0x81
#define edp1out 0x01

#define edp2in 0x82
#define edp2out 0x02

UsbHid::UsbHid(QObject *parent)
{

    tim = new QTimer();
    connect(tim,SIGNAL(timeout()),this,SLOT(revCallBack()));
    init();
}


bool UsbHid::open(struct usb_device *dev)
{
    return true;
}

void UsbHid::close()
{

}

bool UsbHid::send(unsigned char ep, QByteArray data)
{
    int actual_len=0;
    int ret = libusb_bulk_transfer(handle, ep, (unsigned char *)data.data(), data.length(), &actual_len, 100);
    if (actual_len != 0) {
        qDebug()<<"ep "<<ep<<"send len: "<<actual_len<<" data:"<<data;
    }
    return false;
}

QByteArray UsbHid::revice(unsigned char ep, int timeOut)
{
    char data[64];
    int actual_len = 0;
    memset(data,0,64);
    int ret = libusb_bulk_transfer(handle, 0x80|ep, (unsigned char *)data, 64, &actual_len, 100);

    QByteArray b;
    if (actual_len == 0) {
        //qDebug()<<"received nothing";
    } else {
        qDebug()<<"ep "<<ep<<"rev  len: "<<actual_len<<" data:"<<data;
    }

    return b;
}

void UsbHid::init()
{
    int ret, i;
    ssize_t cnt;
    libusb_context *ctx = NULL;

    ret = libusb_init(&ctx);
    if (ret < 0)
        return ;

    libusb_set_debug(ctx, 3);

    cnt = libusb_get_device_list(NULL, &devs);
    if (cnt < 0) {
        qDebug()<<"no usb dev on bus\r\n";
        return  ;
    }

    i = 0;
    while((dev = devs[i++]) != NULL) {
        struct libusb_device_descriptor desc;
        ret = libusb_get_device_descriptor(dev,&desc);
        if (ret < 0) {
            printf("failed to get device descriptor");
            return ;
        }

        if ((desc.idVendor == VID) && (desc.idProduct == PID)) {
            qDebug()<<"bLength: "<<  desc.bLength;
            qDebug()<<"bDescriptorType:"<<  desc.bDescriptorType;
            qDebug()<<"bcdUSB: "<<  desc.bcdUSB;
            qDebug()<<"bDeviceClass: "<<  desc.bDeviceClass;
            qDebug()<<"bDeviceSubClass: "<<  desc.bDeviceSubClass;
            qDebug()<<"bDeviceProtocol: "<<  desc.bDeviceProtocol;
            qDebug()<<"bMaxPacketSize0: "<<  desc.bMaxPacketSize0;
            qDebug()<<"vendor id: " << desc.idVendor;
            qDebug()<<"product id: "<< desc.idProduct;
            qDebug()<<"bcdDevice: "<< desc.bcdDevice;
            qDebug()<<"iManufacturer: "<< desc.iManufacturer;
            qDebug()<<"iProduct: "<< desc.iProduct;
            qDebug()<<"iSerialNumber: "<<desc.iSerialNumber;
            qDebug()<<"bNumConfigurations: "<< desc.bNumConfigurations;

        }

    }

    handle = libusb_open_device_with_vid_pid(ctx, VID, PID);

    if (handle == NULL) {
        qDebug()<<"cant't open device\r\n";
        return ;
    } else {
        qDebug()<<"open device\r\n";
    }

    libusb_free_device_list(devs, 1);

    if (libusb_kernel_driver_active(handle, 0) ==1) {
        qDebug()<<"kernel driver active, detach it \r\n";

        if (libusb_detach_kernel_driver(handle, 0) == 0) {
            qDebug()<<"detached kernel driver\r\n";
        }
        else {
            qDebug()<<"detached kernel driver false\r\n";
            return ;
        }
    }

    ret = libusb_claim_interface(handle, 0);
    if (ret < 0) {
        qDebug()<<"can't claim interface\r\n";
        return ;
    } else {
        qDebug()<<"claimed interface\r\n";
    }
    test();
    tim->start(500);
}

void UsbHid::run()
{
    qDebug()<<"start listen rev:";
    while(1)
    {
        revice(0x02, 1);
        revice(0x01, 1);
    }

}

void UsbHid::test()
{

    int pkgNum = 100;           //包数量
    int byteNum = pkgNum*64;        //一共通信字节数
    qDebug() << "发送包个数:" << pkgNum<< ";每包：64Byte";
    qDebug() << "接收包个数:" << pkgNum<< ";每包：64Byte";
    char data[64];
    int actual_len = 0;
    qDebug()<<"start test";
    QTime timer;
    timer.start();
    for (int i = 0; i< pkgNum; i++) {

        //ep1
        memset(data, 0, sizeof(data));
        int ret = libusb_bulk_transfer(handle, edp1in, (unsigned char *)data, 64, &actual_len, 100);

        if (actual_len == 0) {
            qDebug()<<"received nothing";
        } else {
            qDebug()<<"ep1 rev  len: "<< actual_len<<" data:"<<data;
        }

        memset(data, 0, sizeof(data));

        char *str = "am host";

        sprintf(data, "%d am fghsedfhgsdaxcvbfgsdfgdsfgdfsgdfsgdfgdsfgfsd host %d\r\n", i,i);

        ret = libusb_bulk_transfer(handle, edp1out, (unsigned char *)data, /*64*/strlen(data), &actual_len, 100);

        if (actual_len != 0) {
            qDebug()<<"ep1 send len: "<< actual_len<<" data:"<<data;
        }
//        usleep(500);
    }

    float useTime = timer.elapsed();
    qDebug() << "耗时为: " << useTime << "ms";
    qDebug() << "速度：" << (float)(byteNum)/useTime<< "byte/ms ";

    qDebug()<<"test end!";

}

void UsbHid::revCallBack()
{
    QByteArray a = "123456789asdfafaf";
//    send(a);
}

UsbHid::~UsbHid()
{
    libusb_close(handle);
    libusb_free_device_list(devs, 1);
    libusb_exit(NULL);
}
