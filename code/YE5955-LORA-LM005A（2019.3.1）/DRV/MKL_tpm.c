/****************************************Copyright (c)****************************************************
**               Copyright © 2003~2009 Shenzhen uCdragon Technology Co.,Ltd. All Rights Reserved 
**
**                                 http://www.ucdragon.cn
**
**      ÉîÛÚÊÐÓÅÁú¿Æ¼¼ÓÐÏÞ¹«Ë¾ËùÌá¹©µÄËùÓÐ·þÎñÄÚÈÝÖ¼ÔÚÐ­Öú¿Í»§¼ÓËÙ²úÆ·µÄÑÐ·¢½ø¶È£¬ÔÚ·þÎñ¹ý³ÌÖÐËùÌá¹©
**  µÄÈÎºÎ³ÌÐò¡¢ÎÄµµ¡¢²âÊÔ½á¹û¡¢·½°¸¡¢Ö§³ÖµÈ×ÊÁÏºÍÐÅÏ¢£¬¶¼½ö¹©²Î¿¼£¬¿Í»§ÓÐÈ¨²»Ê¹ÓÃ»ò×ÔÐÐ²Î¿¼ÐÞ¸Ä£¬±¾¹«Ë¾²»
**  Ìá¹©ÈÎºÎµÄÍêÕûÐÔ¡¢¿É¿¿ÐÔµÈ±£Ö¤£¬ÈôÔÚ¿Í»§Ê¹ÓÃ¹ý³ÌÖÐÒòÈÎºÎÔ­ÒòÔì³ÉµÄÌØ±ðµÄ¡¢Å¼È»µÄ»ò¼ä½ÓµÄËðÊ§£¬±¾¹«Ë¾²»
**  ³Ðµ£ÈÎºÎÔðÈÎ¡£
**                                                                        ¡ªÉîÛÚÊÐÓÅÁú¿Æ¼¼ÓÐÏÞ¹«Ë¾
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               MKL_tpm.c
** Last modified date:      
** Last version:            
** Descriptions:            tpmÇý¶¯³ÌÐò
**
**--------------------------------------------------------------------------------------------------------
** Created by:            
** Created date 
** Version:               
** Descriptions:            tpm³õÊ¼»¯ ³õÊ¼°æ±¾
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
** Descriptions:            ptmµÄ³õÊ¼»¯ 
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
    SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK ;            /* ´ò¿ªGPIOÊ±ÖÓ                */
    SIM_SCGC6 |=  SIM_SCGC6_TPM1_MASK;              /* ´ò¿ªTPMÊ±ÖÓ                 */
    SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK | SIM_SOPT2_TPMSRC(1);         /* ´ò¿ªÊ±ÖÓÃÅ      SIM_SOPT2_TPMSRC(3)ÄÚ²¿4MÊ±ÖÓ     SIM_SOPT2_PLLFLLSEL_MASK |Õâ¾ä¼ÓÉÏ¾ÍÊÇÑ¡Ôñ¸ßÆµÊ±ÖÓ×÷ÎªTPMµÄÊ±ÖÓÔ´       */

    PORTB_PCR0 = ( 0 | PORT_PCR_MUX(3));                                /* TPM2_CH0 enable on PB0    */
  //  PORTA_PCR4 = ( 0 | PORT_PCR_MUX(3));                                /* TPM2_CH1 enable on PA4   */
  

  //  TPM0_MOD  = TPM_MODULE;                                              /* 1999 / 4MHz = uS PWM     */
  //  TPM0_C1SC = TPM_Cn_MODE;                                             /* No Interrupts; High True pulses on Edge Aligned PWM */
                                                                           
  //  TPM0_C1V  = TPM_INIT_VAL;                                            /* 90% pulse width             */
    
	  TPM1_MOD  = TPM_MODULE;                                              /* 1999 / 4MHz = uS PWM     */
    TPM1_C0SC = TPM_Cn_MODE;                                             /* No Interrupts; Low True 
                                                                            pulses on Edge Aligned PWM  */
    TPM1_C0V  = TPM_INIT_VAL;                                            /* 90% pulse width             */
   

    TPM1_SC   = TPM_SC_CMOD(0) | TPM_SC_PS(0);                          /* Æô¶¯TP1       TPM_SC_CMOD(1)1µÄÊ±ºò¿ªÆô£¬0µÄÊ±ºò¹Ø±Õ    TPM_SC_PS(0);TPMÊ±ÖÓµÄÔ¤·ÖÆµÏµÊý£¬´ËÊ±1·ÖÆµ        */
   // TPM0_SC   = TPM_SC_CMOD(1) | TPM_SC_PS(0);                          /* Æô¶¯TP0                   */
}

/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/



