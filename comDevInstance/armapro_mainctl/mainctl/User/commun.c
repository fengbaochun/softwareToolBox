#include "commun.h"
#include "usart.h"
#include "dma.h"
#include "power.h"

SemaphoreHandle_t powerOffSem = NULL;
QueueHandle_t cmdMsghandle;
extern bool isSystemPowerOn;

void packDatToBus(cmdTypedef cmd, funsionType fun, uint32_t val){
	uint8_t buf[255];
	memset(buf,0,255);
	int cnt = 0;
	buf[cnt++] = 0xFF;          //头1
	buf[cnt++] = 0xFF;			//头2
	buf[cnt++] = 0x00;			//长度
	
	buf[cnt++] = cmd;			//属性  读 写 应答 主动上报 命令错误
	buf[cnt++] = fun;			//功能码 
	
    buf[cnt++] = BYTE0(val);	//数据 固定4字节
	buf[cnt++] = BYTE1(val);
	buf[cnt++] = BYTE2(val);
	buf[cnt++] = BYTE3(val);		

	buf[2] = cnt-3+1;		    //除去 头(2B),长度(1B);包含校验(1B)

	__IO uint8_t sum = 0;
	for(uint8_t i = 2; i < cnt; i++){		//计算校验
		sum += buf[i];
	}		
		
	buf[cnt++] = sum;
    
    HAL_UART_Transmit(&huart1, buf, cnt, 0xFFFF);
	while( __HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC)==RESET){};
}

//校验
uint8_t checkValCode(uint8_t ary[], uint8_t start, uint8_t end){
    uint8_t ret = 0x00;
    for(uint8_t i = start; i < end; i++){
        ret += ary[i];
    }
    return ret;
}

//关机事件
uint32_t powerOffEvenCallBack(uint8_t cmd, uint32_t data){
	uint32_t ret = 0x00;
	if(cmd == WRITE_CMD && data == 0x01){
		isSystemPowerOn = false;							//开关机状态
		
		//断电、延时关机
		HAL_Delay(1000);
		HAL_Delay(1000);
		HAL_Delay(1000);
		HAL_Delay(1000);
		HAL_Delay(1000);
		//断电
		//power off
		
		HAL_GPIO_WritePin(led_GPIO_Port, led_Pin, (GPIO_PinState)1);
		
		ret = 0x01;
	}else{
		ret = 0x00;
	}
	return ret;
}

//电源IO控制回调
uint32_t powerIoCtlCallBack(uint8_t id, bool isOpen){
	if(id > DEVICE_PWR_MAX)	return 0;
	devicePwrCtrl(id, isOpen);
	return 1;
}

//命令解析
void cmdFunAnalysis(uint8_t cmd, uint8_t fun, uint32_t data){
    bool isErrorCmd = false;
	uint32_t ret = 0x01;
	
	if((cmd == WRITE_CMD && fun == KEY_STATUS)){			//关机命令前置
		uint8_t c = (isErrorCmd == true) ? ERROR_CMD : ANSWER_CMD;
		packDatToBus(c, fun, ret);         
	}
    if(cmd == WRITE_CMD){									//写
        switch (fun){
			case KEY_STATUS:        ret = powerOffEvenCallBack(cmd, data);	break; 		//开关状态
            case RFTI_SW:           ret = data;	
									devicePwrCtrl(13, data);
									break;      //射频开关（脚踏板）
			
            case RFTI_ENERY:        ret = data; break;      //射频能量            
			
			case POWERIO: 			ret = data;	
									u32DataType b;
									b.data = data;
									powerIoCtlCallBack(b.da[0], b.da[1]);
									break;		//电源IO			
			
            default:    isErrorCmd = true;
                break;
        }
    }else if(cmd == READ_CMD){								//读
        switch (fun){
            case KEY_STATUS:        ret = data;break; 		//开关状态
            case E_CODE:            ret = data;break;		//故障码
            case RFTI_SW:           ret = data;	break; 		//射频开关
            case RFTI_ENERY:        ret = 99;	break;		//射频能量  
			case POWERIO:  			ret = data;	break;		//电源IO
                                    
            default: isErrorCmd = true;
                break;
        }
    }else{
        isErrorCmd = true;
    }
	if(!(cmd == WRITE_CMD && fun == KEY_STATUS)){
		uint8_t c = (isErrorCmd == true) ? ERROR_CMD : ANSWER_CMD;
		packDatToBus(c, fun, ret);         
	}
    
}


//协议解析
void uart1DmaHandle(uint8_t *buf, uint8_t len){
	if(!isSystemPowerOn)	return ;
	if((buf[0]==0xFF) && (buf[1]==0xFF))						//检查 包头、ID 
	{						
        uint8_t code = checkValCode(buf, 2, len-1);           	//计算校验码
        if(code != buf[len-1]){	return;	}						//校验失败
        messageTypedef m;
        m.cmd = buf[3];
        m.fun = buf[4];
        memcpy(&m.data, buf+5,sizeof(uint32_t));
				
		if(cmdMsghandle != NULL){
			BaseType_t xHigherPriorityTaskWoken;		
			xQueueSendFromISR(cmdMsghandle, &m, &xHigherPriorityTaskWoken);	//填入队列
		}
	}
}

void USART1_IRQHandler(void)
{
	if(RESET != __HAL_UART_GET_FLAG(&huart1, UART_FLAG_ORE)){
		__HAL_UART_CLEAR_OREFLAG(&huart1);
	}

	uint32_t tmp_flag = __HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE);
	if ((tmp_flag != RESET)){
		__HAL_UART_CLEAR_IDLEFLAG(&huart1);

		HAL_UART_DMAStop(&huart1);  
		uart1Dma.len = RXBUF_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
		uart1DmaHandle(uart1Dma.buf,uart1Dma.len);          					//接收数据回调处理
		HAL_UART_Receive_DMA(&huart1, uart1Dma.buf,RXBUF_SIZE);
		memset(&uart1Dma,0,sizeof(usartDmaTypedef));
	}
 
	__HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_RXNE);
	__HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_NE);
	__HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_FE);
}



//命令解析任务
void cmdTask(void *argument){
	UNUSED(argument);
    cmdMsghandle = xQueueCreate(5, sizeof(messageTypedef)); 
	if(cmdMsghandle == NULL){
		while(1);
	}
	static int cnt = 0;
	while(1) {
        messageTypedef m ;
        if(xQueueReceive(cmdMsghandle, &m, 1)){
            cmdFunAnalysis(m.cmd, m.fun, m.data);			//协议解析
        }
		if(isSystemPowerOn&&((cnt++) > 100)){
			cnt = 0;
			HAL_GPIO_TogglePin(led_GPIO_Port,led_Pin);		//开机情况下，正常闪烁
		}        
		vTaskDelay(1);
	}
}



