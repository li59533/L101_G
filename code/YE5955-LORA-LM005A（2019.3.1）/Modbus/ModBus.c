#include "includes.h"
#include "app.h"
#include "modbus.h"

//#include "thsenser.h"
//#include "display.h"
//#include "eeprom.h"

#define  NULL  0


uint8_t  TxdBuf_uart0[TX_BUFFER_SIZE];
uint8_t  TxdBuf_uart2[TX_BUFFER_SIZE];
uint8_t  TxdCmdBuf_uart0[TX_BUFFER_SIZE];
uint8_t  TxdCmdBuf_uart2[TX_BUFFER_SIZE];
uint8_t  RxdBuf_uart0[RX_BUFFER_SIZE];
uint8_t  RxdBuf_uart2[RX_BUFFER_SIZE];
uint8_t  RxdCmdBuf_uart0[RX_BUFFER_SIZE];
uint8_t  RxdCmdBuf_uart2[RX_BUFFER_SIZE];


uint8_t  TxdBytes_uart0=0;
uint8_t  TxdBytes_uart2=0;
uint8_t  TxdTelBytes_uart0=0;
uint8_t  TxdTelBytes_uart2=0;
uint8_t  TxdCmdBuf_uart0Length=0;
uint8_t  TxdCmdBuf_uart2Length=0;


uint8_t  ReceivedTel_uart0=0;
uint8_t  ReceivedTel_uart2=0;
uint8_t  RxdBytes_uart0=0;
uint8_t  RxdBytes_uart2=0;
uint8_t  RxdCmdBuf_uart0Length=0;
uint8_t  RxdCmdBuf_uart2Length=0;



uint16_t  CRC16ISR0(uint8_t * puchMsg, uint8_t usDataLen);
uint16_t  CRC16ISR1(uint8_t * puchMsg, uint8_t usDataLen);



extern  uint16_t config_addr;
extern void  saveConfig(void);


typedef union  
{  
    float fdata;  
    unsigned long ldata;  
}FloatLongType; 


void  uart_send_data_uart0(uint8_t *sendstring,uint32_t data_length)
{
	uint32_t i=0;
	for(i=0;i<data_length;i++)
	{
		TxdBuf_uart0[i]=*(sendstring+i);
	}
	TxdBytes_uart0=0;
	TxdTelBytes_uart0=data_length;
	startTxd_uart0();		
}


void  uart_send_data_uart2(uint8_t *sendstring,uint32_t data_length)
{
	uint32_t i=0;
	for(i=0;i<data_length;i++)
	{
		TxdBuf_uart2[i]=*(sendstring+i);
	}
	TxdBytes_uart2=0;
	TxdTelBytes_uart2=data_length;
	startTxd_uart2();		
}


uint8_t  set_displacement_uart2(void)
{
	uart_send_data_uart2(RxdCmdBuf_uart2,RxdCmdBuf_uart2Length);
	config.displacement=RxdCmdBuf_uart2[4];
	saveConfig();
	return (1);
}


uint8_t  reply_displacement_uart2(void)
{
	uint16_t  crc;
	
	TxdCmdBuf_uart2[0]=0x7e;
	TxdCmdBuf_uart2[1]=COMMAND_REPLY_DISPLACEMENT;    //0x00    
	TxdCmdBuf_uart2[2]=0x01;         //低位
	TxdCmdBuf_uart2[3]=0x00;         //高位

	TxdCmdBuf_uart2[4]=config.displacement;
	
    crc=CRC16ISR1(&TxdCmdBuf_uart2[1],3+TxdCmdBuf_uart2[2]+(TxdCmdBuf_uart2[3]<<8));
	TxdCmdBuf_uart2[5]=crc;          //低位
	TxdCmdBuf_uart2[6]=crc>>8;       //高位
    TxdCmdBuf_uart2[7]=0x7e;
	TxdCmdBuf_uart2Length=7+TxdCmdBuf_uart2[2]+(TxdCmdBuf_uart2[3]<<8);
    uart_send_data_uart2(TxdCmdBuf_uart2,TxdCmdBuf_uart2Length);
	return (1);
}


uint8_t  set_ack_uart2(void)
{
	uart_send_data_uart2(RxdCmdBuf_uart2,RxdCmdBuf_uart2Length);
	config.netack=RxdCmdBuf_uart2[4];
	saveConfig();
	return (1);
}


uint8_t  reply_ack_uart2(void)
{
	uint16_t  crc;
	
	TxdCmdBuf_uart2[0]=0x7e;
	TxdCmdBuf_uart2[1]=COMMAND_REPLY_ACK;              //0x02   
	TxdCmdBuf_uart2[2]=0x01;        //低位
	TxdCmdBuf_uart2[3]=0x00;        //高位

	TxdCmdBuf_uart2[4]=config.netack;
	
    crc=CRC16ISR1(&TxdCmdBuf_uart2[1],3+TxdCmdBuf_uart2[2]+(TxdCmdBuf_uart2[3]<<8));
	TxdCmdBuf_uart2[5]=crc;         //低位
	TxdCmdBuf_uart2[6]=crc>>8;      //高位
    TxdCmdBuf_uart2[7]=0x7e;
	TxdCmdBuf_uart2Length=7+TxdCmdBuf_uart2[2]+(TxdCmdBuf_uart2[3]<<8);
    uart_send_data_uart2(TxdCmdBuf_uart2,TxdCmdBuf_uart2Length);
	return (1);
}


uint8_t  set_rate_uart2(void)
{
	uart_send_data_uart2(RxdCmdBuf_uart2,RxdCmdBuf_uart2Length);
	config.workcycleminutes=RxdCmdBuf_uart2[4]+(((uint16_t)RxdCmdBuf_uart2[5])<<8);;
	saveConfig();
	return (1);
}


uint8_t  reply_rate_uart2(void)
{
	uint16_t  crc;
	
	TxdCmdBuf_uart2[0]=0x7e;
	TxdCmdBuf_uart2[1]=COMMAND_REPLY_RATE;              //0x04   
	TxdCmdBuf_uart2[2]=0x02;        //低位
	TxdCmdBuf_uart2[3]=0x00;        //高位

	TxdCmdBuf_uart2[4]=config.workcycleminutes;
	TxdCmdBuf_uart2[5]=config.workcycleminutes>>8;
	
    crc=CRC16ISR1(&TxdCmdBuf_uart2[1],3+TxdCmdBuf_uart2[2]+(TxdCmdBuf_uart2[3]<<8));
	TxdCmdBuf_uart2[6]=crc;         //低位
	TxdCmdBuf_uart2[7]=crc>>8;      //高位
    TxdCmdBuf_uart2[8]=0x7e;
	TxdCmdBuf_uart2Length=7+TxdCmdBuf_uart2[2]+(TxdCmdBuf_uart2[3]<<8);
    uart_send_data_uart2(TxdCmdBuf_uart2,TxdCmdBuf_uart2Length);
	return (1);
}


uint8_t  reply_measure_uart2(void)
{
	uint16_t  crc;
	
	FloatLongType acce;    //加速度
	FloatLongType speed;   //速度
	FloatLongType disp;    //位移
	FloatLongType temp;    //温度
	
	acce.fdata=Parameter.adate;
	speed.fdata=Parameter.vdate;
	disp.fdata=Parameter.xdate;
	temp.fdata=Parameter.pdate;
	
	TxdCmdBuf_uart2[0]=0x7e;
	TxdCmdBuf_uart2[1]=COMMAND_REPLY_MEASURE;              //0x06   
	TxdCmdBuf_uart2[2]=0x10;        //低位
	TxdCmdBuf_uart2[3]=0x00;        //高位

	TxdCmdBuf_uart2[4]=acce.ldata;
	TxdCmdBuf_uart2[5]=acce.ldata>>8;
	TxdCmdBuf_uart2[6]=acce.ldata>>16;
	TxdCmdBuf_uart2[7]=acce.ldata>>24;
	
	TxdCmdBuf_uart2[8]=speed.ldata;
	TxdCmdBuf_uart2[9]=speed.ldata>>8;
	TxdCmdBuf_uart2[10]=speed.ldata>>16;
	TxdCmdBuf_uart2[11]=speed.ldata>>24;
	
	TxdCmdBuf_uart2[12]=disp.ldata;
	TxdCmdBuf_uart2[13]=disp.ldata>>8;
	TxdCmdBuf_uart2[14]=disp.ldata>>16;
	TxdCmdBuf_uart2[15]=disp.ldata>>24;
	
	TxdCmdBuf_uart2[16]=temp.ldata;
	TxdCmdBuf_uart2[17]=temp.ldata>>8;
	TxdCmdBuf_uart2[18]=temp.ldata>>16;
	TxdCmdBuf_uart2[19]=temp.ldata>>24;
	
    crc=CRC16ISR1(&TxdCmdBuf_uart2[1],3+TxdCmdBuf_uart2[2]+(TxdCmdBuf_uart2[3]<<8));
	TxdCmdBuf_uart2[20]=crc;         //低位
	TxdCmdBuf_uart2[21]=crc>>8;      //高位
    TxdCmdBuf_uart2[22]=0x7e;
	TxdCmdBuf_uart2Length=7+TxdCmdBuf_uart2[2]+(TxdCmdBuf_uart2[3]<<8);
    uart_send_data_uart2(TxdCmdBuf_uart2,TxdCmdBuf_uart2Length);
	return (1);
}


uint8_t  set_adc_uart2(void)
{
	FloatLongType adc; 
	
	uart_send_data_uart2(RxdCmdBuf_uart2,RxdCmdBuf_uart2Length);
	adc.ldata=RxdCmdBuf_uart2[4];
	adc.ldata+=RxdCmdBuf_uart2[5]<<8;
	adc.ldata+=RxdCmdBuf_uart2[6]<<16;
	adc.ldata+=RxdCmdBuf_uart2[7]<<24;
	config.ADCscale=adc.fdata;
	saveConfig();
	return (1);
}


uint8_t  reply_adc_uart2(void)            
{
	uint16_t  crc;
	
	FloatLongType adc;   	
	adc.fdata=config.ADCscale;	
	
	TxdCmdBuf_uart2[0]=0x7e;
	TxdCmdBuf_uart2[1]=COMMAND_REPLY_ADC;              //0x08   
	TxdCmdBuf_uart2[2]=0x04;        //低位
	TxdCmdBuf_uart2[3]=0x00;        //高位

	TxdCmdBuf_uart2[4]=adc.ldata;
	TxdCmdBuf_uart2[5]=adc.ldata>>8;
	TxdCmdBuf_uart2[6]=adc.ldata>>16;
	TxdCmdBuf_uart2[7]=adc.ldata>>24;
	
    crc=CRC16ISR1(&TxdCmdBuf_uart2[1],3+TxdCmdBuf_uart2[2]+(TxdCmdBuf_uart2[3]<<8));
	TxdCmdBuf_uart2[8]=crc;         //低位
	TxdCmdBuf_uart2[9]=crc>>8;      //高位
    TxdCmdBuf_uart2[10]=0x7e;
	TxdCmdBuf_uart2Length=7+TxdCmdBuf_uart2[2]+(TxdCmdBuf_uart2[3]<<8);
    uart_send_data_uart2(TxdCmdBuf_uart2,TxdCmdBuf_uart2Length);
	return (1);
}


uint8_t  set_lmd_uart2(void)
{
	FloatLongType lmd; 
	
	uart_send_data_uart2(RxdCmdBuf_uart2,RxdCmdBuf_uart2Length);
	lmd.ldata=RxdCmdBuf_uart2[4];
	lmd.ldata+=RxdCmdBuf_uart2[5]<<8;
	lmd.ldata+=RxdCmdBuf_uart2[6]<<16;
	lmd.ldata+=RxdCmdBuf_uart2[7]<<24;
	config.LMDscale=lmd.fdata;
	saveConfig();
	return (1);
}


uint8_t  reply_lmd_uart2(void)
{
	uint16_t  crc;
	
	FloatLongType lmd;   	
	lmd.fdata=config.LMDscale;	
	
	TxdCmdBuf_uart2[0]=0x7e;
	TxdCmdBuf_uart2[1]=COMMAND_REPLY_LMD;              //0x0a   
	TxdCmdBuf_uart2[2]=0x04;        //低位
	TxdCmdBuf_uart2[3]=0x00;        //高位

	TxdCmdBuf_uart2[4]=lmd.ldata;
	TxdCmdBuf_uart2[5]=lmd.ldata>>8;
	TxdCmdBuf_uart2[6]=lmd.ldata>>16;
	TxdCmdBuf_uart2[7]=lmd.ldata>>24;
	
    crc=CRC16ISR1(&TxdCmdBuf_uart2[1],3+TxdCmdBuf_uart2[2]+(TxdCmdBuf_uart2[3]<<8));
	TxdCmdBuf_uart2[8]=crc;         //低位
	TxdCmdBuf_uart2[9]=crc>>8;      //高位
    TxdCmdBuf_uart2[10]=0x7e;
	TxdCmdBuf_uart2Length=7+TxdCmdBuf_uart2[2]+(TxdCmdBuf_uart2[3]<<8);
    uart_send_data_uart2(TxdCmdBuf_uart2,TxdCmdBuf_uart2Length);
	return (1);
}


uint8_t  set_crt_uart2(void)
{
	FloatLongType crt; 
	
	uart_send_data_uart2(RxdCmdBuf_uart2,RxdCmdBuf_uart2Length);
	crt.ldata=RxdCmdBuf_uart2[4];
	crt.ldata+=RxdCmdBuf_uart2[5]<<8;
	crt.ldata+=RxdCmdBuf_uart2[6]<<16;
	crt.ldata+=RxdCmdBuf_uart2[7]<<24;
	config.CRTscale=crt.fdata;
	saveConfig();
	return (1);
}


uint8_t  reply_crt_uart2(void)
{
	uint16_t  crc;
	
	FloatLongType  crt;   	
	crt.fdata=config.CRTscale;	
	
	TxdCmdBuf_uart2[0]=0x7e;
	TxdCmdBuf_uart2[1]=COMMAND_REPLY_CRT;              //0x0c   
	TxdCmdBuf_uart2[2]=0x04;        //低位
	TxdCmdBuf_uart2[3]=0x00;        //高位

	TxdCmdBuf_uart2[4]=crt.ldata;
	TxdCmdBuf_uart2[5]=crt.ldata>>8;
	TxdCmdBuf_uart2[6]=crt.ldata>>16;
	TxdCmdBuf_uart2[7]=crt.ldata>>24;
	
    crc=CRC16ISR1(&TxdCmdBuf_uart2[1],3+TxdCmdBuf_uart2[2]+(TxdCmdBuf_uart2[3]<<8));
	TxdCmdBuf_uart2[8]=crc;         //低位
	TxdCmdBuf_uart2[9]=crc>>8;      //高位
    TxdCmdBuf_uart2[10]=0x7e;
	TxdCmdBuf_uart2Length=7+TxdCmdBuf_uart2[2]+(TxdCmdBuf_uart2[3]<<8);
    uart_send_data_uart2(TxdCmdBuf_uart2,TxdCmdBuf_uart2Length);
	return (1);
}


uint8_t  set_addr_uart2(void)
{
	uart_send_data_uart2(RxdCmdBuf_uart2,RxdCmdBuf_uart2Length);
	config.addr=RxdCmdBuf_uart2[4]+(((uint16_t)RxdCmdBuf_uart2[5])<<8);;
	saveConfig();
	return (1);
}


uint8_t  reply_addr_uart2(void)
{
	uint16_t  crc;
	
	TxdCmdBuf_uart2[0]=0x7e;
	TxdCmdBuf_uart2[1]=COMMAND_REPLY_ADDR;              //0x0e   
	TxdCmdBuf_uart2[2]=0x02;        //低位
	TxdCmdBuf_uart2[3]=0x00;        //高位

	TxdCmdBuf_uart2[4]=config.addr;
	TxdCmdBuf_uart2[5]=config.addr>>8;
	
    crc=CRC16ISR1(&TxdCmdBuf_uart2[1],3+TxdCmdBuf_uart2[2]+(TxdCmdBuf_uart2[3]<<8));
	TxdCmdBuf_uart2[6]=crc;         //低位
	TxdCmdBuf_uart2[7]=crc>>8;      //高位
    TxdCmdBuf_uart2[8]=0x7e;
	TxdCmdBuf_uart2Length=7+TxdCmdBuf_uart2[2]+(TxdCmdBuf_uart2[3]<<8);
    uart_send_data_uart2(TxdCmdBuf_uart2,TxdCmdBuf_uart2Length);
	return (1);
}


uint8_t  getTelLength_uart0(void)
{					
	switch(RxdBuf_uart0[1])
	{
		case 0x03: 
		case 0x04: return(6);
		case 0x05:
		case 0x06: return(6);
		case 0x10: return(RxdBuf_uart0[6]+7);
		default:
				   return(0);
	}					
}


uint8_t  getTelLength_uart2(void)
{					
	switch(RxdBuf_uart2[1])
	{
		case 0x00: return(8);
		case 0x01: return(8);
		case 0x02: return(8);
		case 0x03: return(8);
		case 0x04: return(9);
		case 0x05: return(9);
		case 0x06: return(11);
		case 0x07: return(11);
		case 0x08: return(11);
		case 0x09: return(11);
		case 0x0a: return(11);
		case 0x0b: return(11);
		case 0x0c: return(11);
		case 0x0d: return(9);
		case 0x0e: return(9);
		default:
				   return(0);
	}					
}



uint8_t  isVaildTel_uart0(void)
{
	if(RxdBytes_uart0>=1)if(RxdBuf_uart0[0]!=config_addr)return(0);
	if(RxdBytes_uart0>=2)if(RxdBuf_uart0[1]!=0x03 && RxdBuf_uart0[1]!=0x05 && RxdBuf_uart0[1]!=0x04 && RxdBuf_uart0[1]!=0x06 && RxdBuf_uart0[1]!=0x10)return(0);

	return(1);				          //合法的
}


uint8_t  isVaildTel_uart2(void)
{
	if(RxdBytes_uart2>=1)if(RxdBuf_uart2[0]!=0x7e)return(0);
	if(RxdBytes_uart2>=2)if(RxdBuf_uart2[1]!=0x00 && RxdBuf_uart2[1]!=0x01 && RxdBuf_uart2[1]!=0x02 
		                 && RxdBuf_uart2[1]!=0x03 && RxdBuf_uart2[1]!=0x04 && RxdBuf_uart2[1]!=0x05
						 && RxdBuf_uart2[1]!=0x06 && RxdBuf_uart2[1]!=0x07 && RxdBuf_uart2[1]!=0x08
					     && RxdBuf_uart2[1]!=0x09 && RxdBuf_uart2[1]!=0x0a && RxdBuf_uart2[1]!=0x0b
						 && RxdBuf_uart2[1]!=0x0c && RxdBuf_uart2[1]!=0x0d && RxdBuf_uart2[1]!=0x0e)
						 return(0);
	if(RxdBytes_uart2>=4) 
	{ 
		uint16_t length=getTelLength_uart2();
		if((length>16)) return(0); 
	}
	return(1);				          //合法的
}



uint8_t  isTelComplete_uart0(void)	   // =0 不完整  =1 CRC Error =2 正确
{
	uint16_t  temp16;
	uint8_t  dal, dah, dat_len;

	if(RxdBytes_uart0<8)return(0);
  ////////////////
	dat_len=getTelLength_uart0();	//给数据长度
	if(dat_len==0)return(0);
	if(RxdBytes_uart0<(dat_len+2))return(0);

	temp16=CRC16ISR0(&RxdBuf_uart0[0],dat_len);
 
   	dal=temp16>>8;
	dah=temp16;

	if ((RxdBuf_uart0[dat_len]==dal)&&(RxdBuf_uart0[dat_len+1]==dah))
		return(2); 
	else
	{
		return(1);
	}	
}						 


uint8_t  isTelComplete_uart2(void)	   // =0 不完整  =1 CRC Error =2 正确
{
	uint16_t  temp16;
	uint8_t  dal, dah, dat_len;

	if(RxdBytes_uart2<4)return(0);

	dat_len=getTelLength_uart2();	//给数据长度
	if(dat_len==0)return(0);
	if(RxdBytes_uart2<dat_len)return(0);

	temp16=CRC16ISR0(&RxdBuf_uart2[1],dat_len-4);
 
   	dal=temp16>>8;
	dah=temp16;

	if ((RxdBuf_uart2[dat_len-3]==dal)&&(RxdBuf_uart2[dat_len-2]==dah))
		return(2); 
	else
	{
		return(1);
	}	
}


uint8_t  leftRxdTel_uart0(void)		//数组左移一位
{
	uint8_t i;
	if(RxdBytes_uart0<1)return(0);     // 无法左移
	for	(i=1;i<RxdBytes_uart0;i++)
	{
		RxdBuf_uart0[i-1]=RxdBuf_uart0[i];		
	}
	RxdBytes_uart0--;
	return(1);					 // 丢弃一个字节成功

}


uint8_t  leftRxdTel_uart2(void)		//数组左移一位
{
	uint8_t i;
	if(RxdBytes_uart2<1)return(0);     // 无法左移
	for	(i=1;i<RxdBytes_uart2;i++)
	{
		RxdBuf_uart2[i-1]=RxdBuf_uart2[i];		
	}
	RxdBytes_uart2--;
	return(1);					 // 丢弃一个字节成功

}


void  RxdByte_uart0(uint8_t c)
{	
	uint8_t 	i;
	RxdBuf_uart0[RxdBytes_uart0]=c;
	RxdBytes_uart0++;

	switch(RxdBytes_uart0)
	{
		case 0:	break;
		case 1:
		case 2:
				while(!isVaildTel_uart0())	//如果不合法			 
				{
					if(!leftRxdTel_uart0())break;	  // 丢弃首字节
				}
				break;
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:	break;
		default:		
				i=isTelComplete_uart0();
				if(i==2)
				{
					//do some thing
					for(i=0;i<RxdBytes_uart0;i++)	
					{
						RxdCmdBuf_uart0[i]=RxdBuf_uart0[i];
					}
					RxdCmdBuf_uart0Length=RxdBytes_uart0;
					//ReceivedTel_uart0=1;
					RxdBytes_uart0=0;
					receiveloop_uart0();
				}
				else if(i==1)	 // CRC error
				{
					leftRxdTel_uart0();
					while(!isVaildTel_uart0())	//如果不合法			 
					{
						if(!leftRxdTel_uart0())break;
					}	
				}
				else if(i==0) //没收完继续收
				{
				
				}
				else
				{
				}
				break;			
	}
}


void  RxdByte_uart2(uint8_t c)
{	
	uint8_t 	i;
	RxdBuf_uart2[RxdBytes_uart2]=c;
	RxdBytes_uart2++;

	switch(RxdBytes_uart2)
	{
		case 0:	break;
		case 1:	break;
		case 2:	break;
		case 3: break;
		case 4:	while(!isVaildTel_uart2())	//如果不合法			 
				{
					if(!leftRxdTel_uart2())break;	  // 丢弃首字节
				}
				break;
		
		default:		
				i=isTelComplete_uart2();
				if(i==2)
				{
					//do some thing
					for(i=0;i<RxdBytes_uart2;i++)	
					{
						RxdCmdBuf_uart2[i]=RxdBuf_uart2[i];
					}
					RxdCmdBuf_uart2Length=RxdBytes_uart2;
					//ReceivedTel_uart2=1;
					RxdBytes_uart2=0;
					receiveloop_uart2();
				}
				else if(i==1)	 // CRC error
				{
					leftRxdTel_uart2();
					while(!isVaildTel_uart2())	//如果不合法			 
					{
						if(!leftRxdTel_uart2())break;
					}	
				}
				else if(i==0) //没收完继续收
				{
				
				}
				else
				{
				}
				break;			
	}
}


uint8_t  hasByteToTxd_uart0()		 // =1 有字节待发
{
	if(TxdBytes_uart0<TxdTelBytes_uart0)return(1);
	TxdTelBytes_uart0=0;
	TxdBytes_uart0=0;
	return(0);	
}


uint8_t  hasByteToTxd_uart2()		 // =1 有字节待发
{
	if(TxdBytes_uart2<TxdTelBytes_uart2)return(1);
	TxdTelBytes_uart2=0;
	TxdBytes_uart2=0;
	return(0);	
}



uint8_t  getTxdByte_uart0()   
{
	uint8_t re;
	if(TxdBytes_uart0 <TxdTelBytes_uart0)
	{
		re=TxdBuf_uart0[TxdBytes_uart0];
		TxdBytes_uart0++;
		return(re);
	}
	else
	{
		TxdTelBytes_uart0=0;
		TxdBytes_uart0=0;
		return(0);
	}
}


uint8_t  getTxdByte_uart2()   
{
	uint8_t re;
	if(TxdBytes_uart2 <TxdTelBytes_uart2)
	{
		re=TxdBuf_uart2[TxdBytes_uart2];
		TxdBytes_uart2++;
		return(re);
	}
	else
	{
		TxdTelBytes_uart2=0;
		TxdBytes_uart2=0;
		return(0);
	}
}



void  Enable485TXD_uart0(void) 
{	
	UART0_MemMapPtr uartPtr = UART0_BASE_PTR;
	UART_C2_REG(uartPtr) |= UART_C2_TCIE_MASK;
}


void  Enable485TXD_uart2(void) 
{	
	UART_MemMapPtr uartPtr = UART2_BASE_PTR;
	UART_C2_REG(uartPtr) |= UART_C2_TCIE_MASK;
}


void  Disable485TXD_uart0(void)
{	
	UART0_MemMapPtr uartPtr = UART0_BASE_PTR;
	UART_C2_REG(uartPtr)&= ~(UART_C2_TCIE_MASK);	
}

void  Disable485TXD_uart2(void)
{	
	UART_MemMapPtr uartPtr = UART2_BASE_PTR;
	UART_C2_REG(uartPtr)&= ~(UART_C2_TCIE_MASK);	
}


void  startTxd_uart0()
{
	Enable485TXD_uart0();
	uart0SendChar(getTxdByte_uart0());
}


void  startTxd_uart2()
{
	Enable485TXD_uart2();
	uart2SendChar(getTxdByte_uart2());
}


void  UART0_IRQHandler(void)      // 对网络模块
{ 
	uint8_t c;
	uint8_t inchar;
	UART0_MemMapPtr uartPtr = UART0_BASE_PTR;                             /* 发送中断处理程序             */
	//UART_S1_REG(uartPtr)|= UART_S1_RDRF_MASK;
	if ((UART0_S1_REG(uartPtr) & UART0_S1_RDRF_MASK)&& ((UART0_C2_REG(uartPtr) & UART0_C2_RIE_MASK)))        //用来判断中断源到底来自哪里，后面这句很重要
	{                    /* 清除中断标志                 */
		inchar=(uint8_t)uart0GetChar();                             /* 返回接收数据                 */     
		strLM005A_Fram_Record.Data_RX_BUF [ strLM005A_Fram_Record.InfBit.FramLength++ ]  = inchar;
	} 
	else if(((UART0_S1_REG(uartPtr) & UART0_S1_TC_MASK)) && ((UART0_C2_REG(uartPtr) & UART0_C2_TCIE_MASK)))  //用来判断中断源到底来自哪里，后面这句很重要	
	{		 
		if(hasByteToTxd_uart0())		
		{
		  	Enable485TXD_uart0();
		  	c=getTxdByte_uart0();
			 // UART_D_REG(uartPtr) =c;
		    uart0SendChar(c);                                     /* 返回接收数据         */              
		}
		else
		{
			Disable485TXD_uart0();
		}				
	}
	UART0_S2 |= UART_S2_RXEDGIF_MASK;
}


void  UART2_IRQHandler(void)      // 对上位机
{ 
	uint8_t c;
	UART_MemMapPtr uartPtr = UART2_BASE_PTR;                             /* 发送中断处理程序             */
	if ((UART_S1_REG(uartPtr) & UART_S1_RDRF_MASK)&& ((UART_C2_REG(uartPtr) & UART_C2_RIE_MASK)))        //用来判断中断源到底来自哪里，后面这句很重要
	{                    /* 清除中断标志                 */
		RxdByte_uart2((uint8_t)(uart2GetChar()));		
	} 
	else if(((UART_S1_REG(uartPtr) & UART_S1_TC_MASK)) && ((UART_C2_REG(uartPtr) & UART_C2_TCIE_MASK)))  //用来判断中断源到底来自哪里，后面这句很重要	
	{		 
		if(hasByteToTxd_uart2())		
		{
//		  	Enable485TXD_uart2();
		  	c=getTxdByte_uart2();
		    uart2SendChar(c);                                     /* 返回接收数据         */              
		}
		else
		{
			Disable485TXD_uart2();
		}				
	}
	UART2_S2 |= UART_S2_RXEDGIF_MASK;//外部触发使能，且清标志
	UART2_S1 |= UART_S1_OR_MASK;
}
			


void  receiveloop_uart0()    // 对网络模块
{
	
}


void  receiveloop_uart2()    // 对上位机
{
	switch(RxdCmdBuf_uart2[1])
	{     
		case COMMAND_REPLY_DISPLACEMENT:      //0x00
			   reply_displacement_uart2();
			break;
		case COMMAND_SET_DISPLACEMENT:        //0x01
			   set_displacement_uart2();
			break;
		case COMMAND_REPLY_ACK:               //0x02
			   reply_ack_uart2();
			break;
		case COMMAND_SET_ACK:                 //0x03
			   set_ack_uart2();
			break;
		case COMMAND_REPLY_RATE:              //0x04  
			   reply_rate_uart2();
 			break;
		case COMMAND_SET_RATE:                //0x05  
			   set_rate_uart2();
			break;
		case COMMAND_REPLY_MEASURE:           //0x06  
			   reply_measure_uart2();
			break;
		case COMMAND_REPLY_ADC:               //0x08  
			   reply_adc_uart2();
			break;
		case COMMAND_SET_ADC:                 //0x07  
			   set_adc_uart2();
			break;
		case COMMAND_REPLY_LMD:               //0x0a  
			   reply_lmd_uart2();
			break;
		case COMMAND_SET_LMD:                 //0x09  
			   set_lmd_uart2();
			break;
		case COMMAND_REPLY_CRT:               //0x0c  
			   reply_crt_uart2();
			break;
		case COMMAND_SET_CRT:                 //0x0b  
			   set_crt_uart2();
			break;	
		case COMMAND_REPLY_ADDR:               //0x0e  
			   reply_addr_uart2();
			break;
		case COMMAND_SET_ADDR:                 //0x0d  
			   set_addr_uart2();
			break;			
	default:
			break;
	}
}


uint16_t  getModbusData1(uint16_t addr)
{
	uint16_t r=0;
	
	if(addr==0)
	{
		if(Parameter.adate>620) Parameter.adate=650;
		if(Parameter.adate<0)r=(uint16_t)(-Parameter.adate*100+0.5)|0x8000;
		else r=(uint16_t)(Parameter.adate*100+0.5);	
	}
	if(addr==1)
	{
		if(Parameter.vdate>620) Parameter.vdate=650;
		if(Parameter.vdate<0)r=(uint16_t)(-Parameter.vdate*100+0.5)|0x8000;
		else r=(uint16_t)(Parameter.vdate*100+0.5);
		
	}
	if(addr==2)
	{
		if(Parameter.pdate<0)r=(uint16_t)(-Parameter.pdate*100+0.5)|0x8000;
		else r=(uint16_t)(Parameter.pdate*100+0.5);
		
	}
	if(addr==3)
	{
		r=(uint16_t)(Parameter.battery);	
	}
	if(addr==5)
	{
		r=(uint16_t)(config.SNnumber);	
	}
	if(addr==12)
	{
		r=(uint16_t)(config.addr);	
	}
	if(addr==13)
	{
		r=(uint16_t)(config.baundrate);	
	}
	if(addr==14)
	{
		r=(uint16_t)(config.ParityType);	
	}
	if(addr==15)
	{
		r=(uint16_t)(config.DataLength);	
	}
	if(addr==16)
	{
		r=(uint16_t)(config.StopBit);	
	}	
	return(r);
}



uint16_t  getModbusData2(uint16_t addr)
{
	uint16_t r=0;
	
	if(addr==0)
	{
		if(Parameter.adate>620) Parameter.adate=650;
		if(Parameter.adate<0)r=(uint16_t)(-Parameter.adate*100+0.5)|0x8000;
		else r=(uint16_t)(Parameter.adate*100+0.5);	
	}
	if(addr==1)
	{
		if(Parameter.vdate>620) Parameter.vdate=650;
		if(Parameter.vdate<0)r=(uint16_t)(-Parameter.vdate*100+0.5)|0x8000;
		else r=(uint16_t)(Parameter.vdate*100+0.5);	
	}
	if(addr==2)
	{
		
		if(Parameter.xdate<0)r=(uint16_t)(-Parameter.xdate*100+0.5)|0x8000;
		else r=(uint16_t)(Parameter.xdate*100+0.5);	
	}
	if(addr==3)
	{
		if(Parameter.pdate<0)r=(uint16_t)(-Parameter.pdate*100+0.5)|0x8000;
		else r=(uint16_t)(Parameter.pdate*100+0.5);	
	}
	if(addr==4)
	{
		r=(uint16_t)(Parameter.battery);	
	}
	if(addr==5)
	{
		r=(uint16_t)(config.SNnumber);	
	}
	if(addr==12)
	{
		r=(uint16_t)(config.addr);	
	}
	if(addr==13)
	{
		r=(uint16_t)(config.baundrate);	
	}
	if(addr==14)
	{
		r=(uint16_t)(config.ParityType);	
	}
	if(addr==15)
	{
		r=(uint16_t)(config.DataLength);	
	}
	if(addr==16)
	{
		r=(uint16_t)(config.StopBit);	
	}	
	return(r);
}


uint32_t  OnOrganizeData1(void)
{
	uint8_t   i;
	uint16_t  startaddr=0;
	uint16_t  datanum=3;
	uint16_t  tt;
	uint8_t   *pb;
	uint8_t   command=3;
	TxdBuf_uart0[0] = config.addr>>8;    //高位
	TxdBuf_uart0[1] = config.addr;       //低位
	TxdBuf_uart0[2] = command;
	TxdBuf_uart0[3] = datanum*2;
	pb=&TxdBuf_uart0[4];

	for(i=0;i<datanum;i++)	
	{
		tt=getModbusData1(startaddr+i);
		*pb=tt>>8;          //高位
		pb++;
		*pb=tt&0xff;        //低位
		pb++;
	}
	
	tt=CRC16ISR1(&TxdBuf_uart0[0],4+TxdBuf_uart0[3]);
	*pb=tt/256; *(pb+1)=tt%256; 

	return(6+datanum*2);
}


uint32_t  OnOrganizeData2(void)
{
	uint8_t   i;
	uint16_t  startaddr=0;
	uint16_t  datanum=4;
	uint16_t  tt;
	uint8_t   *pb;
	uint8_t   command=3;
	TxdBuf_uart0[0] = config.addr>>8;    //高位
	TxdBuf_uart0[1] = config.addr;       //低位
	TxdBuf_uart0[2] = command;
	TxdBuf_uart0[3] = datanum*2;
	pb=&TxdBuf_uart0[4];

	for(i=0;i<datanum;i++)	
	{
		tt=getModbusData2(startaddr+i);
		*pb=tt>>8;          //高位
		pb++;
		*pb=tt&0xff;        //低位
		pb++;
	}
	
	tt=CRC16ISR1(&TxdBuf_uart0[0],4+TxdBuf_uart0[3]);
	*pb=tt/256; *(pb+1)=tt%256; //先高后低

	return(6+datanum*2);
}


uint16_t  CRC16ISR0(uint8_t * puchMsg, uint8_t usDataLen) 
{
	uint8_t uchCRCHi=0xFF; // 初始化高字节
	uint8_t uchCRCLo=0xFF; // 初始化低字节
	uint8_t uIndex ;      //把CRC表
	while (usDataLen--) //通过数据缓冲器
	{
		uIndex = uchCRCHi^*puchMsg++ ; //计算CRC 
		uchCRCHi=uchCRCLo^auchCRCHi[uIndex] ;
		uchCRCLo=auchCRCLo[uIndex] ;
	}
	return ((((uint16_t)uchCRCHi )<< 8 )| uchCRCLo);
}


uint16_t  CRC16ISR1(uint8_t * puchMsg, uint8_t usDataLen)
{
	uint8_t uchCRCHi = 0xFF ; // 初始化高字节
	uint8_t uchCRCLo = 0xFF ; // 初始化低字节
	uint8_t uIndex ;          //把CRC表
	while (usDataLen--)     //通过数据缓冲器
	{
		uIndex = uchCRCHi ^ *puchMsg++ ; //计算CRC 
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ;
		uchCRCLo = auchCRCLo[uIndex] ;
	}
	return ((((uint16_t)uchCRCHi )<< 8 )| uchCRCLo);
}


const  uint8_t  auchCRCHi[] = 
{
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
	0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
	0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
	0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
	0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
	0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
	0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
	0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
	0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
	0x40
};


const  uint8_t  auchCRCLo[] = 
{
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
	0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
	0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
	0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
	0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
	0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
	0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
	0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
	0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
	0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
	0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
	0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
	0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
	0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
	0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
	0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
	0x40
};
