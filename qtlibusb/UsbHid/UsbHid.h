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
    bool send(unsigned char ep, QByteArray data);
    QByteArray revice(unsigned char ep, int timeOut);
    void run();

    void test();

private:
    void init();
    struct libusb_device_handle *handle = NULL;

    libusb_device **devs, *dev;
    QTimer *tim;
signals:

private slots:
    void revCallBack();

};

#endif // USBHID_H
