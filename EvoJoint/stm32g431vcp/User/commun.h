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

//应用报文结构
enum{
    CM_H1,
    CM_H2,
    CM_LEN,
    CM_CMD,
    CM_FUN,
    CM_DATA
};

//原生报文结构
typedef struct{
	uint32_t id;
	uint8_t idType;
	uint8_t frameType;
	uint8_t len;
	uint8_t data[8];
	uint8_t res;			
}usartDataStruct;

//命令基地址
#define CMD_BASE_ADDR    	    0x20	   
typedef enum{
    READ_CMD = CMD_BASE_ADDR,          
    WRITE_CMD,                         
    ANSWER_CMD,                        
    AUTO_REPORT_CMD,                   

    ERROR_CMD,                         
    CMD_MAX,                           
}cmdTypedef;


#define FUN_BASE_ADDR			0x30	//参数表的基地址
typedef enum{
    CTL_MODE    =   FUN_BASE_ADDR,      //控制模式
    TAR_POS,		                    //位置
    CUR_POS,
	POS_RANGE,

    TAR_SPEED,                      	//速度
    CUR_SPEED,
	SPEED_RANGE,

    TAR_CURRENT,                    	//电流
    CUR_CURRENT,
	CURRENT_RANGE,

    POLE_PAIRS,                     	//极对数
    PHASE_RES,							//相电阻
    PHASE_IND,							//相电感

    CURRENT_P,							//电流环参数
    CURRENT_I,
    CURRENT_D,
    
    SPEED_P,							//速度环参数
    SPEED_I,
    SPEED_D,
    
    POS_P,								//位置环参数
    POS_I,
    POS_D,
    
    REPORT_FPS,                         //上报帧率
    CLEAR_ERROR,                    	//清除错误
    CLEAR_PARAM,                    	//清除参数
    SAVE_PARAM,                     	//保存参数
    SYNC_PARAM,                     	//参数同步

    MAX_ADDR

}funsionType;

typedef enum{
    CAL_MODE,				//标定模式
    FREE_MODE,				//空闲模式 (空闲模式可以切换到其他模式)

    CURRENT_MODE,			//电流模式
    SPEED_MODE,				//速度模式
    POS_MODE,				//位置模式

    TRAJ_POS_MODE,			//轨迹位置模式

    FAULT_MODE,				//故障模式
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
