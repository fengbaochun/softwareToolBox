#include "can.h"

can::can(QObject *parent)
    : commun{parent}
{
    qRegisterMetaType<VCI_CAN_OBJ>("VCI_CAN_OBJ");
    qRegisterMetaType<DWORD>("DWORD");
    nCANInd = 1;                            //通道选择
    connect(this, SIGNAL(reveiced(VCI_CAN_OBJ)), this, SLOT(revDataSlot(VCI_CAN_OBJ)));
    qDebug()<<"create can object";
}

can *can::instance()
{
    return new can();
}

bool can::open(QString name, int bps)
{
    if(VCI_OpenDevice(VCI_USBCAN2, 0, nCANInd) == STATUS_OK)    {
        qDebug()<<"open succeed";
        getHardwareInfo();
        VCI_ClearBuffer(VCI_USBCAN2, 0, nCANInd);

        VCI_INIT_CONFIG cfg = paramConfig();

        if(VCI_InitCAN(VCI_USBCAN2, 0, nCANInd, &cfg) == STATUS_OK){
            qDebug()<<"init succeed";
        }else{
            return false;
        }

        if(VCI_StartCAN(VCI_USBCAN2, 0, nCANInd) == STATUS_OK){
             qDebug()<<"start"<<nCANInd<<"succeed:";
        }else{
           return false;
        }
    }else{
        qDebug()<<"open failed";
        return false;
    }
    return true;
}

void can::close()
{
    VCI_CloseDevice(VCI_USBCAN2, 0);
}

QByteArray can::getData()
{
    QByteArray data = nullptr;
    VCI_CAN_OBJ buf;
    int frameNum = VCI_GetReceiveNum(VCI_USBCAN2,0,nCANInd);
    if(frameNum){
        VCI_Receive(VCI_USBCAN2, 0, nCANInd, &buf,1,0);
        data.append(buf.ID);                                        //数据 qbytearrary 格式化
        data.append(buf.DataLen);
        for(int i=0; i<buf.DataLen; i++){
            data.append(buf.Data[i]);
        }
    }
    return data;
}

bool can::write(quint32 id, int len, uint8_t *data)
{
    uint8_t bufLen = len - 0;
    if(len > 10)    return false;
    sendData(id, bufLen, data+0);
    return true;
}

bool can::write(QByteArray data)
{
    Q_UNUSED(data);
    return false;
}

void can::rest()
{
    if(VCI_ResetCAN(VCI_USBCAN2, 0, nCANInd) == STATUS_OK){
        qDebug()<<"rest dev succeed";
    }else{
        qDebug()<<"rest dev failed";
    }
}

VCI_INIT_CONFIG can::paramConfig()
{
    VCI_INIT_CONFIG param;
    param.AccCode=0x00000000;
    param.AccMask=0xFFFFFFFF;
    param.Filter=1;
    param.Mode=0;
    param.Timing0=0x00;
    param.Timing1=0x14;//1000K
    return param;
}

bool can::getHardwareInfo()
{
    VCI_BOARD_INFO info;
    DWORD ret;
    ret = VCI_ReadBoardInfo(VCI_USBCAN2, 0, &info);
    if(ret != 1){
        return false;
    }else{
        qDebug()<<"CAN通道数："<<info.can_Num;
        qDebug()<<"硬件版本号:"<<info.hw_Version;
        qDebug()<<"接口库版本号："<<info.in_Version;
        qDebug()<<"中断号"<<info.irq_Num;
        return true;
    }
}

void can::sendData(uint8_t id, int len, uint8_t *data)
{
    VCI_CAN_OBJ buf;
    buf.ID =id ;
    buf.RemoteFlag = 0;
    buf.ExternFlag = 0;
    buf.DataLen = len;
    memcpy(buf.Data,data,len);
    if(VCI_Transmit(VCI_USBCAN2, 0, nCANInd, &buf,1) == STATUS_OK){
//        qDebug()<<"send succeed!!!";
    }else{
        qDebug()<<"send failed!!!";
    }
}

#if 0
void can::run()
{
    int frameNum = 0;
    VCI_CAN_OBJ buf;
    while(true){
        frameNum = VCI_GetReceiveNum(VCI_USBCAN2,0,nCANInd);
        if(frameNum){
            VCI_Receive(VCI_USBCAN2, 0, nCANInd, &buf,1,0);
            emit reveiced(buf);
        }
    }
}
#endif

void can::revDataSlot(VCI_CAN_OBJ buf)
{
    char tempBuf[100];
    sprintf(tempBuf,"0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x",\
            buf.Data[0],buf.Data[1],buf.Data[2],buf.Data[3],\
            buf.Data[4],buf.Data[5],buf.Data[6],buf.Data[7]);

    qDebug()<<"id    :"<<buf.ID;
    qDebug()<<"len   :"<<buf.DataLen;
    qDebug()<<"data  :"<<tempBuf;
    qDebug()<<"------------------------------------------";
}
