/****************************************Copyright (c)****************************************************
**               Copyright � 2003~2009 Shenzhen uCdragon Technology Co.,Ltd. All Rights Reserved 
**
**                                 http://www.ucdragon.com
**
**      �����������Ƽ����޹�˾���ṩ�����з�������ּ��Э���ͻ����ٲ�Ʒ���з����ȣ��ڷ�����������ṩ
**  ���κγ����ĵ������Խ����������֧�ֵ����Ϻ���Ϣ���������ο����ͻ���Ȩ��ʹ�û����вο��޸ģ�����˾��
**  �ṩ�κε������ԡ��ɿ��Եȱ�֤�����ڿͻ�ʹ�ù��������κ�ԭ����ɵ��ر�ġ�żȻ�Ļ��ӵ���ʧ������˾��
**  �е��κ����Ρ�
**                                                                        �������������Ƽ����޹�˾
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               MKL_rtc.c
** Last modified date:      2012-12-14
** Last version:            V1.0
** Descriptions:            rtc��������
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
** Descriptions:            RTC��ʼ��
** input parameters:        ulSeconds:��ʼ�����ֵ
** output parameters:       none
** Returned value:          none

**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
*********************************************************************************************************/
void rtcInit(INT32U ulSeconds)
{
    INT32U i;
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
        RTC_CR |= RTC_CR_OSCE_MASK|RTC_CR_SC16P_MASK;               /* ʹ��1KHzʱ�ӣ�����16pF����        */
        for(i = 0; i < 0x600000; i++);                              /* �ȴ�ʱ��Դ�ȶ�                   */
    }
}
/*********************************************************************************************************
** Function name:           rtcStart
** Descriptions:            ����RTC
** input parameters:        none
** output parameters:       none
** Returned value:          none

**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
*********************************************************************************************************/
void rtcStart(void)
{
    RTC_SR |= RTC_SR_TCE_MASK;                                      /* ʹ��RTC                          */
}
/*********************************************************************************************************
** Function name:           rtcIntConfig
** Descriptions:            RTC�ж�����
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
        NVIC_SetPriority(RTC_Seconds_IRQn,3);                       /* �û��Լ�����                     */
    }                  
    if(ucIntType & MKL_RTC_TAI) {
        RTC_TAR  = ulAlarm;
        RTC_IER |= RTC_IER_TAIE_MASK;
        NVIC_EnableIRQ(RTC_IRQn);
        NVIC_SetPriority(RTC_IRQn,3);                               /* �û��Լ�����                     */
				
}    
    if(ucIntType & MKL_RTC_TOI) {
        RTC_IER |= RTC_IER_TOIE_MASK;
        NVIC_EnableIRQ(RTC_IRQn);
        NVIC_SetPriority(RTC_IRQn,3);                               /* �û��Լ�����                     */
    }
    if(ucIntType & MKL_RTC_TII) {
        RTC_IER |= RTC_IER_TIIE_MASK;
        NVIC_EnableIRQ(RTC_IRQn);
        NVIC_SetPriority(RTC_IRQn,3);                               /* �û��Լ�����                     */
    }
}
/*********************************************************************************************************
** Function name:           RTC_IRQHandler
** Descriptions:            RTC�жϷ�����
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
** Descriptions:            RTC���жϷ�����
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
