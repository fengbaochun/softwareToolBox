/****************************************************************************
**
** Copyright (C) 2016 pkzju
**
**
** Version	: 0.1.1.0
** Author	: pkzju
** Website	: https://github.com/pkzju
** Project	: https://github.com/pkzju/QSuperConsole
** 
****************************************************************************/

#ifndef __canfestival_h__
#define __canfestival_h__
#ifdef __cplusplus
extern "C" {
#endif
#include "ControlCAN.H"
#include "can_driver.h"
#include "timers_driver.h"
#include "timerscfg.h"
#include "data.h"


/* CAN port structure */
typedef struct
canpport{
  char used;  /**< flag indicating CAN port usage, will be used to abort Receiver task*/
  CAN_HANDLE fd; /**< CAN port file descriptor*/
  TASK_HANDLE receiveTask; /**< CAN Receiver task*/
  CO_Data* d; /**< CAN object data*/
}CANPort;


UNS8 usbCanReceive(void* inst, Message *m);
UNS8 usbCanSend(void* inst, const Message *m);

CAN_HANDLE usbCanOpen(s_BOARD *board);
//int usbCanClose(s_BOARD *board);
//UNS8 usbCanChangeBaudrate(void* fd, char* baud);

UNS8 canSend(CAN_PORT port, Message *m);
CAN_PORT canOpen(s_BOARD *board, CO_Data * d);
int canClose(CO_Data * d);
UNS8 canChangeBaudRate(CAN_PORT port, char* baud);

#ifdef __cplusplus
}
#endif


#endif /* __canfestival_h__ */
