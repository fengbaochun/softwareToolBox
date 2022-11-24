#ifndef USBHID_H
#define USBHID_H

#include <QObject>
#include "./hidapi-win/include/hidapi.h"

class usbHid : public QObject
{
    Q_OBJECT
public:
    explicit usbHid(QObject *parent = nullptr);
    char *path;

    void viewDevInfo();
    bool write();
    bool read();
signals:

};

#endif // USBHID_H
