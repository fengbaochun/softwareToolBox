#include "key.h"
#include "commun.h"
#include "flexible_button.h"
#include "main.h"

#define ENUM_TO_STR(e) (#e)

typedef enum
{
    USER_BUTTON_1 = 0,
    // USER_BUTTON_2,
    // USER_BUTTON_3,
    // USER_BUTTON_4,
    USER_BUTTON_MAX
} user_button_t;

static char *enum_event_string[] = {
    ENUM_TO_STR(FLEX_BTN_PRESS_DOWN),
    ENUM_TO_STR(FLEX_BTN_PRESS_CLICK),
    ENUM_TO_STR(FLEX_BTN_PRESS_DOUBLE_CLICK),
    ENUM_TO_STR(FLEX_BTN_PRESS_REPEAT_CLICK),
    ENUM_TO_STR(FLEX_BTN_PRESS_SHORT_START),
    ENUM_TO_STR(FLEX_BTN_PRESS_SHORT_UP),
    ENUM_TO_STR(FLEX_BTN_PRESS_LONG_START),
    ENUM_TO_STR(FLEX_BTN_PRESS_LONG_UP),
    ENUM_TO_STR(FLEX_BTN_PRESS_LONG_HOLD),
    ENUM_TO_STR(FLEX_BTN_PRESS_LONG_HOLD_UP),
    ENUM_TO_STR(FLEX_BTN_PRESS_MAX),
    ENUM_TO_STR(FLEX_BTN_PRESS_NONE),
};

static char *enum_btn_id_string[] = {
    ENUM_TO_STR(F1),
    ENUM_TO_STR(F2),
    ENUM_TO_STR(F3),
    ENUM_TO_STR(F4),
    ENUM_TO_STR(USER_BUTTON_MAX),
};

static flex_button_t user_button[USER_BUTTON_MAX];
static void common_btn_evt_cb(void *arg);

static uint8_t common_btn_read(void *arg)
{
    uint8_t value = 0;
    flex_button_t *btn = (flex_button_t *)arg;
    switch (btn->id){
        case USER_BUTTON_1: value = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);    break;
        // case USER_BUTTON_2: value = HAL_GPIO_ReadPin(PORT_KEY2, PIN_KEY2);  break;
        // case USER_BUTTON_3: value = HAL_GPIO_ReadPin(PORT_KEY3, PIN_KEY3);  break;
        // case USER_BUTTON_4: value = HAL_GPIO_ReadPin(PORT_KEY4, PIN_KEY4);  break;
        default:
            break;
    }
    return value;
}

static void user_button_init(void)
{
    int i;
    memset(&user_button[0], 0x0, sizeof(user_button));
    
    for (i = 0; i < USER_BUTTON_MAX; i ++)
    {
        user_button[i].id = i;
        user_button[i].usr_button_read = common_btn_read;
        user_button[i].cb = common_btn_evt_cb;
        user_button[i].pressed_logic_level = 0;
        user_button[i].short_press_start_tick = FLEX_MS_TO_SCAN_CNT(1500);
        user_button[i].long_press_start_tick = FLEX_MS_TO_SCAN_CNT(3000);
        user_button[i].long_hold_start_tick = FLEX_MS_TO_SCAN_CNT(4500);

        flex_button_register(&user_button[i]);
    }
}


//系统开关机 默认关机
bool isSystemPowerOn = false;

//按键事件回调
static void common_btn_evt_cb(void *arg)
{
    flex_button_t *btn = (flex_button_t *)arg;
    //关机
    if((btn->id == USER_BUTTON_1) && (btn->event == FLEX_BTN_PRESS_LONG_START))
    {
        //DBG_LOG("--------- system power off ---------\r\n\r\n");
        isSystemPowerOn = false;
		HAL_GPIO_WritePin(led_GPIO_Port, led_Pin, (GPIO_PinState)1);
		powerOffCmd();
		
		HAL_Delay(1000);
		HAL_Delay(1000);
		HAL_Delay(1000);
		HAL_Delay(1000);
		HAL_Delay(1000);
		//断电
		//power off
		
        //向上位机交互关机事情
        //延时关机。。。
    }
    //开机
    else if ((btn->id == USER_BUTTON_1) && (btn->event == FLEX_BTN_PRESS_CLICK))
    {
        if(isSystemPowerOn) return;        
        //DBG_LOG("--------- system power on ---------\r\n");
        isSystemPowerOn = true;
		HAL_GPIO_WritePin(led_GPIO_Port, led_Pin, (GPIO_PinState)0);
    }
}


//按键扫描任务
void keyTask(void const *argument){
	UNUSED(argument);
	user_button_init();
	HAL_GPIO_WritePin(led_GPIO_Port, led_Pin, (GPIO_PinState)1);
	while(1) {
        flex_button_scan();
		vTaskDelay(10);
	}
}

