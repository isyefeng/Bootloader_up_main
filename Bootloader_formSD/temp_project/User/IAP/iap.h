#ifndef __IAP_H__
#define __IAP_H__
//#include "sys.h"
#include "includes.h"

#define     __IO    volatile 

typedef __IO uint32_t  vu32;
typedef  unsigned int u32;

typedef  void ( *IAPfun_t )( void );				//定义一个函数类型的参数.

#define  FLASH_BlockSize				2048
#define  FLASH_APP1_ADDR		(FLASH_BASE+FLASH_BlockSize*15)  //第一个应用程序起始地址(存放在FLASH)
#define  FLASH_BOOTLOADER_ADDR	0x08000000  //第一个应用程序起始地址(存放在FLASH)

//保留0X08000000~0X08007FFF的空间为Bootloader使用(共32KB)
void IAP_load( u32 appxaddr );			//跳转到APP程序执行
//void IAP_write( u32 appxaddr, u8 *appbuf, u32 applen );	//在指定地址开始,写入bin
//void IAP_read( u32 start_addr, u8 *pbuf, u32 size );

#endif

