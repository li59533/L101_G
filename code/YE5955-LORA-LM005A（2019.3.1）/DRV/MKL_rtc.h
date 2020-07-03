/*********************************************************************************************************/
#ifndef __ZMD_RTC_MKL_H
#define __ZMD_RTC_MKL_H
#include "includes.h"
//
//-------- <<< Use Configuration Wizard in Context Menu >>> ----------------------------------------------
//
#ifdef __cplusplus
extern "C" {
#endif
/*********************************************************************************************************
  RTC时钟源配置
*********************************************************************************************************/
#define MKL_RTC_SOURCE  2                                  /* Source 0: System oscillator (OSC32KCLK)  */
                                                            /* Source 1: Reserved                       */
                                                            /* Source 2: RTC_CLKIN                      */
                                                            /* Source 3: LPO 1kHz                       */
#define MKL_RTC_TSI     0x01                                /* Time Seconds Interrupt                   */
#define MKL_RTC_TAI     0x02                                /* Time Alarm Interrupt                     */
#define MKL_RTC_TOI     0x04                                /* Time Overflow Interrupt                  */
#define MKL_RTC_TII     0x08                                /* Time Invalid Interrupt                   */
/*********************************************************************************************************
  RTC函数声明
*********************************************************************************************************/
extern void rtcInit(int32_t ulSeconds);
extern void rtcIntConfig(INT32U ulAlarm,INT8U ucIntType);
extern void rtcStart(void);
extern void RTC_IRQHandler(void);
extern void RTC_Seconds_IRQHandler(void);
#ifdef __cplusplus
}
#endif


#endif

/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/  


