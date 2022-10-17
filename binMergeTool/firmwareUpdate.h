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


typedef enum{
    CMD_BASE = 0x40,
    READ_CMD,                           //读
    WRITE_CMD,                          //写
    ANSWER_CMD,                         //应答
    AUTO_REPORT_CMD,                    //主动上报
}cmdType;

typedef enum{
    FUNSION_BASE = 0x50,   //功能码基地址
    ID_ADDR,                            //ID
    CAL_ZERO_ADDR,                      //标零
    CAL_VAL_ADDR,                       //标重
    PEEL_ADDR,                          //去皮
    CANCLE_PEEL_ADDR,                   //取消去皮
    FPS_ADDR,                           //速度设置
    WEIGHT_ADDR,                        //读重量
    REPORT_ENABLE_ADDR,                 //自动上报地址
    VERSION,							//版本查询地址

    UPDATE_BIN_ADDR,					//固件升级地址
    BIN_SIZE_ADDR,						//固件大小地址
    BIN_DATA_ADDR,						//固件数据应答地址

    SAVE_ADDR,							//保存参数

    ALL_ADDR,                           //读所有（重量、帧率）
}funsionType;

class firmwareUpdate : public QObject
{
    Q_OBJECT
public:
    explicit firmwareUpdate(QObject *parent = nullptr);
    binInfo data;
    iapStatus iap;
    void init(int type);                                    //初始化（通信方式？）
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
