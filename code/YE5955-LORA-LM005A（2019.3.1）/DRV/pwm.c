#include "..\..\App\includes.h"

/*********************************************************************************************************
** Function name:           gpioInit
** Descriptions:            gpio�ĳ�ʼ��
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
void pwmInit(void) 
{
    																																				 
		SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK ;            /* ��GPIOʱ��                */
	  PORTB_PCR0 |= (PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK);               /* ����PB0�ܽ�GPIO���� D2* TP0CH5*/
	  FGPIOB_PSOR |= 1<<0;                                               /* ����GPIO �ø�                */
    FGPIOB_PDDR |= 1<<0;                                               /* ����GPIOΪ���               */
	
    SIM_SCGC6 |= SIM_SCGC6_TPM1_MASK ;                /* ��TPMʱ��                 */
    SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK | SIM_SOPT2_TPMSRC(1);         /* ��ʱ����                  */
    
	
    //PORTB_PCR18 = ( 0 | PORT_PCR_MUX(3));                                /* TPM2_CH0 enable on PTB18    */
    //PORTB_PCR19 = ( 0 | PORT_PCR_MUX(3));                                /* TPM2_CH1 enable on PTB19    */
    PORTB_PCR0  = ( 0 | PORT_PCR_MUX(3));                                /* TPM1_CH0 enable on PA0�ĵ�������     */

    TPM1_MOD  = TPM_MODULE;                                              /* 1999 / 50MHz = 40uS PWM     */
    TPM1_C0SC = TPM_Cn_MODE;                                             /* No Interrupts; High True 
                                                                             pulses on Edge Aligned PWM */
    TPM1_C0V  = TPM_INIT_VAL;                                            /* 90% pulse width             */
            

    
    TPM1_SC   = TPM_SC_CMOD(1) | TPM_SC_PS(0);                          /* Edge Aligned PWM running from 
                                                                           BUSCLK / 1                   */
}
