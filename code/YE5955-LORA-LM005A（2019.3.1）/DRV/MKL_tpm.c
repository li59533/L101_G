/****************************************Copyright (c)****************************************************
**               Copyright � 2003~2009 Shenzhen uCdragon Technology Co.,Ltd. All Rights Reserved 
**
**                                 http://www.ucdragon.cn
**
**      �����������Ƽ����޹�˾���ṩ�����з�������ּ��Э���ͻ����ٲ�Ʒ���з����ȣ��ڷ�����������ṩ
**  ���κγ����ĵ������Խ����������֧�ֵ����Ϻ���Ϣ���������ο����ͻ���Ȩ��ʹ�û����вο��޸ģ�����˾��
**  �ṩ�κε������ԡ��ɿ��Եȱ�֤�����ڿͻ�ʹ�ù��������κ�ԭ����ɵ��ر�ġ�żȻ�Ļ��ӵ���ʧ������˾��
**  �е��κ����Ρ�
**                                                                        �������������Ƽ����޹�˾
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               MKL_tpm.c
** Last modified date:      
** Last version:            
** Descriptions:            tpm��������
**
**--------------------------------------------------------------------------------------------------------
** Created by:            
** Created date 
** Version:               
** Descriptions:            tpm��ʼ�� ��ʼ�汾
**--------------------------------------------------------------------------------------------------------
** Modified by:        
** Modified date:      
** Version:            
** Descriptions:       
**
*********************************************************************************************************/
#include "..\..\App\includes.h"
#define TPM_MODULE    100
#define TPM_INIT_VAL   2
#define TPM_Cn_MODE  (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK)

/*********************************************************************************************************
** Function name:           tpmInit
** Descriptions:            ptm�ĳ�ʼ�� 
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              
** Created date:            
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
*********************************************************************************************************/
void tpmInit(void) 
{
    SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK ;            /* ��GPIOʱ��                */
    SIM_SCGC6 |=  SIM_SCGC6_TPM1_MASK;              /* ��TPMʱ��                 */
    SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK | SIM_SOPT2_TPMSRC(1);         /* ��ʱ����      SIM_SOPT2_TPMSRC(3)�ڲ�4Mʱ��     SIM_SOPT2_PLLFLLSEL_MASK |�����Ͼ���ѡ���Ƶʱ����ΪTPM��ʱ��Դ       */

    PORTB_PCR0 = ( 0 | PORT_PCR_MUX(3));                                /* TPM2_CH0 enable on PB0    */
  //  PORTA_PCR4 = ( 0 | PORT_PCR_MUX(3));                                /* TPM2_CH1 enable on PA4   */
  

  //  TPM0_MOD  = TPM_MODULE;                                              /* 1999 / 4MHz = uS PWM     */
  //  TPM0_C1SC = TPM_Cn_MODE;                                             /* No Interrupts; High True pulses on Edge Aligned PWM */
                                                                           
  //  TPM0_C1V  = TPM_INIT_VAL;                                            /* 90% pulse width             */
    
	  TPM1_MOD  = TPM_MODULE;                                              /* 1999 / 4MHz = uS PWM     */
    TPM1_C0SC = TPM_Cn_MODE;                                             /* No Interrupts; Low True 
                                                                            pulses on Edge Aligned PWM  */
    TPM1_C0V  = TPM_INIT_VAL;                                            /* 90% pulse width             */
   

    TPM1_SC   = TPM_SC_CMOD(0) | TPM_SC_PS(0);                          /* ����TP1       TPM_SC_CMOD(1)1��ʱ������0��ʱ��ر�    TPM_SC_PS(0);TPMʱ�ӵ�Ԥ��Ƶϵ������ʱ1��Ƶ        */
   // TPM0_SC   = TPM_SC_CMOD(1) | TPM_SC_PS(0);                          /* ����TP0                   */
}

/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/



