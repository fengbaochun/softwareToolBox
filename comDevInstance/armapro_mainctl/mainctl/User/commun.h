#ifndef __COMMUN_H__
#define __COMMUN_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "util.h"


#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

typedef union{
    uint32_t data;
    uint16_t da[2];
    uint16_t d[4];
}u32DataType;


typedef struct{
	uint8_t cmd;			//命令属性
	uint8_t	fun;			//功能码
	uint32_t data;			//data	
	//u32DataType buf;
}messageTypedef;


#define CMD_BASE_ADDR    	    0x20	    //命令基地址
typedef enum{
    READ_CMD = CMD_BASE_ADDR,           //读
    WRITE_CMD,                          //写
    ANSWER_CMD,                         //应答
    AUTO_REPORT_CMD,                    //主动上报
    ERROR_CMD,                          //命令错误
    CMD_MAX,                            //最大值
}cmdTypedef;

#define FUN_BASE_ADDR			0x30	//参数表的基地址
typedef enum{	
    KEY_STATUS     =   FUN_BASE_ADDR,   //开关按键
    E_CODE,                             //故障码（主动上报）
    RFTI_SW,	                        //射频开关（脚踏板、功率电源开关）
    RFTI_ENERY,                         //射频能量
	POWERIO,							//电源
    MAX_ADDR
}funsionType;


void packDatToBus(cmdTypedef cmd, funsionType fun, uint32_t val);
void cmdTask(void *argument);

//关机命令
#define powerOffCmd() packDatToBus(AUTO_REPORT_CMD, KEY_STATUS, 1)

#ifdef __cplusplus
}
#endif

#endif
