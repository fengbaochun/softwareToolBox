#ifndef __POWER_H__
#define __POWER_H__

#include "util.h"

#ifdef __cplusplus
extern "C" {
#endif

//触发器时间
#define PLUSE_TIME                      (50)

//电源D触发器时钟
#define PWR_CLK_PLUSE(port, pin, time)	do{														\
											HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);		\
											HAL_Delay(time);     								\
											HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);			\
											HAL_Delay(time);	        						\
											HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);		\
										}while(0)	

enum{
	CLK,				            //时钟
	CTL,				            //电源IO
	FEED_BACK,			            //反馈
};

typedef struct{	
    char *name;                     //名字
    bool isUse;                     //是否被使用
	gpioType io[FEED_BACK + 1];     //电源所用IO
	GPIO_PinState powerOn;          //上电状态
	GPIO_PinState powerOff;         //下电状态

    GPIO_PinState curStatus;        //当前状态
    GPIO_PinState lastStatus;       //上一次状态        （状态改变需要上报，自动检测即可）
}devPowerCtrl_t;


//系统电源控制设备枚举
typedef enum{
	DEVICE_PWR_ARM  = 0,            //机械臂（电机、前端所有的模块）
	DEVICE_PWR_COMPUTER,
	DEVICE_PWR_SCREEN,
	DEVICE_PWR_USB_HUB,
	DEVICE_PWR_FAN,
    DEVICE_PWR_LED,
    DEVICE_PWR_TOUCH,
	DEVICE_PWR_MAX = 13+2+2+1
} device_power_dev_enum_t;

extern void powerCtrlInit(void);
void devicePwrCtrl(const device_power_dev_enum_t devId, bool isPowerOn);
#ifdef __cplusplus
}
#endif

#endif
