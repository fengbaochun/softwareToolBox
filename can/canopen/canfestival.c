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


/***************************************************************************/
UNS8 usbCanReceive(void* inst, Message *m)
{

#ifdef _WIN32
    UNS8 ret;
    VCI_CAN_OBJ vco = {0};
    VCI_ERR_INFO errinfo;
    DWORD dwWaitResult;
    dwWaitResult = WaitForSingleObject(ghMutex, 100);
    if(dwWaitResult == WAIT_OBJECT_0){
        ret = (UNS8)VCI_Receive(VCI_USBCAN2, 0, 0, &vco, 1, 100);
        ReleaseMutex(ghMutex);
    }

    if(ret > 0)
    {
        m->cob_id = vco.ID;
        m->rtr = vco.RemoteFlag;
        m->len = vco.DataLen;
        for (int i = 0 ; i < vco.DataLen ; i++) {
           m->data[i] = vco.Data[i];
        }
    }
    else
    {
        dwWaitResult = WaitForSingleObject(ghMutex, 100);
        if(dwWaitResult == WAIT_OBJECT_0){
            VCI_ReadErrInfo(VCI_USBCAN2, 0, 1,&errinfo);
            ReleaseMutex(ghMutex);
        }

    }
    return ret;
#endif

#ifdef linux
    return 0;
#endif
}

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
        ret = VCI_Transmit(VCI_USBCAN2,0,0,&pSend,1);
        ReleaseMutex(ghMutex);
    }
    if(ret != 0)
    {
        ret = 0;
    }
    else
        ret = 1;

    return (UNS8)ret; 
#endif

#ifdef linux
    return 0;
#endif

}

CAN_HANDLE usbCanOpen(s_BOARD *board)
{

#ifdef _WIN32
    VCI_INIT_CONFIG init_config;
    init_config.AccCode = 0;
    init_config.AccMask = 0xffffffff;  //all pass
    init_config.Filter = 1;  //one filter
    init_config.Mode = 0;    //normel mode
    init_config.Timing0 = 0x03;
    init_config.Timing1 = 0x1c; //baud rate set 125kbps
    VCI_INIT_CONFIG init_config1;
    init_config1.AccCode = 0;
    init_config1.AccMask = 0xffffffff;  //all pass
    init_config1.Filter = 1;  //one filter
    init_config1.Mode = 0;    //normel mode
    init_config1.Timing0 = 0x03;
    init_config1.Timing1 = 0x1c; //baud rate set 125kbps

    if(VCI_OpenDevice(VCI_USBCAN2,0,0) != STATUS_OK)
    {
        return NULL;
    }
    else
    {
        if(VCI_InitCAN(VCI_USBCAN2,0,0,&init_config) != STATUS_OK)
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
    printf("open succeed");
#endif

#ifdef linux
    return NULL;
#endif
}

int usbCanClose(s_BOARD *board)
{

#ifdef _WIN32
    int ret;

    ret = VCI_ResetCAN(VCI_USBCAN2, 0, 0);

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


