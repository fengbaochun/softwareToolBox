#ifndef __UTIL_H__
#define __UTIL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#include "stm32f1xx.h"


#if 1
#include "FreeRTOS.h"
#include "semphr.h"
#include "cmsis_os.h"
#include "task.h"

#else

#endif

typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#define DBG_MCU  1  //主板调试信息

extern void printf_DMA(char *fmt,...);
// #define printf printf_DMA

#if DBG_MCU
	//#define DBG_LOG 	printf
	#define DBG_LOG 	
#else
	#define DBG_LOG(str,...)	{}
#endif


extern uint32_t guwTimeCmp(uint32_t const timePre, uint32_t const timeCurrent);

typedef struct{
	GPIO_TypeDef* port;
	uint16_t pin;
}gpioType;


#ifdef __cplusplus
}
#endif

#endif
