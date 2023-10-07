#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32g4xx_hal.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define QUE_PKG_MAXNUM	10          //包数
#define QUE_PKG_MAXLEN	64          //单包长度


//环形队列结构体
typedef struct{
	int head;		//队列头
	int tail;		//队列尾
	int	size;	    
	uint8_t  data[QUE_PKG_MAXNUM][QUE_PKG_MAXLEN];	        //数据
	uint16_t len[QUE_PKG_MAXNUM];							//单包数据长度
}xRingQueueTypedef;

void xRingQueueInit(xRingQueueTypedef *q);
bool xRingQueueIsFull(xRingQueueTypedef *q);
bool xRingQueueIsEmpty(xRingQueueTypedef *q);
bool xRingQueueWrite(xRingQueueTypedef *q, uint8_t *data, uint16_t len);
uint16_t xRingQueueRead(xRingQueueTypedef *q, uint8_t *data);

extern xRingQueueTypedef usartQueue;

#ifdef __cplusplus
}
#endif
#endif 

