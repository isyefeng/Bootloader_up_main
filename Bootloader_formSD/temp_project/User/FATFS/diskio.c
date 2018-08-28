/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
//#include "bsp_spi_flash.h"
#include "includes.h"

/* Definitions of physical drive number for each drive */
//#define ATA		0	/* Example: Map ATA harddisk to physical drive 0 */
//#define MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
//#define USB		2	/* Example: Map USB MSD to physical drive 2 */

#define			SD_CADR						0
#define 		SPI_Flash					1

SD_Error SD_Status = SD_OK;

extern SD_CardInfo SDCardInfo;	  //用于存储卡的信息，DSR的一部分？


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;

	switch (pdrv) {
	case SD_CADR:
		// translate the reslut code here
		stat = RES_OK;
	
		return stat;

	case SPI_Flash :
//		// translate the reslut code here
//		if(SPI_FLASH_ReadID() == FLASH_ID)				//读取FLASH芯片ID是否正常
//		{
//			stat = RES_OK;
//		}
//		else
//		{
//			stat = STA_NOINIT;	
//		}
		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;

	switch (pdrv) {
	case SD_CADR :
		// translate the reslut code here
		if(SD_Init() == SD_OK)
			stat = RES_OK;
		else
			stat = STA_NOINIT;
	
		return stat;

//	case SPI_Flash :
////		// translate the reslut code here
////		SPI_FLASH_Init();								//初始化Flash芯片
////		SPI_Flash_WAKEUP();							//掉电唤醒，以防FLASH芯片在此之前进入低功耗模式
////		stat = disk_status(SPI_Flash);	//读取FLASH芯片状态是否正常
//		return stat;
	}
	return STA_NOINIT;
}


/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;

	switch (pdrv) {
	case SD_CADR :
		// translate the arguments here
		// translate the reslut code here
		SD_Status = SD_ReadMultiBlocks(buff, sector*SDCardInfo.CardBlockSize, SDCardInfo.CardBlockSize, count);
		SD_Status = SD_WaitReadOperation();
    while(SD_GetStatus() != SD_TRANSFER_OK);
	
		if(SD_Status == SD_OK)
			res = RES_OK;
		else
			res = RES_ERROR;
		return res;

	case SPI_Flash :
//		// translate the arguments here
//		// translate the reslut code here
//		SPI_FLASH_BufferRead(buff,sector*4096,count*4096);
//		res = RES_OK;
		return res;

	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;

	switch (pdrv) {
	case SD_CADR :
		// translate the arguments here
		// translate the reslut code here
		/* Write multiple block of many bytes on address 0 */
    SD_Status = SD_WriteMultiBlocks((uint8_t *)buff, sector*SDCardInfo.CardBlockSize, SDCardInfo.CardBlockSize, count);
    /* Check if the Transfer is finished */
    SD_Status = SD_WaitWriteOperation();
    while(SD_GetStatus() != SD_TRANSFER_OK);
	
		if(SD_Status == SD_OK)
			res = RES_OK;
		else
			res = RES_ERROR;
		return res;

	case SPI_Flash :
//		// translate the arguments here
//		// translate the reslut code here   前2M空间储存其他程序   偏移到2M后的地址开始写入数据   2*1024*1024/4096=512
//		sector += 512;
//		
//		SPI_FLASH_SectorErase(sector*4096);
//		SPI_FLASH_BufferWrite((uint8_t*)buff,sector*4096,count*4096);
//		res = RES_OK;
		
		return res;

	}

	return RES_PARERR;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;

	switch (pdrv) {
	case SD_CADR :
		// Process of the command for the ATA drive
	switch(cmd)
		{
			//返回扇区个数
			case GET_SECTOR_COUNT:
				/* 扇区数量：1536*4096/1024/1024=6(MB) */
				*(DWORD*)buff = SDCardInfo.CardCapacity;
				
			break;
			//返回每个扇区大小
			case GET_SECTOR_SIZE:
				*(WORD*)buff = SDCardInfo.CardBlockSize;

			break;
			//返回擦除扇区的最小个数（单位为扇区）
			case GET_BLOCK_SIZE:
			 *(WORD*)buff = 1;
			
			break;
		}
		res = RES_OK;
		return res;

	case SPI_Flash :

//		// Process of the command for the MMC/SD card
//		switch(cmd)
//		{
//			//返回扇区个数
//			case GET_SECTOR_COUNT:
//				/* 扇区数量：1536*4096/1024/1024=6(MB) */
//				*(DWORD*)buff = 1536;
//				
//			break;
//			//返回每个扇区大小
//			case GET_SECTOR_SIZE:
//				*(WORD*)buff = 4096;

//			break;
//			//返回擦除扇区的最小个数（单位为扇区）
//			case GET_BLOCK_SIZE:
//			 *(WORD*)buff = 1;
//			
//			break;
//		}
//		res = RES_OK;
		return res;

	}

	return RES_PARERR;
}
#endif

DWORD get_fattime (void)				//返回时间
{	
	return 0;
}
