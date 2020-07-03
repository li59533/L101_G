#include "conf.h"
#include "MyFont.h"
#include "zhFontCustom.h"
#include <rtthread.h>

#define LCD_WIDTH 128
#define LCD_HEIGHT 64
#define DISPBUFLEN (LCD_WIDTH*LCD_HEIGHT/8)
#define SCR_WIDTH  LCD_HEIGHT
#define SCR_HEIGHT LCD_WIDTH

extern void transfer_command(int data1);
extern void transfer_data(int data1);
extern void lcd_address(uint8_t page,uint8_t column);
extern void initJLX12864(void);
extern uint8_t  drawChar(uint8_t row,uint8_t col,uint8_t* c,uint8_t color);
extern uint8_t  drawCharFlash(uint8_t row,uint8_t col,uint8_t* c,uint8_t color);

extern uint8_t drawCharWithFont(uint8_t row,uint8_t col,uint8_t *cp,const struct Font *font,const struct Font *zhFont,uint8_t color);
extern uint8_t drawCharFlashWithFont(uint8_t row,uint8_t col,uint8_t *cp,const struct Font *font,const struct Font *zhFont,uint8_t color);
extern uint8_t getCharWidthWithFont(uint8_t *cp,const struct Font* font,const struct Font* zhFont);
extern uint8_t getStringWidth(uint8_t* s);

extern void clearDisplay(void);
extern void RefreshLcdTask(void* parameter);
extern void initLCD(void);
extern void updateLcd(void);
extern uint8_t drawString(uint8_t row,uint8_t col,uint8_t* s,uint8_t color);
extern uint8_t drawStringFlash(uint8_t row,uint8_t col,uint8_t* s,uint8_t color);
extern void drawVLine(uint8_t row1,uint8_t row2, uint8_t col,uint8_t color);
extern void drawHLine( uint8_t row,uint8_t col1, uint8_t col2,uint8_t color);
extern void drawRect(uint8_t row1,uint8_t col1,uint8_t row2,uint8_t col2,uint8_t color);
extern void drawImag(uint8_t row,uint8_t col,uint8_t width,uint8_t height,const unsigned char* tab,uint8_t color);
extern void fillRect(uint8_t row1,uint8_t col1,uint8_t row2,uint8_t col2,uint8_t color);

#define lcd_cs_clr() {GPIO_ResetBits(MKL_PORTD,7);}
#define lcd_cs_set() {GPIO_SetBits(MKL_PORTD,7);}
#define lcd_reset_clr() {GPIO_ResetBits(MKL_PORTA,3);}
#define lcd_reset_set() {GPIO_SetBits(MKL_PORTA,3);}
#define lcd_rs_clr() {GPIO_ResetBits(MKL_PORTD,4);}
#define lcd_rs_set() {GPIO_SetBits(MKL_PORTD,4);}

#define lcd_sclk_clr() {GPIO_ResetBits(MKL_PORTD,5);}
#define lcd_sclk_set() {GPIO_SetBits(MKL_PORTD,5);}
#define lcd_data_clr() {GPIO_ResetBits(MKL_PORTD,6);}
#define lcd_data_set() {GPIO_SetBits(MKL_PORTD,6);}
	
//#define lcd_bg_clr() {GPIO_ResetBits(MKL_PORTD,GPIO_Pin_9);}
//#define lcd_bg_set() {GPIO_SetBits(MKL_PORTD,GPIO_Pin_9);}

#endif