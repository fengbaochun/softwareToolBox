#include "util.h"

void gvdRtcTimePrint(void)
{

}

uint32_t guwTimeCmp(uint32_t const timePre, uint32_t const timeCurrent){
#if INCLUDE_vTaskDelay
	if(timeCurrent >= timePre){
		return((timeCurrent - timePre) * portTICK_PERIOD_MS);
	}
	return 0;
#else
	(void)timePre;
	(void)timeCurrent;
	ERR_LOG ("system has no ticks !\r\n");
	return 0;
#endif
}

