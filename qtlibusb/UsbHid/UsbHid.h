#ifndef USBHID_H
#define USBHID_H


#include <QObject>
#include <QDebug>
#include <QThread>
#include <QString>
#include <QDateTime>
#include <QTimer>
#include "lusb0_usb.h"

class UsbHid : public QThread
{
    Q_OBJECT
public:
    UsbHid(QObject *parent = nullptr);
    bool open(struct usb_device *dev);
    void close();
    bool send(QByteArray &data);
    void run();

private:
    struct usb_device * findUSBDev(const unsigned short idVendor,
                                                const unsigned short idProduct);

    void init();
    usb_dev_handle *devHandle;
    bool devOpenFlg;
    QTimer *tim;
signals:

private slots:
    void revCallBack();

};

#endif // USBHID_H
