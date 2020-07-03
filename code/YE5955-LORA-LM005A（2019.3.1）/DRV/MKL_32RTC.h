#include <stdint.h>


/**
 * \struct RTC_DateTime_t
 * \brief RTC初始化结构
 */
typedef struct 
{
   uint16_t year;    ///< ?Range from 200 to 2099.
   uint16_t month;   ///< ?Range from 1 to 12.
   uint16_t day;     ///< ?Range from 1 to 31 (depending on month).
   uint16_t hour;    ///< ?Range from 0 to 23.
   uint16_t minute;  ///< ?Range from 0 to 59.
   uint8_t second;   ///< ?Range from 0 to 59.
}RTC_DateTime_Type;
/**
 * \enum RTC_Int_t
 * \brief RTC interrupt and DMA select
 */
void RTC_GetTime(RTC_DateTime_Type * datetime);
void RTC_SetTime(RTC_DateTime_Type * datetime);

void RTC_SetTSR(uint32_t val);

int  RTC_GetWeek(int year, int month, int days);
uint32_t RTC_GetTSR(void);
void RTC_SetAlarm(RTC_DateTime_Type * datetime);
void RTC_SetCompensation(uint32_t compensationInterval, uint32_t timeCompensation);
