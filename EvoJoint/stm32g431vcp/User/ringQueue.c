#include "ringQueue.h"

//串口接收队列
xRingQueueTypedef usartQueue;

//初始化一个队列
void xRingQueueInit(xRingQueueTypedef *q){	
	q->head = 0;
	q->tail = 0;
    q->size = 0;
	memset(&q->data[0][0],0,QUE_PKG_MAXLEN*QUE_PKG_MAXNUM);
}

//xRingQueueInit(&usartQueue);

//队列是否已满
bool xRingQueueIsFull(xRingQueueTypedef *q){
    return (q->size == QUE_PKG_MAXNUM) ? true : false;
}

//队列是否已空
bool xRingQueueIsEmpty(xRingQueueTypedef *q){
    return (q->size == 0) ? true : false;
}

//写数据到队列
bool xRingQueueWrite(xRingQueueTypedef *q, uint8_t *data, uint16_t len){
	if(xRingQueueIsFull(q)) return false;
    __disable_irq();
    q->size++;	
    memcpy(&q->data[q->tail][0],&data[0],len);
    if(len > QUE_PKG_MAXLEN){                                   //实际写入大于宏定义情况 采取截取
        len = QUE_PKG_MAXLEN;
    }
    q->len[q->tail] = len;                                      //记录单包长度
    q->tail = (q->tail+1) % QUE_PKG_MAXNUM;                     //尾索引更新
    __enable_irq();
    return true;
}

//从队列读数据
uint16_t xRingQueueRead(xRingQueueTypedef *q, uint8_t *data){
	uint16_t len;
	if(xRingQueueIsEmpty(q))    return 0;
    __disable_irq();
    q->size--;
    len = q->len[q->head];	                                    //获取当前包长度
    memcpy(&data[0],&q->data[q->head][0],len);        
    q->head = (q->head+1) % QUE_PKG_MAXNUM;                     //头索引更新
    __enable_irq();
    return len;
}


