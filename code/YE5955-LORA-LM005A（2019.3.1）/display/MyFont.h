#ifndef MYFONTH
#define MYFONTH
extern const unsigned char FontTable[];
extern const unsigned char Asc12x24FontData[];
extern const unsigned char Asc16x32FontData[];
extern const unsigned char Asc8x16FontData[];
extern const unsigned char Asc8x12FontData[];
extern const unsigned char Asc6x12FontData[];
extern const unsigned char Asc6x8FontData[];

extern const struct Font ASC16X32FONT;
extern const struct Font ASC12X24FONT;
extern const struct Font ASC8X16FONT;
extern const struct Font ASC8X12FONT;
extern const struct Font ASC6X12FONT;
extern const struct Font ASC6X8FONT;

struct Font
{
	const unsigned char* FontData;
	uint8_t ArrayWidth;	
	uint8_t Width;
	uint8_t Height;
};
//  专用字符，用于反显下一个字符
#define  INVCOLOR 0x1f
#endif
