#include "conf.h"
#include "BA102142LCD.h"

#include "display.h"
#include "ST7567.h"

uint8 xdata	DispBuf[DISPBUFLEN];   //  ��ʾ������	  =1  ��ʾ
uint8 xdata DispFlashBuf[DISPBUFLEN];  // ��˸��־	  =1  ��˸

void DisplayON()
{
	bg_lcd_set();
}
void DisplayOFF()
{
	bg_lcd_clr();
}
void clearUnit()			// �����λ��ʾ
{
	DispBuf[9]=0;	   DispBuf[10]=0;
}
void clearType()			// ���������ʾ
{	  
	DispBuf[0]=0;	
	DispBuf[47]=0;
	ClrBit(DispBuf[20],2);
}

void displayLoop()
{
	static uint16 idata flashcount=0;
	uint8 ds,fds;
	uint8 j;
	static idata uint8 page=0;
	lcd_cs_clr();	
	lcd_address(page,0);  // ����ҳ, ��ʼ��
	for(j=0;j<128;j++)
	{
		ds=DispBuf[(uint16)page*128+j];
		fds=DispFlashBuf[(uint16)page*128+j];
		if(flashcount<300)	transfer_data(ds);
		else transfer_data(ds & ~fds);
	}

	lcd_cs_set();

	if(flashcount<600)	flashcount++;
	else flashcount=0;
	
	page++;
	if(page<4) return;
	page=0;
}

void updateDisplay(uint8 t)
{
		uint8 ds,fds;
		uint8 j;
		uint8 page;
		t=t%20;
		lcd_cs_clr();	
		for(page=0;page<4;page++)
		{
			lcd_address(page,0);  // ����ҳ, ��ʼ��
			for(j=0;j<128;j++)
			{
				ds=DispBuf[(uint16)page*128+j];
				fds=DispFlashBuf[(uint16)page*128+j];
				if(t<10)	transfer_data(ds);
				else transfer_data(ds & ~fds);
			}
		}
		lcd_cs_set();
}
	
void flashChar(uint8 pos,uint8 c)
{
	if(pos<4)return;
	if(pos>7)return;     //ֻ��ʾһ������
	drawCharFlash(4,34,c);
}
                                
void dispChar(uint8 pos,uint8 c)			    //  0,1,2,3
																					//  4,5,6,7
																					//  8,9,10,11
{
	if(pos<4)return;
	if(pos>7)return;    //ֻ��ʾһ������
	drawChar(4,34,c);
}

void echoChar(uint8 c,uint8 pos)	  // ��ʾ�ַ���	  // ����������ʾ
{
	static code uint8 ptab[]={7,6,5,4,0,0,0,0,0};
	dispChar(ptab[pos],c);	
}

void dispStr(uint8 lines,const unsigned char* s)
{
										 // �˵���ʾ�ڵڶ���
	uchar idata c;uint8 idata i;
	clearLine( MENULINE );
	for(i=0;i<4;i++)
	{
		c=s[i];	
		if(c!=L_END)
		{
			echoChar(c,3-i);
		}
		else
		{
			return;
		}
	}
}

void clearLine(uint8 lines)
{
	switch(lines)   
	{
		case 1:		  // 21 23 25 27
				dispChar(0,L_);
				dispChar(1,L_);
				dispChar(2,L_);
				dispChar(3,L_);
				break;
		case 2:	dispChar(4,L_);
				dispChar(5,L_);
				dispChar(6,L_);
				dispChar(7,L_);
				break;
		case 3: dispChar(8,L_);
				dispChar(9,L_);
				dispChar(10,L_);
				dispChar(11,L_);
				break;
		case 4:							   //14,12,10,8,6,3,2,1,0
				break;
		default:
				break;
	}
	
}

void dispDP(uint8 lines,uint8 deci)    //���ֽ� ��Ӧ������λ��,���ֽ�,��ӦС����λ��
{
	switch(lines)
	{
		case 1:	if(deci==1)		{ SetBit(DispBuf[5],5);  }
				else if(deci==2){ SetBit(DispBuf[3],5);}
				else if(deci==3){ SetBit(DispBuf[1],5);}
				else if(deci==4) {}
				else   {}
				return;
		case 2:	if(deci==1)		{ SetBit(DispBuf[11],7);  }
				else if(deci==2){ SetBit(DispBuf[13],7);}
				else if(deci==3){ SetBit(DispBuf[15],7);}
				else if(deci==4) {}
				else   {}
				return;
		case 3:	if(deci==1)		{ SetBit(DispBuf[12],7);  }
				else if(deci==2){ SetBit(DispBuf[14],7);}
				else if(deci==3){ SetBit(DispBuf[16],7);}
				else if(deci==4) {}
				else   {}
				return;
		case 4:	break;
		default:break;
	}
	
} 	
uint8 code line1PosTab[]={3,2,1,0};
uint8 code line2PosTab[]={7,6,5,4};
uint8 code line3PosTab[]={11,10,9,8};
void dispByte(uint8 lines,uint8 pos,uint8 c)  // ��˸һ���ַ�    lines=1 ��1�У���2�У���3�У���4�У�
								// pos �������� 0,1,2,3
{
		switch(lines)   
		{
			case 1:	dispChar(line1PosTab[pos],c);
					break;
			case 2:	dispChar(line2PosTab[pos],c);
				break;
			case 3: dispChar(line3PosTab[pos],c);
				break;
//			case 4: dispChar(line4PosTab[pos],c);
//				break;
			default:
				break;
	}	
}
void flashByte(uint8 lines,uint8 pos)	   // ��˸һ���ַ�    lines=1 ��1�У���2�У���3�У���4�У�
{											// pos �������� 0,1,2,3
//	uint8 code line4PosTab[]={0,1,2,3,6,8,10,12,14};

	switch(lines)   
	{
		case 1:	flashChar(line1PosTab[pos],8);
				break;
		case 2:	flashChar(line2PosTab[pos],8);
				break;
		case 3: flashChar(line3PosTab[pos],8);
				break;
		case 4: //	flashChar(line4PosTab[pos],8);
				break;
		default:
				break;
	}

}
void unFlashByte(uint8 lines,uint8 pos)	   // ֹͣ��˸һ���ַ�    lines=1 ��1�У���2�У���3�У���4�У�
										// pos �������� 0,1,2,3��
{					
	switch(lines)   
	{
		case 1:	flashChar(line1PosTab[pos],L_);
				break;
		case 2:	flashChar(line2PosTab[pos],L_);
				break;
		case 3: flashChar(line3PosTab[pos],L_);
				break;
		case 4:	//flashChar(line4PosTab[pos],L_);
				break;
		default:
				break;
	}

}
void dispUINT32(uint8 lines,uint32 i,uint8 sign,uint8 deci)	   // ��ʾ����ֵ   lines=1 ��1�У���2�У���3�У���4�У�
{															  // deci С��λ�� =0 ���� 
	switch(lines)   
	{
		case 1:		  // 21 23 25 27
				dispChar(0,(i%10000)/1000);
				dispChar(1,(i%1000)/100);
				dispChar(2,(i%100)/10);
				dispChar(3,(i%10));
				if(sign==1) dispSignLine1();
				
				break;
		case 2:	dispChar(4,(i%10000)/1000);
				dispChar(5,(i%1000)/100);
				dispChar(6,(i%100)/10);
				dispChar(7,(i%10));
				if(sign==1) dispSignLine2();
				break;
		case 3: dispChar(8,(i%10000)/1000);
				dispChar(9,(i%1000)/100);
				dispChar(10,(i%100)/10);
				dispChar(11,(i%10));
 				if(sign==1) dispSignLine1();
				break;
		case 4:							   //14,12,10,8,6,3,2,1,0
				dispChar(12,(i%1000000000)/100000000);
				dispChar(13,(i%100000000)/10000000);
				dispChar(14,(i%10000000)/1000000);
				dispChar(15, (i%1000000)/100000);
				dispChar(16, (i%100000)/10000);
				dispChar(17, (i%10000)/1000);
				dispChar(18, (i%1000)/100);
				dispChar(19, (i%100)/10);
				dispChar(20, (i%10));
				break;
		default:
				break;
	}

	dispDP(lines,deci);
}

void	initDisplay()
{
	uint8 i;	
	INITDISPPORT();
	initST7567();
	
	clearDisplay();
}
