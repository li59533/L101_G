/*----------------------------------------------------------------------------
 * Name:    Retarget.c
 * Purpose: 'Retarget' layer for target-dependent low level functions
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the µVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2009 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <rt_misc.h>
#include "includes.h"
//
//-------- <<< Use Configuration Wizard in Context Menu >>> ----------------------------------------------
//
// <e> Printf Retarget Enable
//     <o1>  	Uart Port Select:
//                     <0=> Uart0
//                     <1=> Uart1
//                     <1=> Uart2
// </e>
#define PRINTF_RE_ENABLE 0
#define UART_TO_PRINTF  1

#if PRINTF_RE_ENABLE
#pragma import(__use_no_semihosting_swi)



extern int sendchar (int c);
extern int getkey   (void);


struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;


int fputc(int c, FILE *f) {
    #if (UART_TO_PRINTF == 0)
    uart0SendChar(c);  
    #endif  
    #if (UART_TO_PRINTF == 1)
    uart1SendChar(c);
    #endif
    #if  (UART_TO_PRINTF == 2)
    uart2SendChar(c);  
    #endif   
    return (c);    
}


int fgetc(FILE *f) {
  //return (getkey());
    #if (UART_TO_PRINTF == 0)
    return (uart0GetChar());
    #endif  
    #if  (UART_TO_PRINTF == 1)
    return (uart1GetChar()); 
    #endif
    #if  (UART_TO_PRINTF == 2)
    return (uart2GetChar()); 
    #endif  
}


int ferror(FILE *f) {
  /* Your implementation of ferror */
    return EOF;
}


void _ttywrch(int c) {
  //sendchar(c);
    #if (UART_TO_PRINTF == 0)
    uart0SendChar(c);
    #endif
    #if (UART_TO_PRINTF == 1)
    uart1SendChar(c);
    #endif    
    #if  (UART_TO_PRINTF == 2)
    uart2SendChar(c); 
    #endif    
}


void _sys_exit(int return_code) {
label:  goto label;  /* endless loop */
}
#endif
