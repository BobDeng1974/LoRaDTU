/**
 *	@作者：杨思源
 *	@日期：2017年12月22日22:49:09
 *	@功能：读取 bh1750 获得光照度值
 *	@版本：1.0
 *	@修改：no
 *	@接口：BH1750_Data_TypeDef BH1750;			光强大小
 *	@接口：Device_node device_bh1750;			设备句柄
 *	@接口：void create_bh1750(void);				初始化函数，初始化bh1750并初始化设备句柄	
 *	@接口：short BH1750_GetData(void* self);		可以无视，直接调用设备对象的update函数指针获取更新
 *
 *	@注意事项：BH1750 是使用IIC协议传输数据的设备，
 *	软件IIC接口由 bsp_i2c.c 初始化IO口决定
 *	IIC函数族则由 soft_i2c.c 实现
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


