/***************************************************
*@		bootloader+SD卡升级转跳程序
*@		参考野火开发板的例程驱动程序
*@		
***************************************************/

#include "includes.h"

#define	 Flash_BlockSize	 2048

FATFS fs;													/* FatFs文件系统对象 */
FIL fnew;													/* 文件对象 */
FRESULT res_flash;                /* 文件操作结果 */
UINT fnum;            					  /* 文件成功读写数量 */
BYTE ReadBuffer[2000]={0};        /* 读缓冲区 */
BYTE WriteBuffer[] =              /* 写缓冲区*/
"\r\n新建文件\r\n"; 
uint32_t flash_addr = FLASH_APP1_ADDR;

void Init_Drive(void)
{
	LED_R_Config();
	Key1_Init();
	USART_Config();
	TIMx_config();
}

void SD_Card_Test(void)
{
	uint32_t i;
	printf("****** 这是一个SPI FLASH 文件系统实验 ******\r\n");
  
	//在外部SPI Flash挂载文件系统，文件系统挂载时会对SPI设备初始化
	//初始化函数调用流程如下
	//f_mount()->find_volume()->disk_initialize->SPI_FLASH_Init()
	res_flash = f_mount(&fs,"0:",1);
	
/*----------------------- 格式化测试 -----------------*/  
	/* 如果没有文件系统就格式化创建创建文件系统 */
	if(res_flash == FR_NO_FILESYSTEM)
	{
		printf("》FLASH还没有文件系统，即将进行格式化...\r\n");
    /* 格式化 */
		res_flash=f_mkfs("0:",0,0);							
		
		if(res_flash == FR_OK)
		{
			printf("》FLASH已成功格式化文件系统。\r\n");
      /* 格式化后，先取消挂载 */
			res_flash = f_mount(NULL,"0:",1);			
      /* 重新挂载	*/			
			res_flash = f_mount(&fs,"0:",1);
		}
		else
		{
//			LED_RED;
			printf("《《格式化失败。》》\r\n");
			while(1);
		}
	}
  else if(res_flash!=FR_OK)
  {
    printf("！！外部Flash挂载文件系统失败。(%d)\r\n",res_flash);
    printf("！！可能原因：SPI Flash初始化不成功。\r\n");
		while(1);
  }
  else
  {
    printf("》文件系统挂载成功，可以进行读写测试\r\n");
  }
 
	
/*------------------- 文件系统测试：读测试 --------------------------*/
	printf("****** 即将进行文件读取测试... ******\r\n");
	res_flash = f_open(&fnew, "0:up.bin",FA_OPEN_EXISTING | FA_READ); 	 
	if(res_flash == FR_OK)
	{
//		LED_GREEN;
		printf("》打开文件成功。\r\n");
		res_flash = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
    if(res_flash==FR_OK)
    {
      printf("》文件读取成功,读到字节数据：%d\r\n",fnum);
//      printf("》读取得的文件数据为：\r\n%s \r\n", ReadBuffer);	
			for(i=0;i<fnum;i++)																									//打印所有升级文件的数据
			printf("0x%x  ",ReadBuffer[i]);
			printf("\n\n\n\n");
    }
    else
    {
      printf("！！文件读取失败：(%d)\n",res_flash);
    }		
	}
	else
	{
		printf("！！打开文件失败。\r\n");
	}
	/* 不再读写，关闭文件 */
	f_close(&fnew);	
  
	/* 不再使用文件系统，取消挂载文件系统 */
	f_mount(NULL,"0:",1);
  
  /* 操作完成，停机 */
}

uint8_t Erase_flash(uint8_t num_block)
{
	uint8_t i;
	uint32_t *p;
	FLASH_Unlock();																									//FLASH解锁
	for(i=0;i<num_block;i++)
	{	
		FLASH_ErasePage(FLASH_APP1_ADDR+Flash_BlockSize*i);
		p = (uint32_t *)(FLASH_APP1_ADDR+Flash_BlockSize*i);
		if(*p != 0xffffffff)																					//查询是否擦除成功  FLASH擦除成功后全为1
		{
			return 0;																										//一次失败，直接跳出返回擦除失败
		}
	}
	FLASH_Lock();																										//FALSH上锁
	
	return 1;
}

uint8_t Write_flash(uint32_t num_size,uint8_t *Data)
{
	uint32_t i;
	uint16_t p;
	uint16_t Data_SD;
	uint16_t Data_buff;
	FLASH_Unlock();
	for(i=0;i<num_size/2;i++)
	{
		Data_SD = *Data;
		Data++;
		Data_buff = *Data;
		Data_SD |= Data_buff<<8;
		FLASH_ProgramHalfWord(flash_addr, Data_SD);					//FLSAH写入按我测试最低只能16位写入
	
		p = *(uint32_t *)(flash_addr);
		if(p != Data_SD)
		{
			return 0;
		}	
		Data++;
		flash_addr+=2;
	}
	FLASH_Lock();
	return 1;
}
	

void delay(unsigned int time)
{
	unsigned int i;
	while(time--)
	{for(i=0;i<0xffff;i++);}
}

int main(void)
{
//	uint32_t i;
//	uint8_t p;
	uint8_t Erase_flag = 0;
	uint8_t Write_flag = 0;
	Init_Drive();
	SD_Card_Test();			//此程序执行完不能马上转跳主程序，否则会出错
	ON_LED_Red;
	delay(100);
	OFF_LED;
	delay(100);

/*这里是查询（FLASH_APP1_ADDR）转跳主程序地址起点开始的1300个字
	节 因为升级程序只有1200多个字节，所以1300次循环足够查询主任务区的代码
	*/	
//	printf("\n\n\n" );
	
//	for(i=0;i<1300;i++)
//	{
//		p = *(uint32_t *)(flash_addr);
//		printf("0x%x  " , p);
//		flash_addr++;
//	}
/************************************************************************/


	printf("开始擦除内部FLASH...\n");
	Erase_flag = Erase_flash(10);
	if(Erase_flag)
	{
		printf("开始升级程序...\n");
		Write_flag = Write_flash(fnum,ReadBuffer);
		if(Write_flag)
		{
			printf("升级成功！\n");
		}
		else
		{
			printf("升级失败！\n");
		}
	}
	else
	{
		printf("擦除扇区失败！\n");
	}
	while(1)
	{
		
	}
}

