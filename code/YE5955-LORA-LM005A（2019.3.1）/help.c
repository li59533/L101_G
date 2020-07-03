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
    
    SIM_SCGC4 |= SIM_SCGC4_UART2_MASK;                                  /* ��������ʱ��                 */  
    
    uart2TranControl(UART_TX_DISABLE, UART_RX_DISABLE);                 /* ���Ƚ�ֹͨ��                 */


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
    
     

    UART_C1_REG(uartPtr) &= ~(UART_C1_M_MASK |                          /* ���ݳ���                     */
                              UART_C1_PT_MASK |                         /* У��λ����                   */
                              UART_C1_PE_MASK);                         /* У��λ                       */
    
    UART_C1_REG(uartPtr) |= ((ucDataLength - 8UL) << UART_C1_M_SHIFT)|
                            (ucParityEnable << UART_C1_PE_SHIFT)|
                            (ucParityType << UART_C1_PT_SHIFT);                

    
    usBaudRate = SystemBusClock/(ulBaudRate * 16);    
    UART_BDH_REG(uartPtr) = (usBaudRate & 0x1F00) >> 8;                 /* ������                       */
    UART_BDL_REG(uartPtr) = (INT8U)(usBaudRate & UART_BDL_SBR_MASK);
    UART_BDH_REG(uartPtr) &= ~UART_BDH_SBNS_MASK;                       /* ֹͣλ                       */
    UART_BDH_REG(uartPtr) |= (ucStopBit - 1) << UART_BDH_SBNS_SHIFT;
    UART_C2_REG(uartPtr) &= ~(UART_C2_TIE_MASK | UART_C2_TCIE_MASK|     /* ����ж�����                 */
                              UART_C2_RIE_MASK | UART_C2_ILIE_MASK);  

     while ((UART_S1_REG(uartPtr) & UART_S1_RDRF_MASK) && 
           (UART_D_REG(uartPtr)));                                      /* ����ջ�����                 */
                                     
     
		
     
		 while ((UART_S1_REG(uartPtr) & UART_S1_TC_MASK) &&(UART_D_REG(uartPtr)));  
    UART_D_REG(uartPtr);
		 uart2TranControl(UART_TX_ENABLE, UART_RX_ENABLE);                   /* �����������ͨ��             */	
		 
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
    NVIC_SetPriority(UART2_IRQn,3);                                     /* �û��Լ�����                 */        
    #endif            
  ;
}

/*********************************************************************************************************
** Function name:           uart2TranControl
** Descriptions:            UART�Ĵ������
** input parameters:        ucTxEnable:����ʹ�ܿ��ƣ�ucRxEnable:����ʹ�ܿ���
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

    while (!(UART_S1_REG(uartPtr) & UART_S1_RDRF_MASK));                /* �ȴ����ջ���������           */
    return UART_D_REG(uartPtr);                                         /* ���ؽ����ֽ�                 */
}
void  uart2SendChar (uint8_t  ucData)
{
    UART_MemMapPtr uartPtr = UART2_BASE_PTR;

    while (!((UART_S1_REG(uartPtr) & UART_S1_TDRE_MASK)));              /* �ȴ�FIFO����                 */
	 
    UART_D_REG(uartPtr) = ucData;                                       /* ������ݼĴ���               */
   
}

void  UART2_IRQHandler (void)
{ 
	uint8_t c;
   UART_MemMapPtr uartPtr = UART2_BASE_PTR;                             /* �����жϴ������             */
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
		    uart2SendChar(c);                                     /* ���ؽ�������         */              

		 }
		
				
	}
 	
	//
}