#ifndef USBHID_H
#define USBHID_H

#include <QObject>
#include "./hidapi-win/include/hidapi.h"
#include <QTimer>

class usbHid : public QObject
{
    Q_OBJECT
public:
    explicit usbHid(QObject *parent = nullptr);

    void scan(void);
    bool open(uint32_t vid, uint32_t pid);
    void close();
    bool write(uint8_t reportId, uint8_t data[64]);
    bool read(uint8_t *data);


private:
    hid_device *handle;         //usb句柄
    QTimer *tim;
signals:

private slots:
    void revCallBack();
};

#endif // USBHID_H
