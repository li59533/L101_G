#ifndef _STM32F103FLASHH
#define _STM32F103FLASHH
#include "conf.h"


extern void WRABYTE(uint16_t addr,uint8_t dat);
extern uint8_t RDABYTE(uint16_t addr);
extern void initEEPROM(void);
extern void saveConfig(void);
extern void loadConfig(void);

extern void saveParameter(void);
extern void loadParameter(void);
#endif
