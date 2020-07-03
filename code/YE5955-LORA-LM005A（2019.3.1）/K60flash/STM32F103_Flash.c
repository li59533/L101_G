#include "conf.h"
#include "STM32F103_Flash.h"
#include "app.h"

void initEEPROM()
{
	loadConfig();

}
void EarsePage(uint32_t address)  //  
{
		FLASH_Unlock();
	  FLASH_ErasePage(address);
		FLASH_Lock();
}


void RDBYTES(uint32_t address,uint16_t num, uint8_t *Data)
{
	uint16_t count;
	for(count=0;count<num/2;count++)
	{
		*((uint16_t *)Data+count)=*((uint16_t *)address+count);
  }
}

void WRBYTES(uint32_t address,uint16_t num, unsigned char *Data)
{
	uint16_t count;
	FLASH_Unlock();
//	FLASH_ErasePage(address);
	for(count=0;count<(num/2+1);count++)
	{
		FLASH_ProgramHalfWord((address+2*count), *((uint16_t*)(Data+2*count)));
  }
	FLASH_Lock();
}		

#define  FlashDataAddress   0x0803fc00   //往片内FLASH写数据的地址
#define  FlashParameterAddress 0x0803f000  //往片内FLASH写参数的地址
void earseParameter(void)
{
		EarsePage(FlashParameterAddress);		
}
void saveParameter(void)
{
	EarsePage(FlashParameterAddress);
	WRBYTES(FlashParameterAddress,sizeof(struct PARAMETER),(uint8_t*)&Parameter);
}

void savePower(void)
{
	WRBYTES(FlashParameterAddress,sizeof(struct PARAMETER),(uint8_t*)&Parameter);
}
void loadParameter(void)
{
	RDBYTES(FlashParameterAddress,sizeof(Parameter.vaildsign),(unsigned char*)&Parameter);
	if(Parameter.vaildsign!=0x55AA)
	{	
		Parameter.vaildsign=0x55AA;
		return;
	}
	RDBYTES(FlashParameterAddress,sizeof(struct PARAMETER),((unsigned char*)&Parameter));

}
void saveConfig()
{
	EarsePage(FlashDataAddress);
	WRBYTES(FlashDataAddress,sizeof(struct CONFIG),(uint8_t*)&config);
//	WRBYTES(FlashDataAddress+0xA0,sizeof(struct ADJUSTDATA),(uint8_t*)&AdjustData);
	
}
void loadConfig()
{
	RDBYTES(FlashDataAddress,sizeof(config.vaildsign),(unsigned char*)&config);
	if(config.vaildsign!=0x45AA)
	{	
		config.vaildsign=0x45AA;
		saveConfig();
		saveParameter();
		return;
	}
	RDBYTES(FlashDataAddress,sizeof(struct CONFIG)+1,((unsigned char*)&config));
//	loadParameter();
}




