#include "includes.h"
#include "MKL25Z4.h"
#include "stdio.h"
#include "powerswitch.h"

int32_t fast_irc_freq=4000000;// default fast irc frequency is 4MHz
int32_t slow_irc_freq=32768; // default slow irc frequency is 32768Hz
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


/*这部分用来判断目前MCG时钟处于哪个模式*/
unsigned char what_mcg_mode(void)
{
  // check if in FEI mode
  if ((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x0) &&      // check CLKS mux has selcted FLL output
      (MCG_S & MCG_S_IREFST_MASK) &&                                     // check FLL ref is internal ref clk
      (!(MCG_S & MCG_S_PLLST_MASK)))                                     // check PLLS mux has selected FLL
  {
    return FEI;                                                          // return FEI code
  }
  // Check MCG is in PEE mode
  else if ((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x3) && // check CLKS mux has selcted PLL output
          (!(MCG_S & MCG_S_IREFST_MASK)) &&                              // check FLL ref is external ref clk
          (MCG_S & MCG_S_PLLST_MASK))                                    // check PLLS mux has selected PLL 
  {
    return PEE;                                                          // return PEE code
  }
  // Check MCG is in PBE mode
  else if ((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x2) && // check CLKS mux has selcted external reference
          (!(MCG_S & MCG_S_IREFST_MASK)) &&                              // check FLL ref is external ref clk
          (MCG_S & MCG_S_PLLST_MASK) &&                                  // check PLLS mux has selected PLL
          (!(MCG_C2 & MCG_C2_LP_MASK)))                                  // check MCG_C2[LP] bit is not set
  {
    return PBE;                                                          // return PBE code
  }
  // Check MCG is in FBE mode
  else if ((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x2) && // check CLKS mux has selcted external reference
          (!(MCG_S & MCG_S_IREFST_MASK)) &&                              // check FLL ref is external ref clk
          (!(MCG_S & MCG_S_PLLST_MASK)) &&                               // check PLLS mux has selected FLL
          (!(MCG_C2 & MCG_C2_LP_MASK)))                                  // check MCG_C2[LP] bit is not set   
  {
    return FBE;                                                          // return FBE code
  }
  // Check MCG is in BLPE mode
  else if ((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x2) && // check CLKS mux has selcted external reference
          (!(MCG_S & MCG_S_IREFST_MASK)) &&                              // check FLL ref is external ref clk
          (MCG_C2 & MCG_C2_LP_MASK))                                     // check MCG_C2[LP] bit is set   
  {
    return BLPE;                                                         // return BLPE code
  }
  // check if in BLPI mode
  else if ((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x1) && // check CLKS mux has selcted int ref clk
          (MCG_S & MCG_S_IREFST_MASK) &&                                 // check FLL ref is internal ref clk
          (!(MCG_S & MCG_S_PLLST_MASK)) &&                               // check PLLS mux has selected FLL
          (MCG_C2 & MCG_C2_LP_MASK))                                     // check LP bit is set
  {
    return BLPI;                                                         // return BLPI code
  }
  // check if in FBI mode
  else if ((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x1) && // check CLKS mux has selcted int ref clk
          (MCG_S & MCG_S_IREFST_MASK) &&                                 // check FLL ref is internal ref clk
          (!(MCG_S & MCG_S_PLLST_MASK)) &&                               // check PLLS mux has selected FLL
          (!(MCG_C2 & MCG_C2_LP_MASK)))                                  // check LP bit is clear
  {  
    return FBI;                                                          // return FBI code 
  }
  // Check MCG is in FEE mode
  else if ((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x0) && // check CLKS mux has selcted FLL
          (!(MCG_S & MCG_S_IREFST_MASK)) &&                              // check FLL ref is external ref clk
          (!(MCG_S & MCG_S_PLLST_MASK)))                                 // check PLLS mux has selected FLL
  {
    return FEE;                                                          // return FEE code
  }
  else
  {
    return 0;                                                            // error condition
  }
} // what_mcg_mode



int PEE_to_BLPE(void)
{
     int mcg_clock_hz;    
     
     mcg_clock_hz = atc(FAST_IRC,fast_irc_freq,MCG_OUT_FREQ);

     mcg_clock_hz = pee_pbe(CLK0_FREQ_HZ);
     mcg_clock_hz = pbe_blpe(CLK0_FREQ_HZ);


      return mcg_clock_hz; 
}


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
void clockMonitor(unsigned char state)
{
    if(state)
      MCG_C6 |= MCG_C6_CME0_MASK;
    else
      MCG_C6 &= ~MCG_C6_CME0_MASK;
}
  /*用来控制时钟的，开关*/

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
/*关闭硬件接口，比如pwm，串口，spi，AD等等*/

