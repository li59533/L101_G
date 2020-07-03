#include "includes.h"
#include "app.h"
#include "modbus.h"
//#include "thsenser.h"
//#include "display.h"
//#include "eeprom.h"


uint8_t 	 	 TxdBuf[TX_BUFFER_SIZE];
uint8_t        RxdBuf[RX_BUFFER_SIZE];
uint8_t    RxdTelBuf[RX_BUFFER_SIZE];
uint8_t TxdBytes=0;
uint8_t RxdBytes=0;
uint8_t TxdTelBytes=0;
uint8_t ReceivedTel=0;





uint8_t 	 	    comtxdframelen=0;
uint8_t         comtxdflag=0;
uint8_t	   	    rxdcomchar;
uint8_t	   	    Updataflag=0;
uint16_t        comtxdflagArr[2];


//void	huihu10(void);
//void    MakeComTxdFrame(void);
void  OnGetData03(void);
void  OnGetData04(void);
void  OnSetData06(void);
void  OnSetData05(void);
uint16_t CRC16ISR0(uint8_t * puchMsg, uint8_t usDataLen);

uint16_t  CRC16ISR1(uint8_t * puchMsg, uint8_t usDataLen);
void MakeComUpdata()
{
	uint8_t	dal;
	

	if(!Updataflag)	return;
	Updataflag=0;
	switch(comtxdflagArr[0])
	{
		case 0x8000:
//			config.pt=RxdTelBuf[7]*256+RxdTelBuf[8];
//		   	config.ct=RxdTelBuf[9]*256+RxdTelBuf[10];

			
		break;
		case 0x8400:
			if(RxdTelBuf[6]!=30)	break;
//			saveConfig();
		
//			for (dal=0;dal<6;dal++)
			{
//				AdjustData.UaSub= *((uint16_t*)&RxdTelBuf[7]);
//				AdjustData.UbSub= *((uint16_t*)&RxdTelBuf[1*2+7]);
//				AdjustData.UcSub= *((uint16_t*)&RxdTelBuf[2*2+7]);
//				AdjustData.IaSub= *((uint16_t*)&RxdTelBuf[3*2+7]);
	//			AdjustData.IbSub= *((uint16_t*)&RxdTelBuf[4*2+7]);
//				AdjustData.IcSub= *((uint16_t*)&RxdTelBuf[5*2+7]);
			}
			for (dal=0;dal<6;dal++)
//				AdjustData.PhaseA= *((uint32_t*)&RxdTelBuf[0*3+18])&0x00ffffff;
//				AdjustData.PhaseB= *((uint32_t*)&RxdTelBuf[1*3+18])&0x00ffffff;
	//			AdjustData.PhaseC= *((uint32_t*)&RxdTelBuf[2*3+18])&0x00ffffff;

		break;  	
   }
   //comtxdflag=1;
}


uint8_t  isVaildTel(void)
{
	if(RxdBytes>=1)if(RxdBuf[0]!=config.addr)return(0);
	if(RxdBytes>=2)if(RxdBuf[1]!=0x03 && RxdBuf[1]!=0x05 && RxdBuf[1]!=0x04 && RxdBuf[1]!=0x06 && RxdBuf[1]!=0x10)return(0);

	return(1);				 // 合法的
}

uint8_t getTelLength(void)
{
					
	switch(RxdBuf[1])
	{
		case 0x03: 
		case 0x04: return(6);
		case 0x05:
		case 0x06: return(6);
		case 0x10: return(RxdBuf[6]+7);
		default:
				return(0);
	}					
}
uint8_t isTelComplete(void)	   // =0 不完整  =1 CRC Error =2 正确
{
	uint16_t  temp16;
	uint8_t  dal, dah, dat_len;

	if(RxdBytes<8)return(0);
  ////////////////
	dat_len=getTelLength();	//给数据长度
	if(dat_len==0)return(0);
	if(RxdBytes<(dat_len+2))return(0);

	temp16=CRC16ISR0(&RxdBuf[0],dat_len);
 
   	dal=temp16>>8;
	dah=temp16;

	if ((RxdBuf[dat_len]==dal)&&(RxdBuf[dat_len+1]==dah))
		return(2); 
	else
	{
		return(1);
	}	
}						 

uint8_t leftRxdTel(void)		//数组左移一位
{
	uint8_t i;
	if(RxdBytes<1)return(0);     // 无法左移
	for	(i=1;i<RxdBytes;i++)
	{
		RxdBuf[i-1]=RxdBuf[i];		
	}
	RxdBytes--;
	return(1);					 // 丢弃一个字节成功

}

 void RxdByte(uint8_t c)
{	
	uint8_t 	i;
	RxdBuf[RxdBytes]=c;
	RxdBytes++;

	switch(RxdBytes)
	{
		case 0:	break;
		case 1:
		case 2:
				while(!isVaildTel())	//如果不合法			 
				{
					if(!leftRxdTel())break;	  // 丢弃首字节
				}
				break;
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:	break;
		default:		
				i=isTelComplete();
				if(i==2)
				{
					//do some thing
					for(i=0;i<RxdBytes;i++)	RxdTelBuf[i]=RxdBuf[i];
					ReceivedTel=1;
					RxdBytes=0;
				}
				else if(i==1)	 // CRC error
				{
					leftRxdTel();
					while(!isVaildTel())	//如果不合法			 
					{
						if(!leftRxdTel())break;
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



uint8_t hasByteToTxd()		 // =1 有字节待发
{
	if(TxdBytes<TxdTelBytes)return(1);
	TxdTelBytes=0;
	TxdBytes=0;
	return(0);	
}

uint8_t  getTxdByte()   //
{
	uint8_t re;
	if(TxdBytes <TxdTelBytes)
	{
		re=TxdBuf[TxdBytes];
		TxdBytes++;
		return(re);
	}
	else
	{
		TxdTelBytes=0;
		TxdBytes=0;
		return(0);
	}
}
		






	
#define Float_Int16 0x14
#define Float_UInt16 0x15
#define UInt16_UInt16 0x16
#define UInt8_UInt16 0x17
#define Int8_Int16 0x18

typedef  __packed struct MODBUSDATA
{
	uint16_t addr;
	void *data;
	uint8_t type;   // =0x14   float
}MODBUSDATA;

const MODBUSDATA modbusData[]=
{
	{0,&config.vlpsseconds,UInt16_UInt16},	// 0	1?????(????)
	{1,&config.vlprseconds,UInt16_UInt16},	// 0	1?????(????)
	{2,&config.runseconds,UInt16_UInt16},	// 0	1?????(????)
	{3,&config.pga,UInt16_UInt16},	// 0	1?????(????)
	


//	{24,&Parameter.CVdc,Float_UInt16},// 24	??????	(0-250)V	R
//	{25,&Parameter.OVdc,Float_UInt16},// 25	??????	(0-250)V	R
//	{26,&Parameter.SWStatus,UInt16_UInt16},// 26	????	0-255	R
//	{27,&config.T1,Int8_Int16},// 27	A??????	(0-99)?	R,W
//{28,&config.T1Correct,Int8_Int16},// 28	A??????	(-20-20)?	R,W
//{29,&config.T1Backlash,UInt8_UInt16},// 29	A??????	(1-20)?	R,W
//{30,&config.H1,Int8_Int16},// 30	A??????	(0-99)%RH	R,W
//{31,&config.H1Correct,Int8_Int16},// 31	A??????	(-20-20)%RH	R,W
//{32,&config.H1Backlash,UInt8_UInt16},// 32	A??????	(1-20)%RH	R,W
//{33,&config.T2,Int8_Int16},// 33	B??????	(0-99)?	R,W
//{34,&config.T2Correct,Int8_Int16},// 34	B??????	(-20-20)?	R,W
//{35,&config.T2Backlash,UInt8_UInt16},// 35	B??????	(1-20)?	R,W
//{36,&config.H2,Int8_Int16},// 36	B??????	(0-99)%RH	R,W
//{37,&config.H2Correct,Int8_Int16},// 37	B??????	(-20-20)%RH	R,W
//{38,&config.H2Backlash,UInt8_UInt16},// 38	B??????	(1-20)%RH	R,W

// 	
};

#define MODBUSDATAS (sizeof(modbusData)/sizeof(modbusData[0]))
#define NULL 0
MODBUSDATA * getModbusDataPoint(uint16_t addr){
	static uint16_t ii=0;
	uint16_t i=ii;
	
	if(addr==0){ii=0;return((MODBUSDATA*)&modbusData[0]);} // ???????
	else if(addr==11){ii=0;return((MODBUSDATA*)&modbusData[11]);} 


	do{
		if(addr==modbusData[i].addr){ //???
			ii=i+1; 
			if(ii==MODBUSDATAS)ii=0;
			return((MODBUSDATA*)&modbusData[i]);
		}
		else{
			i++;
			if(i==MODBUSDATAS)i=0;
		}
	}while(i!=ii);
	ii=i;
	return((MODBUSDATA*)NULL);
}
uint16_t getModbusData(uint16_t addr){
	uint16_t r=0,i;
	for(i=0;i<12;i++){
		if(addr==config.interface_addr[i]){
			if(Parameter.v[i]<0)r=(uint16_t)(-Parameter.v[i]*100+0.5)|0x8000;
			else r=(uint16_t)(Parameter.v[i]*100+0.5);
		}
	}
	return(r);
}
void setModbusData(uint16_t addr,uint16_t data){
	MODBUSDATA *p= getModbusDataPoint(addr);
	if(p==NULL)	return;
	switch(p->type)
	{
		case Float_UInt16 : *((float*)p->data)=data; break;
		case Float_Int16	: *((float*)p->data)=*((int16_t*)(&data)); break;
		case UInt16_UInt16: *((uint16_t*)p->data)=data; break;
		case UInt8_UInt16: *((uint8_t*)p->data)=data; break;
		case	Int8_Int16:			*((int8_t*)p->data)=*((int8_t*)(&data));		break;
		default: return; 
	}
	 return;
}
void OnGetData(uint8_t command)
{
		uint8_t 	i;
	uint16_t startaddr=(uint16_t)RxdTelBuf[2]*256+RxdTelBuf[3];
	uint16_t datanum=(uint16_t)RxdTelBuf[4]*256+RxdTelBuf[5];
	uint16_t tt;
	uint8_t *pb;
	TxdBuf[0] = config.addr;
	TxdBuf[1] = command;
	TxdBuf[2] = datanum*2;
	pb=&TxdBuf[3];

	for(i=0;i<datanum;i++)	{
			tt=getModbusData(startaddr+i);
		*pb=tt>>8;
		pb++;
		*pb=tt&0xff;
		pb++;
	} 
	tt=CRC16ISR1(&TxdBuf[0],3+TxdBuf[2]);
	*pb=tt/256; *(pb+1)=tt%256; 
	TxdBytes=0;
	TxdTelBytes=5+datanum*2;
	
	startTxd();			   // ?????? ,????

}

void SelfsetData(uint8_t command)
{
		uint8_t 	i;
	uint16_t startaddr=0;
	uint16_t datanum=5;
	uint16_t tt;
	uint8_t *pb;
	TxdBuf[0] = config.addr;
	TxdBuf[1] = command;
	TxdBuf[2] = 5*2;
	pb=&TxdBuf[3];

	for(i=0;i<datanum;i++)	{
			tt=getModbusData(startaddr+i);
		*pb=tt>>8;
		pb++;
		*pb=tt&0xff;
		pb++;
	} 
	tt=CRC16ISR1(&TxdBuf[0],3+TxdBuf[2]);
	*pb=tt/256; *(pb+1)=tt%256; 
	TxdBytes=0;
	TxdTelBytes=5+datanum*2;
	
	startTxd();			   // ?????? ,????

}

void OnGetData2()
{
		uint8_t 	i;
	uint8_t 	k;
	uint16_t 	j;
	uint16_t startaddr=(uint16_t)RxdTelBuf[2]*256+RxdTelBuf[3];
	uint16_t datanum=(uint16_t)RxdTelBuf[4]*256+RxdTelBuf[5];
	uint16_t tt;
	uint8_t *pb;
	TxdBuf[0] = config.addr;
	TxdBuf[1] = 2;
	TxdBuf[2] =1;
	j=Parameter.dout;
	pb=&TxdBuf[3];

	for(i=0;i<TxdBuf[2];i++)	{
		//k=j&0x01;
		//k=(j&0x02)*16+k;
		
		*pb=j;
		//*pb=0x02;
		j=j>>2;
		pb++;
		//k=0;
	} 
	tt=CRC16ISR1(&TxdBuf[0],3+TxdBuf[2]);
	*pb=tt/256; *(pb+1)=tt%256; 
	TxdBytes=0;
	TxdTelBytes=5+TxdBuf[2];
	
	startTxd();			   // ?????? ,????

}
void OnGetData03(){	
	OnGetData(3);
}
void OnGetData04(){	
	OnGetData(4);
}

void OnSetData05()			   // ?? dout;
{
	uint8_t	i;
	for(i=0;i<8;i++)	TxdBuf[i]=RxdTelBuf[i];	
	TxdTelBytes = 8;	TxdBytes=0;
	startTxd();	
	if(TxdBuf[4]==0xff)
	{
//		if(TxdBuf[3]==0)	{Dout1 = 1;RelayTimeCount[0]= 200	;flag1|=0x01;}
//		if(TxdBuf[3]==1)	{Dout2 = 1;RelayTimeCount[1]= 200	;flag1|=0x02;}
//		if(TxdBuf[3]==2)	yk3=0;
//		if(TxdBuf[3]==3)	yk4=0;
	}

}
extern void saveConfig(void);
void OnSetData06()			   // ?????????;
{
	uint8_t 	i;
	for(i=0;i<8;i++)	TxdBuf[i]=RxdTelBuf[i];
	TxdTelBytes = 8;	TxdBytes=0;
	startTxd();	
	
	{
		uint16_t addr=(((uint16_t)RxdTelBuf[2])<<8)+RxdTelBuf[3];
		uint16_t data=(((uint16_t)RxdTelBuf[4])<<8)+RxdTelBuf[5];
		setModbusData(addr,data);
	}
	saveConfig();
}



uint16_t CRC16ISR0(uint8_t * puchMsg, uint8_t usDataLen) 	//80109
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
uint16_t CRC16ISR1(uint8_t * puchMsg, uint8_t usDataLen)
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

const     uint8_t  auchCRCHi[] = {
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


const    	 uint8_t  auchCRCLo[] = {
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


void modbusLoop()
{	
	static uint8_t comflag_count=0;   

	if(comflag_count>0)
	{
		comflag_count--;
		if(comflag_count==0)
		{
//			hide485COM();	   // 停止 485标志。
		}
	}	
	
	CloseAllInt();
	if(ReceivedTel)
	{
		ReceivedTel=0;
		OpenAllInt();
 		
		switch(getRxdTelCommand())
		{
			case 0x03:
//	disp485COM();	    // 显示 RS485 通讯标志	  
					comflag_count=100;	 // 显示时间
					OnGetData03();
					break;
			case 0x04:
//	disp485COM();	    // 显示 RS485 通讯标志	  
					comflag_count=100;	 // 显示时间
					OnGetData04();
					break;

			case 0x05:
//	disp485COM();		  // 显示 RS485 通讯标志
					comflag_count=100;	  // 显示时间
					OnSetData05();
					break;	
			case 0x06:
//	disp485COM();		  // 显示 RS485 通讯标志
					comflag_count=100;	  // 显示时间
					OnSetData06();   
					break;	
			case 0x10:
//	dat_len = comrbuf[6]+7;//comrbuf[5]*2+7;
					{
									Updataflag=1;
//									disp485COM();		  // 显示 RS485 通讯标志
									comflag_count=100;	  // 显示时间									break;  
					}

		}
	}
	else
	{	
		OpenAllInt();
	}
}
//typedef enum {BAUND2400=0,BAUND4800,BAUND9600,BAUND19200,BAUND38400,} BAUND_RATE;
const uint32_t BAUNDRATE_INT_TAB[]={2400,4800,9600,19200,38400,};

	//RCC_Configuration
	




unsigned char CheckEven(unsigned char becheck) 
{
		return(0);
}
// PC12
void Enable485TXD(void) {
	UART_MemMapPtr uartPtr = UART2_BASE_PTR;
  
	//while (!UART_D_REG(uartPtr));
	UART_C2_REG(uartPtr) |= UART_C2_TCIE_MASK;
}
void Disable485TXD(void){
  uint16_t xx;
	UART_MemMapPtr uartPtr = UART2_BASE_PTR;
	
  UART_C2_REG(uartPtr)&= ~(UART_C2_TCIE_MASK);
	
	
}

void startTxd()
{
	Enable485TXD();
	//USART_SendData(UART4,getTxdByte()); 
	uart2SendChar(getTxdByte());
}

void  UART2_IRQHandler (void)
{ 
	uint8_t c;
   UART_MemMapPtr uartPtr = UART2_BASE_PTR;                             /* 发送中断处理程序             */
	//UART_S1_REG(uartPtr)|= UART_S1_RDRF_MASK;
   if (UART_S1_REG(uartPtr) & UART_S1_RDRF_MASK){                    /* 清除中断标志                 */
                                   /* 返回接收数据                 */     
	 RxdByte((uint8_t)(uart2GetChar()));
   } 
	else if(((UART_S1_REG(uartPtr) & UART_S1_TC_MASK)) && ((UART_C2_REG(uartPtr) & UART_C2_TCIE_MASK)))
	
	{		 
		   if(hasByteToTxd())		{
		  	Enable485TXD();
		  	c=getTxdByte();
			 // UART_D_REG(uartPtr) =c;
		    uart2SendChar(c);                                     /* 返回接收数据         */              

		 }else {Disable485TXD();}
		
				
	}
 	
	//
}
	
		
	void modbusbegin()
{
	CloseAllInt();
	//Enable485TXD();
	
	if(ReceivedTel)
	{
		ReceivedTel=0;
		OpenAllInt();
 		
		switch(getRxdTelCommand())
		{
			case 0x03:
//	disp485COM();	    // 显示 RS485 通讯标志	  
			//		comflag_count=100;	 // 显示时间
					//OnGetData03();
					break;
			case 0x04:
//	disp485COM();	    // 显示 RS485 通讯标志	  
			//		comflag_count=100;	 // 显示时间
					OnGetData04();
					break;

			case 0x05:
//	disp485COM();		  // 显示 RS485 通讯标志
			//		comflag_count=100;	  // 显示时间
					OnSetData05();
					break;	
			case 0x06:
//	disp485COM();		  // 显示 RS485 通讯标志
			//		comflag_count=100;	  // 显示时间
					OnSetData06();   
					break;	
			case 0x10:
//	dat_len = comrbuf[6]+7;//comrbuf[5]*2+7;
					{
									Updataflag=1;
//									disp485COM();		  // 显示 RS485 通讯标志
				//					comflag_count=100;	  // 显示时间									break;  
					}

		}
	}
	else
	{	
		OpenAllInt();
		Disable485TXD();
	 SelfsetData(3);
	}

	
	
	
	
	
	
	
}
	
	 

