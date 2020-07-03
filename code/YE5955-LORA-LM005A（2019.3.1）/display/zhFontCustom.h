#ifndef ZHFONTCUSTOMH
#define ZHFONTCUSTOMH



#include "MyFont.h"


typedef struct typFNT_GB12		// 汉字字模数据结构
{
	uint8_t Index[2];
	uint8_t Msk[24];
}FNT_GB12;

typedef struct typFNT_GB32		// 汉字字模数据结构
{
	uint8_t Index[2];
	uint8_t Msk[128];
}FNT_GB32;

typedef struct typFNT_GB24		// 汉字字模数据结构
{
	uint8_t Index[2];
	uint8_t Msk[72];
}FNT_GB24;


extern const FNT_GB12 zh12x12FontData[];
extern const FNT_GB32 zh32x32FontData[];
extern const FNT_GB24 zh24x24FontData[];
extern const struct Font ZH12X12FONT;
extern const struct Font ZH32X32FONT;
extern const struct Font ZH24X24FONT;
#endif
