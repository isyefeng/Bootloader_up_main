#ifndef __IAP_H__
#define __IAP_H__
//#include "sys.h"
#include "includes.h"

#define     __IO    volatile 

typedef __IO uint32_t  vu32;
typedef  unsigned int u32;

typedef  void ( *IAPfun_t )( void );				//����һ���������͵Ĳ���.

#define  FLASH_BlockSize				2048
#define  FLASH_APP1_ADDR		(FLASH_BASE+FLASH_BlockSize*15)  //��һ��Ӧ�ó�����ʼ��ַ(�����FLASH)
#define  FLASH_BOOTLOADER_ADDR	0x08000000  //��һ��Ӧ�ó�����ʼ��ַ(�����FLASH)

//����0X08000000~0X08007FFF�Ŀռ�ΪBootloaderʹ��(��32KB)
void IAP_load( u32 appxaddr );			//��ת��APP����ִ��
//void IAP_write( u32 appxaddr, u8 *appbuf, u32 applen );	//��ָ����ַ��ʼ,д��bin
//void IAP_read( u32 start_addr, u8 *pbuf, u32 size );

#endif

