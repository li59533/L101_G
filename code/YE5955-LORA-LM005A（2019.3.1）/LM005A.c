
#include <stdio.h>
#include <app.h>
#include <stdint.h>  
#include <string.h>  
#include <stdbool.h>
#include <stdarg.h>


#if defined ( __CC_ARM   )
#pragma anon_unions
#endif


void   LM005A_GPIO_Config ( void );
void   LM005A_USART_Config ( void );
void   LM005A_USART_NVIC_Configuration ( void );




extern  uint8_t   TxdBuf_uart0[];

struct  STRUCT_USARTx_Fram strLM005A_Fram_Record={ 0 };

uint8_t ATcmd[1024];


extern void  time_delay_ms(unsigned int count_val); //最多延迟60s
extern void  uart_send_data_uart0(uint8_t *sendstring,uint32_t data_length);
uint32_t  ATcmdLength=0;


static char * itoa( int value, char *string, int radix )
{
	int     i, d;
	int     flag = 0;
	char    *ptr = string;

	/* This implementation only works for decimal numbers. */
	if (radix != 10)
	{
		*ptr = 0;
		return string;
	}
	if (!value)
	{
		*ptr++ = 0x30;
		*ptr = 0;
		return string;
	}
	/* if this is a negative value insert the minus sign. */
	if (value < 0)
	{
		*ptr++ = '-';
		/* Make the value positive. */
		value *= -1;		
	}
	for (i = 10000; i > 0; i /= 10)
	{
		d = value / i;
		if (d || flag)
		{
			*ptr++ = (char)(d + 0x30);
			value -= (d * i);
			flag = 1;
		}
	}
	/* Null terminate the string. */
	*ptr = 0;
	return string;
} /* NCL_Itoa */



void  USART_CMD_LM005A( char * Data,... )
{ 
	const char *s;
	int d;   
	char buf[126];  //限制了发送字节的长度，66个
	uint16_t 	j=0;
	va_list ap;
	va_start(ap, Data);

	while ( * Data != 0 )     // 判断是否到达字符串结束符
	{				                          
		if ( * Data == '%')
		{									  //
			switch ( *++Data )
			{				
				case 's':						  //字符串
						s = va_arg(ap, const char *);
						for ( ; *s; s++) 
						{
							ATcmd[j++]=*s;	
							
						}
						Data++;
				break;

				case 'd':	                    //十进制							
						d = va_arg(ap, int);						
						itoa(d, buf, 10);						
						for (s = buf; *s; s++) 
						{
							ATcmd[j++]=*s;
						}
						Data++;
				break;
				
				default:
						Data++;
						j++;
				break;			
			}		 
		}	
		else 
		{	
			ATcmd[j++]=*Data++;
		}
	}	
	ATcmdLength=j;
	uart_send_data_uart0(ATcmd,ATcmdLength);			  
}


extern  uint32_t  OnOrganizeData1(void);
extern  uint32_t  OnOrganizeData2(void);


void  USART_CMD_LM005A_data( char * Data,... )
{ 
	const char *s;
	int d;   
	uint32_t ii=0;
	uint32_t datalength;
	char buf[126];  //限制了发送字节的长度，66个
	uint16_t 	j=0;
	va_list ap;
	va_start(ap, Data);

	while ( * Data != 0 )     // 判断是否到达字符串结束符
	{				                          
		if ( * Data == '%')
		{									
			switch ( *++Data )
			{				
				case 's':					//字符串
						s = va_arg(ap, const char *);				
						for ( ; *s; s++) 
						{
							ATcmd[j++]=*s;		
						}
						Data++;
				break;

				case 'd':		          //十进制						
						d = va_arg(ap, int);						
						itoa(d, buf, 10);		
						for (s = buf; *s; s++) 
						{
							ATcmd[j++]=*s;
						}						
						Data++;
				break;
				
				default:
						Data++;
						j++;
				break;
			}		 
		}		
		else 
		{
			ATcmd[j++]=*Data++;
		}	
	}	
	ATcmdLength=j;
	if(config.displacement==0)    datalength=OnOrganizeData1();
		else    datalength=OnOrganizeData2();
	for(ii=0;ii<datalength;ii++)
		ATcmd[j+ii]=TxdBuf_uart0[ii];
	ATcmd[j+datalength]=0x0d;
	ATcmd[j+datalength+1]=0x0a;
	ATcmdLength=j+datalength+2;
	uart_send_data_uart0(ATcmd,ATcmdLength);			 
}


/*
 * 函数名：LM005A_Rst
 * 描述  ：重启WF-LM005A模块
 * 输入  ：无
 * 返回  : 无
 * 调用  ：被 LM005A_AT_Test 调用
 */
void  LM005A_Rst ( void )
{
	#if 0
	 LM005A_Cmd ( "AT+RST", "OK", "ready", 2500 );   		
	#else	
	#endif
}


/*
 * 函数名：LM005A_senddata

 */
void  LM005A_SEND_DATA ( uint32_t port,uint32_t ack,uint32_t length )
{
	char cCmd [120];
	strLM005A_Fram_Record .InfBit .FramLength = 0; 
	sprintf ( cCmd, "AT+MSGHEX=%d,%d,%d,", port,ack,length);//HEX
	macLM005A_Usart_data( "%s", cCmd );	
}


void  LM005A_SEND_DATA_Without_Receive ( uint32_t port,uint32_t ack,uint32_t length )
{
	char cCmd [120];
	strLM005A_Fram_Record .InfBit .FramLength = 0; 
	sprintf ( cCmd, "AT+SEND=%d,",length);//HEX
	macLM005A_Usart_data( "%s", cCmd );	
}
/*
 * 函数名：LM005A_Cmd
 * 描述  ：对WF-LM005A模块发送AT指令
 * 输入  ：cmd，待发送的指令
 *         reply1，reply2，期待的响应，为NULL表不需响应，两者为或逻辑关系
 *         waittime，等待响应的时间
 * 返回  : 1，指令发送成功
 *         0，指令发送失败
 * 调用  ：被外部调用
 */
bool  LM005A_Cmd ( char * cmd, char * reply1, char * reply2, uint32_t waittime )
{    
	strLM005A_Fram_Record .InfBit .FramLength = 0;               //从新开始接收新的数据包
	macLM005A_Usart ( "%s\r\n", cmd );
//	sprintf ( cmd,"%s\r\n", cmd );
//  USART_CMD_LM005A(cmd);
	if ( ( reply1 == 0 ) && ( reply2 == 0 ) )                      //不需要接收数据
		return true;
	
	time_delay_ms( waittime );                 //延时
	
	strLM005A_Fram_Record .Data_RX_BUF [ strLM005A_Fram_Record .InfBit .FramLength ]  = '\0';
  
	if ( ( reply1 != 0 ) && ( reply2 != 0 ) )
		return ( ( bool ) strstr ( strLM005A_Fram_Record .Data_RX_BUF, reply1 ) || 
						 ( bool ) strstr ( strLM005A_Fram_Record .Data_RX_BUF, reply2 ) ); 
 	
	else if ( reply1 != 0 )
		return ( ( bool ) strstr ( strLM005A_Fram_Record .Data_RX_BUF, reply1 ) );
	
	else
		return ( ( bool ) strstr ( strLM005A_Fram_Record .Data_RX_BUF, reply2 ) );	
}


bool  LM005A_returnOK (  char * reply1, char * reply2, uint32_t waittime )
{    
	//strLM005A_Fram_Record .InfBit .FramLength = 0;               //从新开始接收新的数据包
 
//	sprintf ( cmd,"%s\r\n", cmd );
//  USART_CMD_LM005A(cmd);
	if ( ( reply1 == 0 ) && ( reply2 == 0 ) )                      //不需要接收数据
		return true;
	
	time_delay_ms( waittime );                 //延时
	
	strLM005A_Fram_Record .Data_RX_BUF [ strLM005A_Fram_Record .InfBit .FramLength ]  = '\0';

  
	if ( ( reply1 != 0 ) && ( reply2 != 0 ) )
		return ( ( bool ) strstr ( strLM005A_Fram_Record .Data_RX_BUF, reply1 ) || 
						 ( bool ) strstr ( strLM005A_Fram_Record .Data_RX_BUF, reply2 ) ); 
 	
	else if ( reply1 != 0 )
		return ( ( bool ) strstr ( strLM005A_Fram_Record .Data_RX_BUF, reply1 ) );
	
	else
		return ( ( bool ) strstr ( strLM005A_Fram_Record .Data_RX_BUF, reply2 ) );	
}

/*
 * 函数名：LM005A_AT_Test
 * 描述  ：对WF-LM005A模块进行AT测试启动
 * 输入  ：无
 * 返回  : 无
 * 调用  ：被外部调用
 */
bool  LM005A_AT_Test ( void )
{
//	macLM005A_RST_HIGH_LEVEL();	
	char cCmd [120];
	sprintf ( cCmd, "AT+OK?" );
	return LM005A_Cmd ( cCmd, "OK", NULL, 500 );
//	while (  LM005A_Cmd ( "AT", "OK", NULL, 500 ) ) LM005A_Rst ();  	
}


/*
 * 函数名：LM005A_AT_ADR
 * 描述  ：对LM005A模块ADR设置
 * 输入  ：无
 * 返回  : 无
 * 调用  ：被外部调用
 */
bool  LM005A_ADR_SET ( uint32_t  ADRvalue )
{
	char cCmd [120];
	sprintf ( cCmd, "AT+ADR=%d", ADRvalue );
	return LM005A_Cmd ( cCmd, "OK", "no change", 1000 );   //等1s钟	
}

/*
 * 函数名：LM005A_AT_JOIN
 * 描述  ：对LM005A模块JOIN设置
 * 输入  ：无
 * 返回  : 无
 * 调用  ：被外部调用
 */
bool  LM005A_JOIN_SET (void)
{
	char cCmd [120];
	sprintf (cCmd, "AT+JOIN");
	return LM005A_Cmd ( cCmd, "RXDONE", "no change",15000 );   //等1s钟	
}


bool  LM005A_VERSION (void)
{
	char cCmd [120];
	sprintf (cCmd, "AT+VERSION?");
	return LM005A_Cmd ( cCmd, "RXDONE", "no change",1000 );   //等1s钟	
}


bool  LM005A_RESET (void)
{
	char cCmd [120];
	sprintf (cCmd, "AT+RESET");
	return LM005A_Cmd ( cCmd, "OK", "no change", 3500 );   //等1s钟
}

/*
 * 函数名：LM005A_AT_DR
 * 描述  ：对LM005A模块DR设置
 * 输入  ：无
 * 返回  : 无
 * 调用  ：被外部调用
 */
bool  LM005A_DR_SET ( uint32_t  DRvalue )
{
	char cCmd [120];
	sprintf ( cCmd, "AT+DR=%d", DRvalue );
	return LM005A_Cmd ( cCmd, "OK", "no change", 1000 );   //等1s钟	
}



/*
 * 函数名：LM005A_AT_MODE
 * 描述  ：对LM005A模块MODE设置
 * 输入  ：无
 * 返回  : 无
 * 调用  ：被外部调用
 */
bool  LM005A_MODE_SET ( uint32_t  MODEvalue )
{
	char cCmd [120];
	sprintf ( cCmd, "AT+MODE=%d", MODEvalue );
	return LM005A_Cmd ( cCmd, "OK", "no change", 500 );   //等1s钟	
}


/*
 * 函数名：LM005A_AT_CLASS
 * 描述  ：对LM005A模块CLASS设置
 * 输入  ：无
 * 返回  : 无
 * 调用  ：被外部调用
 */
bool  LM005A_CLASS_SET ( uint32_t  CLASSvalue )
{
	char cCmd [120];
	sprintf ( cCmd, "AT+CLASS=%d", CLASSvalue );
	return LM005A_Cmd ( cCmd, "OK", "no change", 500 );   //等1s钟	
}


/*
 * 函数名：LM005A_AT_POWER
 * 描述  ：对LM005A模块DR设置
 * 输入  ：无
 * 返回  : 无
 * 调用  ：被外部调用
 */
bool  LM005A_POWER_SET ( uint32_t  POWERvalue )
{
	char cCmd [120];
	sprintf ( cCmd, "AT+POWER=%d", POWERvalue );
	return LM005A_Cmd ( cCmd, "OK", "no change", 1000 );   //等1s钟	
}


/*
 * 函数名：LM005A_SEND_STRING
 * 描述  ：对LM005A模块SEND_STRING设置
 * 输入  ：无
 * 返回  : 无
 * 调用  ：
 */
bool  LM005A_SET_STRING ( char *data )
{
	char cCmd [120];
	sprintf ( cCmd, "AT+POWER=%s", data );
	return LM005A_Cmd ( cCmd, "OK", "no change", 1000 );   //等1s钟	
}



///*
// * 函数名：LM005A_Net_Mode_Choose
// * 描述  ：选择LM005A模块的工作模式
// * 输入  ：enumMode，工作模式
// * 返回  : 1，选择成功
// *         0，选择失败
// * 调用  ：被外部调用
// */
//bool LM005A_Net_Mode_Choose ( ENUM_Net_ModeTypeDef enumMode )
//{
//	switch ( enumMode )
//	{
//		case STA:
//			return LM005A_Cmd ( "AT+CWMODE=1", "OK", "no change", 2500 ); 
//		
//	  case AP:
//		  return LM005A_Cmd ( "AT+CWMODE=2", "OK", "no change", 2500 ); 
//		
//		case STA_AP:
//		  return LM005A_Cmd ( "AT+CWMODE=3", "OK", "no change", 2500 ); 
//		
//	  default:
//		  return false;
//  }
//	
//}


///*
// * 函数名：LM005A_JoinAP
// * 描述  ：WF-LM005A模块连接外部WiFi
// * 输入  ：pSSID，WiFi名称字符串
// *       ：pPassWord，WiFi密码字符串
// * 返回  : 1，连接成功
// *         0，连接失败
// * 调用  ：被外部调用
// */
//bool LM005A_JoinAP ( char * pSSID, char * pPassWord )
//{
//	char cCmd [120];

//	sprintf ( cCmd, "AT+CWJAP=\"%s\",\"%s\"", pSSID, pPassWord );
//	
//	return LM005A_Cmd ( cCmd, "OK", NULL, 5000 );
//	
//}


///*
// * 函数名：LM005A_BuildAP
// * 描述  ：WF-LM005A模块创建WiFi热点
// * 输入  ：pSSID，WiFi名称字符串
// *       ：pPassWord，WiFi密码字符串
// *       ：enunPsdMode，WiFi加密方式代号字符串
// * 返回  : 1，创建成功
// *         0，创建失败
// * 调用  ：被外部调用
// */
//bool LM005A_BuildAP ( char * pSSID, char * pPassWord, ENUM_AP_PsdMode_TypeDef enunPsdMode )
//{
//	char cCmd [120];

//	sprintf ( cCmd, "AT+CWSAP=\"%s\",\"%s\",1,%d", pSSID, pPassWord, enunPsdMode );
//	
//	return LM005A_Cmd ( cCmd, "OK", 0, 1000 );
//	
//}


///*
// * 函数名：LM005A_Enable_MultipleId
// * 描述  ：WF-LM005A模块启动多连接
// * 输入  ：enumEnUnvarnishTx，配置是否多连接
// * 返回  : 1，配置成功
// *         0，配置失败
// * 调用  ：被外部调用
// */
//bool LM005A_Enable_MultipleId ( FunctionalState enumEnUnvarnishTx )
//{
//	char cStr [20];
//	
//	sprintf ( cStr, "AT+CIPMUX=%d", ( enumEnUnvarnishTx ? 1 : 0 ) );
//	
//	return LM005A_Cmd ( cStr, "OK", 0, 500 );
//	
//}


///*
// * 函数名：LM005A_Link_Server
// * 描述  ：WF-LM005A模块连接外部服务器
// * 输入  ：enumE，网络协议
// *       ：ip，服务器IP字符串
// *       ：ComNum，服务器端口字符串
// *       ：id，模块连接服务器的ID
// * 返回  : 1，连接成功
// *         0，连接失败
// * 调用  ：被外部调用
// */
//bool LM005A_Link_Server ( ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id)
//{
//	char cStr [100] = { 0 }, cCmd [120];

//  switch (  enumE )
//  {
//		case enumTCP:
//		  sprintf ( cStr, "\"%s\",\"%s\",%s", "TCP", ip, ComNum );
//		  break;
//		
//		case enumUDP:
//		  sprintf ( cStr, "\"%s\",\"%s\",%s", "UDP", ip, ComNum );
//		  break;
//		
//		default:
//			break;
//  }

//  if ( id < 5 )
//    sprintf ( cCmd, "AT+CIPSTART=%d,%s", id, cStr);

//  else
//	  sprintf ( cCmd, "AT+CIPSTART=%s", cStr );

//	return LM005A_Cmd ( cCmd, "OK", "ALREAY CONNECT", 4000 );
//	
//}


///*
// * 函数名：LM005A_StartOrShutServer
// * 描述  ：WF-LM005A模块开启或关闭服务器模式
// * 输入  ：enumMode，开启/关闭
// *       ：pPortNum，服务器端口号字符串
// *       ：pTimeOver，服务器超时时间字符串，单位：秒
// * 返回  : 1，操作成功
// *         0，操作失败
// * 调用  ：被外部调用
// */
//bool LM005A_StartOrShutServer ( FunctionalState enumMode, char * pPortNum, char * pTimeOver )
//{
//	char cCmd1 [120], cCmd2 [120];

//	if ( enumMode )
//	{
//		sprintf ( cCmd1, "AT+CIPSERVER=%d,%s", 1, pPortNum );
//		
//		sprintf ( cCmd2, "AT+CIPSTO=%s", pTimeOver );

//		return ( LM005A_Cmd ( cCmd1, "OK", 0, 500 ) &&
//						 LM005A_Cmd ( cCmd2, "OK", 0, 500 ) );
//	}
//	
//	else
//	{
//		sprintf ( cCmd1, "AT+CIPSERVER=%d,%s", 0, pPortNum );

//		return LM005A_Cmd ( cCmd1, "OK", 0, 500 );
//	}
//	
//}


///*
// * 函数名：LM005A_Get_LinkStatus
// * 描述  ：获取 WF-LM005A 的连接状态，较适合单端口时使用
// * 输入  ：无
// * 返回  : 2，获得ip
// *         3，建立连接
// *         3，失去连接
// *         0，获取状态失败
// * 调用  ：被外部调用
// */
//uint8_t LM005A_Get_LinkStatus ( void )
//{
//	if ( LM005A_Cmd ( "AT+CIPSTATUS", "OK", 0, 500 ) )
//	{
//		if ( strstr ( strLM005A_Fram_Record .Data_RX_BUF, "STATUS:2\r\n" ) )
//			return 2;
//		
//		else if ( strstr ( strLM005A_Fram_Record .Data_RX_BUF, "STATUS:3\r\n" ) )
//			return 3;
//		
//		else if ( strstr ( strLM005A_Fram_Record .Data_RX_BUF, "STATUS:4\r\n" ) )
//			return 4;		

//	}
//	
//	return 0;
//	
//}


///*
// * 函数名：LM005A_Get_IdLinkStatus
// * 描述  ：获取 WF-LM005A 的端口（Id）连接状态，较适合多端口时使用
// * 输入  ：无
// * 返回  : 端口（Id）的连接状态，低5位为有效位，分别对应Id5~0，某位若置1表该Id建立了连接，若被清0表该Id未建立连接
// * 调用  ：被外部调用
// */
//uint8_t LM005A_Get_IdLinkStatus ( void )
//{
//	uint8_t ucIdLinkStatus = 0x00;
//	
//	
//	if ( LM005A_Cmd ( "AT+CIPSTATUS", "OK", 0, 500 ) )
//	{
//		if ( strstr ( strLM005A_Fram_Record .Data_RX_BUF, "+CIPSTATUS:0," ) )
//			ucIdLinkStatus |= 0x01;
//		else 
//			ucIdLinkStatus &= ~ 0x01;
//		
//		if ( strstr ( strLM005A_Fram_Record .Data_RX_BUF, "+CIPSTATUS:1," ) )
//			ucIdLinkStatus |= 0x02;
//		else 
//			ucIdLinkStatus &= ~ 0x02;
//		
//		if ( strstr ( strLM005A_Fram_Record .Data_RX_BUF, "+CIPSTATUS:2," ) )
//			ucIdLinkStatus |= 0x04;
//		else 
//			ucIdLinkStatus &= ~ 0x04;
//		
//		if ( strstr ( strLM005A_Fram_Record .Data_RX_BUF, "+CIPSTATUS:3," ) )
//			ucIdLinkStatus |= 0x08;
//		else 
//			ucIdLinkStatus &= ~ 0x08;
//		
//		if ( strstr ( strLM005A_Fram_Record .Data_RX_BUF, "+CIPSTATUS:4," ) )
//			ucIdLinkStatus |= 0x10;
//		else 
//			ucIdLinkStatus &= ~ 0x10;	

//	}
//	
//	return ucIdLinkStatus;
//	
//}


///*
// * 函数名：LM005A_Inquire_ApIp
// * 描述  ：获取 F-LM005A 的 AP IP
// * 输入  ：pApIp，存放 AP IP 的数组的首地址
// *         ucArrayLength，存放 AP IP 的数组的长度
// * 返回  : 0，获取失败
// *         1，获取成功
// * 调用  ：被外部调用
// */
//uint8_t LM005A_Inquire_ApIp ( char * pApIp, uint8_t ucArrayLength )
//{
//	char uc;
//	
//	char * pCh;
//	
//	
//  LM005A_Cmd ( "AT+CIFSR", "OK", 0, 500 );
//	
//	pCh = strstr ( strLM005A_Fram_Record .Data_RX_BUF, "APIP,\"" );
//	
//	if ( pCh )
//		pCh += 6;
//	
//	else
//		return 0;
//	
//	for ( uc = 0; uc < ucArrayLength; uc ++ )
//	{
//		pApIp [ uc ] = * ( pCh + uc);
//		
//		if ( pApIp [ uc ] == '\"' )
//		{
//			pApIp [ uc ] = '\0';
//			break;
//		}
//		
//	}
//	
//	return 1;
//	
//}


///*
// * 函数名：LM005A_UnvarnishSend
// * 描述  ：配置WF-LM005A模块进入透传发送
// * 输入  ：无
// * 返回  : 1，配置成功
// *         0，配置失败
// * 调用  ：被外部调用
// */
//bool LM005A_UnvarnishSend ( void )
//{
//	if ( ! LM005A_Cmd ( "AT+CIPMODE=1", "OK", 0, 500 ) )
//		return false;
//	
//	return 
//	  LM005A_Cmd ( "AT+CIPSEND", "OK", ">", 500 );
//	
//}


///*
// * 函数名：LM005A_ExitUnvarnishSend
// * 描述  ：配置WF-LM005A模块退出透传模式
// * 输入  ：无
// * 返回  : 无
// * 调用  ：被外部调用
// */
//void LM005A_ExitUnvarnishSend ( void )
//{
//	Delay_ms ( 1000 );
//	
//	macLM005A_Usart ( "+++" );
//	
//	Delay_ms ( 500 ); 
//	
//}


///*
// * 函数名：LM005A_SendString
// * 描述  ：WF-LM005A模块发送字符串
// * 输入  ：enumEnUnvarnishTx，声明是否已使能了透传模式
// *       ：pStr，要发送的字符串
// *       ：ulStrLength，要发送的字符串的字节数
// *       ：ucId，哪个ID发送的字符串
// * 返回  : 1，发送成功
// *         0，发送失败
// * 调用  ：被外部调用
// */
//bool LM005A_SendString ( FunctionalState enumEnUnvarnishTx, char * pStr, u32 ulStrLength, ENUM_ID_NO_TypeDef ucId )
//{
//	char cStr [20];
//	bool bRet = false;
//	
//		
//	if ( enumEnUnvarnishTx )
//	{
//		macLM005A_Usart ( "%s", pStr );
//		
//		bRet = true;
//		
//	}

//	else
//	{
//		if ( ucId < 5 )
//			sprintf ( cStr, "AT+CIPSEND=%d,%d", ucId, ulStrLength + 2 );

//		else
//			sprintf ( cStr, "AT+CIPSEND=%d", ulStrLength + 2 );
//		
//		LM005A_Cmd ( cStr, "> ", 0, 1000 );

//		bRet = LM005A_Cmd ( pStr, "SEND OK", 0, 1000 );
//  }
//	
//	return bRet;

//}


///*
// * 函数名：LM005A_ReceiveString
// * 描述  ：WF-LM005A模块接收字符串
// * 输入  ：enumEnUnvarnishTx，声明是否已使能了透传模式
// * 返回  : 接收到的字符串首地址
// * 调用  ：被外部调用
// */
//char * LM005A_ReceiveString ( FunctionalState enumEnUnvarnishTx )
//{
//	char * pRecStr = 0;
//	
//	
//	strLM005A_Fram_Record .InfBit .FramLength = 0;
//	strLM005A_Fram_Record .InfBit .FramFinishFlag = 0;
//	
//	while ( ! strLM005A_Fram_Record .InfBit .FramFinishFlag );
//	strLM005A_Fram_Record .Data_RX_BUF [ strLM005A_Fram_Record .InfBit .FramLength ] = '\0';
//	
//	if ( enumEnUnvarnishTx )
//		pRecStr = strLM005A_Fram_Record .Data_RX_BUF;
//	
//	else 
//	{
//		if ( strstr ( strLM005A_Fram_Record .Data_RX_BUF, "+IPD" ) )
//			pRecStr = strLM005A_Fram_Record .Data_RX_BUF;

//	}

//	return pRecStr;
//	
//}
