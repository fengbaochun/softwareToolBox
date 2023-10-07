#include "commun.h"
#include "usbd_cdc_if.h"

static const unsigned char crc8_table[256] = {
    0x00, 0x5e, 0xbc, 0xe2, 0x61, 0x3f, 0xdd, 0x83,
    0xc2, 0x9c, 0x7e, 0x20, 0xa3, 0xfd, 0x1f, 0x41,
    0x9d, 0xc3, 0x21, 0x7f, 0xfc, 0xa2, 0x40, 0x1e,
    0x5f, 0x01, 0xe3, 0xbd, 0x3e, 0x60, 0x82, 0xdc,
    0x23, 0x7d, 0x9f, 0xc1, 0x42, 0x1c, 0xfe, 0xa0,
    0xe1, 0xbf, 0x5d, 0x03, 0x80, 0xde, 0x3c, 0x62,
    0xbe, 0xe0, 0x02, 0x5c, 0xdf, 0x81, 0x63, 0x3d,
    0x7c, 0x22, 0xc0, 0x9e, 0x1d, 0x43, 0xa1, 0xff,
    0x46, 0x18, 0xfa, 0xa4, 0x27, 0x79, 0x9b, 0xc5,
    0x84, 0xda, 0x38, 0x66, 0xe5, 0xbb, 0x59, 0x07,
    0xdb, 0x85, 0x67, 0x39, 0xba, 0xe4, 0x06, 0x58,
    0x19, 0x47, 0xa5, 0xfb, 0x78, 0x26, 0xc4, 0x9a,
    0x65, 0x3b, 0xd9, 0x87, 0x04, 0x5a, 0xb8, 0xe6,
    0xa7, 0xf9, 0x1b, 0x45, 0xc6, 0x98, 0x7a, 0x24,
    0xf8, 0xa6, 0x44, 0x1a, 0x99, 0xc7, 0x25, 0x7b,
    0x3a, 0x64, 0x86, 0xd8, 0x5b, 0x05, 0xe7, 0xb9,
    0x8c, 0xd2, 0x30, 0x6e, 0xed, 0xb3, 0x51, 0x0f,
    0x4e, 0x10, 0xf2, 0xac, 0x2f, 0x71, 0x93, 0xcd,
    0x11, 0x4f, 0xad, 0xf3, 0x70, 0x2e, 0xcc, 0x92,
    0xd3, 0x8d, 0x6f, 0x31, 0xb2, 0xec, 0x0e, 0x50,
    0xaf, 0xf1, 0x13, 0x4d, 0xce, 0x90, 0x72, 0x2c,
    0x6d, 0x33, 0xd1, 0x8f, 0x0c, 0x52, 0xb0, 0xee,
    0x32, 0x6c, 0x8e, 0xd0, 0x53, 0x0d, 0xef, 0xb1,
    0xf0, 0xae, 0x4c, 0x12, 0x91, 0xcf, 0x2d, 0x73,
    0xca, 0x94, 0x76, 0x28, 0xab, 0xf5, 0x17, 0x49,
    0x08, 0x56, 0xb4, 0xea, 0x69, 0x37, 0xd5, 0x8b,
    0x57, 0x09, 0xeb, 0xb5, 0x36, 0x68, 0x8a, 0xd4,
    0x95, 0xcb, 0x29, 0x77, 0xf4, 0xaa, 0x48, 0x16,
    0xe9, 0xb7, 0x55, 0x0b, 0x88, 0xd6, 0x34, 0x6a,
    0x2b, 0x75, 0x97, 0xc9, 0x4a, 0x14, 0xf6, 0xa8,
    0x74, 0x2a, 0xc8, 0x96, 0x15, 0x4b, 0xa9, 0xf7,
    0xb6, 0xe8, 0x0a, 0x54, 0xd7, 0x89, 0x6b, 0x35,
};

QueueHandle_t usartMessageQue;
#define CMQUE_LEN   60
TaskHandle_t usartTaskHandle = NULL;
void usartTask(void *pvParameters);

pkgTypedef uart3Dma;

//校验
uint8_t checkValCode(uint8_t ary[], uint8_t start, uint8_t end){
    uint8_t ret = 0x00;
    for(uint8_t i = start; i < end; i++){
        ret += ary[i];
    }
    return ret;
}

uint8_t crc8(uint8_t ary[], uint8_t start, const uint8_t end)
{
    uint8_t crc = 0;
    for (uint8_t i = start; i < end; ++i ) {
         crc = crc8_table[(ary[i] ^ crc) & 0xFF];
    }
    return crc;
}

//添加报文到队列
bool addUsartMsgToQue(pkgTypedef p){
	if(xPortIsInsideInterrupt()){
		if(uxQueueMessagesWaitingFromISR(usartMessageQue) < CMQUE_LEN){
			//xQueueSendFromISR(usartMessageQue, &p, NULL);				//排队响应
			xQueueSendToFrontFromISR(usartMessageQue, &p, NULL);		//插队响应
		}else{
			printf("usartMessageQue remain space is zero !!!\r\n ");
		}		
	}else{
		if(uxQueueMessagesWaiting(usartMessageQue) < CMQUE_LEN){
			xQueueSend(usartMessageQue, &p, portMAX_DELAY);				//排队
		}else{
			printf("usartMessageQue remain space is zero !!!\r\n ");
		}		
	}    
	return true;
}

//打印报文
void printUsartMesage(char *str, uint8_t *d){
//    return;
//	if(d[CM_CMD] == AUTO_REPORT_CMD)	return ;		
//	printf("%5s -> cmd:0x%02X fun: 0x%02X dLen:%2d data : ", str, d[CM_CMD], d[CM_FUN], d[CM_LEN]);	
//	for(int i=CM_DATA; i<d[CM_LEN]-5; i++){
//		printf("0x%02X ", d[i]);
//	}
//	printf("\r\n");
}

//封包
void packDatToBuf(uint8_t *dstBuf,uint8_t cmd, uint8_t fun, uint8_t dLen, uint8_t *data){  
    dstBuf[CM_H1] = 0xFF;
    dstBuf[CM_H2] = 0xFF;
    dstBuf[CM_LEN] = dLen + 2 + 1;          //数据长度 + cmd长度 + fun长度 + 校验长度
    dstBuf[CM_CMD] = cmd;
    dstBuf[CM_FUN] = fun;
    memcpy(dstBuf+CM_DATA, data, dLen);
    //uint8_t code = checkValCode(dstBuf, 2, dLen + CM_FUN+1); 
	uint8_t code = crc8(dstBuf, 2, dLen + CM_FUN+1); 
    dstBuf[CM_DATA + dLen] = code;
//	printUsartMesage("pkg", &m);
}

//自动上报
void usartAutoReportScan(int timeOut){
	static int cnt = 0;
	static float freq = 0.1;
	if((cnt++) < timeOut)	return ;

	static float data[12] = {0.0f,};
	
	freq = freq + 0.2;
	for(int i = 0; i < 12; i++){
		data[i] = ((float)(i+1) * 500.0f * fabs(sin(freq / 57.3f)));
	}

	uint8_t dLen = sizeof(float)*9;				//数据上报个数
	
    uint8_t buf[BUF_MAX_LEN];
    packDatToBuf(buf, AUTO_REPORT_CMD, TAR_POS, dLen, (uint8_t *)data);	//	
	
	pkgTypedef p;								//装载数据
	p.len = CM_LEN + 1 + dLen + 2 + 1;
	memcpy(p.data, buf, p.len);
	
	addUsartMsgToQue(p);						//填队列
	cnt = 0;
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);	
}


//串口传输
void usartTransmit(void* handle, void* id, uint8_t len, uint8_t *data){
    (void )handle;
    (void )id;
//	HAL_UART_Transmit(&huart3, data, len, 1000);
//	while( __HAL_UART_GET_FLAG(&huart3, UART_FLAG_TC)==RESET){};
	CDC_Transmit_FS(data, len);			//通过usb虚拟串口发送回去
}

int fpsTimeOut = 1000000;

//命令解析
void usartCmdFunAnalysis(uint8_t cmd, uint8_t fun, uint8_t dLen, uint8_t *data){
    bool isErrorCmd = false;	
	uint8_t buf[BUF_MAX_LEN];
	uint8_t answData[BUF_MAX_LEN];
	uint8_t answDataLen = 0;								
	memset(answData, 0, BUF_MAX_LEN);
	
    if(cmd == WRITE_CMD){									//写
        switch (fun){
            case CTL_MODE:{									//设置
				motor.mode = *data;
            }
            break;
            case TAR_POS:{
				motor.tarPos = data_to_float(data);
			}
            break;
			case TAR_SPEED:{
				motor.tarSpeed = data_to_float(data);
			}
            break;
			case TAR_CURRENT:{
				motor.tarIq = data_to_float(data);
			}
            break;
			case POS_RANGE:{								
				motor.minPos = data_to_float(data);				
				motor.maxPos = data_to_float(data + 4);
			}
			case SPEED_RANGE:{	
				motor.minSpeed = data_to_float(data);				
				motor.maxSpeed = data_to_float(data + 4);
			}
			break;
			case CURRENT_RANGE:{
				motor.minIq = data_to_float(data);	
				motor.maxIq = data_to_float(data + 4);
			}
			break;
			case POS_P:{
				motor.posP = data_to_float(data);				
			}
			break;
			case POS_D:{
				motor.posD = data_to_float(data);	
			}
			break;
			case SPEED_P:{
				motor.speedP = data_to_float(data);	
			}
			break;
			case SPEED_I:{
				motor.speedI = data_to_float(data);	
			}
			break;
			case CURRENT_P:{
				motor.currentP = data_to_float(data);	
			}
			break;
			case CURRENT_I:{
				motor.currentI = data_to_float(data);	
			}
			break;
			
			case SAVE_PARAM:{
			
			}
			break;
			
			case REPORT_FPS:{								//设置帧率
				fpsTimeOut = data_to_int32(data);
			}
			break;	
       
            default:    isErrorCmd = true;
            break;
       }
    }else if(cmd == READ_CMD){								//读
        switch (fun){	
			case CTL_MODE:{
			
			}
			break;
			case TAR_POS:{
				answDataLen += float_to_data(motor.tarPos, answData);
			}
            break;
			case TAR_SPEED:{
				answDataLen += float_to_data(motor.tarSpeed, answData);
			}
            break;
			case TAR_CURRENT:{
				answDataLen += float_to_data(motor.tarIq, answData);
			}
            break;
			case CURRENT_RANGE:{									//获取电流范围
				answDataLen += float_to_data(motor.minIq, answData);
				answDataLen += float_to_data(motor.maxIq, answData + 4);
			}
			break;
			case SPEED_RANGE:{										//获取速度范围
				answDataLen += float_to_data(motor.minSpeed, answData);
				answDataLen += float_to_data(motor.maxSpeed, answData + 4);
			}
			break;
			case POS_P:{
				answDataLen += float_to_data(motor.posP, answData);			
			}
			break;
			case POS_D:{
				answDataLen += float_to_data(motor.posD, answData);			
			}
			break;
			case SPEED_P:{
				answDataLen += float_to_data(motor.speedP, answData);			
			}
			break;
			case SPEED_I:{
				answDataLen += float_to_data(motor.speedI, answData);			
			}
			break;
			case CURRENT_P:{
				answDataLen += float_to_data(motor.currentP, answData);							
			}
			break;
			case CURRENT_I:{	
				answDataLen += float_to_data(motor.currentI, answData);	
			}
			break;
			
            default:    isErrorCmd = true;
                break;
       }
    }else if(cmd == AUTO_REPORT_CMD){						//主动上报
		packDatToBuf(buf, AUTO_REPORT_CMD, fun, dLen, data);
        switch (fun){
            case 0:        ; break;   
            default:
                break;
        }
    }else{
        isErrorCmd = true;
    }
	if(cmd != AUTO_REPORT_CMD){
		cmdTypedef c = (isErrorCmd) ? ERROR_CMD : ANSWER_CMD;		
		packDatToBuf(buf, c, (funsionType)fun, answDataLen, answData);
	}	
	printUsartMesage("send", buf);
    usartTransmit(0, 0, CM_LEN + buf[CM_LEN] + 1, buf);
	memset(buf, 0, sizeof(uint8_t)*BUF_MAX_LEN);
}

// usart任务
void usartTask(void *pvParameters)
{
	pkgTypedef pkg;
    while (1){        
		if(uxQueueMessagesWaiting(usartMessageQue) != 0){
			if (xQueueReceive(usartMessageQue, &pkg, portMAX_DELAY) == pdPASS){			//暂时未做粘包处理
				if(pkg.data[CM_H1]==0xFF && pkg.data[CM_H2]==0xFF){						//帧头
					uint8_t code = crc8(pkg.data, 2, pkg.len-1);          				//计算校验码
					if(code != pkg.data[pkg.len-1]){	return;	}						//校验失败 直接返回
					usartCmdFunAnalysis(pkg.data[CM_CMD], pkg.data[CM_FUN], pkg.data[CM_LEN] - 3 , pkg.data+CM_DATA);
				}				
			}
		}        
		if(fpsTimeOut != 1000000){
			usartAutoReportScan(fpsTimeOut-1);
		}			
		vTaskDelay(1);
    }
}

motorType motor;

//串口通信初始化
void usartCommunInit(void){
    usartMessageQue = xQueueCreate(CMQUE_LEN, sizeof(pkgTypedef));
    if(usartMessageQue == NULL){
        printf("creat usartMessageQue failed !!!\r\n");
    }
	memset(&motor, 0, sizeof(motorType));
	//param init
	motor.maxSpeed = 1000.0f;
	motor.minSpeed = -1000.0f;
	
	motor.maxIq = 7.5f;
	motor.minIq = -2.5f;
	
	motor.posP = 0.005;
	motor.posD = 0.0025;
	motor.speedP = 0.001;
	motor.speedI = 0.002;
	motor.currentP = 0.005;
	motor.currentI = 0.0032;	
	
	if(xTaskCreate(	(TaskFunction_t  ) usartTask,            	
					(const char *    ) "usartTask",          	
					(uint16_t        ) 1024,           		
					(void *          ) NULL,                    
					(UBaseType_t     ) 2,                       
					(TaskHandle_t *  ) usartTaskHandle ) != pdPASS)
    {
        printf(" [%s] could not be created as there was insufficient heap memory remaining.\r\n", __FUNCTION__);
    }
}
