#ifndef FIRMWAREUPDATE_H
#define FIRMWAREUPDATE_H

#include <QObject>
#include <QString>

typedef struct{
    uint16_t size;				//固件大小
    uint16_t perPkgSize;		//每包大小

    uint16_t pkgNum;            //固件包数
    uint16_t lastPkgSize;		//余包字节数
    uint16_t pkgIndex;			//大包索引
    uint16_t pIndex;			//小包索引
    QString path;
}binInfo;

typedef struct{
    bool enterBootStatus;
    bool binSizeStatus;
    bool binDataStatus;
    uint8_t answerPkgNum;
}iapStatus;                     //升级状态

class firmwareUpdate : public QObject
{
    Q_OBJECT
public:
    explicit firmwareUpdate(QObject *parent = nullptr);
    binInfo data;
    iapStatus iap;
    void getBinInfo(QString path);                          //获取bin信息
    bool enterBootloader();                                 //进入boot
    bool sendBinInfo();                                     //发送固件信息
    bool startUpdate();                                     //开始更新

//    static void messageCallBack(QByteArray buf);            //报文回调

signals:

public slots:
    void messageCallBack(QByteArray buf);                   //报文回调

};

#endif // FIRMWAREUPDATE_H
