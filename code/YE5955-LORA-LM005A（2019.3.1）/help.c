void  uart2Init (INT32U  ulBaudRate, 
                 INT8U   ucParityEnable,
                 INT8U   ucParityType,
                 INT8U   ucDataLength, 
                 INT8U   ucStopBit) 
{
    UART_MemMapPtr uartPtr     = UART2_BASE_PTR;
    register INT16U usBaudRate  = 0;  
    
#if UART_PARAM_DEBUG
    UART_CHECK_PARAM(UART_PARAM_LENGTH(ucDataLength)); 
    UART_CHECK_PARAM(UART_PARAM_STOP(ucStopBit));
    UART_CHECK_PARAM(UART_PARAM_LOGIC(ucParityEnable));
    UART_CHECK_PARAM(UART_PARAM_PARITY(ucParityType));   
#endif  
    
    SIM_SCGC4 |= SIM_SCGC4_UART2_MASK;                                  /* 允许外设时钟                 */  
    
    uart2TranControl(UART_TX_DISABLE, UART_RX_DISABLE);                 /* 首先禁止通信                 */


    #if 0                                                               
    PORTD_PCR3 = PORT_PCR_MUX(0x3);                                     /* UART2_TXD                    */
    PORTD_PCR2 = PORT_PCR_MUX(0x3);                                     /* UART2_RXD                    */
    #endif
    #if 1                                                               
    PORTD_PCR5 = PORT_PCR_MUX(0x3);                                     /* UART2_TXD                    */
    PORTD_PCR4 = PORT_PCR_MUX(0x3);                                     /* UART2_RXD                    */
    #endif
    #if 0                                                               
    PORTE_PCR22 = PORT_PCR_MUX(0x4);                                    /* UART2_TXD                    */
    PORTE_PCR23 = PORT_PCR_MUX(0x4);                                    /* UART2_RXD                    */
    #endif           
    
     

    UART_C1_REG(uartPtr) &= ~(UART_C1_M_MASK |                          /* 数据长度                     */
                              UART_C1_PT_MASK |                         /* 校验位类型                   */
                              UART_C1_PE_MASK);                         /* 校验位                       */
    
    UART_C1_REG(uartPtr) |= ((ucDataLength - 8UL) << UART_C1_M_SHIFT)|
                            (ucParityEnable << UART_C1_PE_SHIFT)|
                            (ucParityType << UART_C1_PT_SHIFT);                

    
    usBaudRate = SystemBusClock/(ulBaudRate * 16);    
    UART_BDH_REG(uartPtr) = (usBaudRate & 0x1F00) >> 8;                 /* 波特率                       */
    UART_BDL_REG(uartPtr) = (INT8U)(usBaudRate & UART_BDL_SBR_MASK);
    UART_BDH_REG(uartPtr) &= ~UART_BDH_SBNS_MASK;                       /* 停止位                       */
    UART_BDH_REG(uartPtr) |= (ucStopBit - 1) << UART_BDH_SBNS_SHIFT;
    UART_C2_REG(uartPtr) &= ~(UART_C2_TIE_MASK | UART_C2_TCIE_MASK|     /* 清除中断设置                 */
                              UART_C2_RIE_MASK | UART_C2_ILIE_MASK);  

     while ((UART_S1_REG(uartPtr) & UART_S1_RDRF_MASK) && 
           (UART_D_REG(uartPtr)));                                      /* 清接收缓冲区                 */
                                     
     
		
     
		 while ((UART_S1_REG(uartPtr) & UART_S1_TC_MASK) &&(UART_D_REG(uartPtr)));  
    UART_D_REG(uartPtr);
		 uart2TranControl(UART_TX_ENABLE, UART_RX_ENABLE);                   /* 配置完成允许通信             */	
		 
    #if UART2_IRQ_ENABLE       
        #if UART2_SEND_IRQ
        UART_C2_REG(uartPtr) |= UART_C2_TCIE_MASK;
        #endif
        #if UART2_RECEIVE_IRQ
        UART_C2_REG(uartPtr) |= UART_C2_RIE_MASK;
		   
        #endif  
    //UART_S1_REG(uartPtr) &= ~UART_S1_TC_MASK;	
   //	UART_D_REG(uartPtr)=0x00;;
		//UART_S1_REG(uartPtr);
	  
    
		
    NVIC_EnableIRQ(UART2_IRQn);
    NVIC_SetPriority(UART2_IRQn,3);                                     /* 用户自己定义                 */        
    #endif            
  ;
}

/*********************************************************************************************************
** Function name:           uart2TranControl
** Descriptions:            UART的传输控制
** input parameters:        ucTxEnable:发送使能控制；ucRxEnable:接收使能控制
** output parameters:       none
** Returned value:          none
** Created by:              
** Created date:            
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
*********************************************************************************************************/
void  uart2TranControl (INT8U  ucTxEnable, 
                        INT8U  ucRxEnable)
{
    UART_MemMapPtr uartPtr = UART2_BASE_PTR;
    
#if UART_PARAM_DEBUG
    UART_CHECK_PARAM(UART_PARAM_LOGIC(ucTxEnable));
    UART_CHECK_PARAM(UART_PARAM_LOGIC(ucRxEnable));
#endif   

    UART_C2_REG(uartPtr) &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);
    UART_C2_REG(uartPtr) |= (ucTxEnable << UART_C2_TE_SHIFT)|
                            (ucRxEnable << UART_C2_RE_SHIFT);
}

uint8_t  uart2GetChar (void)
{
    UART_MemMapPtr uartPtr = UART2_BASE_PTR;

    while (!(UART_S1_REG(uartPtr) & UART_S1_RDRF_MASK));                /* 等待接收缓冲区可用           */
    return UART_D_REG(uartPtr);                                         /* 返回接收字节                 */
}
void  uart2SendChar (uint8_t  ucData)
{
    UART_MemMapPtr uartPtr = UART2_BASE_PTR;

    while (!((UART_S1_REG(uartPtr) & UART_S1_TDRE_MASK)));              /* 等待FIFO可用                 */
	 
    UART_D_REG(uartPtr) = ucData;                                       /* 填充数据寄存器               */
   
}

void  UART2_IRQHandler (void)
{ 
	uint8_t c;
   UART_MemMapPtr uartPtr = UART2_BASE_PTR;                             /* 发送中断处理程序             */
	//UART_S1_REG(uartPtr)|= UART_S1_RDRF_MASK;
  if(((UART_S1_REG(uartPtr) & UART_S1_RDRF_MASK)))//&&((UART_C2_REG(uartPtr)) & UART_C2_RIE_MASK)
	{// 
		//RxdByte((uint8_t)(UART_D_REG(uartPtr)));
		 RxdByte((uint8_t)(uart2GetChar()));
  //	 
	}
	else if((UART_S1_REG(uartPtr) & UART_S1_TC_MASK))//&&((UART_C2_REG(uartPtr) & UART_C2_TCIE_MASK))
	
	{		 // while (UART_S1_REG(uartPtr) & UART_S1_TC_MASK);
		   //UART_S1_REG(uartPtr);
		//  UART_D_REG(uartPtr)=0x00;
		   if(hasByteToTxd())		{
		  	Enable485TXD();
		  	c=getTxdByte();
			 // UART_D_REG(uartPtr) =c;
		    uart2SendChar(c);                                     /* 返回接收数据         */              

		 }
		
				
	}
 	
	//
}