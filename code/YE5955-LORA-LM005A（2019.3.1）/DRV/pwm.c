#include "..\..\App\includes.h"

/*********************************************************************************************************
** Function name:           gpioInit
** Descriptions:            gpio的初始化
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:             
** Created date:            
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
*********************************************************************************************************/

#define TPM_MODULE    600
#define TPM_INIT_VAL   200
#define TPM_Cn_MODE  (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK)

/*********************************************************************************************************
** Function name:           tpmInit
** Descriptions:            ptm的初始化 
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              
** Created date:            
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
*********************************************************************************************************/
void pwmInit(void) 
{
    																																				 
		SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK ;            /* 打开GPIO时钟                */
	  PORTB_PCR0 |= (PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK);               /* 开启PB0管脚GPIO功能 D2* TP0CH5*/
	  FGPIOB_PSOR |= 1<<0;                                               /* 配置GPIO 置高                */
    FGPIOB_PDDR |= 1<<0;                                               /* 配置GPIO为输出               */
	
    SIM_SCGC6 |= SIM_SCGC6_TPM1_MASK ;                /* 打开TPM时钟                 */
    SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK | SIM_SOPT2_TPMSRC(1);         /* 打开时钟门                  */
    
	
    //PORTB_PCR18 = ( 0 | PORT_PCR_MUX(3));                                /* TPM2_CH0 enable on PTB18    */
    //PORTB_PCR19 = ( 0 | PORT_PCR_MUX(3));                                /* TPM2_CH1 enable on PTB19    */
    PORTB_PCR0  = ( 0 | PORT_PCR_MUX(3));                                /* TPM1_CH0 enable on PA0的第三功能     */

    TPM1_MOD  = TPM_MODULE;                                              /* 1999 / 50MHz = 40uS PWM     */
    TPM1_C0SC = TPM_Cn_MODE;                                             /* No Interrupts; High True 
                                                                             pulses on Edge Aligned PWM */
    TPM1_C0V  = TPM_INIT_VAL;                                            /* 90% pulse width             */
            

    
    TPM1_SC   = TPM_SC_CMOD(1) | TPM_SC_PS(0);                          /* Edge Aligned PWM running from 
                                                                           BUSCLK / 1                   */
}
