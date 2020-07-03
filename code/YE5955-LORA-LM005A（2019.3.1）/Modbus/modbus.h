#ifndef _MODBUS_H
#define _MODBUS_H


#define  RX_BUFFER_SIZE 0x20
#define  TX_BUFFER_SIZE 0x20

extern uint8_t 	 	 TxdBuf_uart0[];
extern uint8_t 	 	 TxdBuf_uart2[];
extern uint8_t       RxdBuf_uart0[];
extern uint8_t       RxdBuf_uart2[];
extern uint8_t 	     RxdCmdBuf_uart0[];
extern uint8_t 	     RxdCmdBuf_uart2[];


extern void  RxdByte_uart0(uint8_t c);
extern void  RxdByte_uart2(uint8_t c);


extern uint8_t  hasByteToTxd_uart0(void);		 // =1 有字节待发
extern uint8_t  hasByteToTxd_uart2(void);		 // =1 有字节待发
extern uint8_t  getTxdByte_uart0(void);
extern uint8_t  getTxdByte_uart2(void);


extern void   startTxd_uart0(void);   // 定义在CPU中
extern void   startTxd_uart2(void);   // 定义在CPU中

extern void  receiveloop_uart0(void);
extern void  receiveloop_uart2(void);


extern uint16_t  CRC16ISR0(uint8_t * puchMsg, uint8_t usDataLen);
extern uint16_t  CRC16ISR1(uint8_t * puchMsg, uint8_t usDataLen);


extern const  uint8_t      auchCRCHi[];
extern const  uint8_t      auchCRCLo[];



#endif										
