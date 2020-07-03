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
** File name:               MKL_gpio.c
** Last modified date:     
** Last version:            V1.0
** Descriptions:            gpio��������
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
** Descriptions:            gpio�ĳ�ʼ��
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
     *  ����PB PD PE PC GPIOʱ��
     */
    SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK | SIM_SCGC5_PORTC_MASK;
    
    PORTB_PCR18 |= (PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK);               /* ����PB18�ܽ�GPIO���� Red LED */
    PORTB_PCR19 |= (PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK);               /* ����PB19�ܽ�GPIO���� GREENLED*/
    PORTD_PCR1  |= (PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK);               /* ����PD1�ܽ�GPIO����  BLUE LED*/
	
	  PORTD_PCR0  |= (PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK);               /* ����PD0�ܽ�GPIO����  TF_nCD*/
    /*
     *  SPI�Ĺܽų�ʼ��
     */
    PORTC_PCR4  |= PORT_PCR_MUX(1);                                     /* ����PC4�ܽ�GPIO���� SPI0 CS0 */
	  PORTB_PCR0  |= PORT_PCR_MUX(1);                                     /* ����PC4�ܽ�GPIO���� SPI0 CS0 */
    PORTC_PCR5  |= PORT_PCR_MUX(2);                                     /* ����PC5�ܽ�SPI0���� SPI0 SCK */
    PORTC_PCR6  |= PORT_PCR_MUX(2);                                     /* ����PC6�ܽ�SPI0���� SPI0 MOSI*/
    PORTC_PCR7  |= PORT_PCR_MUX(2);                                     /* ����PC7�ܽ�SPI0���� SPI0 MISO*/
    FGPIOC_PSOR |= 1<<4;                                                /* ����CS0�ø�                  */
    FGPIOC_PDDR |= 1<<4;                                                /* ����CS0Ϊ���                */
  	FGPIOB_PSOR |= 1<<0;                                                /* ����CS1�ø�                  */
    FGPIOB_PDDR |= 1<<0;                                                /* ����CS1Ϊ���                */
    
    /*
     *  ����PE4�ܽ�GPIO����  ������������
     */
    PORTA_PCR4  |= (PORT_PCR_MUX(1) | PORT_PCR_PE_MASK);
    
    FGPIOB_PSOR |= 1<<18;                                               /* ����GPIO �ø�                */
    FGPIOB_PDDR |= 1<<18;                                               /* ����GPIOΪ���               */
    FGPIOB_PSOR |= 1<<19;                                               /* ����GPIO �ø�                */
    FGPIOB_PDDR |= 1<<19;                                               /* ����GPIOΪ���               */
    FGPIOD_PSOR |= 1<<1;                                                /* ����GPIO �ø�                */
    FGPIOD_PDDR |= 1<<1;                                                /* ����GPIOΪ���               */
	
    FGPIOD_PDDR &= ~(1<<0);                                                /* ����GPIOΪ����               */
    
    FGPIOA_PDDR &= ~(1<<4);                                             /* ����GPIOΪ����               */
}

