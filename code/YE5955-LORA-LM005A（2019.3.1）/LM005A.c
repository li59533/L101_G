
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


extern void  time_delay_ms(unsigned int count_val); //����ӳ�60s
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
	char buf[126];  //�����˷����ֽڵĳ��ȣ�66��
	uint16_t 	j=0;
	va_list ap;
	va_start(ap, Data);

	while ( * Data != 0 )     // �ж��Ƿ񵽴��ַ���������
	{				                          
		if ( * Data == '%')
		{									  //
			switch ( *++Data )
			{				
				case 's':						  //�ַ���
						s = va_arg(ap, const char *);
						for ( ; *s; s++) 
						{
							ATcmd[j++]=*s;	
							
						}
						Data++;
				break;

				case 'd':	                    //ʮ����							
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
	char buf[126];  //�����˷����ֽڵĳ��ȣ�66��
	uint16_t 	j=0;
	va_list ap;
	va_start(ap, Data);

	while ( * Data != 0 )     // �ж��Ƿ񵽴��ַ���������
	{				                          
		if ( * Data == '%')
		{									
			switch ( *++Data )
			{				
				case 's':					//�ַ���
						s = va_arg(ap, const char *);				
						for ( ; *s; s++) 
						{
							ATcmd[j++]=*s;		
						}
						Data++;
				break;

				case 'd':		          //ʮ����						
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
 * ��������LM005A_Rst
 * ����  ������WF-LM005Aģ��
 * ����  ����
 * ����  : ��
 * ����  ���� LM005A_AT_Test ����
 */
void  LM005A_Rst ( void )
{
	#if 0
	 LM005A_Cmd ( "AT+RST", "OK", "ready", 2500 );   		
	#else	
	#endif
}


/*
 * ��������LM005A_senddata

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
 * ��������LM005A_Cmd
 * ����  ����WF-LM005Aģ�鷢��ATָ��
 * ����  ��cmd�������͵�ָ��
 *         reply1��reply2���ڴ�����Ӧ��ΪNULL������Ӧ������Ϊ���߼���ϵ
 *         waittime���ȴ���Ӧ��ʱ��
 * ����  : 1��ָ��ͳɹ�
 *         0��ָ���ʧ��
 * ����  �����ⲿ����
 */
bool  LM005A_Cmd ( char * cmd, char * reply1, char * reply2, uint32_t waittime )
{    
	strLM005A_Fram_Record .InfBit .FramLength = 0;               //���¿�ʼ�����µ����ݰ�
	macLM005A_Usart ( "%s\r\n", cmd );
//	sprintf ( cmd,"%s\r\n", cmd );
//  USART_CMD_LM005A(cmd);
	if ( ( reply1 == 0 ) && ( reply2 == 0 ) )                      //����Ҫ��������
		return true;
	
	time_delay_ms( waittime );                 //��ʱ
	
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
	//strLM005A_Fram_Record .InfBit .FramLength = 0;               //���¿�ʼ�����µ����ݰ�
 
//	sprintf ( cmd,"%s\r\n", cmd );
//  USART_CMD_LM005A(cmd);
	if ( ( reply1 == 0 ) && ( reply2 == 0 ) )                      //����Ҫ��������
		return true;
	
	time_delay_ms( waittime );                 //��ʱ
	
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
 * ��������LM005A_AT_Test
 * ����  ����WF-LM005Aģ�����AT��������
 * ����  ����
 * ����  : ��
 * ����  �����ⲿ����
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
 * ��������LM005A_AT_ADR
 * ����  ����LM005Aģ��ADR����
 * ����  ����
 * ����  : ��
 * ����  �����ⲿ����
 */
bool  LM005A_ADR_SET ( uint32_t  ADRvalue )
{
	char cCmd [120];
	sprintf ( cCmd, "AT+ADR=%d", ADRvalue );
	return LM005A_Cmd ( cCmd, "OK", "no change", 1000 );   //��1s��	
}

/*
 * ��������LM005A_AT_JOIN
 * ����  ����LM005Aģ��JOIN����
 * ����  ����
 * ����  : ��
 * ����  �����ⲿ����
 */
bool  LM005A_JOIN_SET (void)
{
	char cCmd [120];
	sprintf (cCmd, "AT+JOIN");
	return LM005A_Cmd ( cCmd, "RXDONE", "no change",15000 );   //��1s��	
}


bool  LM005A_VERSION (void)
{
	char cCmd [120];
	sprintf (cCmd, "AT+VERSION?");
	return LM005A_Cmd ( cCmd, "RXDONE", "no change",1000 );   //��1s��	
}


bool  LM005A_RESET (void)
{
	char cCmd [120];
	sprintf (cCmd, "AT+RESET");
	return LM005A_Cmd ( cCmd, "OK", "no change", 3500 );   //��1s��
}

/*
 * ��������LM005A_AT_DR
 * ����  ����LM005Aģ��DR����
 * ����  ����
 * ����  : ��
 * ����  �����ⲿ����
 */
bool  LM005A_DR_SET ( uint32_t  DRvalue )
{
	char cCmd [120];
	sprintf ( cCmd, "AT+DR=%d", DRvalue );
	return LM005A_Cmd ( cCmd, "OK", "no change", 1000 );   //��1s��	
}



/*
 * ��������LM005A_AT_MODE
 * ����  ����LM005Aģ��MODE����
 * ����  ����
 * ����  : ��
 * ����  �����ⲿ����
 */
bool  LM005A_MODE_SET ( uint32_t  MODEvalue )
{
	char cCmd [120];
	sprintf ( cCmd, "AT+MODE=%d", MODEvalue );
	return LM005A_Cmd ( cCmd, "OK", "no change", 500 );   //��1s��	
}


/*
 * ��������LM005A_AT_CLASS
 * ����  ����LM005Aģ��CLASS����
 * ����  ����
 * ����  : ��
 * ����  �����ⲿ����
 */
bool  LM005A_CLASS_SET ( uint32_t  CLASSvalue )
{
	char cCmd [120];
	sprintf ( cCmd, "AT+CLASS=%d", CLASSvalue );
	return LM005A_Cmd ( cCmd, "OK", "no change", 500 );   //��1s��	
}


/*
 * ��������LM005A_AT_POWER
 * ����  ����LM005Aģ��DR����
 * ����  ����
 * ����  : ��
 * ����  �����ⲿ����
 */
bool  LM005A_POWER_SET ( uint32_t  POWERvalue )
{
	char cCmd [120];
	sprintf ( cCmd, "AT+POWER=%d", POWERvalue );
	return LM005A_Cmd ( cCmd, "OK", "no change", 1000 );   //��1s��	
}


/*
 * ��������LM005A_SEND_STRING
 * ����  ����LM005Aģ��SEND_STRING����
 * ����  ����
 * ����  : ��
 * ����  ��
 */
bool  LM005A_SET_STRING ( char *data )
{
	char cCmd [120];
	sprintf ( cCmd, "AT+POWER=%s", data );
	return LM005A_Cmd ( cCmd, "OK", "no change", 1000 );   //��1s��	
}



///*
// * ��������LM005A_Net_Mode_Choose
// * ����  ��ѡ��LM005Aģ��Ĺ���ģʽ
// * ����  ��enumMode������ģʽ
// * ����  : 1��ѡ��ɹ�
// *         0��ѡ��ʧ��
// * ����  �����ⲿ����
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
// * ��������LM005A_JoinAP
// * ����  ��WF-LM005Aģ�������ⲿWiFi
// * ����  ��pSSID��WiFi�����ַ���
// *       ��pPassWord��WiFi�����ַ���
// * ����  : 1�����ӳɹ�
// *         0������ʧ��
// * ����  �����ⲿ����
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
// * ��������LM005A_BuildAP
// * ����  ��WF-LM005Aģ�鴴��WiFi�ȵ�
// * ����  ��pSSID��WiFi�����ַ���
// *       ��pPassWord��WiFi�����ַ���
// *       ��enunPsdMode��WiFi���ܷ�ʽ�����ַ���
// * ����  : 1�������ɹ�
// *         0������ʧ��
// * ����  �����ⲿ����
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
// * ��������LM005A_Enable_MultipleId
// * ����  ��WF-LM005Aģ������������
// * ����  ��enumEnUnvarnishTx�������Ƿ������
// * ����  : 1�����óɹ�
// *         0������ʧ��
// * ����  �����ⲿ����
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
// * ��������LM005A_Link_Server
// * ����  ��WF-LM005Aģ�������ⲿ������
// * ����  ��enumE������Э��
// *       ��ip��������IP�ַ���
// *       ��ComNum���������˿��ַ���
// *       ��id��ģ�����ӷ�������ID
// * ����  : 1�����ӳɹ�
// *         0������ʧ��
// * ����  �����ⲿ����
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
// * ��������LM005A_StartOrShutServer
// * ����  ��WF-LM005Aģ�鿪����رշ�����ģʽ
// * ����  ��enumMode������/�ر�
// *       ��pPortNum���������˿ں��ַ���
// *       ��pTimeOver����������ʱʱ���ַ�������λ����
// * ����  : 1�������ɹ�
// *         0������ʧ��
// * ����  �����ⲿ����
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
// * ��������LM005A_Get_LinkStatus
// * ����  ����ȡ WF-LM005A ������״̬�����ʺϵ��˿�ʱʹ��
// * ����  ����
// * ����  : 2�����ip
// *         3����������
// *         3��ʧȥ����
// *         0����ȡ״̬ʧ��
// * ����  �����ⲿ����
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
// * ��������LM005A_Get_IdLinkStatus
// * ����  ����ȡ WF-LM005A �Ķ˿ڣ�Id������״̬�����ʺ϶�˿�ʱʹ��
// * ����  ����
// * ����  : �˿ڣ�Id��������״̬����5λΪ��Чλ���ֱ��ӦId5~0��ĳλ����1���Id���������ӣ�������0���Idδ��������
// * ����  �����ⲿ����
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
// * ��������LM005A_Inquire_ApIp
// * ����  ����ȡ F-LM005A �� AP IP
// * ����  ��pApIp����� AP IP ��������׵�ַ
// *         ucArrayLength����� AP IP ������ĳ���
// * ����  : 0����ȡʧ��
// *         1����ȡ�ɹ�
// * ����  �����ⲿ����
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
// * ��������LM005A_UnvarnishSend
// * ����  ������WF-LM005Aģ�����͸������
// * ����  ����
// * ����  : 1�����óɹ�
// *         0������ʧ��
// * ����  �����ⲿ����
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
// * ��������LM005A_ExitUnvarnishSend
// * ����  ������WF-LM005Aģ���˳�͸��ģʽ
// * ����  ����
// * ����  : ��
// * ����  �����ⲿ����
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
// * ��������LM005A_SendString
// * ����  ��WF-LM005Aģ�鷢���ַ���
// * ����  ��enumEnUnvarnishTx�������Ƿ���ʹ����͸��ģʽ
// *       ��pStr��Ҫ���͵��ַ���
// *       ��ulStrLength��Ҫ���͵��ַ������ֽ���
// *       ��ucId���ĸ�ID���͵��ַ���
// * ����  : 1�����ͳɹ�
// *         0������ʧ��
// * ����  �����ⲿ����
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
// * ��������LM005A_ReceiveString
// * ����  ��WF-LM005Aģ������ַ���
// * ����  ��enumEnUnvarnishTx�������Ƿ���ʹ����͸��ģʽ
// * ����  : ���յ����ַ����׵�ַ
// * ����  �����ⲿ����
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
