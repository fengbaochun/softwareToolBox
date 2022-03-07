#include "candev.h"

canDev::canDev()
{
    qRegisterMetaType<VCI_CAN_OBJ>("VCI_CAN_OBJ");
    qRegisterMetaType<DWORD>("DWORD");
    nCANInd = 1;    //通道选择
    connect(this, SIGNAL(reveiced(VCI_CAN_OBJ)), this, SLOT(revDataSlot(VCI_CAN_OBJ)));
    force.val = 0;
}

canDev::~canDev(){
}


bool canDev::getHardwareInfo()
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

VCI_INIT_CONFIG canDev::paramConfig(){
    VCI_INIT_CONFIG param;
    param.AccCode=0x00000000;
    param.AccMask=0xFFFFFFFF;
    param.Filter=1;
    param.Mode=0;
    param.Timing0=0x00;
    param.Timing1=0x14;//1000K
    return param;
}

bool canDev::open()
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


void canDev::sendData(uint8_t id,int len, uint8_t *data)
{
    VCI_CAN_OBJ buf;
    buf.ID =id ;
    buf.RemoteFlag = 0;
    buf.ExternFlag = 0;
    buf.DataLen = len;
    memcpy(buf.Data,data,len);
    if(VCI_Transmit(VCI_USBCAN2, 0, nCANInd, &buf,1) == STATUS_OK){
        qDebug()<<"send succeed!!!";
    }else{
        qDebug()<<"send failed!!!";
    }
}

void canDev::revDataSlot(VCI_CAN_OBJ buf)
{
//    char tempBuf[100];
//    sprintf(tempBuf,"0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x",\
//            buf.Data[0],buf.Data[1],buf.Data[2],buf.Data[3],\
//            buf.Data[4],buf.Data[5],buf.Data[6],buf.Data[7]);

//    qDebug()<<"id    :"<<buf.ID;
//    qDebug()<<"len   :"<<buf.DataLen;
//    qDebug()<<"data  :"<<tempBuf;
//    qDebug()<<"------------------------------------------";

    memcpy(force.buf[buf.Data[0]],buf.Data,8);                                      //copy到对应属性的buf中
    force.val = force.buf[AUTO_REPORT_CMD][2]<<8 | force.buf[AUTO_REPORT_CMD][3];   //合成压力
//    qDebug()<< force.val;
}

void canDev::run()
{
    int frameNum = 0;
    VCI_CAN_OBJ buf;
    open();
    while (true)
    {
        frameNum = VCI_GetReceiveNum(VCI_USBCAN2,0,nCANInd);
        if(frameNum){
            VCI_Receive(VCI_USBCAN2, 0, nCANInd, &buf,1,0);
            emit reveiced(buf);
        }
    }
}

void canDev::rest(){
    if(VCI_ResetCAN(VCI_USBCAN2, 0, nCANInd) == STATUS_OK){
        qDebug()<<"rest dev succeed";
    }else{
        qDebug()<<"rest dev failed";
    }
}

void canDev::close(){
    VCI_CloseDevice(VCI_USBCAN2, 0);
}






