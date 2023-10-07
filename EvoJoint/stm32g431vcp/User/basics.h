/*******************************************************************************
*   COPYRIGHT (C) 2018 Aisono, INC. ALL RIGHTS RESERVED.
* ------------------------------------------------------------------------------
*  This software embodies materials and concepts which are proprietary and  
*  confidential to Aisono, Inc.                                         
*  Aisono distributes this software to its customers pursuant to the terms 
*  and conditions of the Software License Agreement contained in the text 
*  file software. This software can only be utilized if all terms and conditions 
*  of the Software License Agreement are accepted. If there are any questions, 
*  concerns, or if the Software License Agreement text file is missing please    
*  contact Aisono for assistance.                                       
* ------------------------------------------------------------------------------
*
*  File: basics.h
*
*  Date: 2022-06-15 10:34:00
*
*  Revision: 0.1
*
*  Author: liwenchao
*
*  Description: programming infrastructure
*
*  Notes:
*					
*******************************************************************************/


#ifndef __BASICS_H__
#define __BASICS_H__

#include "stm32g4xx_hal.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef struct {
	u8  major;
	u8  minor;
	u8  patch;
   char drTag;
}__PACKED version_t;

//软件发布版本号
static version_t softwareVersion = {
	.major = 1,
	.minor = 0,
	.patch = 0,
	.drTag = 'D'
};

//硬件版本号
static version_t hardwareVersion = {
	.major = 0,
	.minor = 0,
	.patch = 2,
	.drTag = 'D'
};

#ifndef PACKED__
#define PACKED__ __attribute__((packed))
#endif

//#include "rtc.h"	// for rtcTimePrint();

#if 1
#define DBG_LOG(format, args...)	\
		do{							\
			printf("DBG: ");		\
			printf(format, ## args);\
		}while(0)

#define WRN_LOG(format, args...)	\
		do{ 						\
			printf("WRN@ ");		\
			printf(format, ## args);\
		}while(0)
	
#define ERR_LOG(format, args...)	\
		do{							\
			printf("ERR$ ");		\
			printf(format, ## args);\
		}while(0)
#else
#define DBG_LOG(format, ...)	{}
#define WRN_LOG(format, ...)	{}
#define ERR_LOG(format, ...)	{}
#endif


//#define MAX(a, b)		(a > b ? a : b)
//#define MIN(a, b)		(a < b ? a : b)
//#define LIMIT(x, a, b)  (MIN(MAX(x, a), b))  


#define BYTE_REVERSE_16(x)                    \
        (u16)(((((u16)(x)) >> 8) & 0xFF)   |  \
               ((((u16)(x)) << 8) & 0xFF00))
                 
#define BYTE_REVERSE_32(x)                          \
        (u32)(((((u32)(x)) >>   24) & 0xFF)      |  \
               ((((u32)(x)) >>   8) & 0xFF00)    |  \
               ((((u32)(x)) <<   8) & 0xFF0000)  |  \
               ((((u32)(x)) << 24) & 0xFF000000))
                
#define BYTE_REVERSE_64(x) 																			   	  \
        (u64)(((((u64)(x)) >> 56) & 0x00000000000000FF) | ((((u64)(x)) >> 40) & 0x000000000000FF00)  | \
             ((((u64)(x)) >> 24) & 0x0000000000FF0000) | ((((u64)(x)) >>  8) & 0x00000000FF000000) 	 	| \
             ((((u64)(x)) <<   8) & 0x000000FF00000000) | ((((u64)(x)) << 24) & 0x0000FF0000000000)   | \
             ((((u64)(x)) << 40) & 0x00FF000000000000) | ((((u64)(x)) << 56) & 0xFF00000000000000))

#define IS_LITTLE_ENDIAN_CPU 1
#ifdef IS_LITTLE_ENDIAN_CPU
	#define BIG_ENDIAN_16(x) BYTE_REVERSE_16(x)
	#define BIG_ENDIAN_32(x) BYTE_REVERSE_32(x)
	#define BIG_ENDIAN_64(x) BYTE_REVERSE_64(x)
	#define LITTLE_ENDIAN_16(x) (x)
	#define LITTLE_ENDIAN_32(x) (x)
	#define LITTLE_ENDIAN_64(x) (x)
#else
	#define BIG_ENDIAN_16(x) (x)
	#define BIG_ENDIAN_32(x) (x)
	#define BIG_ENDIAN_64(x) (x)
	#define LITTLE_ENDIAN_16(x) BYTE_REVERSE_16(x)
	#define LITTLE_ENDIAN_32(x) BYTE_REVERSE_32(x)
	#define LITTLE_ENDIAN_64(x) BYTE_REVERSE_64(x)
#endif

#ifdef __cplusplus
#define CPP_ASMLINKAGE extern "C"
#else
#define CPP_ASMLINKAGE
#endif

#ifndef ASMLINKAGE
	#define ASMLINKAGE CPP_ASMLINKAGE
#endif

static inline int uint32_to_data(uint32_t val, uint8_t *data)
{
    data[0] = *(((uint8_t*)(&val)) + 0);
    data[1] = *(((uint8_t*)(&val)) + 1);
    data[2] = *(((uint8_t*)(&val)) + 2);
    data[3] = *(((uint8_t*)(&val)) + 3);
    return 4;
}

static inline int int32_to_data(int32_t val, uint8_t *data)
{
    data[0] = *(((uint8_t*)(&val)) + 0);
    data[1] = *(((uint8_t*)(&val)) + 1);
    data[2] = *(((uint8_t*)(&val)) + 2);
    data[3] = *(((uint8_t*)(&val)) + 3);
    return 4;
}

static inline int uint16_to_data(uint16_t val, uint8_t *data)
{
    data[0] = *(((uint8_t*)(&val)) + 0);
    data[1] = *(((uint8_t*)(&val)) + 1);
    return 2;
}

static inline int int16_to_data(int16_t val, uint8_t *data)
{
    data[0] = *(((uint8_t*)(&val)) + 0);
    data[1] = *(((uint8_t*)(&val)) + 1);
    return 2;
}

static inline int float_to_data(float val, uint8_t *data)
{
    data[0] = *(((uint8_t*)(&val)) + 0);
    data[1] = *(((uint8_t*)(&val)) + 1);
    data[2] = *(((uint8_t*)(&val)) + 2);
    data[3] = *(((uint8_t*)(&val)) + 3);
    return 4;
}

static inline uint32_t data_to_uint32(uint8_t *data)
{
    uint32_t tmp;
    *(((uint8_t*)(&tmp)) + 0) = data[0];
    *(((uint8_t*)(&tmp)) + 1) = data[1];
    *(((uint8_t*)(&tmp)) + 2) = data[2];
    *(((uint8_t*)(&tmp)) + 3) = data[3];
    return tmp;
}

static inline int32_t data_to_int32(uint8_t *data)
{
    int32_t tmp;
    *(((uint8_t*)(&tmp)) + 0) = data[0];
    *(((uint8_t*)(&tmp)) + 1) = data[1];
    *(((uint8_t*)(&tmp)) + 2) = data[2];
    *(((uint8_t*)(&tmp)) + 3) = data[3];
    return tmp;
}

static inline uint16_t data_to_uint16(uint8_t *data)
{
    uint16_t tmp;
    *(((uint8_t*)(&tmp)) + 0) = data[0];
    *(((uint8_t*)(&tmp)) + 1) = data[1];
    return tmp;
}

static inline int16_t data_to_int16(uint8_t *data)
{
    int16_t tmp;
    *(((uint8_t*)(&tmp)) + 0) = data[0];
    *(((uint8_t*)(&tmp)) + 1) = data[1];
    return tmp;
}

static inline float data_to_float(uint8_t *data)
{
    float tmp_float;
    *(((uint8_t*)(&tmp_float)) + 0) = data[0];
    *(((uint8_t*)(&tmp_float)) + 1) = data[1];
    *(((uint8_t*)(&tmp_float)) + 2) = data[2];
    *(((uint8_t*)(&tmp_float)) + 3) = data[3];
    return tmp_float;
}


extern void delayInit(const u8 mcuClk);
extern void delayUs(const u32 nus);
extern void delayMs(const u32 nms);
extern void delayXms(const u32 nms);
extern u32 timeCmpMs(const u32 timePre, const u32 timeCurrent);

#ifdef __cplusplus
}
#endif

#endif
/************************* COPYRIGHT (C) 2018 Aisono ****** END OF FILE ******/
