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
** File name:               MKL_gpio.c
** Last modified date:     
** Last version:            V1.0
** Descriptions:            gpioÇý¶¯³ÌÐò
**
* Modified by:        
** Modified date:      
** Version:            
** Descriptions:       
**
*********************************************************************************************************/
#include "includes.h"

/*********************************************************************************************************
** Function name:           gpioInit
** Descriptions:            gpioµÄ³õÊ¼»¯
** input parameters:        none
** output parameters:       none
** Returned value:          none

**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
*********************************************************************************************************/
void gpioInit(void) 
{
    /*
     *  ¿ªÆôPB PD PE PC GPIOÊ±ÖÓ
     */
    SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK | SIM_SCGC5_PORTC_MASK;
    
    PORTB_PCR18 |= (PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK);               /* ¿ªÆôPB18¹Ü½ÅGPIO¹¦ÄÜ Red LED */
    PORTB_PCR19 |= (PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK);               /* ¿ªÆôPB19¹Ü½ÅGPIO¹¦ÄÜ GREENLED*/
    PORTD_PCR1  |= (PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK);               /* ¿ªÆôPD1¹Ü½ÅGPIO¹¦ÄÜ  BLUE LED*/
	
	  PORTD_PCR0  |= (PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK);               /* ¿ªÆôPD0¹Ü½ÅGPIO¹¦ÄÜ  TF_nCD*/
    /*
     *  SPIµÄ¹Ü½Å³õÊ¼»¯
     */
    PORTC_PCR4  |= PORT_PCR_MUX(1);                                     /* ¿ªÆôPC4¹Ü½ÅGPIO¹¦ÄÜ SPI0 CS0 */
	  PORTB_PCR0  |= PORT_PCR_MUX(1);                                     /* ¿ªÆôPC4¹Ü½ÅGPIO¹¦ÄÜ SPI0 CS0 */
    PORTC_PCR5  |= PORT_PCR_MUX(2);                                     /* ¿ªÆôPC5¹Ü½ÅSPI0¹¦ÄÜ SPI0 SCK */
    PORTC_PCR6  |= PORT_PCR_MUX(2);                                     /* ¿ªÆôPC6¹Ü½ÅSPI0¹¦ÄÜ SPI0 MOSI*/
    PORTC_PCR7  |= PORT_PCR_MUX(2);                                     /* ¿ªÆôPC7¹Ü½ÅSPI0¹¦ÄÜ SPI0 MISO*/
    FGPIOC_PSOR |= 1<<4;                                                /* ÅäÖÃCS0ÖÃ¸ß                  */
    FGPIOC_PDDR |= 1<<4;                                                /* ÅäÖÃCS0ÎªÊä³ö                */
  	FGPIOB_PSOR |= 1<<0;                                                /* ÅäÖÃCS1ÖÃ¸ß                  */
    FGPIOB_PDDR |= 1<<0;                                                /* ÅäÖÃCS1ÎªÊä³ö                */
    
    /*
     *  ¿ªÆôPE4¹Ü½ÅGPIO¹¦ÄÜ  °´¼üÊäÈëÉÏÀ­
     */
    PORTA_PCR4  |= (PORT_PCR_MUX(1) | PORT_PCR_PE_MASK);
    
    FGPIOB_PSOR |= 1<<18;                                               /* ÅäÖÃGPIO ÖÃ¸ß                */
    FGPIOB_PDDR |= 1<<18;                                               /* ÅäÖÃGPIOÎªÊä³ö               */
    FGPIOB_PSOR |= 1<<19;                                               /* ÅäÖÃGPIO ÖÃ¸ß                */
    FGPIOB_PDDR |= 1<<19;                                               /* ÅäÖÃGPIOÎªÊä³ö               */
    FGPIOD_PSOR |= 1<<1;                                                /* ÅäÖÃGPIO ÖÃ¸ß                */
    FGPIOD_PDDR |= 1<<1;                                                /* ÅäÖÃGPIOÎªÊä³ö               */
	
    FGPIOD_PDDR &= ~(1<<0);                                                /* ÅäÖÃGPIOÎªÊäÈë               */
    
    FGPIOA_PDDR &= ~(1<<4);                                             /* ÅäÖÃGPIOÎªÊäÈë               */
}

