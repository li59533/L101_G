/**
  ******************************************************************************
  * @file    rtc.h
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.26
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  ******************************************************************************
  */
#ifndef __CH_LIB_RTC_H__
#define __CH_LIB_RTC_H__

#include <stdint.h>

#ifndef __bool_true_false_are_defined
#define __bool_true_false_are_defined 1
#define __ARMCLIB_VERSION 5050106

  #ifndef __cplusplus /* In C++, 'bool', 'true' and 'false' and keywords */
    #define bool _Bool
    #define true 1
    #define false 0
  #else
    #ifdef __GNUC__
      /* GNU C++ supports direct inclusion of stdbool.h to provide C99
         compatibility by defining _Bool */
      #define _Bool bool
    #endif
  #endif

#endif /* __bool_true_false_are_defined */


/**
 * \struct RTC_DateTime_Type
 * \brief RTC��ʼ���ṹ
 */
typedef struct 
{
   uint16_t year;    ///< ��Range from 200 to 2099.
   uint16_t month;   ///< ��Range from 1 to 12.
   uint16_t day;     ///< ��Range from 1 to 31 (depending on month).
   uint16_t hour;    ///< ʱRange from 0 to 23.
   uint16_t minute;  ///< ��Range from 0 to 59.
   uint8_t second;   ///< ��Range from 0 to 59.
}RTC_DateTime_Type;

/**
 * \enum RTC_OscLoad_Type
 * \brief RTC���õ��ݲ���
 */
typedef enum
{
    kRTC_OScLoad_0PF,       /**< Oscillator 0pF Load */
    kRTC_OScLoad_2PF,       /**< Oscillator 2pF Load */
    kRTC_OScLoad_4PF,       /**< Oscillator 4pF Load */
    kRTC_OScLoad_8PF,       /**< Oscillator 8pF Load */
    kRTC_OScLoad_16PF,      /**< Oscillator 16pF Load */
}RTC_OscLoad_Type;

/**
 * \struct RTC_InitTypeDef
 * \brief OSC ���ݸ���
 */
typedef struct
{
    RTC_OscLoad_Type        oscLoad;            // OSC ���ݸ���
}RTC_InitTypeDef;

/**
 * \enum RTC_ITDMAConfig_Type
 * \brief RTC interrupt and DMA select
 */
typedef enum
{
    kRTC_IT_TimeAlarm,             /**< ���������ж� */
    kRTC_IT_TimeOverflow,          /**< ����ʱ������ж� */ 
    kRTC_IT_TimeSeconds,
}RTC_ITDMAConfig_Type;

//!< CallbackType
typedef void (*RTC_CallBackType)(void);

//!<API functions
void RTC_QuickInit(void);
void RTC_GetTime(RTC_DateTime_Type * datetime);
void RTC_SetTime(RTC_DateTime_Type * datetime);
void RTC_ITDMAConfig(RTC_ITDMAConfig_Type config, bool status);
bool RTC_IsTimeValid(void);
void RTC_CallbackInstall(RTC_CallBackType AppCBFun);
void RTC_SetTSR(uint32_t val);

int  RTC_GetWeek(int year, int month, int days);
uint32_t RTC_GetTSR(void);
void RTC_SetAlarm(RTC_DateTime_Type * datetime);
void RTC_SetCompensation(uint32_t compensationInterval, uint32_t timeCompensation);



#endif
