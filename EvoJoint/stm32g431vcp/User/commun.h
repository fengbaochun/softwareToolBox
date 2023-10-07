#ifndef __COMMUN_H__
#define __COMMUN_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <math.h>

#include "basics.h"
#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "timers.h"

typedef union{
    uint32_t data;
    uint16_t da[2];
    uint16_t d[4];
}u32DataType;

//Ӧ�ñ��Ľṹ
enum{
    CM_H1,
    CM_H2,
    CM_LEN,
    CM_CMD,
    CM_FUN,
    CM_DATA
};

//ԭ�����Ľṹ
typedef struct{
	uint32_t id;
	uint8_t idType;
	uint8_t frameType;
	uint8_t len;
	uint8_t data[8];
	uint8_t res;			
}usartDataStruct;

//�������ַ
#define CMD_BASE_ADDR    	    0x20	   
typedef enum{
    READ_CMD = CMD_BASE_ADDR,          
    WRITE_CMD,                         
    ANSWER_CMD,                        
    AUTO_REPORT_CMD,                   

    ERROR_CMD,                         
    CMD_MAX,                           
}cmdTypedef;


#define FUN_BASE_ADDR			0x30	//������Ļ���ַ
typedef enum{
    CTL_MODE    =   FUN_BASE_ADDR,      //����ģʽ
    TAR_POS,		                    //λ��
    CUR_POS,
	POS_RANGE,

    TAR_SPEED,                      	//�ٶ�
    CUR_SPEED,
	SPEED_RANGE,

    TAR_CURRENT,                    	//����
    CUR_CURRENT,
	CURRENT_RANGE,

    POLE_PAIRS,                     	//������
    PHASE_RES,							//�����
    PHASE_IND,							//����

    CURRENT_P,							//����������
    CURRENT_I,
    CURRENT_D,
    
    SPEED_P,							//�ٶȻ�����
    SPEED_I,
    SPEED_D,
    
    POS_P,								//λ�û�����
    POS_I,
    POS_D,
    
    REPORT_FPS,                         //�ϱ�֡��
    CLEAR_ERROR,                    	//�������
    CLEAR_PARAM,                    	//�������
    SAVE_PARAM,                     	//�������
    SYNC_PARAM,                     	//����ͬ��

    MAX_ADDR

}funsionType;

typedef enum{
    CAL_MODE,				//�궨ģʽ
    FREE_MODE,				//����ģʽ (����ģʽ�����л�������ģʽ)

    CURRENT_MODE,			//����ģʽ
    SPEED_MODE,				//�ٶ�ģʽ
    POS_MODE,				//λ��ģʽ

    TRAJ_POS_MODE,			//�켣λ��ģʽ

    FAULT_MODE,				//����ģʽ
}ctlModeTypedef;


typedef struct{
	ctlModeTypedef mode;

	float curPos,tarPos;
	float curSpeed,tarSpeed;
	float curIq,tarIq;
	float maxPos,minPos;
	float maxSpeed,minSpeed;
	float maxIq,minIq;

	float posP, posI, posD;
	float speedP, speedI, speedD;
	float currentP, currentI, currentD;

	float temp, busVoltage;

}motorType;

extern motorType motor;

#define BUF_MAX_LEN (64)
#pragma pack(2)
typedef struct{
	uint16_t len;  
	uint8_t data[BUF_MAX_LEN];
}pkgTypedef;
  
void usartCommunInit(void);
void usartTransmit(void* handle, void* id, uint8_t len, uint8_t *data);
bool addUsartMsgToQue(pkgTypedef p);

#ifdef __cplusplus
}
#endif
#endif 
