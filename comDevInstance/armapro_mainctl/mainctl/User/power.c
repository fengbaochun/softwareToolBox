#include "power.h"
#include "commun.h"


static devPowerCtrl_t* pDevPowers = NULL;

//T0 电源板(301电源板)
static devPowerCtrl_t idhrDevPowers[DEVICE_PWR_MAX] = {
                            /*	                        isUse                             port      pin		    power on        power off      */
	[0] 		= { .name = "MOTOR",        .isUse = true,              .io[CLK]        = {NULL , NULL       },                                             //机械臂（电机、前端所有的模块）
                                                                        .io[CTL]        = {GPIOA, GPIO_PIN_4 },  
                                                                        .io[FEED_BACK]  = {NULL , NULL       },  GPIO_PIN_SET, 	GPIO_PIN_RESET},   
    [1]         = { .name = "EN1    ",      .isUse = true,              .io[CLK]        = {NULL , NULL       },                                             //工控机 && 显示器
                                                                        .io[CTL]        = {GPIOA, GPIO_PIN_7 },  
                                                                        .io[FEED_BACK]  = {NULL , NULL       },  GPIO_PIN_SET, 	GPIO_PIN_RESET},   
    [2]         = { .name = "EN2    ",      .isUse = true,              .io[CLK]        = {NULL , NULL       },                                             //工控机 && 显示器
                                                                        .io[CTL]        = {GPIOB, GPIO_PIN_11},  
                                                                        .io[FEED_BACK]  = {NULL , NULL       },  GPIO_PIN_SET, 	GPIO_PIN_RESET},   
    [3]         = { .name = "EN3    ",      .isUse = true,              .io[CLK]        = {NULL , NULL       },                                             //USB 拓展
                                                                        .io[CTL]        = {GPIOA, GPIO_PIN_6 },  
                                                                        .io[FEED_BACK]  = {NULL , NULL       },  GPIO_PIN_SET, 	GPIO_PIN_RESET},  
    [8]         = { .name = "EN8    ",      .isUse = true,              .io[CLK]        = {NULL , NULL       },                                             //散热风扇
                                                                        .io[CTL]        = {GPIOC, GPIO_PIN_1 },  
                                                                        .io[FEED_BACK]  = {NULL , NULL       },  GPIO_PIN_SET, 	GPIO_PIN_RESET},    
    [7]         = { .name = "EN7    ",      .isUse = true,              .io[CLK]        = {NULL , NULL       },                                             
                                                                        .io[CTL]        = {GPIOC, GPIO_PIN_2 },  
                                                                        .io[FEED_BACK]  = {NULL , NULL       },  GPIO_PIN_SET, 	GPIO_PIN_RESET},     
    [4]         = { .name = "EN4    ",      .isUse = true,              .io[CLK]        = {NULL , NULL       },                                             
                                                                        .io[CTL]        = {GPIOA, GPIO_PIN_3 },  
                                                                        .io[FEED_BACK]  = {NULL , NULL       },  GPIO_PIN_SET, 	GPIO_PIN_RESET},         
    [5]         = { .name = "EN5    ",      .isUse = true,              .io[CLK]        = {NULL , NULL       },                                             
                                                                        .io[CTL]        = {GPIOC, GPIO_PIN_3 },  
                                                                        .io[FEED_BACK]  = {NULL , NULL       },  GPIO_PIN_SET, 	GPIO_PIN_RESET},     
    [6]         = { .name = "EN6    ",      .isUse = true,              .io[CLK]        = {NULL , NULL       },                                             
                                                                        .io[CTL]        = {GPIOA, GPIO_PIN_2 },  
                                                                        .io[FEED_BACK]  = {NULL , NULL       },  GPIO_PIN_SET, 	GPIO_PIN_RESET},    

    [9]         = { .name = "EN9    ",      .isUse = true,              .io[CLK]        = {NULL , NULL       },                                             
                                                                        .io[CTL]        = {GPIOB, GPIO_PIN_0 },  
                                                                        .io[FEED_BACK]  = {NULL , NULL       },  GPIO_PIN_SET, 	GPIO_PIN_RESET},     
    [10]        = { .name = "EN10   ",      .isUse = true,              .io[CLK]        = {NULL , NULL       },                                             
                                                                        .io[CTL]        = {GPIOC, GPIO_PIN_5 },  
                                                                        .io[FEED_BACK]  = {NULL , NULL       },  GPIO_PIN_SET, 	GPIO_PIN_RESET},                                                                                 
    [11]         = { .name = "SH1    ",     .isUse = true,              .io[CLK]        = {NULL , NULL       },                                             
                                                                        .io[CTL]        = {GPIOB, GPIO_PIN_1 },  
                                                                        .io[FEED_BACK]  = {NULL , NULL       },  GPIO_PIN_SET, 	GPIO_PIN_RESET},     
    [12]        = { .name = "SH2   ",      .isUse = true,               .io[CLK]        = {NULL , NULL      },                                             
                                                                        .io[CTL]        = {GPIOB, GPIO_PIN_10},  
                                                                        .io[FEED_BACK]  = {NULL , NULL       },  GPIO_PIN_SET, 	GPIO_PIN_RESET},      

    //继电器 * 2
    [13]        = { .name = "jdq1   ",     .isUse = true,              .io[CLK]        = {NULL , NULL       },                                             
                                                                        .io[CTL]        = {GPIOB, GPIO_PIN_6 },  
                                                                        .io[FEED_BACK]  = {NULL , NULL       },  GPIO_PIN_SET, 	GPIO_PIN_RESET},     
    [14]        = { .name = "jdq2 ",       .isUse = true,              .io[CLK]        = {NULL , NULL       },                                             
                                                                        .io[CTL]        = {GPIOB, GPIO_PIN_7 },  
                                                                        .io[FEED_BACK]  = {NULL , NULL       },  GPIO_PIN_SET, 	GPIO_PIN_RESET},   

    //气阀 * 2
    [15]        = { .name = "qf1    ",     .isUse = true,              .io[CLK]        = {NULL , NULL       },                                             
                                                                        .io[CTL]        = {GPIOC, GPIO_PIN_0 },  
                                                                        .io[FEED_BACK]  = {NULL , NULL       },  GPIO_PIN_SET, 	GPIO_PIN_RESET},     
    [16]        = { .name = "qf2  ",       .isUse = true,              .io[CLK]        = {NULL , NULL       },                                             
                                                                        .io[CTL]        = {GPIOC, GPIO_PIN_4 },  
                                                                        .io[FEED_BACK]  = {NULL , NULL       },  GPIO_PIN_SET, 	GPIO_PIN_RESET},       

    [17]        = { .name = "rift  ",       .isUse = true,              .io[CLK]        = {NULL , NULL       },                                             
                                                                        .io[CTL]        = {GPIOA, GPIO_PIN_5 },  
                                                                        .io[FEED_BACK]  = {NULL , NULL       },  GPIO_PIN_SET, 	GPIO_PIN_RESET},                                                                                                                                                                                                                                                                                                               
};

static __inline devPowerCtrl_t* getDevPowerP(const device_power_dev_enum_t devId){
	return (pDevPowers + devId);
}

//电源控制初始化
void powerCtrlInit(void){
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    pDevPowers = idhrDevPowers;
	GPIO_InitTypeDef outIoCfg = {.Pull = GPIO_NOPULL, .Speed = GPIO_SPEED_FREQ_LOW, .Mode = GPIO_MODE_OUTPUT_PP};
	GPIO_InitTypeDef inputIoCfg = {.Pull = GPIO_PULLDOWN, .Mode = GPIO_MODE_INPUT};
    for(int i = 0; i < DEVICE_PWR_MAX; i++){
		devPowerCtrl_t* pwr = getDevPowerP((device_power_dev_enum_t)i);
        if(pwr->isUse){    
            if(pwr->io[CLK].pin != NULL && pwr->io[CLK].port != NULL){
                outIoCfg.Pin = pwr->io[CLK].pin;                         //时钟引脚初始化(NULL为不使用使能引脚)
                HAL_GPIO_Init(pwr->io[CLK].port, &outIoCfg);
            }                                              

            outIoCfg.Pin = pwr->io[CTL].pin;                             //控制引脚初始化
            HAL_GPIO_Init(pwr->io[CTL].port, &outIoCfg);

            if(pwr->io[FEED_BACK].pin != NULL && pwr->io[FEED_BACK].port != NULL){
                inputIoCfg.Pin = pwr->io[FEED_BACK].pin;                 //反馈引脚初始化
                HAL_GPIO_Init(pwr->io[FEED_BACK].port, &inputIoCfg);            
            }
        }else{
//            DBG_LOG("%-8s power is not used \r\n", pwr->name);
        }
	}
}

//读取当前电源电平(暂时不使用反馈引脚)
bool isDevicePowerOn(const device_power_dev_enum_t devId){
	devPowerCtrl_t *dev = getDevPowerP(devId);
	if((!dev->io[CTL].port) || (!dev->isUse)) return false;
	
	bool ret = (dev->io[FEED_BACK].pin == NULL && dev->io[FEED_BACK].port == NULL) 				 ? 
     			HAL_GPIO_ReadPin(dev->io[CTL].port, dev->io[CTL].pin) == dev->powerOn			 :
				HAL_GPIO_ReadPin(dev->io[FEED_BACK].port, dev->io[FEED_BACK].pin) == dev->powerOn;
    // bool ret = (HAL_GPIO_ReadPin(dev->io[CTL].port, dev->io[CTL].pin) == dev->powerOn);
	return ret;
}

//电源接口
void devicePwrCtrl(const device_power_dev_enum_t devId, bool isPowerOn){
	devPowerCtrl_t *dev = getDevPowerP(devId);
	if((!dev->io[CTL].port) || (!dev->isUse)) return ;
	HAL_GPIO_WritePin(dev->io[CTL].port, dev->io[CTL].pin,							//写入控制引脚电平
							(isPowerOn ? dev->powerOn : dev->powerOff));
    
    if(dev->io[CLK].pin != NULL && dev->io[CLK].port != NULL){                      //根据IO使能情况 开启D触发器
        PWR_CLK_PLUSE(dev->io[CLK].port, dev->io[CLK].pin, PLUSE_TIME);		        //D触发器时钟
    }
	
//    char *status = (isPowerOn == true) ? "on" : "off";
//	DBG_LOG("%-8s devId : %d , power %-3s \r\n", dev->name, devId, status);
}

//关机
void powerOff(void){
	
}

//测试
void powerTest(void){
    static bool ret = true;
    for(int i=0; i<DEVICE_PWR_MAX; i++){
        // devicePwrCtrl(i, ret);
    }
    vTaskDelay(1000);
    ret = !ret;
}
