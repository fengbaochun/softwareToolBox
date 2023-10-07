#include "ringQueue.h"

//���ڽ��ն���
xRingQueueTypedef usartQueue;

//��ʼ��һ������
void xRingQueueInit(xRingQueueTypedef *q){	
	q->head = 0;
	q->tail = 0;
    q->size = 0;
	memset(&q->data[0][0],0,QUE_PKG_MAXLEN*QUE_PKG_MAXNUM);
}

//xRingQueueInit(&usartQueue);

//�����Ƿ�����
bool xRingQueueIsFull(xRingQueueTypedef *q){
    return (q->size == QUE_PKG_MAXNUM) ? true : false;
}

//�����Ƿ��ѿ�
bool xRingQueueIsEmpty(xRingQueueTypedef *q){
    return (q->size == 0) ? true : false;
}

//д���ݵ�����
bool xRingQueueWrite(xRingQueueTypedef *q, uint8_t *data, uint16_t len){
	if(xRingQueueIsFull(q)) return false;
    __disable_irq();
    q->size++;	
    memcpy(&q->data[q->tail][0],&data[0],len);
    if(len > QUE_PKG_MAXLEN){                                   //ʵ��д����ں궨����� ��ȡ��ȡ
        len = QUE_PKG_MAXLEN;
    }
    q->len[q->tail] = len;                                      //��¼��������
    q->tail = (q->tail+1) % QUE_PKG_MAXNUM;                     //β��������
    __enable_irq();
    return true;
}

//�Ӷ��ж�����
uint16_t xRingQueueRead(xRingQueueTypedef *q, uint8_t *data){
	uint16_t len;
	if(xRingQueueIsEmpty(q))    return 0;
    __disable_irq();
    q->size--;
    len = q->len[q->head];	                                    //��ȡ��ǰ������
    memcpy(&data[0],&q->data[q->head][0],len);        
    q->head = (q->head+1) % QUE_PKG_MAXNUM;                     //ͷ��������
    __enable_irq();
    return len;
}


