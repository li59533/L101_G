
 /**
  \mainpage 
  \n Copyright (c) 2011 Freescale
  \brief 
  \author   	Freescale 
  \author    
  \version      m.n
  \date         Feb 15, 2012
  
  Put here all the information needed of the Project. Basic configuration as well as information on
  the project definition will be very useful 
*/
/****************************************************************************************************/
/*                                                                                                  */
/* All software, source code, included documentation, and any implied know-how are property of      */
/* Freescale Semiconductor and therefore considered CONFIDENTIAL INFORMATION.                       */
/* This confidential information is disclosed FOR DEMONSTRATION PURPOSES ONLY.                      */
/*                                                                                                  */
/* All Confidential Information remains the property of Freescale Semiconductor and will not be     */
/* copied or reproduced without the express written permission of the Discloser, except for copies  */
/* that are absolutely necessary in order to fulfill the Purpose.                                   */
/*                                                                                                  */
/* Services performed by FREESCALE in this matter are performed AS IS and without any warranty.     */
/* CUSTOMER retains the final decision relative to the total design and functionality of the end    */
/* product.                                                                                         */
/* FREESCALE neither guarantees nor will be held liable by CUSTOMER for the success of this project.*/
/*                                                                                                  */
/* FREESCALE disclaims all warranties, express, implied or statutory including, but not limited to, */
/* implied warranty of merchantability or fitness for a particular purpose on any hardware,         */
/* software ore advise supplied to the project by FREESCALE, and or any product resulting from      */
/* FREESCALE services.                                                                              */
/* In no event shall FREESCALE be liable for incidental or consequential damages arising out of     */
/* this agreement. CUSTOMER agrees to hold FREESCALE harmless against any and all claims demands or */
/* actions by anyone on account of any damage,or injury, whether commercial, contractual, or        */
/* tortuous, rising directly or indirectly as a result of the advise or assistance supplied CUSTOMER*/ 
/* in connectionwith product, services or goods supplied under this Agreement.                      */
/*                                                                                                  */
/****************************************************************************************************/

/****************************************************************************************************/
/****************************************************************************************************/
/****************************************NOTES*******************************************************/
/****************************************************************************************************/
/****************************************************************************************************/
/* This project was designed to work on a TWR-KL25Z48M board, configured with PEE at 48Mhz               */
/*      Consult your tower board (or Freedom board) schematics for which jumper to use for current 
*       consumption measurements.  
*/
/****************************************************************************************************/

/*****************************************************************************************************
* Include files
*****************************************************************************************************/
#include "common.h"
#include "includes.h"
#include "stdio.h"
#include "app.h"
#include "modbus.h"
#include "mcg.h"




extern void main_app(void);

/*****************************************************************************************************
* Declaration of module wide TYPEs - NOT for use in other modules
*****************************************************************************************************/

/*****************************************************************************************************
* Definition of module wide VARIABLEs - NOT for use in other modules
*****************************************************************************************************/
unsigned char measureFlag = 0;
unsigned char enterVLLSOflag = 0;
unsigned long ram_data[216];

int mcg_clk_hz;
int mcg_clk_khz;
int core_clk_khz;
int uart0_clk_khz;

extern void enable_NMI_button(void);


int fast_irc_freq = 4000000; //4000000; // default fast irc frequency is 4MHz
int slow_irc_freq = 32768; // default slow irc frequency is 32768Hz

/*****************************************************************************************************
* Definition of module wide (CONST-) CONSTANTs - NOT for use in other modules
*****************************************************************************************************/
#define MCG_OUT_FREQ 48000000
#define CPO_LOOP_CNT 2
#define IO_NMI_DEF 1  // 1 - sets the Port interrupt, 0 - sets the NMI interrupt
/****************************************************************************************************/
//#define NOT_LOWEST_PWR 1    // define this to leave debugger enabled
//#define NOT_LOWEST_PWR_2 1  // define this to leave uart clock gate on
/*****************************************************************************************************
* Code of project wide FUNCTIONS
*****************************************************************************************************/


/*******************************************************************************************************/


void LPTMR_init(int count, int clock_source)
{
    SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;

#ifdef CMSIS
	  NVIC_EnableIRQ(LPTimer_IRQn);
#else
    enable_irq(LPTMR_irq_no);
#endif

    LPTMR0_PSR = ( LPTMR_PSR_PRESCALE(0) // 0000 is div 2
                 | LPTMR_PSR_PBYP_MASK  // LPO feeds directly to LPT
                 | LPTMR_PSR_PCS(clock_source)) ; // use the choice of clock
    if (clock_source== 0)
      printf("\n\r LPTMR Clock source is the MCGIRCLK \n\r");
    if (clock_source== 1)
      printf("\n\r LPTMR Clock source is the LPOCLK \n\r");
    if (clock_source== 2)
      printf("\n\r LPTMR Clock source is the ERCLK32 \n\r");
    if (clock_source== 3)
      printf("\n\r LPTMR Clock source is the OSCERCLK \n\r");
             
    LPTMR0_CMR = LPTMR_CMR_COMPARE(count);  //Set compare value

    LPTMR0_CSR =(  LPTMR_CSR_TCF_MASK   // Clear any pending interrupt
                 | LPTMR_CSR_TIE_MASK   // LPT interrupt enabled
                 | LPTMR_CSR_TPS(0)     //TMR pin select
                 |!LPTMR_CSR_TPP_MASK   //TMR Pin polarity
                 |!LPTMR_CSR_TFC_MASK   // Timer Free running counter is reset whenever TMR counter equals compare
                 |!LPTMR_CSR_TMS_MASK   //LPTMR0 as Timer
                );
    LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;   //Turn on LPT and start counting
}
/*******************************************************************************************************/
void llwu_configure(unsigned int pin_en, unsigned char rise_fall, unsigned char module_en ) {
   
    
    
    
    if (module_en == 0){
      LLWU_ME = 0;
    }else  {
    LLWU_ME |= module_en;  //Set up more modules to wakeup up
   // printf("\n LLWU configured modules as LLWU wakeup sources = 0x%02X,",(LLWU_ME));

    }
} // End LLWU Configuration



void LLWU_Init(void)
{
   
	  NVIC_EnableIRQ(LLW_IRQn);
		NVIC_EnableIRQ(LPTimer_IRQn);


     llwu_configure(0x0000, LLWU_PIN_FALLING, 0x1);
}



/* VLPR mode exit routine. Puts the processor into normal run mode
 * from VLPR mode. You can transition from VLPR to normal run using
 * this function.
 *
 * Mode transitions:
 * VLPR -> RUN
 *
 * Parameters:
 * none
 */
 /********************************************************************/

void exit_vlpr(void)
{
    int i;
    /* check to make sure in VLPR before exiting    */
    if  ((SMC_PMSTAT & SMC_PMSTAT_PMSTAT_MASK)== 4) {
      
       /* Clear RUNM */
       SMC_PMCTRL &= ~SMC_PMCTRL_RUNM_MASK;
                      
       /* Wait for normal RUN regulation mode to be confirmed */                   
       // 1 PMSTAT MCU is in RUN  mode
       // 4 PMSTAT MCU is in VLPR mode
       for (i=0;i<0xff;i++)
       {
           if ((SMC_PMSTAT & SMC_PMSTAT_PMSTAT_MASK)== SMC_PMSTAT_PMSTAT_MASK)
             if ((PMC_REGSC & PMC_REGSC_REGONS_MASK) == PMC_REGSC_REGONS_MASK)
    	        break;
       }           
    }  //if in VLPR mode
     // else if not in VLPR ignore call
}




int PEE_to_BLPE(void)
{
     int mcg_clock_hz;    
     
     mcg_clock_hz = atc(FAST_IRC,fast_irc_freq,MCG_OUT_FREQ);

     mcg_clock_hz = pee_pbe(CLK0_FREQ_HZ);
     mcg_clock_hz = pbe_blpe(CLK0_FREQ_HZ);


      return mcg_clock_hz; 
}


void clockMonitor(unsigned char state)
{
    if(state)
      MCG_C6 |= MCG_C6_CME0_MASK;
    else
      MCG_C6 &= ~MCG_C6_CME0_MASK;
}
  /*用来控制时钟的，开关*/

void vlp_clock_config(char next_mode)
{
  int current_mcg_mode,uart_clk_khz;
  unsigned char return_code;
  current_mcg_mode = what_mcg_mode();
  if ((SMC_PMSTAT & SMC_PMSTAT_PMSTAT_MASK)== 4){
     //  printf("\n\rIn VLPR Mode ! Must go to RUN to change Clock Modes --> ");
       exit_vlpr();
  }
  if ((SMC_PMSTAT & SMC_PMSTAT_PMSTAT_MASK)== 1)
   //   printf("In RUN Mode !\n\r");
  switch(current_mcg_mode){
              
  case 1:  //current_mcg_mode ==BLPI
   // printf("\n\r in BLPI mode with fast IRC \n\r");
    if(next_mode == FEI){
        mcg_clk_hz = blpi_fbi(slow_irc_freq, 0);
        mcg_clk_hz = fbi_fei(slow_irc_freq);
        MCG_C4 |= (MCG_C4_DRST_DRS(1) | MCG_C4_DMX32_MASK);
        mcg_clk_hz = 48000000; //FEI mode
        
        SIM_CLKDIV1 = (   SIM_CLKDIV1_OUTDIV1(0)
                    | SIM_CLKDIV1_OUTDIV4(1) );
       // uart_clk_khz = (mcg_clk_hz  / 1000); // the uart0 clock frequency will equal the FLL frequency
        /*Configure UART for the new clock frequency and IR clk*/
       // uart_configure(uart_clk_khz,SIM_SOPT2_UART0SRC(1));                  
        SIM_SOPT2 &= ~SIM_SOPT2_PLLFLLSEL_MASK; // clear PLLFLLSEL to select the FLL for this clock source
    }
    break;
  case 2:  // current_mcg_mode ==FBI 
   // printf("\n\r in FBI mode now \n\r");
    break;
  case 3:  // current_mcg_mode ==FEI) 
   // printf("\n\r in FEI mode ");
    if (next_mode == BLPE)
    {
        mcg_clk_hz =  fei_fbe( CLK0_FREQ_HZ,  1, CRYSTAL);
        mcg_clk_hz = fbe_blpe(CLK0_FREQ_HZ);   
        OSC0_CR |= OSC_CR_ERCLKEN_MASK;
       // uart_clk_khz = ((CLK0_FREQ_HZ ) / 1000); // UART0 clock frequency will equal ERCLK
    } else if (next_mode == BLPI)
    { //next_mode is BLPI
      MCG_SC &=  ~MCG_SC_FCRDIV_MASK;  //set to div by 1    
      mcg_clk_hz = fei_fbi(fast_irc_freq,FAST_IRC);
      mcg_clk_hz = fbi_blpi(fast_irc_freq,FAST_IRC);
      /* Internal Fast IRC is 4 MHz so div by 1 for sysclk and div 4 for flash clock*/
      SIM_CLKDIV1 = (   SIM_CLKDIV1_OUTDIV1(0)
                      | SIM_CLKDIV1_OUTDIV4(4) ); // div 5 for flash clk margin
     // uart_clk_khz = (mcg_clk_hz / 1000); // the uart0 clock frequency will equal the FAST IRC frequency
      MCG_C1 |= MCG_C1_IRCLKEN_MASK;
      MCG_C2 |= MCG_C2_IRCS_MASK;
    }
    /*Configure UART for the oscerclk frequency*/
    //uart_configure(uart_clk_khz,SIM_SOPT2_UART0SRC(3));                  
    /* External Reference is 8 MHz so div by 2 for sysclk and div 4 for flash clock*/
   // printf("\n\r moved to BLPE mode \n\r");
    break;
  case 4:  // current_mcg_mode ==4) 
    printf("\n\r in FEE mode \n\r");
    if (next_mode == BLPI){
        MCG_C2 &= ~MCG_C2_IRCS_MASK;
        MCG_C1 |= MCG_C1_IRCLKEN_MASK;
        MCG_SC &=  ~MCG_SC_FCRDIV_MASK;  //set to div by 1  
        mcg_clk_hz = fee_fbi(fast_irc_freq, FAST_IRC);
        mcg_clk_hz = fbi_blpi(fast_irc_freq,FAST_IRC);
        /* Internal Fast IRC is 4 MHz so div by 1 for sysclk and div 4 for flash clock*/
        SIM_CLKDIV1 = (   SIM_CLKDIV1_OUTDIV1(0)
                        | SIM_CLKDIV1_OUTDIV4(4) ); // div 5 for flash clk margin
       // uart_clk_khz = (mcg_clk_hz / 1000); // the uart0 clock frequency will equal the FAST IRC frequency
       // uart_configure(uart_clk_khz,SIM_SOPT2_UART0SRC(3));                  
        /* External Reference is 8 MHz so div by 2 for sysclk and div 4 for flash clock*/
       // printf("\n moved to BLPI mode \n");
    }
    else if (next_mode == BLPE){
        mcg_clk_hz =  fee_fbe( CLK0_FREQ_HZ);
        mcg_clk_hz = fbe_blpe(CLK0_FREQ_HZ);   
        OSC0_CR |= OSC_CR_ERCLKEN_MASK;
       // uart_clk_khz = ((CLK0_FREQ_HZ ) / 1000); // UART0 clock frequency will equal ERCLK
       // uart_configure(uart_clk_khz,SIM_SOPT2_UART0SRC(3));                  
        /* External Reference is 8 MHz so div by 2 for sysclk and div 4 for flash clock*/
       // printf("\n moved to BLPE mode \n");
    }
    
    break;
  case 5:  // current_mcg_mode ==FBE) 
   // printf("\n\r in FBE mode \n\r");
    break;
  case 6:  // current_mcg_mode ==BLPE) 
   // printf("\n\r in BLPE mode \n\r");
    if (next_mode == BLPI){
        MCG_C2 &= ~MCG_C2_IRCS_MASK;
        MCG_C1 |= MCG_C1_IRCLKEN_MASK;
        MCG_SC &=  ~MCG_SC_FCRDIV_MASK;  //set to div by 1  
        mcg_clk_hz = blpe_fbe(CLK0_FREQ_HZ);
        mcg_clk_hz = fbe_fbi(fast_irc_freq, FAST_IRC);
        mcg_clk_hz = fbi_blpi(fast_irc_freq,FAST_IRC);
        /* Internal Fast IRC is 4 MHz so div by 1 for sysclk and div 4 for flash clock*/
        SIM_CLKDIV1 = (   SIM_CLKDIV1_OUTDIV1(0)
                        | SIM_CLKDIV1_OUTDIV4(4) ); // div 5 for flash clk margin
       // uart_clk_khz = (mcg_clk_hz / 1000); // the uart0 clock frequency will equal the FAST IRC frequency
       // uart_configure(uart_clk_khz,SIM_SOPT2_UART0SRC(3));                  
        /* External Reference is 8 MHz so div by 2 for sysclk and div 4 for flash clock*/
       // printf("\n\r moved to BLPI mode \n\r");
    }else if (next_mode == PEE){
       SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0)|
                     SIM_CLKDIV1_OUTDIV4(0x1);     
       //After wake up back to the original clock frequency          
       mcg_clk_hz = blpe_pbe(CLK0_FREQ_HZ, PLL0_PRDIV,PLL0_VDIV);
       mcg_clk_hz = pbe_pee(CLK0_FREQ_HZ);
       //Configure UART for the oscerclk frequency
     //  uart0_clk_khz = ((mcg_clk_hz / 2) / 1000); 
     //  uart_configure(uart0_clk_khz,SIM_SOPT2_UART0SRC(1));      
     //  printf("moved to PEE clock mode \n\r");
    }    
    break;
  case 7:  // current_mcg_mode ==PBE) 
   // printf("\n\r in PBE mode \n\r");
    if (next_mode == PEE){
       SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0)|
                     SIM_CLKDIV1_OUTDIV4(0x1);     
       /*After wake up back to the original clock mode*/            
       mcg_clk_hz = pbe_pee(CLK0_FREQ_HZ);
       /*Configure UART for the oscerclk frequency*/
     //  uart_configure(mcg_clk_hz,SIM_SOPT2_UART0SRC(1));      
    }
  //  printf("moved to PEE clock mode \n\r");

    break;
  case 8:  // current_mcg_mode ==8) PEE
    if (next_mode == BLPI)
    {
      // run ATC test to determine irc trim
        return_code = atc(FAST_IRC,fast_irc_freq,mcg_clk_hz);
        if (return_code != 0){
     //     printf("\n\rAutotrim Failed\n\r");
        }
        mcg_clk_hz =  pee_pbe(CRYSTAL);
        mcg_clk_hz = pbe_fbe(CRYSTAL);   
        MCG_SC &=  ~MCG_SC_FCRDIV_MASK;  //set to div by 1    
        mcg_clk_hz = fbe_fbi(fast_irc_freq, FAST_IRC);
        mcg_clk_hz = fbi_blpi(fast_irc_freq, FAST_IRC);
        uart_clk_khz = ((mcg_clk_hz ) / 1000); // UART0 clock frequency will equal ERCLK
        SIM_CLKDIV1 = (   SIM_CLKDIV1_OUTDIV1(0)
                        | SIM_CLKDIV1_OUTDIV4(4) );// div 5 for flash clk margin
        MCG_C1 |= MCG_C1_IRCLKEN_MASK;
        MCG_C2 |= MCG_C2_IRCS_MASK;
        clockMonitor(OFF);
     //   uart_configure(uart_clk_khz,SIM_SOPT2_UART0SRC(3));// use IRCLK
    
    //    printf("\n\r Now moved to BLPI clock mode \n\r");
    } else
    {
      printf("\n\r in PEE clock mode moving to BLPE clock mode \n\r");
    return_code = atc(FAST_IRC,fast_irc_freq,mcg_clk_hz);
    if (return_code != 0){
          printf("\n\rAutotrim Failed\n\r");
    }
    mcg_clk_hz = PEE_to_BLPE();
    OSC0_CR |= OSC_CR_ERCLKEN_MASK;
    /*Configure UART for the oscerclk frequency*/
   // uart_clk_khz = ((fast_irc_freq ) / 1000); // UART0 clock frequency will equal fast irc
    /* External Reference is 8 MHz so div by 2 for sysclk and div 4 for flash clock*/
    SIM_CLKDIV1 = (   SIM_CLKDIV1_OUTDIV1(1)
                    | SIM_CLKDIV1_OUTDIV4(3) );            
    mcg_clk_hz =  mcg_clk_hz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV1_MASK) >> 28)+ 1);    
    MCG_C1 |= MCG_C1_IRCLKEN_MASK; // enable irc
    MCG_C2 |= MCG_C2_IRCS_MASK;    // select fast irc  
    MCG_SC &=  ~MCG_SC_FCRDIV_MASK;  //set to div by 1    
   // uart_configure(uart_clk_khz,SIM_SOPT2_UART0SRC(3));// use MCGIRCLK
   // printf("\n\r Now moved to BLPE clock mode \n\r");
    }
    break;
   /* end of case statement*/
  }
}
 

/***********************************************************/
int enter_vlpr(void)
{
  int i; 
  unsigned int return_value = 0;  //default return value = indicates error
  if ((SMC_PMSTAT & SMC_PMSTAT_PMSTAT_MASK)== 4){
         return_value = 0x14;
         }
  /* The PMPROT register may have already been written by init code
     If so then this next write is not done.  
     PMPROT is write once after RESET  
     this write-once bit allows the MCU to enter the
     very low power modes: VLPR, VLPW, and VLPS   */
  SMC_PMPROT = SMC_PMPROT_AVLP_MASK;  
    
  /* Set the (for MC1)LPLLSM or (for MC2)STOPM field 
     to 0b010 for VLPS mode -
     and RUNM bits to 0b010 for VLPR mode  */
  SMC_PMCTRL &= ~SMC_PMCTRL_RUNM_MASK; 
  SMC_PMCTRL  |= SMC_PMCTRL_RUNM(0x2);
  /* Wait for VLPS regulator mode to be confirmed */
  for (i = 0 ; i < 10000 ; i++)
    {     /* check that the value of REGONS bit is not 0
             once it is a zero we can stop checking */
      if ((PMC_REGSC & PMC_REGSC_REGONS_MASK) ==0x04){
       /* 0 Regulator is in stop regulation or in transition 
            to/from it
          1 MCU is in Run regulation mode */
      }  
      else  break;
    }  
  if ((PMC_REGSC & PMC_REGSC_REGONS_MASK) ==0x04) 
    {  
      return_value = 0x24; 
    }
  /* SMC_PMSTAT register only exist in Mode Controller 2 MCU versions */
  if ((SMC_PMSTAT & SMC_PMSTAT_PMSTAT_MASK) == 4)  
    {
      return_value = SMC_PMSTAT;
    }
  return (return_value);
}
/********************************************************************/
/* VLPR mode exit routine. Puts the processor into normal run mode
 * from VLPR mode. You can transition from VLPR to normal run using
 * this function.
 *
 * Mode transitions:
 * VLPR -> RUN
 *
 * Parameters:
 * none
 */
 /********************************************************************/


/***************************************************************/
/* VLPS mode entry routine. Puts the processor into VLPS mode 
 * directly from run or VLPR modes. 
 *
 * Mode transitions:
 * RUN  -> VLPS
 * VLPR -> VLPS
 *
 * Note, when VLPS is entered directly from RUN or VLPR mode, 
 * exit to VLPR is disabled by hardware and the system will
 * always exit back to RUN.
 *
 * Parameters:  
 * none
 */
 /****************************************************************/

void enter_vlps(void)
{
  volatile unsigned int dummyread;
  /* The PMPROT register may have already been written by init code
     If so then this next write is not done since  
     PMPROT is write once after RESET 
     allows the MCU to enter the VLPR, VLPW, and VLPS modes.
     If AVLP is already writen to 0 
     Stop is entered instead of VLPS*/
  SMC_PMPROT = SMC_PMPROT_AVLP_MASK;           
  /* Set the STOPM field to 0b010 for VLPS mode */
  SMC_PMCTRL &= ~SMC_PMCTRL_STOPM_MASK; 
  SMC_PMCTRL |=  SMC_PMCTRL_STOPM(0x2); 
  /*wait for write to complete to SMC before stopping core */  
  dummyread = SMC_PMCTRL;
  dummyread++;
  /* Now execute the stop instruction to go into VLPS */
  #ifdef CMSIS
  /* Set the SLEEPDEEP bit to enable deep sleep mode (STOP) */
  SCB_SCR |= SCB_SCR_SLEEPDEEP_MASK;
  __wfi();
#else
  stop();
#endif
}
/****************************************************************/	
void uart_init (UART_MemMapPtr uartch, int sysclk, int baud)
{
    register uint16 sbr;
    uint8 temp;
                                
         
      if (uartch == UART1_BASE_PTR)
        SIM_SCGC4 |= SIM_SCGC4_UART1_MASK;
      else
    	SIM_SCGC4 |= SIM_SCGC4_UART2_MASK;
      /* Make sure that the transmitter and receiver are disabled while we 
       * change settings.
       */
      UART_C2_REG(uartch) &= ~(UART_C2_TE_MASK
				| UART_C2_RE_MASK );

      /* Configure the uart for 8-bit mode, no parity */
      UART_C1_REG(uartch) = 0;	/* We need all default settings, so entire register is cleared */
    
      /* Calculate baud settings */
      sbr = (uint16)((sysclk*1000)/(baud * 16));
        
      /* Save off the current value of the uartx_BDH except for the SBR field */
      temp = UART_BDH_REG(uartch) & ~(UART_BDH_SBR(0x1F));
    
      UART_BDH_REG(uartch) = temp |  UART_BDH_SBR(((sbr & 0x1F00) >> 8));
      UART_BDL_REG(uartch) = (uint8)(sbr & UART_BDL_SBR_MASK);
  
      /* Enable receiver and transmitter */
      UART_C2_REG(uartch) |= (UART_C2_TE_MASK
	    		  | UART_C2_RE_MASK );
    
}

void uart_configure(int clock_khz,int uart0_clk_src )
{
     int mcg_clock_khz;
     int core_clock_khz;
     
    
     
    
     mcg_clock_khz = clock_khz / 1000;
     core_clock_khz = mcg_clock_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV1_MASK) >> 28)+ 1);
     uart_init (UART2_BASE_PTR, core_clock_khz, 9600);
     
}


/****************************************************************/
void set_4_lp(unsigned char outof_lp)
{
    if(outof_lp)
    {
        
        {
           SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;
  	   /* Enable the pins for the selected SCI */
           /* Enable the SCI1_TXD function on PTC4 */
  	   PORTA_PCR14 = PORT_PCR_MUX(0x3); // LPSCI is alt3 function for this pin
  		
  	   /* Enable the SCI1_RXD function on PTC3 */
  	   PORTA_PCR15 = PORT_PCR_MUX(0x3); // LPSCI is alt3 function for this pin
          
           SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1); // select the PLLFLLCLK as UART0 clock source
        }
      PORTA_PCR0 =  PORT_PCR_MUX(7); // Enable Debug 
      PORTA_PCR3 =  PORT_PCR_MUX(7); // Enable Debug  
      // re-init ports
     
    
    }
}
void LowPowerModes_test(void)
{
  int i;
  unsigned char op_mode,fcrdiv_val;  
  int test_num = 0;
  int wait_count;
  char test_val;
  
  printf("\n\r*------------------------------------------------------------*");
  printf("\n\r*                    KL Low Power DEMO                       *");
  printf("\n\r*                    %s %s                    *", __DATE__, __TIME__);
  printf("\n\r*------------------------------------------------------------*\n\r");
                       
	while(1)
	{
		            if ((SMC_PMSTAT & SMC_PMSTAT_PMSTAT_MASK)== 4){
    	            printf("  in VLPR Mode !  ");
                } else if ((SMC_PMSTAT & SMC_PMSTAT_PMSTAT_MASK)== 1){
    	            printf("  in Run Mode  !  ");
                }
                op_mode = what_mcg_mode();
                if (op_mode ==1) 
                    printf("in BLPI mode now at %d Hz\r\n",mcg_clk_hz );
                if (op_mode ==2) 
                    printf(" in FBI mode now at %d Hz\r\n",mcg_clk_hz );
                if (op_mode ==3) 
                    printf(" in FEI mode now at %d Hz\r\n",mcg_clk_hz );
                if (op_mode ==4) 
                    printf(" in FEE mode now at %d Hz\r\n",mcg_clk_hz );
                if (op_mode ==5) 
                    printf(" in FBE mode now at %d Hz\r\n",mcg_clk_hz );
                if (op_mode ==6) 
                    printf(" in BLPE mode now at %d Hz\r\n",mcg_clk_hz );
                if (op_mode ==7) 
                    printf(" in PBE mode now at %d Hz\r\n",mcg_clk_hz );
                if (op_mode ==8) 
                    printf(" in PEE mode now at %d Hz\r\n",mcg_clk_hz );
	              exit_vlpr();  // get out of VLPR back to RUN
                vlp_clock_config(BLPI);
                // default clock divider is set to divide by 1 of Fast IRC and Div2 for Bus
                SIM_CLKDIV1 = (   SIM_CLKDIV1_OUTDIV1(1)
                                | SIM_CLKDIV1_OUTDIV4(2) ); 
                
                fcrdiv_val = (1 << ((MCG_SC & MCG_SC_FCRDIV_MASK) >> MCG_SC_FCRDIV_SHIFT)); // calculate the fast IRC divder factor
                mcg_clk_hz =  (fast_irc_freq / fcrdiv_val); // MCGOUT frequency equals fast IRC frequency divided by 2

                mcg_clk_hz =  mcg_clk_hz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV1_MASK) >> 28)+ 1);      
                /*Go to VLPR*/
                enter_vlpr();   // get into VLPR
								if ((SMC_PMSTAT & SMC_PMSTAT_PMSTAT_MASK)== 4){
    	            printf("  in VLPR Mode !\n\r ");
                } 
		            time_delay_ms(5000);
/*进入VLPR模式，给电池冲放电***************************/

/*从VLPR进入RUN Set PMCTRL[RUNM]=00 or Reset************/
                 exit_vlpr();
								 vlp_clock_config(FEI);
								 uart2Init(9600,0,0,8,1); 
		             printf("ENTER RUN MODLE\n\r "); 
                 time_delay_ms(10000);	
		             printf("RUN MODLE OVER\n\r "); 
		
/*进入LLS模式休眠**********************************/
                 clockMonitor(OFF);
                 llwu_configure(0x0000, LLWU_PIN_FALLING, 0x1);
                 LPTMR_init(50000,LPTMR_USE_LPOCLK);
                /*Go to VLPS*/
                 enter_vlps();  //1 Means: Any interrupt could wake up from this mode   
                 op_mode = what_mcg_mode();								
                 if ((SMC_PMSTAT & SMC_PMSTAT_PMSTAT_MASK)== 4){
    	            printf("  in VLPR Mode !\n\r ");
									SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;
                  LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK;
                 } else if ((SMC_PMSTAT & SMC_PMSTAT_PMSTAT_MASK)== 1){
    	            op_mode = what_mcg_mode(); // check if in PBE mode and if so, switch to PEE
                    if(op_mode==PBE)
                    {
                      mcg_clk_hz = pbe_pee(CLK0_FREQ_HZ);
                      uart0_clk_khz = ((mcg_clk_hz / 2) / 1000); // UART0 clock frequency will equal half the PLL frequency
                      uart_configure(uart0_clk_khz,SIM_SOPT2_UART0SRC(1));
                      printf("  in Run Mode  !\n\r ");                  
                    }
										SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;
                    LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK;
                    clockMonitor(ON);
                }
		
      
                
                
             
              
            
                
              
                
              
             

	} 
}

int main2 (void)
{
        /*Enable all operation modes because this is a write once register*/  
       SMC_PMPROT =  SMC_PMPROT_AVLLS_MASK |
                      SMC_PMPROT_ALLS_MASK  |    
                      SMC_PMPROT_AVLP_MASK;
	
    
       	SystemCoreClockUpdate();

         SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK
                      | SIM_SCGC5_PORTB_MASK
                      | SIM_SCGC5_PORTC_MASK
                      | SIM_SCGC5_PORTD_MASK
                      | SIM_SCGC5_PORTE_MASK );
        /*PTC3(SW2) is configured to wake up MCU from VLLSx and LLS modes, Interrup is ne*/
        LLWU_Init();
        uart2Init(9600,0,0,8,1); 
         __enable_irq();
	      printf("  begin testt  !\n\r "); 
	      mcg_clk_hz = SystemCoreClock;
         /*Start test*/
        LowPowerModes_test();
}
