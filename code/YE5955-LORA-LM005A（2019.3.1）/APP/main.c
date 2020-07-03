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
** File Name:               Main.c
** Last modified date:      
** Last version:            V1.0
** Description:             Ö÷º¯ÊýÎÄ¼þ
**
--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
*********************************************************************************************************/
#include "includes.h"
#include "stdio.h"
#include "app.h"
#include "modbus.h"
#include "common.h"
#include "mcg.h"
#include "flash.h"

#include "MKL_gpio.h"
#include "filter.h"
/*********************************************************************************************************
** Function name:           Delayus
** Descriptions:            Çý¶¯GPIO ·­×ªDemo
** input parameters:        ulTime:ÑÓÊ±Ê±¼ä
** output parameters:       none
** Returned value:          none             

*********************************************************************************************************/


/************** *******************************************************************************************
** Function name:           main
** Descriptions:            Èí¼þ´¥·¢16Î»µ¥¶ËADC²É¼¯£¬Ê¹ÓÃPE20¹Ü½ÅÊäÈë£¬²Î¿¼Ô´Ñ¡ÔñµçÔ´,Ê¹ÓÃ²éÑ¯µÄ·½Ê½¶ÁÈ¡ADC
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              

*********************************************************************************************************/

uint32_t  ZERO;
#define  COUNTER 10
uint64_t  AD_ZERO;
int32_t  AD_ASquare;
int32_t  AD_VSquare;
int32_t  AD_XSquare;
int64_t  AD_Inter;
int64_t  AD_AInter;
int64_t  AD_VInter;
int64_t  AD_Adate;
int64_t  AD_Vdate;
int64_t  AD_Xdate;
int64_t  AD_add;
int16_t  COUNT;
int16_t  SHOWCOUNT;
int32_t  waitforreceivetime;
#define  SAVE_BUFFER_SIZE  20
#define  KEYNUMBER  8888
#define  RD_BUFFER_SIZE  20
uint32_t  FLASH_SIZE=16*1024*1024;
uint32_t  FLASH_WRADDR=0;
uint8_t   SaveBuf[SAVE_BUFFER_SIZE];
uint8_t   RDBuf[RD_BUFFER_SIZE];
uint8_t   tx_condition=0;
uint32_t  TX_Time_counter=0;
uint32_t  TX_pre_counter=0;
uint32_t  temp_data=0;
uint32_t  temp_data0=0;
uint32_t  temp=0;
uint8_t   GDConnect1=0;
uint8_t   GDConnect2=0;


extern void  pwmInit(void);
INT8U    SaveBuf[SAVE_BUFFER_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};

int32_t value;
int32_t value1;
int64_t vvalue;
int64_t xvalue;
	
struct  CONFIG  config=
{
	0xAA55, //	uint16_t vaildsign;
	3,//uint8_t baundrate;   /* =0:4800    =1:9600 		=2:38400 		=3:115200 		 */ 
	140,//uint16_t addr; 
	0,//Ð£Ñé¸ñÊ½ 0²»¼ìÑé£¬1ÆæÐ£Ñé2Å¼Ð£Ñé
	8,//8Î»³¤¶È
	1,//1¸öÍ£Ö¹Î»
	0x0603 ,
	0, //uint8_t parity;		// =0 : n,8,1   =1: o,8,1  =2: e,8,1	 Êý¾Ý¸ñÊ½
		
	0, //uint8_t DisplayMode;  // ÏÔÊ¾Ä£Ê½¡¡=0¡¡¹Ì¶¨¡¡=1 Ñ­»·
	{1,1,1,1,1,1,1,1,1,1,1,1}, //uint8_t interface_type[12]; // ÊäÈëÀàÐÍ
	{1,1,1,1,1,1,1,1,1,1,1,1},//uint8_t unit[12];  // µ¥Î»
	{980,980,980,980,1000,1000,1000,1000,10000,10000,10000,10000},//uint32_t scale[12]; // ×ª»»ÏµÊý
	{0,0,0,0,0,0,0,0,8192,8192,8192,8192},//uint32_t adjust[12]; // ÐÞÕýÏµÊý
	{0,1,2,~0,~0,~0,~0,~0,3,4,5,6},//uint16_t interface_addr[12]; // modbus µØÖ· ÉÏ´«
	{0,0,0,0,0,0,0,0,0,0,0,0},//	uint8_t abs[12]; // ¾ø¶ÔÖµ 1´ò¿ª
	0,//uint8_t means	;// ¾ùÖµÀàÐÍ
	1,//uint16_t means_times; // ¾ùÖµ»ýËãÖÜÆÚ
	20000,//uint16_t freq;  // ²ÉÑùÆµÂÊ Hz
	4096,//uint16_t avr_count;
	0,0,0,0, //uint8_t yellow_ch,blue_ch,green_ch,red_ch; // ÏÔÊ¾²¨ÐÎÍ¨µÀ
	2, // uint8_t reflash; // Ë¢ÐÂÊ±¼ä 
	~0, //	uint16_t din_addr;  //  ¿ª¹ØÁ¿ÊäÈë¼Ä´æÆ÷µØÖ·
	~0, // uint16_t dout_addr; //  ¿ª¹ØÁ¿Êä³ö¼Ä´æÆ÷µØÖ·
	20,20,0,0,//	uint32_t gate0,gate1,gate2,gate3;  // ¶¯×÷ãÐÖµ
	100, 100,0,0,//uint32_t backlash0,backlash1,backlash2,backlash3; // ¶¯×÷»Ø²î
	500, 50, //uint32_t vibrate_gate,vibrate_backlash;  // Õñ¶¯Æô¶¯ãÐÖµ£¬»Ø²î
	300, 30, // uint32_t force_gate,force_backlash,   // Ó¦±äÆô¶¯ãÐÖµ£¬ »Ø²î¡£
	~0,~0, //	uint16_t max_addr0,max_addr1; ×î´óÖµµØÖ·
	300,
	300,
	300,
	0x4a,                          //PGA
	1,                          //¹¤×÷ÖÜÆÚ(·Ö)
//	155,                         //·Å´ó±¶Êý
	0x11,                      //  ·¢Éä¹¦ÂÊÉèÖÃ
	0x01,                    //AD²ÉÑùÆµÂÊ
	20,                     //AD²ÉÑùÊ±¼ä£¬¸ú¹¤×÷ÖÜÆÚÒ»¸öµÀÀí
	
	0x59B6A4C3,           //8µã00·Ö£¬aabbµÄ¸ñÊ½£¬aa´ú±íÊ±¼ä£¬bb´ú±í·ÖÖÓ£¬ÃëÄ¬ÈÏÎª0
	0,                //flashÆðÊ¼µØÖ·
	1,
	
	1,               //displacement
	0,               //netack
	
	1.0,            //ADCscale     ADÏµÊý	
	1.0,            //LMDscale     ÁéÃô¶ÈÏµÊý
	1.0             //CRTscale     ÐÞÕýÏµÊý
};


struct  PARAMETER Parameter=
{
	0x55AA,
	0,0,//float CVdc,OVdc;  // ºÏÕ¢Ö±Á÷µçÑ¹
	0,//uint16_t SWStatus;  //  ¿ª¹Ø×´Ì¬			
};		  //Î´¼Ó±ä±ÈµÄ²ÎÊý



struct  ADJUSTDATA AdjustData=	
{ 
	0x55AA,
	4362L,4362L,4362L,     //  1<<12 ±íÊ¾1.0
	4406L,4406L,4406L,
	0,0,0,
	1L<<12,1L<<12
};

			
void  spigpioInit (void)
{
    PORTC_PCR4  |= PORT_PCR_MUX(1);                                     /* ¿ªÆôPC4¹Ü½ÅGPIO¹¦ÄÜ SPI0 CS0 */
    PORTC_PCR5  |= PORT_PCR_MUX(2)|(uint32_t)~(uint32_t)PORT_PCR_MUX(5);                                   /* ¿ªÆôPC5¹Ü½ÅSPI0¹¦ÄÜ SPI0 SCK */
    PORTC_PCR6  |= PORT_PCR_MUX(5)|(uint32_t)~(uint32_t)PORT_PCR_MUX(2);                                     /* ¿ªÆôPC6¹Ü½ÅSPI0¹¦ÄÜ SPI0 MOSI*/
    PORTC_PCR7  |= PORT_PCR_MUX(5)|(uint32_t)~(uint32_t)PORT_PCR_MUX(2);                                     /* ¿ªÆôPC7¹Ü½ÅSPI0¹¦ÄÜ SPI0 MISO*/
    FGPIOC_PSOR |= 1<<4;                                                /* ÅäÖÃCS0ÖÃ¸ß                  */
    FGPIOC_PDDR |= 1<<4;                                                /* ÅäÖÃCS0ÎªÊä³ö                */
}


uint16_t  batteryCanboardtime=2000;
uint16_t  currentboardtime = 0;


unsigned long  int_sqrt(unsigned long x)  
{  
	register unsigned long op, res, one;  
	op = x;  
	res = 0;  
 /* "one" starts at the highest power of four <= than the argument. */  
	one = 1 << 30;  /* second-to-top bit set */  
	while (one > op) one >>= 2;  
	while (one != 0) 
	{  
		if (op >= res + one)			
		{  
			op -= res + one;  
			res += one << 1;  // <-- faster than 2 * one  
		}  
		res >>= 1;  
		one >>= 2;  
	}  
	return res;  
}

/*********************************************************************************************************
** Function name:           LPTimer_IRQHandler
** Descriptions:            lptmrÖÐ¶Ï
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              
** Created date:           
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
*********************************************************************************************************/

float     RMS_value;
uint32_t  RTC_SECONDS;


void  LPTimer_IRQHandler(void)
{
    LPTMR0_CSR |= LPTMR_CSR_TCF_MASK;                                   /* lptmr±È½Ï±êÖ¾Î»Çå³ý          */
    // RTC_SECONDS++;                                                       /* gpio·­×ª                     */
}



//void  PIT_IRQHandler(void)
//{   
//	int32_t value;
//	int64_t vvalue;
//	int64_t xvalue;
//	PIT_TFLG0 |= PIT_TFLG_TIF_MASK;                                     /* ±êÖ¾Î»Çå¿Õ                   */
////  GPIO_TOGGLE(MKL_PORTB,1);                                                                          /* ÅäÖÃGPIO ·­×ª                */  
//	value=(int32_t)adcGetUint ();  //adc´¥·¢·½·¨£¬²ÉÑùÊ±¼ä

//	
//	AD_ZERO=value+((uint64_t)AD_ZERO*(511)>>9);  //Ò»½×µÍÍ¨ÂË²¨
//	value=value-(int32_t)(AD_ZERO>>9);           //È¥³ýÖ±Á÷·ÖÁ¿
//  	AD_Adate+=value*value;                       //´ËÖµ¶ÔÓ¦µÄÊÇ²âÁ¿µÄ¼ÓËÙ¶ÈÖµ

//	
//	AD_AInter=AD_AInter+value;                   //¸ø³õÖµ
//	AD_AInter=((int64_t)AD_AInter*(511)>>9);  
//	vvalue=(AD_AInter*200)>>15;
//	AD_Vdate+=(int64_t)vvalue*vvalue;            // ´ËÖµ¶ÔÓ¦µÄÊÇ²âÁ¿µÄËÙ¶ÈÖµ

//	
//	xvalue=(AD_AInter)>>5;                       //ÇóË²Ê±ËÙ¶È£¬Ó¦¸Ã³ýÒÔ20K,Õâ±ß³ýÁË81.92±¶£¬±£Áô¾«¶È£¬ºóÆÚ»¹Òª³ýÒÔ24.4±¶
//	AD_VInter=AD_VInter+xvalue;
// 	AD_VInter=(AD_VInter*255)>>8;                //Ò»½×¸ßÍ¨ÂË²¨
//	xvalue=(AD_VInter)>>5;
//	AD_Xdate+=(int64_t)xvalue*xvalue;            // ´ËÖµ¶ÔÓ¦µÄÊÇ²âÁ¿µÄÎ»ÒÆ
//	  
//	  
//	COUNT++;
//	if(COUNT>16384) 
//	{ 
//		AD_ASquare=AD_Adate>>14;
//		AD_Adate=0;
//		AD_VSquare=AD_Vdate>>14;
//		AD_Vdate=0;
//		AD_XSquare=AD_Xdate>>14;
//		AD_Xdate=0;
//		
//		COUNT=0;
//		//AD_AInter=0;
//		//AD_VInter=0;   //Á½¸öÖÐ¼ä±äÁ¿Òª±£Áô
//		SHOWCOUNT++;		
//	}
//	if(SHOWCOUNT>5)  //·¢ËÍÊý¾Ý
//	{
//			
//	}	
//}


void  ADPowerSelect(uint8_t yesorno)      /*ÓÃÔÚ¿ØÖÆAD²ÉÑùµçÂ·µÄÍ¨¶Ï  */    
{                                   
	if(yesorno==1)
	{GPIO_SET(MKL_PORTE,0); }                                           
	if(yesorno==0)
	{GPIO_CLR(MKL_PORTE,0); }										
}


void  GDPowerSelect(uint8_t yesorno)       /*ÓÃÔÚ¿ØÖÆÎÞÏßÄ£¿éµÄÍ¨¶Ï  */
{                              	   
	if(yesorno==1)
	{GPIO_SET(MKL_PORTE,1); }                                           
	if(yesorno==0)
	{GPIO_CLR(MKL_PORTE,1); }										
}


uint32_t  countter=0;
RTC_DateTime_Type  td;


void  RTCBEGIN(void)    //ÓÃÀ´¿ªÊ¼µ±Ç°µÄRTCÊ±ÖÓ
{
    rtcInit(0);
	
    td.day = 5;
    td.hour = 11;
    td.minute = 0;
    td.second = 0;
    td.year = 2018;
    td.month = 6;
    
    RTC_SetTime(&td);
}


void  W25Qinit(void)
{
	spigpioInit();
	spiInit(SPI0_BASE_PTR , Master);
}


void  WRW32Q128(void)
{
	uint16_t a,v,x;
	a=(uint16_t)(Parameter.adate*100+0.5);
	v=(uint16_t)(Parameter.vdate*100+0.5);
	x=(uint16_t)(Parameter.xdate*100+0.5);
	RTC_GetTime(&td);
	SaveBuf[0]=config.addr>>8;
	SaveBuf[1]=config.addr%256;
	SaveBuf[2]=a>>8;
	SaveBuf[3]=a%256;
	SaveBuf[4]=v>>8;
	SaveBuf[5]=v%256;
	SaveBuf[6]=x>>8;
	SaveBuf[7]=x%256;
	SaveBuf[8]=td.year>>8;
	SaveBuf[9]=td.year%256;
	SaveBuf[10]=td.month>>8;
	SaveBuf[11]=td.month%256;
	SaveBuf[12]=td.day>>8;
	SaveBuf[13]=td.day%256;
	SaveBuf[14]=td.hour>>8;
	SaveBuf[15]=td.hour%256;
	SaveBuf[16]=td.minute>>8;
	SaveBuf[17]=td.minute%256;
	SaveBuf[18]=td.second>>8;
	SaveBuf[19]=td.second%256;
	FLASH_WRADDR=config.FLASH_WRADDR;
	FLASH_WRADDR=FLASH_WRADDR+SAVE_BUFFER_SIZE;
	flash_write_data (FLASH_WRADDR,SaveBuf,SAVE_BUFFER_SIZE);
	config.FLASH_WRADDR=FLASH_WRADDR;
	saveConfig();
}



unsigned char  measureFlag = 0;
unsigned char  enterVLLSOflag = 0;
unsigned long  ram_data[216];


int  mcg_clk_hz;
int  mcg_clk_khz;
int  core_clk_khz;
int  uart0_clk_khz;


extern void  enable_NMI_button(void);


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


void  LPTMR_init(int count, int clock_source)
{
    SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;

#ifdef CMSIS
	  NVIC_EnableIRQ(LPTimer_IRQn);
#else
    enable_irq(LPTMR_irq_no);
#endif

    LPTMR0_PSR = ( LPTMR_PSR_PRESCALE(8)| LPTMR_PSR_PCS(clock_source)) ; // 0000 is div 2  //512·ÖÆµ£¬0.512sÆô¶¯Ò»´Î¼¼Êõ
                 //|LPTMR_PSR_PBYP_MASK  // LPO feeds directly to LPT
                  // use the choice of clock
    
             
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
//void  llwu_configure(unsigned int pin_en, unsigned char rise_fall, unsigned char module_en ) 
//{  
//    if (module_en == 0)
//	{
//		LLWU_ME = 0;
//    }
//	else  
//	{
//		LLWU_ME |= module_en;  //Set up more modules to wakeup up
////		printf("\n LLWU configured modules as LLWU wakeup sources = 0x%02X,",(LLWU_ME));
//    }
//} // End LLWU Configuration



//void  LLWU_Init(void)
//{
//	NVIC_EnableIRQ(LLW_IRQn);
//	NVIC_EnableIRQ(LPTimer_IRQn);
//    // llwu_configure(0x0000, LLWU_PIN_FALLING, 0x1);
//}



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

//void  exit_vlpr(void)
//{
//    int i;
//    /* check to make sure in VLPR before exiting    */
//    if((SMC_PMSTAT & SMC_PMSTAT_PMSTAT_MASK)== 4) 
//	{
//		/* Clear RUNM */
//		SMC_PMCTRL &= ~SMC_PMCTRL_RUNM_MASK;                      
//        /* Wait for normal RUN regulation mode to be confirmed */                   
//		// 1 PMSTAT MCU is in RUN  mode
//		// 4 PMSTAT MCU is in VLPR mode
//		for (i=0;i<0xff;i++)
//		{
//			if((SMC_PMSTAT & SMC_PMSTAT_PMSTAT_MASK)== SMC_PMSTAT_PMSTAT_MASK)
//				if((PMC_REGSC & PMC_REGSC_REGONS_MASK) == PMC_REGSC_REGONS_MASK)
//    	        break;
//		}           
//    }  //if in VLPR mode
//     // else if not in VLPR ignore call
//}



//int  PEE_to_BLPE(void)
//{
//    int mcg_clock_hz;    
//     
//    mcg_clock_hz = atc(FAST_IRC,fast_irc_freq,MCG_OUT_FREQ);
//    mcg_clock_hz = pee_pbe(CLK0_FREQ_HZ);
//    mcg_clock_hz = pbe_blpe(CLK0_FREQ_HZ);

//	return mcg_clock_hz; 
//}


void  clockMonitor(unsigned char state)
{
    if(state)
		MCG_C6 |= MCG_C6_CME0_MASK;
    else
		MCG_C6 &= ~MCG_C6_CME0_MASK;
}
  /*ÓÃÀ´¿ØÖÆÊ±ÖÓµÄ£¬¿ª¹Ø*/


 

/***********************************************************/
//int  enter_vlpr(void)
//{
//	int i; 
//	unsigned int return_value = 0;  //default return value = indicates error
//	if((SMC_PMSTAT & SMC_PMSTAT_PMSTAT_MASK)== 4)
//	{
//		return_value = 0x14;
//    }
//	/* The PMPROT register may have already been written by init code
//    If so then this next write is not done.  
//    PMPROT is write once after RESET  
//    this write-once bit allows the MCU to enter the
//    very low power modes: VLPR, VLPW, and VLPS   */
//	SMC_PMPROT = SMC_PMPROT_AVLP_MASK;      
//	/* Set the (for MC1)LPLLSM or (for MC2)STOPM field 
//    to 0b010 for VLPS mode -
//    and RUNM bits to 0b010 for VLPR mode  */
//	SMC_PMCTRL &= ~SMC_PMCTRL_RUNM_MASK; 
//	SMC_PMCTRL  |= SMC_PMCTRL_RUNM(0x2);
//	/* Wait for VLPS regulator mode to be confirmed */
//	for (i = 0 ; i < 10000 ; i++)
//    {     
//		/* check that the value of REGONS bit is not 0
//			once it is a zero we can stop checking */
//		if ((PMC_REGSC & PMC_REGSC_REGONS_MASK) ==0x04)
//		{
//			/* 0 Regulator is in stop regulation or in transition 
//            to/from it
//			1 MCU is in Run regulation mode */
//		}  
//		else  break;
//	}  
//	if((PMC_REGSC & PMC_REGSC_REGONS_MASK) ==0x04) 
//    {  
//		return_value = 0x24; 
//    }
//	/* SMC_PMSTAT register only exist in Mode Controller 2 MCU versions */
//	if((SMC_PMSTAT & SMC_PMSTAT_PMSTAT_MASK) == 4)  
//    {
//		return_value = SMC_PMSTAT;
//    }
//	return (return_value);
//}
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

void  enter_vlps(void)
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
		//SMC_STOPCTRL|= 0x40;             Õâ¸öµ½µ×ÐÐ²»ÐÐ
	__wfi();
#else
	stop();
#endif
}



void  IOinit()                                                
{
	/* PD7Íâ²¿ÖÐ¶Ï³õÊ¼»¯,ÓÃÓÚÎÂ¶È¼ì²â */
	IO_FUN_SEL(MKL_PORTD,7,1);       
	GPIO_DDR_INPUT(MKL_PORTD,7);
	IO_IRQ_CONFIG(MKL_PORTD,7,IO_RISE_EDGE_IRQ);    //ÉÏÉýÑØ       
	NVIC_EnableIRQ(PORTC_PORTD_IRQn);               //Íâ²¿ÖÐ¶ÏÊ¹ÄÜ
	
//	/* PD6Íâ²¿ÖÐ¶Ï³õÊ¼»¯,ÓÃÓÚ»Ö¸´ÉèÖÃ */
//	IO_FUN_SEL(MKL_PORTD,6,1);       
//	GPIO_DDR_INPUT(MKL_PORTD,6);
//	IO_IRQ_CONFIG(MKL_PORTD,6,IO_RISE_EDGE_IRQ);     //ÉÏÉýÑØ 
//	NVIC_EnableIRQ(PORTC_PORTD_IRQn);                //Íâ²¿ÖÐ¶ÏÊ¹ÄÜ

	IO_FUN_SEL(MKL_PORTE,19,1);      //ÍøÂçÄ£¿éË¯Ãß/Æô¶¯¿ØÖÆÒý½Å
	GPIO_DDR_OUTPUT(MKL_PORTE,19); 
	GPIO_SET(MKL_PORTE,19);          //À­¸ßÕý³£¹¤×÷
	//GPIO_CLR(MKL_PORTE,19);        //À­µÍ½øÈëµÍ¹¦ºÄ
	
	IO_FUN_SEL(MKL_PORTE,0,1);       //ADÄ£¿éµçÔ´
	GPIO_DDR_OUTPUT(MKL_PORTE,0);
	
	IO_FUN_SEL(MKL_PORTE,1,1);       //ÎÞÏßÄ£¿éµçÔ´
	GPIO_DDR_OUTPUT(MKL_PORTE,1); 
	
	IO_FUN_SEL(MKL_PORTE,16,1);      //ºìÉ«µçÔ´µÆ
	GPIO_DDR_OUTPUT(MKL_PORTE,16); 
	GPIO_SET(MKL_PORTE,16);
	
	IO_FUN_SEL(MKL_PORTB,0,1);       //ÂÌÉ«ÐÅºÅµÆ
	GPIO_DDR_OUTPUT(MKL_PORTB,0);
	GPIO_SET(MKL_PORTB,0);
}


void  LED_POWER(uint8_t yesorno)         //¿ØÖÆLED1µçÔ´£º0°µ1ÁÁ 
{                                        
	if(yesorno==0)
		{GPIO_SET(MKL_PORTB,0); }                                           
	if(yesorno==1)
		{GPIO_CLR(MKL_PORTB,0); }										
}


void  LED_SIGNAL(uint8_t yesorno)        //¿ØÖÆLED2µçÔ´£º0°µ1ÁÁ 
{                                        
	if(yesorno==0)
		{GPIO_SET(MKL_PORTB,0); }                                           
	if(yesorno==1)
		{GPIO_CLR(MKL_PORTB,0); }										
}

void  ConnectIndication()
{
	LED_SIGNAL(1);
	time_delay_ms(500);
	LED_SIGNAL(0);
	time_delay_ms(500);
	LED_SIGNAL(1);
	time_delay_ms(500);
	LED_SIGNAL(0);
//	GPIO_TOGGLE(MKL_PORTB,0);
//	time_delay_ms(500);
//	GPIO_TOGGLE(MKL_PORTB,0);
//	time_delay_ms(500);
//	GPIO_TOGGLE(MKL_PORTB,0);
}

void  Connectfail()
{
	LED_POWER(1);
	time_delay_ms(500);
	LED_POWER(0);
	time_delay_ms(500);
	LED_POWER(1);
	time_delay_ms(500);
	LED_POWER(0);
//	GPIO_TOGGLE(MKL_PORTE,16);
//	time_delay_ms(500);
//	GPIO_TOGGLE(MKL_PORTE,16);
//	time_delay_ms(500);
//	GPIO_TOGGLE(MKL_PORTE,16);
}

void  ADanalogyinit()                                                 /*AD³õÊ¼»¯£¬PITÖÐ¶Ï³õÊ¼»¯*/
{
    adcInit(); 
	
	SIM_SOPT7|=SIM_SOPT7_ADC0ALTTRGEN_MASK;               //ADC0½»Ìæ´¥·¢Ê¹ÄÜ
	SIM_SOPT7&=~SIM_SOPT7_ADC0PRETRGSEL_MASK;             //Ô¤´¥·¢A(1)
	SIM_SOPT7|=SIM_SOPT7_ADC0TRGSEL(4);                   //´¥·¢Ô´PIT0
	
	pitInit();
//  NVIC_SetPriority(PIT_IRQn,2);
	NVIC_SetPriority(ADC0_IRQn,1);	
}


void  ADC0_IRQHandler(void)
{	


//  GPIO_TOGGLE(MKL_PORTB,1);         /* ÅäÖÃGPIO ·­×ª                */  
	value=(int32_t)adcGetUint();   \
	//adc´¥·¢·½·¨£¬²ÉÑùÊ±¼ä
    

	AD_ZERO=value+((uint64_t)AD_ZERO*(1023)>>10);  //Ò»½×µÍÍ¨ÂË²¨
	value=value-(int32_t)(AD_ZERO>>10);           //È¥³ýÖ±Á÷·ÖÁ¿
	AD_Adate+=value*value;                     //´ËÖµ¶ÔÓ¦µÄÊÇ²âÁ¿µÄ¼ÓËÙ¶ÈÖµ

	
	AD_AInter=AD_AInter+value;                   //»ý·Ö
	AD_AInter=((int64_t)AD_AInter*(511)>>9);     //¸ßÍ¨ÂË²¨
	vvalue=(AD_AInter*200)>>15;                  //µ¥Î»»»Ëã£¬ÓëºóÃæµÄ5·Ö¿ªËãµÄ
	AD_Vdate+=(int64_t)vvalue*vvalue;            //´ËÖµ¶ÔÓ¦µÄÊÇ²âÁ¿µÄËÙ¶ÈÖµ

	
	xvalue=(AD_AInter)>>5;                       //ÇóË²Ê±ËÙ¶È£¬Ó¦¸Ã³ýÒÔ20K,Õâ±ß³ýÁË81.92±¶£¬±£Áô¾«¶È£¬ºóÆÚ»¹Òª³ýÒÔ24.4±¶
	AD_VInter=AD_VInter+xvalue;
 	AD_VInter=(AD_VInter*255)>>8;                //Ò»½×¸ßÍ¨ÂË²¨
	xvalue=(AD_VInter)>>5;
	AD_Xdate+=(int64_t)xvalue*xvalue;            // ´ËÖµ¶ÔÓ¦µÄÊÇ²âÁ¿µÄÎ»ÒÆ
	  
	  
	COUNT++;
	if(COUNT>16384) 
	{ 
		AD_ASquare=AD_Adate>>14;
		AD_Adate=0;
		AD_VSquare=AD_Vdate>>14;
		AD_Vdate=0;
		AD_XSquare=AD_Xdate>>14;
		AD_Xdate=0;
		
		COUNT=0;
		//AD_AInter=0;
		//AD_VInter=0;   //Á½¸öÖÐ¼ä±äÁ¿Òª±£Áô
//		SHOWCOUNT++;		
	}
}


void  ADBEGIN()                                                        /*¿ªÆôAD²ÉÑù£¬×¼±¸½øÈëÆäËûÄ£Ê½*/
{   
//	NVIC_EnableIRQ(PIT_IRQn); 
	NVIC_EnableIRQ(ADC0_IRQn); 	
	COUNT=0; 	
}


void  ADOVER()                                                          /*¹Ø±ÕAD²ÉÑù£¬×¼±¸½øÈëÆäËûÄ£Ê½*/
{     
//	NVIC_DisableIRQ(PIT_IRQn); 
	NVIC_DisableIRQ(ADC0_IRQn);	
}



const int32_t df_temp_table[]={-200,-100,00,100,200,300,400,500,600,700};
const int32_t df_resister_table[]={921,960,1000,1039,1078,1116,1155,1194,1232,1271}; 

	
uint32_t  df_table_num=sizeof(df_temp_table)/sizeof(int32_t);
uint16_t  lastworkcycle;
uint16_t  boardtime=0;

void  EMU()
{
	float RMS_Adate,RMS_Vdate,RMS_Xdate;
//	float fangdabeishu;  
//	fangdabeishu=(float)config.fangda*0.02;
	
	RMS_Adate=int_sqrt(AD_ASquare);
	RMS_Adate=(float)RMS_Adate*0.050354;//3.3VµÄ»ù×¼µçÑ¹£¬65536ÂúÁ¿³Ì  µ¥Î»ºÁ·ü    0.050354
	//RMS_Adate=RMS_Adate*5*0.8192;//5ÓëÇ°Ãæ200¶ÔÓ¦£¬ËÙ¶È¹ØÏµÖÐµÄm×ªmm£¬ºóÃæµÄ0.8192ÓëÇ°ÃæµÄ16384¶ÔÓ¦£¬³ýÍê¾ÍÊÇ20K£¬²ÉÑùÆµÂÊ¡£
	RMS_Adate=RMS_Adate*config.ADCscale*config.CRTscale/config.LMDscale;         //Õâ¸öÓÉÓÚÄ£ÄâµçÂ·±¾ÉíÓÐ1±¶µÄÔöÒæ£¬ÔÚPGAÊÇ0x44µÄÇé¿öÏÂ£¬Òò´ËÔÚ10mvÃ¿¼ÓËÙ¶ÈÊ±£¬³ËÒÔ0.05
	Parameter.adate=RMS_Adate;

	RMS_Vdate=int_sqrt(AD_VSquare);
	RMS_Vdate=(float)RMS_Vdate*0.050354;//3.3VµÄ»ù×¼µçÑ¹£¬65536ÂúÁ¿³Ì
	RMS_Vdate=RMS_Vdate*5*1.6384;//5ÓëÇ°Ãæ200¶ÔÓ¦£¬ËÙ¶È¹ØÏµÖÐµÄm×ªmm£¬ºóÃæµÄ1.6384ÓëÇ°ÃæµÄ32768¶ÔÓ¦£¬³ýÍê¾ÍÊÇ20K£¬²ÉÑùÆµÂÊ¡£
	RMS_Vdate=RMS_Vdate*config.ADCscale*config.CRTscale/config.LMDscale;  //¸ù¾Ý3¸öÏµÊý½øÐÐ´¦Àí
	Parameter.vdate=RMS_Vdate;
	
	RMS_Xdate=int_sqrt(AD_XSquare);
	RMS_Xdate=(float)RMS_Xdate*0.050354;//3.3VµÄ»ù×¼µçÑ¹£¬65536ÂúÁ¿³Ì
	RMS_Xdate=RMS_Xdate*0.0128*4;//
	RMS_Xdate=RMS_Xdate*100*config.ADCscale*config.CRTscale/config.LMDscale;     //Ô­Òò¸ú¼ÓËÙÊÇÏàÍ¬µÄ
	Parameter.xdate=RMS_Xdate*0.5f;			
}


float  reference_voltage=0;
float  temperature_voltage=0;



void  EXITGPIOinit()
{
	IO_FUN_SEL(MKL_PORTB,1,1); 
	GPIO_DDR_INPUT(MKL_PORTB,1);              	 
}


void  rtcIntConfig(INT32U ulAlarm,INT8U ucIntType)
{
	if(ucIntType & MKL_RTC_TSI) 
	{
        RTC_IER |= RTC_IER_TSIE_MASK;
        NVIC_EnableIRQ(RTC_Seconds_IRQn);
        NVIC_SetPriority(RTC_Seconds_IRQn,3);                       /* ÓÃ»§×Ô¼º¶¨Òå                     */
    }                  
    if(ucIntType & MKL_RTC_TAI) 
	{
        RTC_TAR  = ulAlarm;
        RTC_IER |= RTC_IER_TAIE_MASK;
        NVIC_EnableIRQ(RTC_IRQn);
        NVIC_SetPriority(RTC_IRQn,3);                               /* ÓÃ»§×Ô¼º¶¨Òå                     */			
	}    
    if(ucIntType & MKL_RTC_TOI) 
	{
        RTC_IER |= RTC_IER_TOIE_MASK;
        NVIC_EnableIRQ(RTC_IRQn);
        NVIC_SetPriority(RTC_IRQn,3);                               /* ÓÃ»§×Ô¼º¶¨Òå                     */
    }
    if(ucIntType & MKL_RTC_TII) 
	{
        RTC_IER |= RTC_IER_TIIE_MASK;
        NVIC_EnableIRQ(RTC_IRQn);
        NVIC_SetPriority(RTC_IRQn,3);                               /* ÓÃ»§×Ô¼º¶¨Òå                     */
    }
}


void  RTC_IRQHandler(void)
{
    if (RTC_SR & RTC_SR_TAF_MASK)
	{
        RTC_TAR = RTC_TAR + 1;			 
	}
}


uint32_t  intervalclock = 0;
uint32_t  nextclock = 0;		
uint32_t  vlpsstartclock = 0;
uint64_t  intervalue;

uint32_t  outsidenum = 0;
uint8_t   Resetflag = 0;
uint32_t  InputIOtimer = 0;


void  ENTER_VLPS(void)
{
	unsigned char op_mode; 
	
	enter_vlps();  //1 Means: Any interrupt could wake up from this mode   Go to VLPS				 

//	op_mode = what_mcg_mode();								
	if ((SMC_PMSTAT & SMC_PMSTAT_PMSTAT_MASK)== 4)
	{
		// exit_vlpr();
		SIM_SCGC4 |= SIM_SCGC4_UART2_MASK; 
		SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;  	
	} 
	else if ((SMC_PMSTAT & SMC_PMSTAT_PMSTAT_MASK)== 1)
	{		
		op_mode = what_mcg_mode(); // check if in PBE mode and if so, switch to PEE
		if(op_mode==PBE)
		{
			mcg_clk_hz = pbe_pee(CLK0_FREQ_HZ);			
		}

		SIM_SCGC4 |= SIM_SCGC4_UART2_MASK; 
		SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;
	
		clockMonitor(ON);
	}	
}


void  PORTC_PORTD_IRQHandler(void)                           //Íâ²¿ÖÐ¶Ï
{
//    PORTD_ISFR |= PORT_ISFR_ISF_MASK;                        /* ±êÖ¾Î»Çå³ý */
//    temp_data++;
   if(PORTD_ISFR==(1<<7))
   {
		PORTD_PCR7 |= PORT_PCR_ISF_MASK;	   
	    temp_data++; 
   }
//   if(PORTD_ISFR==(1<<6))
//   {
//		PORTD_PCR6 |= PORT_PCR_ISF_MASK;
////		time_delay_ms(100);
//		if(GPIO_GET_VALUE(MKL_PORTD,6))
//		{
//			InputIOtimer=4000;
//		}
//		else
//		{
//			Resetflag=0;
//			InputIOtimer=0;
//		}
//   }		
}


void  TPM1_init(void) 
{ 
	SIM_SCGC6 |= (SIM_SCGC6_TPM1_MASK ); //¿ªÆôTPM1Ê±ÖÓ
	SIM_SOPT2 |=  SIM_SOPT2_PLLFLLSEL_MASK | SIM_SOPT2_TPMSRC(1); //input clock source is MCGPLL/2=24MHz Ê±ÖÓÔ´ÎªMCGPLL/2=24MHz
	TPM1_SC = TPM_SC_TOIE_MASK | TPM_SC_PS(0);//Ê¹ÄÜÒç³öÖÐ¶Ï    
	TPM1_CNT = 0; 
	TPM1_MOD = 600;             //40K==600
	TPM1_SC |= TPM_SC_CMOD(1);    //start the counter 
	NVIC_EnableIRQ(TPM1_IRQn);
	
}


void  TPM1_IRQHandler(void)
{
	TPM1_SC |=TPM_SC_TOF_MASK;
//	GPIO_TOGGLE(MKL_PORTD,7);
	outsidenum++;
	
	if(tx_condition==1)
	{
		TX_Time_counter++;
	}
	else if(tx_condition==0)
	{
		if(!GPIO_GET_VALUE(MKL_PORTD,7))
			TX_pre_counter++;
		else
			TX_pre_counter=0;	
	}
	if((TX_pre_counter>2240)&&(tx_condition==0))    //56ms
	{
		tx_condition=1;
		temp_data=0;
	}
	if((TX_Time_counter>2000)&&(tx_condition==1))   //50ms
	{
		Parameter.pdate=(float)temp_data/4096*256-50;
		tx_condition=0;
		TX_pre_counter=0;
		TX_Time_counter=0;
	}

	
//	if(outsidenum>=19)
//	{
//		outsidenum=0;
//		if(InputIOtimer!=0)
//		{
//			InputIOtimer--;
//			if(InputIOtimer==0)
//			{
//				uint16_t t=GPIO_GET_VALUE(MKL_PORTD,6);
//				if(t)
//					Resetflag=1;
//			}
//		}
//	}
}


void  Init_temp_parameter(void)
{
	tx_condition=0;
	temp_data=0;
	TX_pre_counter=0;
	TX_Time_counter=0;
}


void  Init_uart0_TO_GPIO(void)
{
	IO_FUN_SEL(MKL_PORTA,1,1); 
	GPIO_DDR_OUTPUT(MKL_PORTA,1);
	GPIO_CLR(MKL_PORTA,1);
	
	IO_FUN_SEL(MKL_PORTA,2,1); 
	GPIO_DDR_OUTPUT(MKL_PORTA,2);
	GPIO_SET(MKL_PORTA,2);	
}


uint16_t  config_addr; //Õâ¸öÌØÊâ±äÒì°æ±¾£¬·½±ãËûÃÇ¸ü¸ÄIDºóÈÔÄÜÍ¨Ñ¶
extern bool  LM005A_ADR_SET ( uint32_t  ADRvalue );
extern bool  LM005A_DR_SET ( uint32_t  DRvalue );
extern bool  LM005A_JOIN_SET (void);
extern void  LM005A_SEND_DATA ( uint32_t port,uint32_t ack,uint32_t length );
extern void  LM005A_SEND_DATA_Without_Receive ( uint32_t port,uint32_t ack,uint32_t length );
extern bool  LM005A_MODE_SET ( uint32_t  MODEvalue );
extern bool  LM005A_CLASS_SET ( uint32_t  CLASSvalue );
extern bool  LM005A_POWER_SET ( uint32_t  POWERvalue );
extern bool  LM005A_AT_Test ( void );
extern bool  LM005A_RESET (void);
extern bool  LM005A_returnOK (  char * reply1, char * reply2, uint32_t waittime );
extern bool  LM005A_VERSION (void);
//uint32_t  WAIT_COUNTER=0;	
uint32_t  waitreceive_count=0;


int main(void)            
{
	uint32_t  currentclock = 0;
	uint32_t  saveconfig_flag = 0;
	uint32_t  vlpstime=0;
	
    Parameter.selffangda =0x00;

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
	
	__enable_irq();              //¿ªÆô×ÜÖÐ¶Ï
	
	IOinit();
	LED_POWER(1);
	
	Configinit();                //¼ÓÔØÉèÖÃ²ÎÊý
	ADanalogyinit();             //AD¼°PIT³õÊ¼»¯£¬¿ªÆôADÖÐ¶Ï
	TPM1_init();                 //¿ªÆô¶¨Ê±Æ÷
	uart0Init(7,0,0,8,1);        //´®¿Ú0³õÊ¼»¯£¬¸úÎÞÏßÄ£¿éÍ¨ÐÅ£¬²¨ÌØÂÊ115200
	uart2Init(9600,0,0,8,1);     //´®¿Ú2³õÊ¼»¯£¬¸úÉÏÎ»»úÍ¨ÐÅ£¬´®¿Ú2´ò¿ªÍâ²¿´¥·¢¹¦ÄÜ£¬¿ÉÒÔ´òÆÆÐÝÃßÖÐµÄÎÞÏß´«¸ÐÆ÷½øÐÐÉèÖÃ 
   	RTCBEGIN();                  //¿ªÆôRTC	

	AD_ZERO=0xb5b6e0;    //¼ÓËÙ¶È,³õÖµ
	AD_Inter=0;
	AD_VInter=0;
	AD_AInter=0;

	
	ADPowerSelect(1);	
	GDPowerSelect(1); 
	
	if(1)
	{
		while(!LM005A_ADR_SET(1));
		while(!LM005A_DR_SET(5));
		while(!LM005A_MODE_SET(0));
		while(!LM005A_CLASS_SET(0));
		while(!LM005A_POWER_SET(20));

		while(!LM005A_JOIN_SET())
		{
			GDConnect1++;
			/*3´ÎÁ¬½ÓÊ§°Ü£¬½øÈëÀëÏßÄ£Ê½£¬30·ÖÖÓºóÖØÆô*/
			if(GDConnect1>=3)
			{
				GDConnect1=0;
				ADPowerSelect(0);	
				GDPowerSelect(0); 
				GPIO_CLR(MKL_PORTE,19);
				Connectfail();                 //LEDÏÔÊ¾
				
				time_delay_ms(200);		       //±ØÐëÒª¼Ó£¬²»È»»áÁ¬ÐøÁ¬½Ó
				NVIC_DisableIRQ(UART0_IRQn);			
				uart0TranControl(UART_TX_DISABLE, UART_RX_DISABLE);                 /* Ê×ÏÈ½ûÖ¹Í¨ÐÅ  */
				SIM_SCGC4 &= ~SIM_SCGC4_UART0_MASK;                                 /* ÔÊÐíÍâÉèÊ±ÖÓ  */
				Init_uart0_TO_GPIO();       //´®¿Ú0ÊÇ¿ÉÒÔÔÚ´ËµÍ¹¦ºÄÄ£Ê½ÏÂÒ»Ö±¹¤×÷µÄ£¬²»¹ØËû£¬ÏûºÄµçÁ÷´ó						
				
				currentclock=(uint32_t)(RTC_TSR); 
				vlpstime=currentclock+30*60;
				rtcIntConfig(vlpstime,MKL_RTC_TAI);  
				
				ENTER_VLPS(); 
				
				NVIC_SystemReset();
			}
		}		
	}

	LED_POWER(0);
    ConnectIndication();        	
	
	while(1)
	{	
		memset(strLM005A_Fram_Record.Data_RX_BUF,0,96);
		saveconfig_flag++;
		if(saveconfig_flag>10)
		{
			saveconfig_flag=0;
			saveConfig();
		}
		
//		if(Resetflag)
//		{
//			Resetflag=0;
//			config.addr=1;
//			config.workcycleminutes=1;
//			config.fangda=213;
//			config.displacement=1;
//			config.netack=0;
//			saveConfig();
//			NVIC_SystemReset();
//		}
					
		uart0Init(7,0,0,8,1);         //ÖØÐÂ³õÊ¼»¯Ò»ÏÂ´®¿Ú
		
/*´ÓVLPR½øÈëRUN Set PMCTRL[RUNM]=00 or Reset************/
		ADBEGIN();                    //´ò¿ªAD£¬¿ªÊ¼²ÉÑù

		Init_temp_parameter();        //²ÎÊý³õÊ¼»¯£¬²»È»»á³öÏÖ¾Þ´óµÄÎó²î
		NVIC_EnableIRQ(PORTC_PORTD_IRQn);     //Íâ²¿ÖÐ¶ÏÊ¹ÄÜ
		NVIC_EnableIRQ(TPM1_IRQn);            //TPMÖÐ¶ÏÊ¹ÄÜ
		TPM1_SC |= TPM_SC_CMOD(1);    //start the counter 
		
		time_delay_ms(1000);								
		time_delay_ms(4000);	
		RTC_GetTime(&td);
		EMU();
		
		ADOVER();                    //¹Ø±ÕAD
		
		NVIC_DisableIRQ(PORTC_PORTD_IRQn);     //Íâ²¿ÖÐ¶Ï¹Ø±Õ
		NVIC_DisableIRQ(TPM1_IRQn);            //TPMÖÐ¶Ï¹Ø±Õ
		TPM1_SC |= TPM_SC_CMOD(0);    //close the counter 
		
		GPIO_CLR(MKL_PORTE,19);   
		time_delay_ms(1);
		GPIO_SET(MKL_PORTE,19);   //ÌáÇ°Ò»ÃëÐÑÀ´£¬À­¸ßÕý³£¹¤×÷
		time_delay_ms(1);
		
		time_delay_ms(100);

		if(config.netack==1)
		{
//			if(config.displacement==0)
//				LM005A_SEND_DATA(2,1,12);
//			else if(config.displacement==1)
//				LM005A_SEND_DATA(2,1,14);			

//			while(1)
//			{
//				if(LM005A_returnOK("OK","OK",100))
//				{
//					GDConnect2=0;
//					break;
//				}	
//				if((LM005A_returnOK("ERROR:4","ERROR:4",100))||(waitreceive_count>8))
//				{
//					GDConnect2++;
//					waitreceive_count=0;
//					if(GDConnect2>=3)
//					{
//						GDConnect2=0;

//						ADPowerSelect(0);	
//						GDPowerSelect(0); 
//						GPIO_CLR(MKL_PORTE,19);
//						Connectfail();              //LEDÏÔÊ¾
//						
//						time_delay_ms(200);		    //±ØÐëÒª¼Ó£¬²»È»»áÁ¬ÐøÁ¬½Ó
//						NVIC_DisableIRQ(UART0_IRQn);			
//						uart0TranControl(UART_TX_DISABLE, UART_RX_DISABLE);                 /* Ê×ÏÈ½ûÖ¹Í¨ÐÅ  */
//						SIM_SCGC4 &= ~SIM_SCGC4_UART0_MASK;                                 /* ÔÊÐíÍâÉèÊ±ÖÓ  */
//						Init_uart0_TO_GPIO();       //´®¿Ú0ÊÇ¿ÉÒÔÔÚ´ËµÍ¹¦ºÄÄ£Ê½ÏÂÒ»Ö±¹¤×÷µÄ£¬²»¹ØËû£¬ÏûºÄµçÁ÷´ó						
//						
//						currentclock=(uint32_t)(RTC_TSR); 
//						vlpstime=currentclock+30*60;
//						rtcIntConfig(vlpstime,MKL_RTC_TAI);  
//						
//						ENTER_VLPS(); 
//						
//						NVIC_SystemReset();
//					}
//					break;
//				}
//				waitreceive_count++;
//				time_delay_ms(1000);
//			}
		}
		else if(config.netack==0)
		{
			if(config.displacement==0)
				LM005A_SEND_DATA_Without_Receive(2,0,12);
			else if(config.displacement==1)
				LM005A_SEND_DATA_Without_Receive(2,0,14);
			while((!LM005A_returnOK("OK","OK",100))&&(waitreceive_count<8))
			{
				waitreceive_count++;
				time_delay_ms(1000);
			}
		}
		time_delay_ms(1000);

		waitreceive_count=0;
		GPIO_CLR(MKL_PORTE,19);              //ÍøÂçÄ£¿éË¯Ãß
		LED_SIGNAL(1);
			
		NVIC_DisableIRQ(UART0_IRQn);			
		uart0TranControl(UART_TX_DISABLE, UART_RX_DISABLE);                 /* Ê×ÏÈ½ûÖ¹Í¨ÐÅ  */
		SIM_SCGC4 &= ~SIM_SCGC4_UART0_MASK;                                 /* ÔÊÐíÍâÉèÊ±ÖÓ  */
		Init_uart0_TO_GPIO();  //´®¿Ú0ÊÇ¿ÉÒÔÔÚ´ËµÍ¹¦ºÄÄ£Ê½ÏÂÒ»Ö±¹¤×÷µÄ£¬²»¹ØËû£¬ÏûºÄµçÁ÷´ó
								
		time_delay_ms(100);
		LED_SIGNAL(0);
		/*¼ÆËã»½ÐÑÊ±¼ä**********************************/								 
		vlpsstartclock=(uint32_t)(RTC_TSR);                    //»ñÈ¡µ±Ç°Ê±¼ä
		if(config.alarmminutetime>vlpsstartclock)              //config.alarmminutetimeÊÇÄÖÖÓ¿ªÊ¼Ê±¼ä
		{
			intervalclock=config.alarmminutetime-vlpsstartclock;
			nextclock=intervalclock%(config.workcycleminutes*60);
		}
		else
		{
			intervalclock=vlpsstartclock-config.alarmminutetime;
			nextclock=(config.workcycleminutes*60)-intervalclock%(config.workcycleminutes*60);
		}
		if(nextclock==0) nextclock=1; //·ÀÖ¹´íÂÒ

		/*½øÈëVLPSÐÝÃßÄ£Ê½**********************************/ 
		ADPowerSelect(0);                           

		currentclock=RTC_TSR;
		vlpstime=currentclock+nextclock;
		rtcIntConfig(vlpstime,MKL_RTC_TAI);             //50Ãë½øÈëË¯ÃßÊ±¼ä£¬VLPS×´Ì¬£¬¹¦ºÄ×îµÍ£¬1.7uaµÄ´ý»úµçÁ÷£¬¹Ø±Õ2.8VµçÑ¹
															
		ENTER_VLPS(); 
			
		ADPowerSelect(1);
	} 
} 

				 

	 

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
