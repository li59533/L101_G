#ifndef  __INCLUDES_H
#define  __INCLUDES_H

#ifdef __cplusplus
extern "C" {
#endif
//#define   True 1

/*********************************************************************************************************
  通用数据类型重定义
*********************************************************************************************************/
typedef unsigned char           BOOLEAN;                                /* 布尔变量                     */
typedef unsigned char           INT8U;                                  /* 无符号8位整型变量            */
typedef signed   char           INT8S;                                  /* 有符号8位整型变量            */
typedef unsigned short          INT16U;                                 /* 无符号16位整型变量           */
typedef signed   short          INT16S;                                 /* 有符号16位整型变量           */
typedef unsigned long           INT32U;                                 /* 无符号32位整型变量           */
typedef signed   long           INT32S;                                 /* 有符号32位整型变量           */
typedef unsigned long long      INT64U;                                 /* 无符号64位整型变量           */
typedef signed long long        INT64S;                                 /* 有符号64位整型变量           */
typedef float                   FP32;                                   /* 单精度浮点数（32位长度）     */
typedef double                  FP64;                                   /* 双精度浮点数（64位长度）     */

typedef unsigned char		uint8;                                      /* 无符号8位整型变量            */
typedef unsigned short int	uint16;                                     /* 无符号16位整型变量           */
typedef unsigned long int	uint32;                                     /* 无符号32位整型变量           */

typedef char			    int8;                                       /* 有符号8位整型变量            */
typedef short int	        int16;                                      /* 有符号16位整型变量           */
typedef int		            int32;                                      /* 有符号32位整型变量           */

typedef volatile int8		vint8;                                      /*  8 bits */
typedef volatile int16		vint16;                                     /* 16 bits */
typedef volatile int32		vint32;                                     /* 32 bits */

typedef volatile uint8		vuint8;                                     /*  8 bits */
typedef volatile uint16		vuint16;                                    /* 16 bits */
typedef volatile uint32		vuint32;                                    /* 32 bits */


typedef   signed          char int8_t;
typedef   signed short     int int16_t;
typedef   signed           int int32_t;


    /* exact-width unsigned integer types */
typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;


/*********************************************************************************************************
  Standard header files 标准头文件
*********************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


/*********************************************************************************************************
  CMSIS 头文件
*********************************************************************************************************/
/* 
 * Configuration of the Cortex-M0 Processor and Core Peripherals
 */
#define __CM0_REV                 0x0000                                /* Core revision r0p0           */
#define __NVIC_PRIO_BITS          2                                     /* Number of Bits used for      */
                                                                        /* Priority Levels              */
#define __Vendor_SysTickConfig    0                                     /* Set to 1 if different SysTick*/
                                                                        /* Config is used               */

/** Interrupt Number Definitions */
typedef enum IRQn {
  /* Core interrupts */
  NonMaskableInt_IRQn          = -14,              /**< Non Maskable Interrupt */
  HardFault_IRQn               = -13,              /**< Cortex-M0 SV Hard Fault Interrupt */
  SVCall_IRQn                  = -5,               /**< Cortex-M0 SV Call Interrupt */
  PendSV_IRQn                  = -2,               /**< Cortex-M0 Pend SV Interrupt */
  SysTick_IRQn                 = -1,               /**< Cortex-M0 System Tick Interrupt */

  /* Device specific interrupts */
  DMA0_IRQn                    = 0,                /**< DMA channel 0 transfer complete interrupt */
  DMA1_IRQn                    = 1,                /**< DMA channel 1 transfer complete interrupt */
  DMA2_IRQn                    = 2,                /**< DMA channel 2 transfer complete interrupt */
  DMA3_IRQn                    = 3,                /**< DMA channel 3 transfer complete interrupt */
  Reserved20_IRQn              = 4,                /**< Reserved interrupt 20 */
  FTFA_IRQn                    = 5,                /**< FTFA interrupt */
  LVD_LVW_IRQn                 = 6,                /**< Low Voltage Detect, Low Voltage Warning */
  LLW_IRQn                     = 7,                /**< Low Leakage Wakeup */
  I2C0_IRQn                    = 8,                /**< I2C0 interrupt */
  I2C1_IRQn                    = 9,                /**< I2C0 interrupt 25 */
  SPI0_IRQn                    = 10,               /**< SPI0 interrupt */
  SPI1_IRQn                    = 11,               /**< SPI1 interrupt */
  UART0_IRQn                   = 12,               /**< UART0 status/error interrupt */
  UART1_IRQn                   = 13,               /**< UART1 status/error interrupt */
  UART2_IRQn                   = 14,               /**< UART2 status/error interrupt */
  ADC0_IRQn                    = 15,               /**< ADC0 interrupt */
  CMP0_IRQn                    = 16,               /**< CMP0 interrupt */
  TPM0_IRQn                    = 17,               /**< TPM0 fault, overflow and channels interrupt */
  TPM1_IRQn                    = 18,               /**< TPM1 fault, overflow and channels interrupt */
  TPM2_IRQn                    = 19,               /**< TPM2 fault, overflow and channels interrupt */
  RTC_IRQn                     = 20,               /**< RTC interrupt */
  RTC_Seconds_IRQn             = 21,               /**< RTC seconds interrupt */
  PIT_IRQn                     = 22,               /**< PIT timer interrupt */
  I2S_IRQn                     = 23,               /**< Reserved interrupt 39 */
  USB0_IRQn                    = 24,               /**< USB0 interrupt */
  DAC0_IRQn                    = 25,               /**< DAC interrupt */
  TSI0_IRQn                    = 26,               /**< TSI0 interrupt */
  MCG_IRQn                     = 27,               /**< MCG interrupt */
  LPTimer_IRQn                 = 28,               /**< LPTimer interrupt */
  Reserved45_IRQn              = 29,               /**< Reserved interrupt 45 */
  PORTA_IRQn                   = 30,               /**< Port A interrupt */
  PORTC_PORTD_IRQn             = 31                /**< Port C D interrupt */
} IRQn_Type;



/*********************************************************************************************************
  Common's header files  公共头文件 
*********************************************************************************************************/
#include "MKL25Z4.h"
#include "system_MKL25Z4.h"

/* * * * * * * * 暂时使用* * * * * * * * */

#include <core_cm0.h>                                                   /* CMSIS File of Cortex-M0      */

/*********************************************************************************************************
  Driver's header files  驱动头文件 
*********************************************************************************************************/
#include "MKL_adc.h"
#include "MKL_dac.h"
#include "MKL_uart.h"
#include "MKL_spi.h"
#include "MKL_rtc.h"
#include "MKL_pit.h"
//#include "MKL_gpio.h"
#include "MKL_tpm.h"
#include "MKL_lptmr.h"
#include "MKL_LPO_RTC.h"
#include "flash.h"
#include "mcg.h"
//#include "powerswitch.h"
#include "w25qxx.h"
/*********************************************************************************************************
  User's header files 用户头文件
*********************************************************************************************************/

/********************************************************************************************************
  PORT配置
********************************************************************************************************/
#define MKL_PORTA               A
#define MKL_PORTB               B
#define MKL_PORTC               C
#define MKL_PORTD               D
#define MKL_PORTE               E
    
#define IO_DIS_IRQ            0x0
#define IO_DMA_RISE_IRQ       0x1
#define IO_DMA_FALL_IRQ       0x2
#define IO_DMA_EITHER_IRQ     0x3
#define IO_LOW_LEVEL_IRQ      0x8
#define IO_HIGH_LEVEL_IRQ     0xc
#define IO_RISE_EDGE_IRQ      0x9
#define IO_FALL_EDGE_IRQ      0xa
#define IO_EITHER_EDGE_IRQ    0xb   
/*
 * 寄存器访问宏定义
 */
#define PCR(PTO,BIT)  PORT##PTO##_PCR##BIT
/*
 * 时钟配置
 */ 
#define V_PORT_ENABLE_CLK(V_PTO)    SIM_SCGC5 |=  SIM_SCGC5_PORT##V_PTO##_MASK  /* 二次连接，防止出错   */
#define V_PORT_DISABLE_CLK(V_PTO)   SIM_SCGC5 &= ~SIM_SCGC5_PORT##V_PTO##_MASK

#define PORT_ENABLE_CLK(PTO)        V_PORT_ENABLE_CLK(PTO)
#define PORT_DISABLE_CLK(PTO)       V_PORT_DISABLE_CLK(PTO)
/*
 * 引脚功能配置 PTO 端口号 BIT 引脚位序 FUN 第几项功能
 */
#define  IO_FUN_SEL(PTO, BIT, FUN)      \
    PCR(PTO,BIT) = (PCR(PTO,BIT) & (~PORT_PCR_MUX_MASK))|PORT_PCR_MUX(FUN);   
 

#define  IO_PULL_SEL(PTO, BIT, PS)      \
    PCR(PTO,BIT) = (PCR(PTO,BIT) & (~PORT_PCR_PS_MASK))|(PS << PORT_PCR_PS_SHIFT);   

#define  IO_PULLENABLE_CONFIG(PTO, BIT, PE)    \
    PCR(PTO,BIT) = (PCR(PTO,BIT) & (~PORT_PCR_PE_MASK))|(PE << PORT_PCR_PE_SHIFT) ;
 
/*
 * 引脚中断配置 PTO 端口号 BIT 引脚位序 IRQC 中断配置选择
 */    
#define  IO_IRQ_CONFIG(PTO, BIT, IRQ)   \
    PCR(PTO,BIT) = (PCR(PTO,BIT) & (~PORT_PCR_IRQC_MASK))|PORT_PCR_IRQC(IRQ);   

#define  IO_PE_CONFIG(PTO, BIT, PE)    \
    PCR(PTO,BIT) = (PCR(PTO,BIT) & (~PORT_PCR_PE_MASK))|(PE << PORT_PCR_PE_SHIFT) ; 
    
#define  IO_SRE_CONFIG(PTO, BIT, SRE)    \
    PCR(PTO,BIT)) = (PCR(PTO,BIT) & (~PORT_PCR_SRE_MASK))|(SRE << PORT_PCR_SRE_SHIFT); 
    
#define  IO_PFE_CONFIG(PTO, BIT, PFE)    \
    PCR(PTO,BIT)) = (PCR(PTO,BIT) & (~PORT_PCR_PFE_MASK))|(PFE << PORT_PCR_PFE_SHIFT);  

#define  IO_DSE_CONFIG(PTO, BIT, PFE)    \
    PCR(PTO,BIT)) = (PCR(PTO,BIT) & (~PORT_PCR_DSE_MASK))|(DSE << PORT_PCR_DSE_SHIFT);  


/********************************************************************************************************
  GPIO配置
********************************************************************************************************/
#define PDDR(PTO)     GPIO##PTO##_PDDR
#define PSOR(PORT)    GPIO##PORT##_PSOR
#define PCOR(PORT)    GPIO##PORT##_PCOR
#define PTOR(PORT)    GPIO##PORT##_PTOR
#define PDOR(PORT)    GPIO##PORT##_PDOR
#define PDIR(PORT)    GPIO##PORT##_PDIR
/*
 * GPIO按位输入输出控制
 */
#define GPIO_DDR_OUTPUT(POT,BIT)        PDDR(POT) |= (1 << BIT)
#define GPIO_DDR_INPUT(POT,BIT)         PDDR(POT) &= ~(1 << BIT)
/*
 * GPIO按PORT输入输出控制
 */
#define PORT_DDR_OUTPUT(POT,BITS)       PDDR(POT) |= BITS 
#define PORT_DDR_INPUT(POT,BITS)        PDDR(POT) &= ~BITS
/*
 * GPIO按位置位清零翻转控制
 */
#define GPIO_SET(POT,BIT)               PSOR(POT) = (1 << BIT)     
#define GPIO_CLR(POT,BIT)               PCOR(POT) = (1 << BIT)     
#define GPIO_TOGGLE(POT,BIT)            PTOR(POT) = (1 << BIT)     

/*
 * GPIO按PORT置位清零控制
 */
#define PORT_SET(POT,BITS)              PSOR(POT) = BITS   
#define PORT_CLR(POT,BITS)              PCOR(POT) = BITS   
#define PORT_TOGGLE(POT,BITS)           PTOR(POT) = BITS 
/*
 * GPIO输入值读取
 */
#define GPIO_GET_VALUE(POT,BIT)         ~(~(PDIR(POT) & (1 << BIT)))
#define PORT_GET_VALUE(POT)             PDIR(POT)

#define GPIO_PARAM_DEBUG        0                                               /* 是否进行参数检查     */
  
#define GPIO_PARAM_PORTNUM(x)   ((x >= 'A')&&(x <= 'D')? 1 : 0)                 /* 端口号检查           */
#define GPIO_CHECK_PARAM(x)     (x? (while (1)):)                               /* 错误处理             */
	
#include "main.h"
#define OFF     0
#define ON      1
//#include "zhFontCustom.h"
//#include "MyFont.h"
//#include "jlx12864.h"
typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;
#ifdef __cplusplus
}
#endif

#endif


/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/
