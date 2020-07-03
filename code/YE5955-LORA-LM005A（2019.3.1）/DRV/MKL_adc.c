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
** File name:               MKL_adc.c
** Last modified date:      2012-12-12
** Last version:            V1.0
** Descriptions:            adcÇý¶¯³ÌÐò
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoLong Liu
** Created date:            2012-12-12
** Version:                 V0.9
** Descriptions:            ADC³õÊ¼»¯ ³õÊ¼°æ±¾
**--------------------------------------------------------------------------------------------------------
** Modified by:        
** Modified date:      
** Version:            
** Descriptions:       
**
*********************************************************************************************************/
#include "includes.h"

/*********************************************************************************************************
** Function name:           adcInit
** Descriptions:            adcµÄ³õÊ¼»¯
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              JiaoLong Liu
** Created date:            2012-12-12
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
*********************************************************************************************************/
void  adcInit (void) 
{
    SIM_SCGC5  |= SIM_SCGC5_PORTE_MASK;                    /* ¿ªÆôPE GPIOÊ±ÖÓ         */
    PORTE_PCR18 = PORT_PCR_MUX(0);                         /* ¿ªÆôPE18¹Ü½ÅÄ£Äâ¹¦ÄÜ    */
	PORTC_PCR2  = PORT_PCR_MUX(0);            //1.25V      /* ¿ªÆôPC2¹Ü½ÅÄ£Äâ¹¦ÄÜ     */
    SIM_SCGC6  |= SIM_SCGC6_ADC0_MASK;                     /* ¿ªÆôADCÊ±ÖÓ             */
    /*
     * ÅäÖÃADCÏà¹Ø¹¦ÄÜ
     */
    ADC0_CFG1 = ADC_CFG1_ADIV(2)|ADC_CFG1_ADLSMP_MASK | ADC_CFG1_MODE(3) | ADC_CFG1_ADICLK(0);
	ADC0_CFG2 = ADC_CFG2_ADLSTS(0);                     // long sample time select :20 extra ADCK cycles,24 ADCK cycles total
    
	ADC0_SC1(0) &=(~0x1f);                                //Çå0£¬ÒòÎªÄ¬ÈÏÈ«ÖÃ1
	ADC0_SC1(0) |= ADC_SC1_ADCH(2);  // 2                 //¿ªÆô2Í¨µÀ
	ADC0_SC1(0) |= ADC_SC1_AIEN_MASK;                      //¿ªÆôADCÖÐ¶Ï
	
    //ADC0_SC3 = ADC_SC3_AVGE_MASK                          // hardware average enable
    //         | ADC_SC3_AVGS(3)                            // 32 samples average
    //         | ADC_SC3_ADCO_MASK;                         // continuous conversion
    //ADC0_SC2 |= ADC_SC2_REFSEL(0);                                      /* Ñ¡ÔñµçÔ´²Î¿¼Ô´               */
    ADC0_SC2 |= (ADC_SC2_REFSEL(0)                        //Ñ¡ÔñÐ¾Æ¬µÄVREFHºÍVREFLÁ½¸öÒý½Å×÷ÎªAD×ª»»µÄ²Î¿¼µçÑ¹
				|ADC_SC2_ADTRG_MASK);                     //Ó²¼þ´¥·¢£¬ÔÚÕâ¼´PIT´¥·¢ 
	 
}
int8_t adcjiaozhun(void)
{
unsigned short cal_var; 
  
  ADC0_SC2 &=  ~ADC_SC2_ADTRG_MASK ; // Enable Software Conversion Trigger for Calibration Process    - ADC0_SC2 = ADC0_SC2 | ADC_SC2_ADTRGW(0);   
  ADC0_SC3 &= ( ~ADC_SC3_ADCO_MASK & ~ADC_SC3_AVGS_MASK ); // set single conversion, clear avgs bitfield for next writing 
  ADC0_SC3 |= ( ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(32) );  // Turn averaging ON and set at max value ( 32 ) 
   
  ADC0_SC3 |= ADC_SC3_CAL_MASK ;      // Start CAL 
  while ( (ADC0_SC1(0) & ADC_SC1_COCO_MASK ) == 0); // Wait calibration end 
      
  if ((ADC0_SC3 & ADC_SC3_CALF_MASK) == ADC_SC3_CALF_MASK ) 
  {  
   return(1);    // Check for Calibration fail error and return 
  } 
  // Calculate plus-side calibration 
  cal_var = 0x00; 
  
  cal_var =  ADC0_CLP0; 
  cal_var += ADC0_CLP1; 
  cal_var += ADC0_CLP2; 
  cal_var += ADC0_CLP3; 
  cal_var += ADC0_CLP4; 
  cal_var += ADC0_CLPS;
  cal_var = cal_var/2; 
  cal_var |= 0x8000; // Set MSB
  ADC0_PG = ADC_PG_PG(cal_var); 
 
  // Calculate minus-side calibration 
  cal_var = 0x00;
   cal_var =  ADC0_CLM0; 
  cal_var += ADC0_CLM1; 
  cal_var += ADC0_CLM2; 
  cal_var += ADC0_CLM3; 
  cal_var += ADC0_CLM4; 
  cal_var += ADC0_CLMS;
  cal_var = cal_var/2;
  cal_var |= 0x8000; // Set MSB
  ADC0_MG = ADC_MG_MG(cal_var); 
  
  ADC0_SC3 &= ~ADC_SC3_CAL_MASK ; /* Clear CAL bit */
	return 1;
}
/*********************************************************************************************************
** Function name:           adcGetUint
** Descriptions:            adc¶ÁÈ¡
** input parameters:        none
** output parameters:       adcÊýÖµ
** Returned value:          none
** Created by:              JiaoLong Liu
** Created date:            2012-12-12
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
*********************************************************************************************************/
uint16_t  adcGetUint (void) 
{
//    ADC0_SC1(0) = ADC_SC1_ADCH(2);                                      /* Ñ¡ÔñADCÐÅµÀ                  */
    while ((ADC0_SC1(0) & ADC_SC1_COCO_MASK) != ADC_SC1_COCO_MASK);     /* µÈ´ý×ª»»Íê³É                 */
    return ADC0_R(0);                                                   /* Êä³ö½á¹û                     */
}



uint16_t  adcGetExterlPowerUint (void)                   /* ²âÁ¿1.25v»ù×¼µçÑ¹£¬µ«ÊÇÕâ¸öREF3012Êä³öµÄÊÇ1.33v×óÓÒ               */
{
     ADC0_SC1(0) = ADC_SC1_ADCH(23);                                      /* Ñ¡ÔñADCÐÅµÀ                  */
    while ((ADC0_SC1(0) & ADC_SC1_COCO_MASK) != ADC_SC1_COCO_MASK);     /* µÈ´ý×ª»»Íê³É                 */
    return ADC0_R(0);                                                   /* Êä³ö½á¹û                     */
}

uint16_t  adcGetPT1000 (void) 
{
    ADC0_SC1(0) = ADC_SC1_ADCH(11);                                      /* Ñ¡ÔñADCÐÅµÀ                  */
    while ((ADC0_SC1(0) & ADC_SC1_COCO_MASK) != ADC_SC1_COCO_MASK);     /* µÈ´ý×ª»»Íê³É                 */
    return ADC0_R(0);                                                   /* Êä³ö½á¹û                     */
}
uint16_t adcGetBattery(void)
{
	 ADC0_SC1(0) = ADC_SC1_ADCH(0);                                      /* Ñ¡ÔñADCÐÅµÀ                  */
   while ((ADC0_SC1(0) & ADC_SC1_COCO_MASK) != ADC_SC1_COCO_MASK);     /* µÈ´ý×ª»»Íê³É                 */
   return ADC0_R(0);  
}
/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/








