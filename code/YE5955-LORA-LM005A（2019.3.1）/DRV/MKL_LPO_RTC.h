/**
  ******************************************************************************
  * @file    	mkl_lpo_rtc.h
  * @author  shaoyonghua
  * @version V2.5
  * @date    2017.3.26
  * @brief   www.china-yec.com 
  ******************************************************************************
  */

#include <stdint.h>


typedef struct 
{
   uint16_t year;    ///< 年Range from 200 to 2099.
   uint16_t month;   ///< 月Range from 1 to 12.
   uint16_t day;     ///< 日Range from 1 to 31 (depending on month).
   uint16_t hour;    ///< 时Range from 0 to 23.
   uint16_t minute;  ///< 分Range from 0 to 59.
   uint8_t second;   ///< 秒Range from 0 to 59.
}RTC_DateTime_Type;


void rtcInit(int32_t ulSeconds);
void RTC_DateTimeToSecond(const RTC_DateTime_Type * datetime, uint32_t * seconds);
void RTC_SecondToDateTime(const uint32_t * seconds, RTC_DateTime_Type * datetime);
void RTC_GetTime(RTC_DateTime_Type * datetime);
void RTC_SetTime(RTC_DateTime_Type * datetime);
