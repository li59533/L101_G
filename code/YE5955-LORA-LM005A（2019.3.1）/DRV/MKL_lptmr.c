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
** File name:               MKL_lptmr.c
** Last modified date:      
** Last version:            V1.0
** Descriptions:            lptmr��������
**
**--------------------------------------------------------------------------------------------------------
** Created by:             
** Created date 
** Version:                 
** Descriptions:            lptmr��ʼ�� ��ʼ�汾
**--------------------------------------------------------------------------------------------------------
** Modified by:        
** Modified date:      
** Version:            
** Descriptions:       
**
*********************************************************************************************************/
#include "..\..\App\includes.h"
#include "app.h"

/*********************************************************************************************************
** Function name:           lptmrInit
** Descriptions:            lptmr�ĳ�ʼ�� 
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              
** Created date:           
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
*********************************************************************************************************/

/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/

/*********************************************************************************************************
** Function name:           LPTimer_IRQHandler
** Descriptions:            lptmr 1s�ж�
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              
** Created date:           
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
*********************************************************************************************************/
void  lptmrInit (void) 
{
    SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;                                   /* ��lptmrʱ��               */
    LPTMR0_CSR |= LPTMR_CSR_TCF_MASK | LPTMR_CSR_TIE_MASK;               /* �����־λ �����ж�         */
    LPTMR0_PSR |= LPTMR_PSR_PCS(1);                                      /* ʹ��1KHzʱ��Դ              */
    LPTMR0_CMR = 1000;                                                    /* Ĭ��2��Ƶ 500��Ϊ1s         */

    LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;                                    /* ����LPTMR                   */
}



void time_delay_ms(unsigned int count_val) //����ӳ�60s
{
  /* Make sure the clock to the LPTMR is enabled */
  SIM_SCGC5|=SIM_SCGC5_LPTMR_MASK;

  /* Reset LPTMR settings */
  LPTMR0_CSR=0;

  /* Set the compare value to the number of ms to delay */
  LPTMR0_CMR = count_val;

  /* Set up LPTMR to use 1kHz LPO with no prescaler as its clock source */
  LPTMR0_PSR = LPTMR_PSR_PCS(1)|LPTMR_PSR_PBYP_MASK;//LPTMR_PSR_PRESCALE(8)

  /* Start the timer */
  LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;

  /* Wait for counter to reach compare value */
  while (!(LPTMR0_CSR & LPTMR_CSR_TCF_MASK));

  /* Disable counter and Clear Timer Compare Flag */
  LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK;

  return;
}


void time_delay_s(unsigned int count_val) //����ӳ�90Сʱ
{ count_val=(int)((float)count_val*1.953125);
  /* Make sure the clock to the LPTMR is enabled */
  SIM_SCGC5|=SIM_SCGC5_LPTMR_MASK;

  /* Reset LPTMR settings */
  LPTMR0_CSR=0;

  /* Set the compare value to the number of ms to delay */
  LPTMR0_CMR = count_val;

  /* Set up LPTMR to use 1kHz LPO with no prescaler as its clock source */
  LPTMR0_PSR = LPTMR_PSR_PCS(1)|LPTMR_PSR_PRESCALE(8);

  /* Start the timer */
  LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;

  /* Wait for counter to reach compare value */
   while (!(LPTMR0_CSR & LPTMR_CSR_TCF_MASK));

  /* Disable counter and Clear Timer Compare Flag */
  LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK;

  return;
}
/*********************************************************************************************************
  END FILE
**/

