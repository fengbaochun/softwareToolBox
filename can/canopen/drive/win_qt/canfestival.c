/*
 CAN driver interface.
*/

#ifdef _WIN32
#include <windows.h>
#include <stdio.h>

#pragma comment(lib,"ControlCAN.lib")
#endif

#include "canfestival.h"
#include "timer.h"
#include "timers_driver.h"

#define MAX_NB_CAN_PORTS 16

CANPort canports[MAX_NB_CAN_PORTS] = {{0,},{0,},{0,},{0,},{0,},{0,},{0,},{0,},{0,},{0,},{0,},{0,},{0,},{0,},{0,},{0,}};

#ifdef _WIN32
HANDLE ghMutex;
#endif

int nCANInd = 1;

/***************************************************************************/
//usb can总线接收
UNS8 usbCanReceive(void* inst, Message *m)
{
#ifdef _WIN32
    VCI_CAN_OBJ buf = {0};
    WaitForSingleObject(ghMutex, 100);                                  //加锁
    int frameNum = VCI_GetReceiveNum(VCI_USBCAN2,0,nCANInd);            //获取总线帧数
    if(frameNum){
        VCI_Receive(VCI_USBCAN2, 0, nCANInd, &buf, 1, 0);               //获取报文
        m->cob_id = buf.ID;
        m->rtr = buf.RemoteFlag;
        m->len = buf.DataLen;
        for (int i = 0 ; i < buf.DataLen ; i++) {
           m->data[i] = buf.Data[i];
        }
    }
    ReleaseMutex(ghMutex);                                              //释放锁
    return frameNum;
#endif

#ifdef linux
    return 0;
#endif
}

//usb can总线发送
UNS8 usbCanSend(void* inst, const Message *m)
{

#ifdef _WIN32
    UNS8 ret;
    VCI_CAN_OBJ pSend;
    pSend.ID = m->cob_id;
    pSend.RemoteFlag = m->rtr;
    pSend.ExternFlag = 0;
    pSend.DataLen = m->len;
    for (int i = 0 ; i < m->len ; i++) {
       pSend.Data[i] = m->data[i];
    }

    DWORD dwWaitResult;
    dwWaitResult = WaitForSingleObject(ghMutex, 100);
    if(dwWaitResult == WAIT_OBJECT_0){
        ret = VCI_Transmit(VCI_USBCAN2,0,nCANInd,&pSend,1);
        ReleaseMutex(ghMutex);
    }
    ret = (ret != 0) ? 0 : 1;
    return (UNS8)ret; 
#endif

#ifdef linux
    return 0;
#endif

}

VCI_INIT_CONFIG paramConfig()
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

CAN_HANDLE usbCanOpen(s_BOARD *board)
{
#ifdef _WIN32
    VCI_INIT_CONFIG init_config = paramConfig();
    VCI_INIT_CONFIG init_config1 = paramConfig();

    if(VCI_OpenDevice(VCI_USBCAN2,0,nCANInd) != STATUS_OK)
    {
        return NULL;
    }
    else
    {
        if(VCI_InitCAN(VCI_USBCAN2,0,nCANInd,&init_config) != STATUS_OK)
        {
            VCI_CloseDevice(VCI_USBCAN2,0);
            return NULL;
        }
        else
        {
            VCI_InitCAN(VCI_USBCAN2,0,1,&init_config1);
            if(VCI_StartCAN(VCI_USBCAN2, 0, 0) == 1)
            {
                VCI_StartCAN(VCI_USBCAN2, 0, 1);
                return (CAN_HANDLE)board;
            }
            else
                return NULL;
        }
    }
    VCI_ClearBuffer(VCI_USBCAN2,0,nCANInd);
    printf("open succeed");
#endif

#ifdef linux
    return NULL;
#endif
}

int usbCanClose(s_BOARD *board)
{

#ifdef _WIN32
    int ret = VCI_ResetCAN(VCI_USBCAN2, 0, 0);
    if(ret)
        return VCI_CloseDevice(VCI_USBCAN2,0);
    else
        return ret;
#endif

#ifdef linux
    return NULL;
#endif
}

UNS8 usbCanChangeBaudrate(void* fd, char* baud)
{

#ifdef _WIN32
    int ret;
    if(baud = ("125"))
        ret = VCI_SetReference(VCI_USBCAN2, 0, 0, 0, (PVOID)0x1C0011);
    return ret;
#endif

#ifdef linux
    return 0;
#endif
}


/***************************************************************************/
UNS8 canSend(CAN_PORT port, Message *m)
{
    if (port && (usbCanSend != NULL))
	{
        return usbCanSend(((CANPort*)port)->fd, m);
	}
	return 1; /* NOT OK */	
}

/***************************************************************************/
int canReceiveLoop(CAN_PORT port)
{
	Message m;
	while(((CANPort*)port)->used)
	{
        if(usbCanReceive(((CANPort*)port)->fd, &m) != 0)
            break;
	}
	return 0;
}

/***************************************************************************/
CAN_PORT canOpen(s_BOARD *board, CO_Data *d)
{
	int i;
	CAN_HANDLE fd0;

#ifdef _WIN32
    if(!ghMutex)
        ghMutex = CreateMutex(
        NULL,              // default security attributes
        FALSE,             // initially not owned
        NULL);
#endif

	if(d->canHandle)
	{
	  canClose(d);
	}

	for(i=0; i < MAX_NB_CAN_PORTS; i++)
	{
		if(!canports[i].used)
		break;
	}

    fd0 = usbCanOpen(board);
	if(fd0)
    {
		canports[i].used = 1;
		canports[i].fd = fd0;
		canports[i].d = d;
		d->canHandle = (CAN_PORT)&canports[i];
		return (CAN_PORT)&canports[i];
	}
	else
	{
		return NULL;
	}
}

/***************************************************************************/
int canClose(CO_Data * d)
{
    int res = 0;
    CANPort* port = (CANPort*)d->canHandle;
    if(port){
        ((CANPort*)d->canHandle)->used = 0;

        res = usbCanClose((port->fd));

        d->canHandle = NULL;
    }

    return res;
}

UNS8 canChangeBaudRate(CAN_PORT port, char* baud)
{
   if(port){
		UNS8 res;

        res = usbCanChangeBaudrate(((CANPort*)port)->fd, baud);
		return res; // OK
	}
	return 1; // NOT OK
}


