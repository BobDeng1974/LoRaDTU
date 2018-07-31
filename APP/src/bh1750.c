/**
 *	@���ߣ���˼Դ
 *	@���ڣ�2017��12��22��22:49:09
 *	@���ܣ���ȡ bh1750 ��ù��ն�ֵ
 *	@�汾��1.0
 *	@�޸ģ�no
 *	@�ӿڣ�BH1750_Data_TypeDef BH1750;			��ǿ��С
 *	@�ӿڣ�Device_node device_bh1750;			�豸���
 *	@�ӿڣ�void create_bh1750(void);				��ʼ����������ʼ��bh1750����ʼ���豸���	
 *	@�ӿڣ�short BH1750_GetData(void* self);		�������ӣ�ֱ�ӵ����豸�����update����ָ���ȡ����
 *
 *	@ע�����BH1750 ��ʹ��IICЭ�鴫�����ݵ��豸��
 *	���IIC�ӿ��� bsp_i2c.c ��ʼ��IO�ھ���
 *	IIC���������� soft_i2c.c ʵ��
 *
 *
 */





#include "bh1750.h"
BH1750_Data_TypeDef BH1750 = 0;
Device_node device_bh1750;




void create_bh1750(void){
	I2C_CheckDevice(ADDRESS);
	device(&device_bh1750,2,"BH1750",&BH1750,BH1750_GetData);
}


short BH1750_GetData(void* self)
{
	unsigned char H,L;
	Device_node *this = self;
	if(strcmp(this->name,"BH1750"))return ERROR;
	BH1750_Data_TypeDef *BH1750_Data_update = this->pvalue;
	BH1750_Data_TypeDef BH1750_Data;
//	Device_node *this = self;
//	if(strcmp(this->name,"DHT11"))return ERROR;
//	DHT11_Data_TypeDef * DHT11_Data_update = this->pvalue;
//	DHT11_Data_TypeDef DHT11_Data;
//	uint8_t return_flag = ERROR;
	
	taskENTER_CRITICAL();
	I2C_Start();
	I2C_SendByte(ADDRESS|0);
	I2C_WaitAck();
	I2C_SendByte((unsigned char)BH1750_MODE);
	I2C_WaitAck();
	I2C_Stop();
	taskEXIT_CRITICAL();
	delay_ms(120);
	taskENTER_CRITICAL();
	I2C_Start();
	I2C_SendByte(ADDRESS|1);
	I2C_WaitAck();
	H=I2C_ReadByte();
 	I2C_Ack();
	L=I2C_ReadByte();
	I2C_NAck();
	I2C_Stop();
	taskEXIT_CRITICAL();
	BH1750_Data=H;
	BH1750_Data<<=8;
	BH1750_Data|=L;
	BH1750_Data = BH1750_Data*10/12;
	if(BH1750_Data != *BH1750_Data_update){
		*BH1750_Data_update = BH1750_Data;
		this->update_event = true;
	}
	
	return SUCCESS;
//	printf("%d\n",DATA);
	//return DATA*10/12;
}


