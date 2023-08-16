#ifndef USBHID_H
#define USBHID_H


#include <QObject>
#include <QDebug>
#include <QThread>
#include <QString>
#include <QDateTime>
#include <QTimer>
#include "libusb.h"

class UsbHid : public QThread
{
    Q_OBJECT
public:
    UsbHid(QObject *parent = nullptr);
    ~UsbHid();
    bool open(struct usb_device *dev);
    void close();
    bool send(uint8_t epNum, QByteArray data);
    QByteArray revice(uint8_t epNum, int timeOut);

    static void sendCallBack(libusb_transfer *t);
    void asyncWrite(uint8_t epNum, QByteArray d);
//    static void revCallBack(libusb_transfer *t);
    void asyncRead(uint8_t epNum);
    void run();

    void test();



private:
    void init();
    libusb_context *ctx = NULL;
    struct libusb_device_handle *handle = NULL;
    libusb_device **devs, *dev;
    QTimer *tim;

signals:

private slots:
    void revCallBack();

};

#endif // USBHID_H
