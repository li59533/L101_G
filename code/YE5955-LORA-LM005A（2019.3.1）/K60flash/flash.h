/**
  ******************************************************************************
  * @file    flash.h
  * @author  YANDLD
  * @version V3.0.0
  * @date    2015.8.28
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  ******************************************************************************
  */
#ifndef __CH_LIB_IFLASH_H__
#define __CH_LIB_IFLASH_H__

#include <stdint.h>
#include "includes.h"

/* function return type */
#define FLASH_OK                    0x00
#define FLASH_OVERFLOW              0x01
#define FLASH_BUSY                  0x02
#define FLASH_ERROR                 0x04
#define FLASH_TIMEOUT               0x08
#define FLASH_NOT_ERASED            0x10
#define FLASH_CONTENTERR            0x11

//!< API functions
void FLASH_Init(void);
void Configinit(void);
void saveConfig(void);
uint32_t FLASH_GetSectorSize(void);
uint8_t WRBYTES(uint32_t addr, uint32_t len, const uint8_t *buf);
void RDBYTES(uint32_t addr, uint32_t len, const uint8_t *buf);
uint8_t FLASH_EraseSector(uint32_t addr);
uint32_t FLASH_Test(uint32_t startAddr, uint32_t len);

#endif

