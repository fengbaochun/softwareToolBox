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

bool isFinash = false;

//异步写 回调
void UsbHid::sendCallBack(libusb_transfer *t)
{
    if (t->status != LIBUSB_TRANSFER_COMPLETED) {   //传输失败，可以尝试重新提交传输结构
        int rc = libusb_submit_transfer(t);
        if(rc < 0){                                 //判断是否传输成功
            libusb_free_transfer(t);                //释放传输结构
        }
        qCritical()<<"transfer failed,status : "<<t->status;
    }else{
//        qDebug()<<"transfer completed";           //传输完成
        QByteArray s(reinterpret_cast<char*>(t->buffer),t->actual_length);
        qDebug()<<"ep "<<QString::number(t->endpoint,16)<<"send len: "<<t->actual_length<<" data:"<<s.toHex(' ').toUpper();
    }
    libusb_free_transfer(t);                        //释放传输结构
    t = nullptr;
    isFinash = true;
}

//异步写
void UsbHid::asyncWrite(uint8_t epNum, QByteArray d)
{
    isFinash = false;
    libusb_transfer* transfer = libusb_alloc_transfer(0);       //创建异步传输结构
    //填充异步传输结构
    libusb_fill_bulk_transfer(transfer,                                 //传输结构
                              handle,                                   //设备句柄
                              epNum,                                     //地址
                              (unsigned char *)d.data(),                               //要发送的数据
                              static_cast<int>(d.length()),                    //数据长度
                              (libusb_transfer_cb_fn)sendCallBack,      //回调函数(完成、失败、超时都会回调)
                              this,                                     //用户数据，此处可以放任意数据
                              10);                                      //超时时间

    int rc = libusb_submit_transfer(transfer);  //提交传输结构
    if(rc < 0){                                 //判断是否传输成功
        libusb_free_transfer(transfer);         //释放传输结构
    }
    while(!isFinash){
        libusb_handle_events(ctx);
    }
}

void callbackRevc(struct libusb_transfer *t)
{
    if (t->status == LIBUSB_TRANSFER_COMPLETED) {       //数据传输完成
        if(t->actual_length > 0){                       //有数据，打包 qbytearrary
            QByteArray s(reinterpret_cast<char*>(t->buffer),t->actual_length);
            qDebug()<<"ep "<<QString::number(t->endpoint,16)<<"rev  len: "<<t->actual_length<<" data:"<<s.toHex(' ').toUpper();
        }
        int rv = libusb_submit_transfer(t);             //再次提交传输用于接受
        if (rv < 0){
            qDebug()<<"error libusb_submit_transfer : "<< libusb_strerror(libusb_error(rv));
            libusb_cancel_transfer(t);                   //异步取消之前提交传输
        }
    }else if (t->status == LIBUSB_TRANSFER_CANCELLED) {  //取消传输
        libusb_free_transfer(t);                         //释放传输结构
    }else{
    }
}

void UsbHid::asyncRead(uint8_t epNum)
{
//    for(int i = 0;i<100;i++)     //循环100次 提交100次传输结构，类似于提交了100个缓存等待接受，这样不会丢包
    {
        libusb_transfer* transfer = libusb_alloc_transfer(0);               //创建异步传输结构
        unsigned char *buf = new unsigned char[64];                         //开辟接受内存地址
        memset(buf,0,64);       //清空内存
        transfer->actual_length = 0;
        //填充传输结构
        libusb_fill_bulk_transfer(transfer,                                 //传输结构
                                  handle,                                   //设备句柄
                                  0x80|epNum,                               //设备端点，这里是我使用的读数据端点
                                  buf,                                      //数据接受存放缓存
                                  64,                                       //缓存大小
                                  (libusb_transfer_cb_fn)&callbackRevc,     //接收回调，接收完成、超时、失败等状态都会调用第回调
                                  this,                                     //this 用户数据 ，此处可以放任意数据，
                                  0);                                       //0超时时间此处写的是0代表是无限等待超时直到有数据触发。

        //提交传输
        int rc = libusb_submit_transfer(transfer);
//        qDebug()<<"ep "<<2<<"rev  len: "<<transfer->actual_length<<" data:"<<buf;
        if(rc < 0){
            //取消传输
            libusb_cancel_transfer(transfer);
            //释放传输结构
            libusb_free_transfer(transfer);
            transfer = nullptr;
            return;
        }

    }
}




void UsbHid::init()
{
    int ret, i;
    ssize_t cnt;


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
//    test();
//    tim->start(500);
}

void UsbHid::run()
{
    qDebug()<<"start listen rev:";
    static int num = 222;
    while(1)
    {
//        AsyncRead();
        libusb_handle_events(ctx);


//        QString numStr = QString::number(num++);
//        QByteArray qbStr = numStr.toLocal8Bit();
//        QByteArray a = qbStr + "qsx" +qbStr + "fgh" +qbStr+ "fgh" +qbStr+ "fgh" +qbStr ;
//        send(0x01, a);
//        revice(0x01, 100);

//        send(0x02, a);
//        revice(0x02, 100);
//        usleep(1);
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
