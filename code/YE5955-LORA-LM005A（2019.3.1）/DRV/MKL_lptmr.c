/****************************************Copyright (c)****************************************************
**               Copyright © 2003~2009 Shenzhen uCdragon Technology Co.,Ltd. All Rights Reserved 
**
**                                 http://www.ucdragon.com
**
**      ÉîÛÚÊÐÓÅÁú¿Æ¼¼ÓÐÏÞ¹«Ë¾ËùÌá¹©µÄËùÓÐ·þÎñÄÚÈÝÖ¼ÔÚÐ­Öú¿Í»§¼ÓËÙ²úÆ·µÄÑÐ·¢½ø¶È£¬ÔÚ·þÎñ¹ý³ÌÖÐËùÌá¹©
**  µÄÈÎºÎ³ÌÐò¡¢ÎÄµµ¡¢²âÊÔ½á¹û¡¢·½°¸¡¢Ö§³ÖµÈ×ÊÁÏºÍÐÅÏ¢£¬¶¼½ö¹©²Î¿¼£¬¿Í»§ÓÐÈ¨²»Ê¹ÓÃ»ò×ÔÐÐ²Î¿¼ÐÞ¸Ä£¬±¾¹«Ë¾²»
**  Ìá¹©ÈÎºÎµÄÍêÕûÐÔ¡¢¿É¿¿ÐÔµÈ±£Ö¤£¬ÈôÔÚ¿Í»§Ê¹ÓÃ¹ý³ÌÖÐÒòÈÎºÎÔ­ÒòÔì³ÉµÄÌØ±ðµÄ¡¢Å¼È»µÄ»ò¼ä½ÓµÄËðÊ§£¬±¾¹«Ë¾²»
**  ³Ðµ£ÈÎºÎÔðÈÎ¡£
**                                                                        ¡ªÉîÛÚÊÐÓÅÁú¿Æ¼¼ÓÐÏÞ¹«Ë¾
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               MKL_lptmr.c
** Last modified date:      
** Last version:            V1.0
** Descriptions:            lptmrÇý¶¯³ÌÐò
**
**--------------------------------------------------------------------------------------------------------
** Created by:             
** Created date 
** Version:                 
** Descriptions:            lptmr³õÊ¼»¯ ³õÊ¼°æ±¾
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
** Descriptions:            lptmrµÄ³õÊ¼»¯ 
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
** Descriptions:            lptmr 1sÖÐ¶Ï
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
    SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;                                   /* ´ò¿ªlptmrÊ±ÖÓ               */
    LPTMR0_CSR |= LPTMR_CSR_TCF_MASK | LPTMR_CSR_TIE_MASK;               /* Çå³ý±êÖ¾Î» ¿ªÆôÖÐ¶Ï         */
    LPTMR0_PSR |= LPTMR_PSR_PCS(1);                                      /* Ê¹ÓÃ1KHzÊ±ÖÓÔ´              */
    LPTMR0_CMR = 1000;                                                    /* Ä¬ÈÏ2·ÖÆµ 500¼´Îª1s         */

    LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;                                    /* ¿ªÆôLPTMR                   */
}



void time_delay_ms(unsigned int count_val) //×î¶àÑÓ³Ù60s
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


void time_delay_s(unsigned int count_val) //×î¶àÑÓ³Ù90Ð¡Ê±
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

