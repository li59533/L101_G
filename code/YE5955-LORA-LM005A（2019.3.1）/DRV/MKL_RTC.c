/****************************************Copyright (c)****************************************************
**               Copyright © 2003~2009 Shenzhen uCdragon Technology Co.,Ltd. All Rights Reserved 
**
**                                 http://www.ucdragon.com
**
**      ÉîÛÚÊĞÓÅÁú¿Æ¼¼ÓĞÏŞ¹«Ë¾ËùÌá¹©µÄËùÓĞ·şÎñÄÚÈİÖ¼ÔÚĞ­Öú¿Í»§¼ÓËÙ²úÆ·µÄÑĞ·¢½ø¶È£¬ÔÚ·şÎñ¹ı³ÌÖĞËùÌá¹©
**  µÄÈÎºÎ³ÌĞò¡¢ÎÄµµ¡¢²âÊÔ½á¹û¡¢·½°¸¡¢Ö§³ÖµÈ×ÊÁÏºÍĞÅÏ¢£¬¶¼½ö¹©²Î¿¼£¬¿Í»§ÓĞÈ¨²»Ê¹ÓÃ»ò×ÔĞĞ²Î¿¼ĞŞ¸Ä£¬±¾¹«Ë¾²»
**  Ìá¹©ÈÎºÎµÄÍêÕûĞÔ¡¢¿É¿¿ĞÔµÈ±£Ö¤£¬ÈôÔÚ¿Í»§Ê¹ÓÃ¹ı³ÌÖĞÒòÈÎºÎÔ­ÒòÔì³ÉµÄÌØ±ğµÄ¡¢Å¼È»µÄ»ò¼ä½ÓµÄËğÊ§£¬±¾¹«Ë¾²»
**  ³Ğµ£ÈÎºÎÔğÈÎ¡£
**                                                                        ¡ªÉîÛÚÊĞÓÅÁú¿Æ¼¼ÓĞÏŞ¹«Ë¾
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               MKL_rtc.c
** Last modified date:      2012-12-14
** Last version:            V1.0
** Descriptions:            rtcÇı¶¯³ÌĞò
**
** Modified by:        
** Modified date:      
** Version:            
** Descriptions:       
**
*********************************************************************************************************/
#include "includes.h"    

/*********************************************************************************************************
** Function name:           rtcInit
** Descriptions:            RTC³õÊ¼»¯
** input parameters:        ulSeconds:³õÊ¼Ãë¼ÆÊıÖµ
** output parameters:       none
** Returned value:          none

**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
*********************************************************************************************************/
void rtcInit(INT32U ulSeconds)
{
    INT32U i;
    SIM_SCGC6 |= SIM_SCGC6_RTC_MASK;                                /* RTC¿ØÖÆÊ±ÖÓÊ¹ÄÜ                  */
    SIM_SOPT1  = SIM_SOPT1_OSC32KSEL(MKL_RTC_SOURCE);               /* RTCÊ±ÖÓÔ´Ñ¡Ôñ                    */
    RTC_CR     = RTC_CR_SWR_MASK;                                   /* RTCÈí¼ş¸´Î»                      */
    RTC_CR    &= ~RTC_CR_SWR_MASK;                                  /* ½â³ı¸´Î»                         */
    if (RTC_SR & RTC_SR_TIF_MASK){                                  /* Çå³ıTIF±êÖ¾                      */
        RTC_TSR = 0x00000000;  
    }
    if (RTC_SR & RTC_SR_TCE_MASK) {                                 /* Èç¹ûRTCÒÑ¾­Ê¹ÄÜ£¬Ôò½ûÄÜ          */
        RTC_SR &= ~RTC_SR_TCE_MASK;
    }
    RTC_TSR = ulSeconds;
    if (MKL_RTC_SOURCE != 3) {
        RTC_CR |= RTC_CR_OSCE_MASK|RTC_CR_SC16P_MASK;               /* Ê¹ÄÜ1KHzÊ±ÖÓ£¬½ÓÈë16pFµçÈİ        */
        for(i = 0; i < 0x600000; i++);                              /* µÈ´ıÊ±ÖÓÔ´ÎÈ¶¨                   */
    }
}
/*********************************************************************************************************
** Function name:           rtcStart
** Descriptions:            Æô¶¯RTC
** input parameters:        none
** output parameters:       none
** Returned value:          none

**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
*********************************************************************************************************/
void rtcStart(void)
{
    RTC_SR |= RTC_SR_TCE_MASK;                                      /* Ê¹ÄÜRTC                          */
}
/*********************************************************************************************************
** Function name:           rtcIntConfig
** Descriptions:            RTCÖĞ¶ÏÅäÖÃ
** input parameters:        none
** output parameters:       none
** Returned value:          none

**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
*********************************************************************************************************/
void rtcIntConfig(INT32U ulAlarm,INT8U ucIntType)
{
    if(ucIntType & MKL_RTC_TSI) {
        RTC_IER |= RTC_IER_TSIE_MASK;
        NVIC_EnableIRQ(RTC_Seconds_IRQn);
        NVIC_SetPriority(RTC_Seconds_IRQn,3);                       /* ÓÃ»§×Ô¼º¶¨Òå                     */
    }                  
    if(ucIntType & MKL_RTC_TAI) {
        RTC_TAR  = ulAlarm;
        RTC_IER |= RTC_IER_TAIE_MASK;
        NVIC_EnableIRQ(RTC_IRQn);
        NVIC_SetPriority(RTC_IRQn,3);                               /* ÓÃ»§×Ô¼º¶¨Òå                     */
				
}    
    if(ucIntType & MKL_RTC_TOI) {
        RTC_IER |= RTC_IER_TOIE_MASK;
        NVIC_EnableIRQ(RTC_IRQn);
        NVIC_SetPriority(RTC_IRQn,3);                               /* ÓÃ»§×Ô¼º¶¨Òå                     */
    }
    if(ucIntType & MKL_RTC_TII) {
        RTC_IER |= RTC_IER_TIIE_MASK;
        NVIC_EnableIRQ(RTC_IRQn);
        NVIC_SetPriority(RTC_IRQn,3);                               /* ÓÃ»§×Ô¼º¶¨Òå                     */
    }
}
/*********************************************************************************************************
** Function name:           RTC_IRQHandler
** Descriptions:            RTCÖĞ¶Ï·şÎñº¯Êı
** input parameters:        none
** output parameters:       none
** Returned value:          none

**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
*********************************************************************************************************/
void RTC_IRQHandler(void)
{
    if (RTC_SR & RTC_SR_TAF_MASK) {
        RTC_TAR = RTC_TAR + 1;
			 
       GPIO_TOGGLE(MKL_PORTB,19);
			 
    }
    if (RTC_SR & RTC_SR_TOF_MASK) {
        
    }
    if (RTC_SR & RTC_SR_TIF_MASK) {
        
    }
}
/*********************************************************************************************************
** Function name:           RTC_Seconds_IRQHandler
** Descriptions:            RTCÃëÖĞ¶Ï·şÎñº¯Êı
** input parameters:        none
** output parameters:       none
** Returned value:          none

**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
*********************************************************************************************************/
void RTC_Seconds_IRQHandler(void)
{		//printf("%ds\n",1);
    GPIO_TOGGLE(MKL_PORTB,18);
}




/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/
