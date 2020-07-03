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
** File name:               MKL_adc.c
** Last modified date:      2012-12-12
** Last version:            V1.0
** Descriptions:            adc��������
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoLong Liu
** Created date:            2012-12-12
** Version:                 V0.9
** Descriptions:            ADC��ʼ�� ��ʼ�汾
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
** Descriptions:            adc�ĳ�ʼ��
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
    SIM_SCGC5  |= SIM_SCGC5_PORTE_MASK;                    /* ����PE GPIOʱ��         */
    PORTE_PCR18 = PORT_PCR_MUX(0);                         /* ����PE18�ܽ�ģ�⹦��    */
	PORTC_PCR2  = PORT_PCR_MUX(0);            //1.25V      /* ����PC2�ܽ�ģ�⹦��     */
    SIM_SCGC6  |= SIM_SCGC6_ADC0_MASK;                     /* ����ADCʱ��             */
    /*
     * ����ADC��ع���
     */
    ADC0_CFG1 = ADC_CFG1_ADIV(2)|ADC_CFG1_ADLSMP_MASK | ADC_CFG1_MODE(3) | ADC_CFG1_ADICLK(0);
	ADC0_CFG2 = ADC_CFG2_ADLSTS(0);                     // long sample time select :20 extra ADCK cycles,24 ADCK cycles total
    
	ADC0_SC1(0) &=(~0x1f);                                //��0����ΪĬ��ȫ��1
	ADC0_SC1(0) |= ADC_SC1_ADCH(2);  // 2                 //����2ͨ��
	ADC0_SC1(0) |= ADC_SC1_AIEN_MASK;                      //����ADC�ж�
	
    //ADC0_SC3 = ADC_SC3_AVGE_MASK                          // hardware average enable
    //         | ADC_SC3_AVGS(3)                            // 32 samples average
    //         | ADC_SC3_ADCO_MASK;                         // continuous conversion
    //ADC0_SC2 |= ADC_SC2_REFSEL(0);                                      /* ѡ���Դ�ο�Դ               */
    ADC0_SC2 |= (ADC_SC2_REFSEL(0)                        //ѡ��оƬ��VREFH��VREFL����������ΪADת���Ĳο���ѹ
				|ADC_SC2_ADTRG_MASK);                     //Ӳ�����������⼴PIT���� 
	 
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
** Descriptions:            adc��ȡ
** input parameters:        none
** output parameters:       adc��ֵ
** Returned value:          none
** Created by:              JiaoLong Liu
** Created date:            2012-12-12
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
*********************************************************************************************************/
uint16_t  adcGetUint (void) 
{
//    ADC0_SC1(0) = ADC_SC1_ADCH(2);                                      /* ѡ��ADC�ŵ�                  */
    while ((ADC0_SC1(0) & ADC_SC1_COCO_MASK) != ADC_SC1_COCO_MASK);     /* �ȴ�ת�����                 */
    return ADC0_R(0);                                                   /* ������                     */
}



uint16_t  adcGetExterlPowerUint (void)                   /* ����1.25v��׼��ѹ���������REF3012�������1.33v����               */
{
     ADC0_SC1(0) = ADC_SC1_ADCH(23);                                      /* ѡ��ADC�ŵ�                  */
    while ((ADC0_SC1(0) & ADC_SC1_COCO_MASK) != ADC_SC1_COCO_MASK);     /* �ȴ�ת�����                 */
    return ADC0_R(0);                                                   /* ������                     */
}

uint16_t  adcGetPT1000 (void) 
{
    ADC0_SC1(0) = ADC_SC1_ADCH(11);                                      /* ѡ��ADC�ŵ�                  */
    while ((ADC0_SC1(0) & ADC_SC1_COCO_MASK) != ADC_SC1_COCO_MASK);     /* �ȴ�ת�����                 */
    return ADC0_R(0);                                                   /* ������                     */
}
uint16_t adcGetBattery(void)
{
	 ADC0_SC1(0) = ADC_SC1_ADCH(0);                                      /* ѡ��ADC�ŵ�                  */
   while ((ADC0_SC1(0) & ADC_SC1_COCO_MASK) != ADC_SC1_COCO_MASK);     /* �ȴ�ת�����                 */
   return ADC0_R(0);  
}
/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/








