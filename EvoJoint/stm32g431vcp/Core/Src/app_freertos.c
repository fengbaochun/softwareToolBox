/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_cdc_if.h"
#include "commun.h"
#include "ringQueue.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  usartCommunInit();
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
//#define USB_MAX_RECEIVE_LEN 4096
//uint8_t rxData[USB_MAX_RECEIVE_LEN] = {0};
uint32_t rxLen = 0;
#include "mpu6050.h"
/* USER CODE END Header_StartDefaultTask */


void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
//	uint8_t buf[60] = { 0xFF,0xFF,0x07,0x23,0x31,0x00,0x00,0x9F,0x02,0xFC,0xFF,0xFF,0x07,0x23,0x31,0x00,0x00,0x9F,0x02,0xFC,
//						0xFF,0xFF,0x07,0x23,0x31,0x00,0x00,0x9F,0x02,0xFC,0xFF,0xFF,0x07,0x23,0x31,0x00,0x00,0x9F,0x02,0xFC,
//						0xFF,0xFF,0x07,0x23,0x31,0x00,0x00,0x9F,0x02,0xFC,0xFF,0xFF,0x07,0x23,0x31,0x00,0x00,0x9F,0x02,0xFC	};
////	xRingQueueInit(&usartQueue);
	while(1){
		
		vTaskDelay(1);
	}
	
	MPU_Init();
	mpu_dmp_init();
	for(;;)
	{	
//		uint8_t buf[25];
//		sprintf((char *)buf,"size = %d \r\n",usartQueue.size);
//		CDC_Transmit_FS((uint8_t *)buf, strlen((char *)buf));
//		
//		uint8_t data[QUE_PKG_MAXLEN];
//		int len = xRingQueueRead(&usartQueue, data);
//		if(len > 0){
//			CDC_Transmit_FS(data, len);//通过usb虚拟串口发送回去
//		}

		mpu_dmp_get_data(&imu.pitch, &imu.roll, &imu.yaw);
//		MPU_Get_Gyroscope(&imu.gy.x, &imu.gy.y, &imu.gy.z);
		short x,y,z;
		MPU_Get_Accelerometer(&x, &y, &z);
		imu.aa.x = x;
		imu.aa.y = y;
		imu.aa.z = z;
//		
//		imu.gy.x = (float)imu.gy.x * mpu6050_get_gyro_res(3);
//		imu.gy.y = (float)imu.gy.y * mpu6050_get_gyro_res(3);
//		imu.gy.z = (float)imu.gy.z * mpu6050_get_gyro_res(3);

		//加速度计转换到米每平方秒
		imu.aa.x = (float)imu.aa.x * mpu6050_get_accel_res(0) * 9.80f * 1.0f; 
		imu.aa.y = (float)imu.aa.y * mpu6050_get_accel_res(0) * 9.80f * 1.0f;
		imu.aa.z = (float)imu.aa.z * mpu6050_get_accel_res(0) * 9.80f * 1.0f;
	
		dmpGetGravity(&imu.gravity, &imu.q);
		
		dmpGetLinearAccel(&imu.aaReal, &imu.aa, &imu.gravity);
		
		dmpGetLinearAccelInWord(&imu.aaWorld, &imu.aaReal, &imu.q);
		
		imu.pos.x += 0.5f * imu.aaWorld.x * 0.005f*0.005f * 1000;
		imu.pos.y += 0.5f * imu.aaWorld.y * 0.005f*0.005f * 1000;
		
		osDelay(5);
	}
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

