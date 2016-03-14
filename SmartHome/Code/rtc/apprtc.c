#include "apprtc.h"
#include "rtc.h"
#include "deveice.h"
#include "lcddriver.h"
#include "time.h"
#include "stdio.h"

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
//	static u8 num = 0;
//	num ^= 1;
//	if(0 != num)
//	{
		lcdpara.state = 1;
//	}
	
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;
	
	HAL_RTC_GetTime(hrtc, &sTime, FORMAT_BIN);
	HAL_RTC_GetDate(hrtc, &sDate, FORMAT_BIN);
#ifdef KONGTIAO
//	devstate.time[0] = sTime.Hours;
//	devstate.time[1] = sTime.Minutes;
//	devstate.time[2] = sTime.Seconds;
//
//	devstate.date[0] = (sDate.Year+2000)>>8;
//    devstate.date[1] = (sDate.Year+2000)&0xff;
//	devstate.date[2] = sDate.Month;
//	devstate.date[3] = sDate.Date;
#endif
}

void SetRTCDate()
{
	RTC_DateTypeDef sDate;
#ifdef KONGTIAO
//	sDate.Year = (devstate.date[0]<<8 | devstate.date[1])-2000;
//	sDate.Month = devstate.date[2];
//	sDate.Date = devstate.date[3];
#endif
	HAL_RTC_SetDate(&hrtc, &sDate, FORMAT_BIN);
}

void SetRTCTime()
{
	RTC_TimeTypeDef sTime;
#ifdef KONGTIAO
//	sTime.Hours= devstate.time[0];
//	sTime.Minutes= devstate.time[1];
//	sTime.Seconds= devstate.time[2];
#endif
	HAL_RTC_SetTime(&hrtc, &sTime, FORMAT_BIN);
}

void AppRtcInit()
{
	printf("\r\n时间开始");
	struct tm tmappdata;
	struct tm* tmappp;
	tmappp = &tmappdata;
	time_t timep = 1454039477; 
	printf("\r\n开始时间获取");
#if !defined(_NO_DEFINITIONS_IN_HEADER_FILES) && !__AEABI_PORTABILITY_INTERNAL_LEVEL
//	time (&timep);
	printf("\r\n时间获取结束");
    tmappp = localtime(&timep);
#endif
	printf("\r\n时间转换结束");
	
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;

	sTime.Hours = tmappp->tm_hour+8;
	sTime.Minutes = tmappp->tm_min;
	sTime.Seconds = tmappp->tm_sec;
	HAL_RTC_SetTime(&hrtc, &sTime, FORMAT_BIN);

	sDate.Month = tmappp->tm_mon + 1;
	sDate.Date = tmappp->tm_mday;
	sDate.Year = tmappp->tm_year-100;

	HAL_RTC_SetDate(&hrtc, &sDate, FORMAT_BIN);
	printf("\r\n时间结束");
}
