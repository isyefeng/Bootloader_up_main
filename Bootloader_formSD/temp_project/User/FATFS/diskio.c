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

extern SD_CardInfo SDCardInfo;	  //���ڴ洢������Ϣ��DSR��һ���֣�


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
//		if(SPI_FLASH_ReadID() == FLASH_ID)				//��ȡFLASHоƬID�Ƿ�����
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
////		SPI_FLASH_Init();								//��ʼ��FlashоƬ
////		SPI_Flash_WAKEUP();							//���绽�ѣ��Է�FLASHоƬ�ڴ�֮ǰ����͹���ģʽ
////		stat = disk_status(SPI_Flash);	//��ȡFLASHоƬ״̬�Ƿ�����
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
//		// translate the reslut code here   ǰ2M�ռ䴢����������   ƫ�Ƶ�2M��ĵ�ַ��ʼд������   2*1024*1024/4096=512
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
			//������������
			case GET_SECTOR_COUNT:
				/* ����������1536*4096/1024/1024=6(MB) */
				*(DWORD*)buff = SDCardInfo.CardCapacity;
				
			break;
			//����ÿ��������С
			case GET_SECTOR_SIZE:
				*(WORD*)buff = SDCardInfo.CardBlockSize;

			break;
			//���ز�����������С��������λΪ������
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
//			//������������
//			case GET_SECTOR_COUNT:
//				/* ����������1536*4096/1024/1024=6(MB) */
//				*(DWORD*)buff = 1536;
//				
//			break;
//			//����ÿ��������С
//			case GET_SECTOR_SIZE:
//				*(WORD*)buff = 4096;

//			break;
//			//���ز�����������С��������λΪ������
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

DWORD get_fattime (void)				//����ʱ��
{	
	return 0;
}
