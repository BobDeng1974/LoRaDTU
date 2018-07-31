#include "soft_i2c.h"

static void I2C_Delay(void)
{
	unsigned char i;

	/*　
	 	下面的时间是通过逻辑分析仪测试得到的。
    工作条件：CPU主频72MHz ，MDK编译环境，1级优化
  
		循环次数为10时，SCL频率 = 205KHz 
		循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us 
	 	循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us 
	*/
	for (i = 0; i < 10; i++);
}



//void I2C_Stop(void)
//{
////	/* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
//	I2C_SCL_H;
//	I2C_SDA_L;
//	I2C_Delay();
//	I2C_SDA_H;
////	SDA_OUT();//sda线输出
////	IIC_SCL(0);
////	IIC_SDA(0);//STOP:when CLK is high DATA change form low to high
// 	delay_us(4);
//	IIC_SCL(1); 
//	IIC_SDA(1);//发送I2C总线结束信号
//	delay_us(4);
//}

//void I2C_Start(void)
//{
////	/* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
////	I2C_SCL_H;
////	I2C_SDA_H;
////	I2C_Delay();
////	I2C_SDA_L;
////	I2C_Delay();
////	I2C_SCL_L;
////	I2C_Delay();
//	SDA_OUT();     //sda线输出
//	IIC_SDA(1);	  	  
//	IIC_SCL(1);
//	delay_us(4);
// 	IIC_SDA(0);//START:when CLK is high,DATA change form high to low 
//	delay_us(4);
//	IIC_SCL(0);//钳住I2C总线，准备发送或接收数据 
//}

//void I2C_Ack(void)
//{
////	I2C_SDA_L;
////	I2C_Delay();
////	I2C_SCL_H;
////	I2C_Delay();
////	I2C_SCL_L;
////	I2C_Delay();
////	I2C_SDA_H;
//	IIC_SCL(0);
//	SDA_OUT();
//	IIC_SDA(0);
//	delay_us(2);
//	IIC_SCL(1);
//	delay_us(2);
//	IIC_SCL(0);
//}

//void I2C_NAck(void)
//{
////	I2C_SDA_H;
////	I2C_Delay();
////	I2C_SCL_H;
////	I2C_Delay();
////	I2C_SCL_L;
////	I2C_Delay();
//	IIC_SCL(0);
//	SDA_OUT();
//	IIC_SDA(1);
//	delay_us(2);
//	IIC_SCL(1);
//	delay_us(2);
//	IIC_SCL(0);
//}

//unsigned char I2C_WaitAck(void)
//{
////	unsigned char re;
////	I2C_SDA_H;
////	I2C_Delay();
////	I2C_SCL_H;
////	I2C_Delay();
////	if( I2C_SDA_READ )
////	{
////		re=1;
////	}
////	else
////	{
////		re=0;
////	}
////	I2C_SCL_L;
////	I2C_Delay();
////	return re;
//	u8 ucErrTime=0;
//	SDA_IN();      //SDA设置为输入  
//	IIC_SDA(1);delay_us(1);	   
//	IIC_SCL(1);delay_us(1);	 
//	while(READ_SDA)
//	{
//		ucErrTime++;
//		if(ucErrTime>250)
//		{
//			I2C_Stop();
//			return 1;
//		}
//	}
//	IIC_SCL(0);//时钟输出0 	   
//	return 0;
//}

//unsigned char I2C_ReadByte(void)
//{
////	unsigned char i;
////	unsigned char value=0;
////	//高位先行
////	for(i=0;i<8;i++)
////	{
////		value <<=1;
////		I2C_SCL_H;
////		I2C_Delay();
////		if(I2C_SDA_READ )
////		{
////			value++;
////		}
////		I2C_SCL_L;
////		I2C_Delay();
////	}
////	return value;
//	unsigned char i,receive=0;
//	SDA_IN();//SDA设置为输入
//    for(i=0;i<8;i++ )
//	{
//        IIC_SCL(0); 
//        delay_us(2);
//		IIC_SCL(1);
//        receive<<=1;
//        if(READ_SDA)receive++;   
//		delay_us(1); 
//    }					   
//    return receive;
//}

//void I2C_SendByte(unsigned char sendbyte)
//{
////	unsigned char i;
////	for(i=0;i<8;i++)
////	{
////		if(sendbyte & 0x80)
////		{
////			I2C_SDA_H;
////		}
////		else
////		{
////			I2C_SDA_L;
////		}
////		I2C_Delay();
////		I2C_SCL_H;
////		I2C_Delay();
////		I2C_SCL_L;
////		if(7==i)
////		{
////			I2C_SDA_H;
////		}
////		sendbyte<<=1;
////		I2C_Delay();
////	}
//    u8 t;   
//	SDA_OUT(); 	    
//    IIC_SCL(0);//拉低时钟开始数据传输
//    for(t=0;t<8;t++)
//    {              
//        IIC_SDA((sendbyte&0x80)>>7);
//        sendbyte<<=1; 	  
//		delay_us(2);   //对TEA5767这三个延时都是必须的
//		IIC_SCL(1);
//		delay_us(2); 
//		IIC_SCL(0);	
//		delay_us(2);
//    }
//}


void I2C_Start(void){
/* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
    I2C_SDA_H;
    I2C_SCL_H;
    I2C_Delay();
    I2C_SDA_L;
    I2C_Delay();
    I2C_SCL_L;
    I2C_Delay();
}

void I2C_Stop(void){
    /* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
    I2C_SDA_L;
    I2C_SCL_H;
    I2C_Delay();
    I2C_SDA_H;
}

void I2C_Ack(void){
    I2C_SDA_L;
    I2C_Delay();
    I2C_SCL_H;
    I2C_Delay();
    I2C_SCL_L;
    I2C_Delay();
    I2C_SDA_H;
}

void I2C_NAck(void){
    I2C_SDA_H;
    I2C_Delay();
    I2C_SCL_H;
    I2C_Delay();
    I2C_SCL_L;
    I2C_Delay();
}

unsigned char I2C_WaitAck(void){
    uint8_t re;

	I2C_SDA_H;	/* CPU释放SDA总线 */
	I2C_Delay();
	I2C_SCL_H;	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	I2C_Delay();
	if ((I2C_SDA_PORT->IDR&I2C_SDA_PIN) != 0)	/* CPU读取SDA口线状态 */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	I2C_SCL_L;
	I2C_Delay();
	return re;
}


void I2C_SendByte(unsigned char sendbyte){
    uint8_t i;

	/* 先发送字节的高位bit7 */
	for (i = 0; i < 8; i++)
	{		
		if (sendbyte & 0x80)
		{
			I2C_SDA_H;
		}
		else
		{
			I2C_SDA_L;
		}
		I2C_Delay();
		I2C_SCL_H;
		I2C_Delay();	
		I2C_SCL_L;
		if (i == 7)
		{
			 I2C_SDA_H; // 释放总线
		}
		sendbyte <<= 1;	/* 左移一个bit */
		I2C_Delay();
	}
}


unsigned char I2C_ReadByte(void){
	uint8_t i;
	uint8_t value;

	/* 读到第1个bit为数据的bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		I2C_SCL_H;
		I2C_Delay();
		if ((I2C_SDA_PORT->IDR&I2C_SDA_PIN) != 0)
		{
			value++;
		}
		I2C_SCL_L;
		I2C_Delay();
	}
	return value;
}


/* 返回0表示正常 1为错误 */
unsigned char I2C_CheckDevice(unsigned char address)
{
	unsigned char ack;
	I2C_GPIO_Config();
	I2C_Start();
	I2C_SendByte(address|0);
	ack = I2C_WaitAck();
	I2C_Stop();
	return ack;
}



