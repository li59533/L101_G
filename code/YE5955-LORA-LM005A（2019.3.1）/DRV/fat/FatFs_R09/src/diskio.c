/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               diskio.c
** Descriptions:            The FATFS Diskio
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-4
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "diskio.h"
#include "SD_Drv.h"
  //#include "sdio_sd.h"
//extern char SD_Card_Init(void);
 //extern void wait_for_program_finish(void) ;
/* Private variables ---------------------------------------------------------*/
// extern SD_CardInfo SDCardInfo;
//extern  struct _mci_card_struct sdcardinfo;

//显示扇区内容
//extern void DisplaySector(unsigned long ID);

void dly(unsigned long j)
{
	while(j--);
	
}
char SD_Init(void)
{
	
	if(0== SD_Card_Init()) return 0;
	else return 1;
  	
}


DSTATUS disk_initialize (
	BYTE drv				/* Physical drive nmuber (0..) */
)
{	
	
  char Status;
	
// //   switch (drv) 
// //   {
// // 	 case 0 :	 
// // 		 
// // 	 
    // Status = SD_Init();
// //     if(Status == 0)
// // 	  return 0;
// //      else 
// //  	  return STA_NOINIT;

// // 	 case 1 :	  
// // 		return STA_NOINIT;
// //  		  
// // 	 case 2 :
// //  		return STA_NOINIT;
// //    }
// // 	
// // 	
// // 	return STA_NOINIT;
  //Status=sdmmc_get_state();
// return  0;//Status;
if(0!=(SD_Init())) return STA_NOINIT;
	
return  0;
}



/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */

DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0..) */
)
{
	
	
     switch (drv) 
	{
	  case 0 :		
	  /* translate the reslut code here	*/
		 //if(0== sdmmc_get_state())
		
	    return 0;
// 	  case 1 :
// 	  /* translate the reslut code here	*/
// 	    return 0;
// 	  case 2 :
// 	  /* translate the reslut code here	*/
// 	    return 0;
	  default:
        break;
	}
	return STA_NOINIT;
//	return 0;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */

DRESULT disk_read (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..255) */
)

{
 unsigned	long i;
	//HZ_PutString(500, 580 , " read sector ...",0xffff,0);
	
	
	for(i=sector;i<sector+count;i++)	//显示扇区内容
	DisplaySector(i);
	

	if(drv==0)
	{	
	if(count>0) 
	{
			
		if(0==sdmmc_read_blocks((void *) buff,sector,sector+count-1)) return RES_ERROR ;
	 return RES_OK ;
	}
	
 }
 else return RES_ERROR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */

#if _READONLY == 0

  DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	        /* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..255) */
)
{
	
	
	if(count>0) 
   
	{
  if(0==sdmmc_write_blocks((void*) buff,sector,sector+count-1))  return RES_ERROR ;
			 wait_for_program_finish();			 
			 return   RES_OK;			 
		  
		 }
	

 return RES_ERROR;
}
#endif /* _READONLY */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */


//格式化用
DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{ 
	
	
    if (drv)
     {    
         return RES_PARERR;  /* 仅支持单磁盘操作，否则返回参数错误 */
     }
 	switch (ctrl) 
	{
	  case CTRL_SYNC :	
		    
		return RES_OK;
	  case GET_SECTOR_COUNT : 
	        *(DWORD*)buff =0x440a0dff;
   //		SDCardInfo.CardCapacity/SDCardInfo.CardBlockSize;
	    return RES_OK;
	  case GET_BLOCK_SIZE :		           
	             *(WORD*)buff = 512;//SDCardInfo.CardBlockSize;
	    return RES_OK;	
	  case CTRL_POWER :
		break;
	  case CTRL_LOCK :
		break;
	  case CTRL_EJECT :
		break;
      /* MMC/SDC command */
	  case MMC_GET_TYPE :
		break;
	  case MMC_GET_CSD :
		break;
	  case MMC_GET_CID :
		break;
	  case MMC_GET_OCR :
		break;
	  case MMC_GET_SDSTAT :
		break;	
	}
	return RES_PARERR;   
}

/* 得到文件Calendar格式的建立日期,是DWORD get_fattime (void) 逆变换 */							
/*-----------------------------------------------------------------------*/
/* User defined function to give a current time to fatfs module          */
/* 31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31) */                                                                                                                                                                                                                                          
/* 15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) */                                                                                                                                                                                                                                                
DWORD get_fattime (void)
{
	/*
	 bit31:25 ? (0~127)(?1980 ??)
bit24:21 ? (1~12)
bit20:16 ? (1~31)
bit15:11 ??(0~23)
bit10:5 ??(0~59)
bit4:0 ? (0~29) 

	
	*/
   unsigned long tm;
	tm=((2013-1980)<<25)|(06<<21)|(27<<16)|(10<<11)|(30<<5)|(20);
	


}



/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
