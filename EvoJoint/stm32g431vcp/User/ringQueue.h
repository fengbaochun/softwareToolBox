#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32g4xx_hal.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define QUE_PKG_MAXNUM	10          //����
#define QUE_PKG_MAXLEN	64          //��������


//���ζ��нṹ��
typedef struct{
	int head;		//����ͷ
	int tail;		//����β
	int	size;	    
	uint8_t  data[QUE_PKG_MAXNUM][QUE_PKG_MAXLEN];	        //����
	uint16_t len[QUE_PKG_MAXNUM];							//�������ݳ���
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

