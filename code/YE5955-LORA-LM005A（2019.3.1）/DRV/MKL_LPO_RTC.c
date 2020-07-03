#include "includes.h"
#define NULL 0
 

 
#define SECONDS_IN_A_DAY     (86400U)
#define SECONDS_IN_A_HOUR    (3600U)
#define SECONDS_IN_A_MIN     (60U)
#define DAYS_IN_A_YEAR       (365U)
#define DAYS_IN_A_LEAP_YEAR  (366U)
// Table of month length (in days) for the Un-leap-year
static const uint8_t ULY[] = {0U, 31U, 28U, 31U, 30U, 31U, 30U, 31U, 31U, 30U, 31U, 30U, 31U};
// Table of month length (in days) for the Leap-year
static const uint8_t  LY[] = {0U, 31U, 29U, 31U, 30U, 31U, 30U, 31U, 31U, 30U, 31U, 30U, 31U};
// Number of days from begin of the non Leap-year
static const uint16_t MONTH_DAYS[] = {0U, 0U, 31U, 59U, 90U, 120U, 151U, 181U, 212U, 243U, 273U, 304U, 334U};

void rtcInit(int32_t ulSeconds)
{
    int32_t i;
    SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK ;            /* ��GPIOʱ��                */
   
	  PORTC_PCR1 =  PORT_PCR_MUX(1); 
    SIM_SCGC6 |= SIM_SCGC6_RTC_MASK;                                /* RTC����ʱ��ʹ��                  */
	  SIM_SOPT1  = SIM_SOPT1_OSC32KSEL(MKL_RTC_SOURCE);               /* RTCʱ��Դѡ��                    */
    RTC_CR     = RTC_CR_SWR_MASK;                                   /* RTC�����λ                      */
    RTC_CR    &= ~RTC_CR_SWR_MASK;                                  /* �����λ                         */
    if (RTC_SR & RTC_SR_TIF_MASK){                                  /* ���TIF��־                      */
        RTC_TSR = 0x00000000;  
    }
    if (RTC_SR & RTC_SR_TCE_MASK) {                                 /* ���RTC�Ѿ�ʹ�ܣ������          */
        RTC_SR &= ~RTC_SR_TCE_MASK;
    }
    RTC_TSR = ulSeconds;
    if (MKL_RTC_SOURCE != 3) {
         //RTC_CR |= RTC_CR_OSCE_MASK|RTC_CR_SC16P_MASK;               /* ʹ��32Kʱ�ӣ�����16pF����        */
        for(i = 0; i < 0x600000; i++);                              /* �ȴ�ʱ��Դ�ȶ�                   */
    }
}
/*********************************************************************************************************
** Function name:           rtcStart
** Descriptions:            ����RTC
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              
** Created date:            2012-05-14
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
*********************************************************************************************************/
void rtcStart(void)
{
    RTC_SR |= RTC_SR_TCE_MASK;                                      /* ʹ��RTC                          */
}



int RTC_GetWeek(int year, int month, int days)
{  
    static int mdays[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30 };  
    int i, y = year - 1;  
    for (i=0; i<month; ++i) days += mdays[i];  
    if (month > 2) 
    { 
         /* Increment date if this is a leap year after February */
        if (((year%400) == 0) || ((year&3) == 0 && (year%100))) ++days;  
    }  
    return (y+y/4-y/100+y/400+days)%7;  
}
 
/**
 * @brief  �����ڼ������
 * @note   None
 * @param  datetime ��ָ��RTCʱ�����ݽṹ���ָ�룬���rtc.h
 * @param  seconds ����������ݵĵ�ַ
 * @retval None
 */
void RTC_DateTimeToSecond(const RTC_DateTime_Type * datetime, uint32_t * seconds)
{
    /* Compute num`1ber of days from 1970 till given year*/
    *seconds = (datetime->year - 1970U) * DAYS_IN_A_YEAR;
    /* Add leap year days */
    *seconds += ((datetime->year / 4) - (1970U / 4));
    /* Add number of days till given month*/
    *seconds += MONTH_DAYS[datetime->month];
    /* Add days in given month*/
    *seconds += datetime->day;
    /* For leap year if month less than or equal to Febraury, decrement day counter*/
    if ((!(datetime->year & 3U)) && (datetime->month <= 2U))
    {
        (*seconds)--;
    }

    *seconds = ((*seconds) * SECONDS_IN_A_DAY) + (datetime->hour * SECONDS_IN_A_HOUR) + 
               (datetime->minute * SECONDS_IN_A_MIN) + datetime->second;
    (*seconds)++;
}

/**
 * @brief  ������������
 * @note   None
 * @param  seconds ��������루�����洢��ַ��
 * @param  datetime ����ָ�������յ���Ϣ�ṹ��ָ��
 * @retval None
 */
void RTC_SecondToDateTime(const uint32_t * seconds, RTC_DateTime_Type * datetime)
{
    uint32_t x;
    uint32_t Seconds, Days, Days_in_year;
    const uint8_t *Days_in_month;
    /* Start from 1970-01-01*/
    Seconds = *seconds;
    /* days*/
    Days = Seconds / SECONDS_IN_A_DAY;
    /* seconds left*/
    Seconds = Seconds % SECONDS_IN_A_DAY;
    /* hours*/
    datetime->hour = Seconds / SECONDS_IN_A_HOUR;
    /* seconds left*/
    Seconds = Seconds % SECONDS_IN_A_HOUR;
    /* minutes*/
    datetime->minute = Seconds / SECONDS_IN_A_MIN;
    /* seconds*/
    datetime->second = Seconds % SECONDS_IN_A_MIN;
    /* year*/
    datetime->year = 1970;
    Days_in_year = DAYS_IN_A_YEAR;

    while (Days > Days_in_year)
    {
        Days -= Days_in_year;
        datetime->year++;
        if  (datetime->year & 3U)
        {
            Days_in_year = DAYS_IN_A_YEAR;
        }
        else
        {
            Days_in_year = DAYS_IN_A_LEAP_YEAR;    
        }
    }

    if  (datetime->year & 3U)
    {
        Days_in_month = ULY;
    }
    else
    {
        Days_in_month = LY;    
    }

    for (x=1U; x <= 12U; x++)
    {
        if (Days <= (*(Days_in_month + x)))
        {
            datetime->month = x;
            break;
        }
        else
        {
            Days -= (*(Days_in_month + x));
        }
    }
    datetime->day = Days;
}

/**
 * @brief  ���RTC��ʱ��
 * @code
 *      //���RTC��ʱ��
 *      RTC_DateTime_Type ts;   //����һ���ṹ��
 *      RTC_GetTime(&ts);    //�����ڴ洢��ts��
 * @endcode
 * @note   None
 * @param  datetime �����ؼ�������������յ���Ϣ�ṹ��
 * @retval None
 */
void RTC_GetTime(RTC_DateTime_Type * datetime)
{
    if(!datetime)
    {
        return;
    }
	{
		uint32_t i = RTC_TSR;
		RTC_SecondToDateTime(&i, datetime);
	}
}

/**
 * @brief  �жϵ�ǰRTCʱ��ģ��ʱ���Ƿ���Ч
 * \code
 *      ��ʱ����Ч������δִ�й�RTCʱ����ʼ��RTC��ʱ�䣩 
 *    if(RTC_IsTimeValid())
 *    {
 *       printf("time invalid, reset time!\r\n");
 *       RTC_SetTime(&td);
 *    }
 * \endcode
 * @retval 0 ����Ч ����������Ч
 */

/**
 * @brief  ��������ʱ��
 * @note   None
 * @param  datetime ��ʱ����ṹ�壬���rtc.h�ļ�
 * @retval None
 */
void RTC_SetAlarm(RTC_DateTime_Type * datetime)
{
    uint32_t seconds;
    RTC_DateTimeToSecond(datetime, &seconds);
    RTC_TAR = seconds - 1;
}

/**
 * @brief  �������ʱ��
 * @note   None
 * @param  datetime ��ʱ����ṹ�壬���rtc.h�ļ�
 * @retval None
 */
void RTC_GetAlarm(RTC_DateTime_Type * datetime)
{
    uint32_t seconds;
    seconds = RTC_TAR;
    RTC_SecondToDateTime(&seconds, datetime);
}

void RTC_SetCompensation(uint32_t compensationInterval, uint32_t timeCompensation)
{
    RTC_TCR &= ~RTC_TCR_CIR_MASK;
    RTC_TCR &= ~RTC_TCR_TCR_MASK;
    RTC_TCR |= RTC_TCR_CIR(compensationInterval);
    RTC_TCR |= RTC_TCR_TCR(timeCompensation);
}

/**
 * @brief  ��ʼ��RTCģ��
 * @retval None
 */

uint32_t RTC_GetCounter(void)
{
    return RTC_TSR;
}

uint32_t RTC_GetTAR(void)
{
    return RTC_TAR;
}

/**
 * @brief  ����RTC��ʱ��
 * @note   None
 * @param  datetime ��ʱ����ṹ�壬���rtc.h�ļ�
 * @code
 *   RTC_Init();
 *   RTC_DateTime_Type td = {0};
 *   td.day = 20;
 *   td.hour = 23;
 *   td.minute = 59;
 *   td.second = 50;
 *   td.year = 2014;
 *   td.month = 11;
 *   RTC_SetTime(&td);
 * @endcode
 * @retval None
 */
void RTC_SetTime(RTC_DateTime_Type * datetime)
{
    uint32_t i;
    if(!datetime)
    {
        return;
    }
    RTC_DateTimeToSecond(datetime, &i);
   
    RTC_SR &= ~RTC_SR_TCE_MASK;
    RTC_TSR = RTC_TSR_TSR(i);
    RTC_SR |= RTC_SR_TCE_MASK;
}

void RTC_SetTSR(uint32_t val)
{
    RTC_SR &= ~RTC_SR_TCE_MASK;
    RTC_TSR = RTC_TSR_TSR(val);
    RTC_SR |= RTC_SR_TCE_MASK;  
}

uint32_t RTC_GetTSR(void)
{
    return RTC_TSR;
}
