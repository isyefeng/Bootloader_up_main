/***************************************************
*@		bootloader+SD������ת������
*@		�ο�Ұ�𿪷����������������
*@		
***************************************************/

#include "includes.h"

#define	 Flash_BlockSize	 2048

FATFS fs;													/* FatFs�ļ�ϵͳ���� */
FIL fnew;													/* �ļ����� */
FRESULT res_flash;                /* �ļ�������� */
UINT fnum;            					  /* �ļ��ɹ���д���� */
BYTE ReadBuffer[2000]={0};        /* �������� */
BYTE WriteBuffer[] =              /* д������*/
"\r\n�½��ļ�\r\n"; 
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
	printf("****** ����һ��SPI FLASH �ļ�ϵͳʵ�� ******\r\n");
  
	//���ⲿSPI Flash�����ļ�ϵͳ���ļ�ϵͳ����ʱ���SPI�豸��ʼ��
	//��ʼ������������������
	//f_mount()->find_volume()->disk_initialize->SPI_FLASH_Init()
	res_flash = f_mount(&fs,"0:",1);
	
/*----------------------- ��ʽ������ -----------------*/  
	/* ���û���ļ�ϵͳ�͸�ʽ�����������ļ�ϵͳ */
	if(res_flash == FR_NO_FILESYSTEM)
	{
		printf("��FLASH��û���ļ�ϵͳ���������и�ʽ��...\r\n");
    /* ��ʽ�� */
		res_flash=f_mkfs("0:",0,0);							
		
		if(res_flash == FR_OK)
		{
			printf("��FLASH�ѳɹ���ʽ���ļ�ϵͳ��\r\n");
      /* ��ʽ������ȡ������ */
			res_flash = f_mount(NULL,"0:",1);			
      /* ���¹���	*/			
			res_flash = f_mount(&fs,"0:",1);
		}
		else
		{
//			LED_RED;
			printf("������ʽ��ʧ�ܡ�����\r\n");
			while(1);
		}
	}
  else if(res_flash!=FR_OK)
  {
    printf("�����ⲿFlash�����ļ�ϵͳʧ�ܡ�(%d)\r\n",res_flash);
    printf("��������ԭ��SPI Flash��ʼ�����ɹ���\r\n");
		while(1);
  }
  else
  {
    printf("���ļ�ϵͳ���سɹ������Խ��ж�д����\r\n");
  }
 
	
/*------------------- �ļ�ϵͳ���ԣ������� --------------------------*/
	printf("****** ���������ļ���ȡ����... ******\r\n");
	res_flash = f_open(&fnew, "0:up.bin",FA_OPEN_EXISTING | FA_READ); 	 
	if(res_flash == FR_OK)
	{
//		LED_GREEN;
		printf("�����ļ��ɹ���\r\n");
		res_flash = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
    if(res_flash==FR_OK)
    {
      printf("���ļ���ȡ�ɹ�,�����ֽ����ݣ�%d\r\n",fnum);
//      printf("����ȡ�õ��ļ�����Ϊ��\r\n%s \r\n", ReadBuffer);	
			for(i=0;i<fnum;i++)																									//��ӡ���������ļ�������
			printf("0x%x  ",ReadBuffer[i]);
			printf("\n\n\n\n");
    }
    else
    {
      printf("�����ļ���ȡʧ�ܣ�(%d)\n",res_flash);
    }		
	}
	else
	{
		printf("�������ļ�ʧ�ܡ�\r\n");
	}
	/* ���ٶ�д���ر��ļ� */
	f_close(&fnew);	
  
	/* ����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ */
	f_mount(NULL,"0:",1);
  
  /* ������ɣ�ͣ�� */
}

uint8_t Erase_flash(uint8_t num_block)
{
	uint8_t i;
	uint32_t *p;
	FLASH_Unlock();																									//FLASH����
	for(i=0;i<num_block;i++)
	{	
		FLASH_ErasePage(FLASH_APP1_ADDR+Flash_BlockSize*i);
		p = (uint32_t *)(FLASH_APP1_ADDR+Flash_BlockSize*i);
		if(*p != 0xffffffff)																					//��ѯ�Ƿ�����ɹ�  FLASH�����ɹ���ȫΪ1
		{
			return 0;																										//һ��ʧ�ܣ�ֱ���������ز���ʧ��
		}
	}
	FLASH_Lock();																										//FALSH����
	
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
		FLASH_ProgramHalfWord(flash_addr, Data_SD);					//FLSAHд�밴�Ҳ������ֻ��16λд��
	
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
	SD_Card_Test();			//�˳���ִ���겻������ת�������򣬷�������
	ON_LED_Red;
	delay(100);
	OFF_LED;
	delay(100);

/*�����ǲ�ѯ��FLASH_APP1_ADDR��ת���������ַ��㿪ʼ��1300����
	�� ��Ϊ��������ֻ��1200����ֽڣ�����1300��ѭ���㹻��ѯ���������Ĵ���
	*/	
//	printf("\n\n\n" );
	
//	for(i=0;i<1300;i++)
//	{
//		p = *(uint32_t *)(flash_addr);
//		printf("0x%x  " , p);
//		flash_addr++;
//	}
/************************************************************************/


	printf("��ʼ�����ڲ�FLASH...\n");
	Erase_flag = Erase_flash(10);
	if(Erase_flag)
	{
		printf("��ʼ��������...\n");
		Write_flag = Write_flash(fnum,ReadBuffer);
		if(Write_flag)
		{
			printf("�����ɹ���\n");
		}
		else
		{
			printf("����ʧ�ܣ�\n");
		}
	}
	else
	{
		printf("��������ʧ�ܣ�\n");
	}
	while(1)
	{
		
	}
}

