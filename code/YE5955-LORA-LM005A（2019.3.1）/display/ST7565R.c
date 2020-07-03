#include "conf.h"
#include "ST7565R.h"

uint8_t DispBuf[DISPBUFLEN];
uint8_t DispFlashBuf[DISPBUFLEN];
#define Delay(d) {rt_thread_delay(20*RT_TICK_PER_SECOND/1000);}




/*写指令到LCD 模块*/
void transfer_command(int data1)
{
	lcd_cs_clr();
	lcd_rs_clr();
			/* Loop while DR register in not emplty */
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) != RESET)
	{};
	/* Send byte through the SPI2peripheral */
	SPI_I2S_SendData(SPI2,data1);	
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) != RESET)
	{};
// 	for(i=0;i<8;i++)
// 	{
// 		lcd_sclk_clr();
// 		if(data1&0x80){ lcd_data_set();}
// 		else {lcd_data_clr();}
// 		lcd_sclk_set();
// 		data1=data1<<=1;
// 	}
}

/*写数据到LCD 模块*/

void transfer_data(int data1)
{

	lcd_cs_clr();
	lcd_rs_set();
				/* Loop while DR register in not emplty */
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) != RESET)
	{};
	/* Send byte through the SPI2peripheral */
	SPI_I2S_SendData(SPI2,data1);	

		while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) != RESET)
	{};

// 	for(i=0;i<8;i++)
// 	{
// 		lcd_sclk_clr();
// 		if(data1&0x80){ lcd_data_set();}
// 		else{ lcd_data_clr();}
// 		lcd_sclk_set();
// 		data1=data1<<=1;
// 	}
}

/*LCD 模块初始化*/
void initST7565R()
{
	lcd_cs_clr();
	lcd_reset_clr(); /*低电平复位*/
	Delay(100);
	lcd_reset_set(); /*复位完毕*/
	Delay(20);
	transfer_command(0xe2); /*软复位*/
	Delay(5);
	transfer_command(0x2c); /*升压步聚1*/
	Delay(5);
	transfer_command(0x2e); /*升压步聚2*/
	Delay(5);
	transfer_command(0x2f); /*升压步聚3*/
	Delay(5);
	transfer_command(0x23); /*粗调对比度，可设置范围0x20～0x27*/
	transfer_command(0x81); /*微调对比度*/

	transfer_command(0x28); /*0x1a,微调对比度的值，可设置范围0x00～0x3f*/
	transfer_command(0xa2); /*1/9 偏压比（bias）*/
	transfer_command(0xc0); /*行扫描顺序：从上到下*/
	transfer_command(0xa1); /*列扫描顺序：从左到右*/
	transfer_command(0x40); /*起始行：第一行开始*/
	transfer_command(0xaf); /*开显示*/
	lcd_cs_set();
}



void lcd_address(uint8_t page,uint8_t column)
{
	lcd_cs_clr();
	transfer_command(0xb0+page); //设置页地址。每页是8 行。一个画面的64 行被分成8 个页*/
	transfer_command(((column>>4)&0x0f)+0x10); //设置列地址的高4 位
	transfer_command(column&0x0f); //设置列地址的低4 位
}


// void drawPoint(uint8_t row,uint8_t col,uint8_t color)  // color=0 不显示，　color!=0 显示点
// {
// 	uint16_t addr,bits;
// 	if(row>=64)return;
// 	if(col>=128)return;
// 	addr=(uint16_t)(row/8)*128+col;   // 字节地址
// 	bits=(row%8);
// 	if(color!=0)DispBuf[addr] |= (1<<bits);
// 	else DispBuf[addr] &= ~(1<<bits);
// }

void drawPoint(uint8_t srow,uint8_t scol,uint8_t color)  // color=0 不显示，　color!=0 显示点
{
	uint16_t addr,bits;
	if(srow>=LCD_WIDTH)return;
	if(scol>=LCD_HEIGHT)return;
	addr=(uint16_t)((LCD_HEIGHT-1-scol)/8)*128+srow;   // 字节地址
	bits=((LCD_HEIGHT-1-scol)%8);
	if(color!=0)DispBuf[addr] |= (1<<bits);
	else DispBuf[addr] &= ~(1<<bits);
}

void flashPoint(uint8_t srow,uint8_t scol,uint8_t color)  // flash=0  不闪　flash!=0 闪
{
		uint16_t addr,bits;
	if(srow>=LCD_WIDTH)return;
	if(scol>=LCD_HEIGHT)return;
	addr=(uint16_t)((LCD_HEIGHT-1-scol)/8)*128+srow;   // 字节地址
	bits=((LCD_HEIGHT-1-scol)%8);
	if(color!=0)DispFlashBuf[addr] |= (1<<bits);
	else DispFlashBuf[addr] &= ~(1<<bits);
}

uint8_t getCharWidth(uint8_t* c)
{
	return(getCharWidthWithFont(c,&ASC6X12FONT,&ZH12X12FONT));
}
uint8_t drawChar(uint8_t row,uint8_t col,uint8_t* c,uint8_t color)	//  c为ASCII码  // 返回值为下一个字的列位置
{
	return(drawCharWithFont(row,col,c,&ASC6X12FONT,&ZH12X12FONT,color));
}
uint8_t getStringWidth(uint8_t* s)
{
	uint8_t col=0;
	do
	{
		if(s[0]==0)	{	return(col);		}
		else if(s[0]<127)
		{
			if(s[0]==INVCOLOR){    // 反显下一个字符
			}
			else	{
				col+=getCharWidth(s);
			}
			s++;
			
		}
		else
		{
			col+=getCharWidth(s);
			s+=2;
		}
		if((*s)==0)return(col);
		if((*s)>127 && (*(s+1))==0)return(col);
	}while(1);
}

uint8_t drawString(uint8_t row,uint8_t col,uint8_t* s,uint8_t color)
{
	uint8_t invcolor=color;
	do
	{
		if(s[0]==0)return(col);
		if(s[0]<127)
		{
			if(s[0]==INVCOLOR){    // 反显下一个字符
				invcolor=!color;
			}
			else	{
				col=drawChar(row,col,s,invcolor);
				invcolor=color;
			}
			s++;
			
		}
		else
		{
			col=drawChar(row,col,s,invcolor);
			invcolor=color;
			s+=2;
		}
		if((*s)==0)return(col);
		if((*s)>127 && (*(s+1))==0)return(col);
	}while(1);
	
	
}
uint8_t drawStringFlash(uint8_t row,uint8_t col,uint8_t* s,uint8_t color)
{
	uint8_t invcolor=color;
	do
	{
		if(s[0]==0)return(col);
		if(s[0]<127)
		{
			if(s[0]==INVCOLOR){    // 反显下一个字符
				invcolor=!color;
			}
			else	{
				col=drawCharFlash(row,col,s,invcolor);
				invcolor=color;
			}
			s++;
			
		}
		else
		{
			col=drawCharFlash(row,col,s,invcolor);
			invcolor=color;
			s+=2;
		}
		if((*s)==0)return(col);
		if((*s)>127 && (*(s+1))==0)return(col);
	}while(1);
	
	
}
uint8_t drawCharFlash(uint8_t row,uint8_t col,uint8_t *c,uint8_t color)	 //  c为ASCII码
{
	return(drawCharFlashWithFont(row,col,c,&ASC6X12FONT,&ZH12X12FONT,color));
}

uint8_t getCharWidthWithFont(uint8_t *cp,const struct Font* font,const struct Font* zhFont)	//  c为ASCII码
{
	uint8_t mw;
	const unsigned char *tab;
	if(*cp<128)
	{
		tab=font->FontData+(*cp-0x20)*font->ArrayWidth;  // 取得字模指针
		mw=font->Width;
	}
	else
	{
		tab=zhFont->FontData;
		while(tab[0]!=0 && tab[1]!=0)
		{
			if((tab[0]==*cp) && (tab[1]==*(cp+1)))break;  // 取得汉字模指针
			tab+=(zhFont->ArrayWidth+2);
		}
		tab+=2;
		mw=zhFont->Width;
	}
	return(mw);  // 返回下一个字的列位置．
	
}
uint8_t drawCharWithFont(uint8_t row,uint8_t col,uint8_t *cp,const struct Font* font,const struct Font* zhFont,uint8_t color)	//  c为ASCII码
{
	uint8_t	i,j,k,kk;
	uint8_t mw,mh;
	const unsigned char *tab;
	if(*cp<128)
	{
		tab=font->FontData+(*cp-0x20)*font->ArrayWidth;  // 取得字模指针
		mh=font->Height;
		mw=font->Width;
	}
	else
	{
		tab=zhFont->FontData;
		while(tab[0]!=0 && tab[1]!=0)
		{
			if((tab[0]==*cp) && (tab[1]==*(cp+1)))break;  // 取得汉字模指针
			tab+=(zhFont->ArrayWidth+2);
		}
		tab+=2;
		mh=zhFont->Height;
		mw=zhFont->Width;
	}
	
	for(i=0;i<(mh-1)/8+1;i++)   //  列行式扫描
	{
		for(j=0;j<mw;j++)   //  8行点阵
		{
			if((mh-(i+1)*8)<0)kk=mh%8;
			else kk=8;     //  计算最后一列行的高度
			for(k=0;k<kk;k++)
			{
				drawPoint(row+i*8+k,col+j,(((*tab)&(1<<k))!=0) ^ (color==0));
			}
			tab++;
		}
	}
	return(col+mw);  // 返回下一个字的列位置．
	
}

uint8_t drawCharFlashWithFont(uint8_t row,uint8_t col,uint8_t *cp,const struct Font* font,const struct Font *zhFont,uint8_t color)   //  c为ASCII码
{
	uint8_t	i,j,k,kk;
	uint8_t mw,mh;
	const unsigned char *tab;
	if(*cp<128)
	{
		tab=font->FontData+(*cp-0x20)*font->ArrayWidth;  // 取得字模指针
		mh=font->Height;
		mw=font->Width;
	}
	else
	{
		tab=zhFont->FontData;
		while(tab[0]!=0 && tab[1]!=0)
		{
			if((tab[0]==*cp) && (tab[1]==*(cp+1)))break;  // 取得汉字模指针
			tab+=(zhFont->ArrayWidth+2);
		}
		tab+=2;
		mh=zhFont->Height;
		mw=zhFont->Width;
	}
	
	for(i=0;i<(mh-1)/8+1;i++)   //  列行式扫描
	{
		for(j=0;j<mw;j++)   //  8行点阵
		{
			if((mh-(i+1)*8)<0)kk=mh%8;
			else kk=8;     //  计算最后一列行的高度
			for(k=0;k<kk;k++)
			{
				flashPoint(row+i*8+k,col+j,(((*tab)&(1<<k))!=0) ^ (color==0));
			}
			tab++;
		}
	}
	return(col+mw);  // 返回下一个字的列位置．
	
}

void fillRect(uint8_t row1,uint8_t col1,uint8_t row2,uint8_t col2,uint8_t color)
{
	uint8_t i;
	for(i=row1;i<=row2;i++) drawHLine(i,col1,col2,color);
}

// void fillRect(uint8_t row,uint8_t col,uint8_t width,uint8_t height,uint8_t color)
// {
// 	uint8_t i,j;
// 	for(i=0;i<width;i++)
// 	for(j=0;j<height;j++)
// 		drawPoint(j+row,i+col,color);
// }

void updateLcd(void)   // 100mS 一次
{
		uint32_t i,j;
		static uint8_t loops=0;
		
		for(i=0;i<8;i++)
		{
			lcd_cs_clr();
			transfer_command(0xb0+i);
			transfer_command(0x10);
			transfer_command(0x00);
			if(loops<4){
				for(j=0;j<128;j++)			{					transfer_data(DispBuf[i*128+j]);			}
			}else{
				for(j=0;j<128;j++)			{	transfer_data(DispBuf[i*128+j] & ~DispFlashBuf[i*128+j]);			}

			}
	 }
	 lcd_cs_set();
	 
	 loops++;
	 if(loops>6)loops=0;
}				
void clearDisplay(void)
{
	uint16_t i;
	for(i=0;i<DISPBUFLEN;i++)
	{
		DispBuf[i]=0x00;
		DispFlashBuf[i]=0x00;
		
//					IO_OUTPUT_FLASH_BUFFER[i]=0x00;
	}
	
}
uint8_t lcd_inited=0;
void initLCD()
{
	uint16_t i;
				SPI_InitTypeDef  SPI_InitStructure;
			GPIO_InitTypeDef GPIO_InitStructure;
			

			/* Enable SPI2 GPIOB clocks */
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);


			// LED_CS  // Key_LOCK	
			/* Configure Pa8as outputs push-pull, max speed 50 MHz               */
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10| GPIO_Pin_11 | GPIO_Pin_12;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
			GPIO_Init(GPIOC, &GPIO_InitStructure);
			
			lcd_bg_clr();
	
			/* Configure SPI2 pins: SCK, MISO and MOSI ---------------------------------*/
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13  | GPIO_Pin_14 | GPIO_Pin_15;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_Init(GPIOB, &GPIO_InitStructure);

			/* SPI2 configuration */ 
			SPI_Cmd(SPI2, DISABLE);  
			SPI_StructInit(&SPI_InitStructure);
 			SPI_I2S_DeInit(SPI2);
 			SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
 			SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
 			SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
 			SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;   // 平时为低电平
 			SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
 			SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
 			SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
 			SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
 			SPI_Init(SPI2, &SPI_InitStructure);
 			
 			/* Enable SPI2  */
 			SPI_Cmd(SPI2, ENABLE);   
			
	
			for(i=0;i<DISPBUFLEN;i++)
			{
					DispBuf[i]=0x00;
					DispFlashBuf[i]=0x00;
				//					IO_OUTPUT_FLASH_BUFFER[i]=0x00;
			}
			initST7565R();				
			updateLcd();
			lcd_bg_set();

}
void RefreshLcdTask(void* parameter)
{
		if(!lcd_inited)	
		{
			initLCD();
			lcd_inited=1;
		}

		while(1)
		{
//			  drawChar(0,0,"设");
//			 drawChar(0,SCR_WIDTH-12,"B");
//			 drawChar(SCR_HEIGHT-12,0,"C");
//			 drawChar(SCR_HEIGHT-12,SCR_WIDTH-12,"置");
//	    drawPoint(23,11,1);
			updateLcd();
			
				rt_thread_delay(100);

		}
}

/* 画水平线 */
void drawHLine( uint8_t row,uint8_t col1, uint8_t col2,uint8_t color)
{
	if(col1<col2)	
	{    
		while (col1<=col2)   {      drawPoint(row,col1,color);      col1++;   }
	}
	else if(col1>col2)
	{
    while (col2 <= col1)   {    drawPoint(row,col2,color);     col1++;   }
	}
	else
	{
       drawPoint(row,col1,color);
	}
}

/* 垂直线 */
void drawVLine(uint8_t row1,uint8_t row2, uint8_t col,uint8_t color)
{
	if(row1<row2)
	{	
    while (row1 <= row2)    {    drawPoint(row1,col,color);row1++;}
	}
	else if(row1 > row2)
	{
    while (row2 <= row1)   {    drawPoint(row2,col,color);row2++;}
	}
	else
	{
		drawPoint(row1,col,color);
	}
	
}
void drawRect(uint8_t row1,uint8_t col1,uint8_t row2,uint8_t col2,uint8_t color)
{
	drawVLine(row1,row2,col1,color);
	drawVLine(row1,row2,col2,color);
	drawHLine(row1,col1,col2,color);
	drawHLine(row2,col1,col2,color);
}
void drawImag(uint8_t row,uint8_t col,uint8_t width,uint8_t height,const unsigned char* tab,uint8_t color)	//  
{
	uint8_t	i,j,k,kk;
	uint8_t mw,mh;

	mh=height;
	mw=width;

	for(i=0;i<(mh-1)/8+1;i++)   //  列行式扫描
	{
		for(j=0;j<mw;j++)   //  8行点阵
		{
			if((mh-(i+1)*8)<0)kk=mh%8;
			else kk=8;     //  计算最后一列行的高度
			for(k=0;k<kk;k++)
			{
				drawPoint(row+i*8+k,col+j,(((*tab)&(1<<k))!=0) ^ (color==0));
			}
			tab++;
		}
	}
	
}
