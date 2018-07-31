#include "soft_i2c.h"

static void I2C_Delay(void)
{
	unsigned char i;

	/*��
	 	�����ʱ����ͨ���߼������ǲ��Եõ��ġ�
    ����������CPU��Ƶ72MHz ��MDK���뻷����1���Ż�
  
		ѭ������Ϊ10ʱ��SCLƵ�� = 205KHz 
		ѭ������Ϊ7ʱ��SCLƵ�� = 347KHz�� SCL�ߵ�ƽʱ��1.5us��SCL�͵�ƽʱ��2.87us 
	 	ѭ������Ϊ5ʱ��SCLƵ�� = 421KHz�� SCL�ߵ�ƽʱ��1.25us��SCL�͵�ƽʱ��2.375us 
	*/
	for (i = 0; i < 10; i++);
}



//void I2C_Stop(void)
//{
////	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
//	I2C_SCL_H;
//	I2C_SDA_L;
//	I2C_Delay();
//	I2C_SDA_H;
////	SDA_OUT();//sda�����
////	IIC_SCL(0);
////	IIC_SDA(0);//STOP:when CLK is high DATA change form low to high
// 	delay_us(4);
//	IIC_SCL(1); 
//	IIC_SDA(1);//����I2C���߽����ź�
//	delay_us(4);
//}

//void I2C_Start(void)
//{
////	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
////	I2C_SCL_H;
////	I2C_SDA_H;
////	I2C_Delay();
////	I2C_SDA_L;
////	I2C_Delay();
////	I2C_SCL_L;
////	I2C_Delay();
//	SDA_OUT();     //sda�����
//	IIC_SDA(1);	  	  
//	IIC_SCL(1);
//	delay_us(4);
// 	IIC_SDA(0);//START:when CLK is high,DATA change form high to low 
//	delay_us(4);
//	IIC_SCL(0);//ǯסI2C���ߣ�׼�����ͻ�������� 
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
//	SDA_IN();      //SDA����Ϊ����  
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
//	IIC_SCL(0);//ʱ�����0 	   
//	return 0;
//}

//unsigned char I2C_ReadByte(void)
//{
////	unsigned char i;
////	unsigned char value=0;
////	//��λ����
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
//	SDA_IN();//SDA����Ϊ����
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
//    IIC_SCL(0);//����ʱ�ӿ�ʼ���ݴ���
//    for(t=0;t<8;t++)
//    {              
//        IIC_SDA((sendbyte&0x80)>>7);
//        sendbyte<<=1; 	  
//		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
//		IIC_SCL(1);
//		delay_us(2); 
//		IIC_SCL(0);	
//		delay_us(2);
//    }
//}


void I2C_Start(void){
/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
    I2C_SDA_H;
    I2C_SCL_H;
    I2C_Delay();
    I2C_SDA_L;
    I2C_Delay();
    I2C_SCL_L;
    I2C_Delay();
}

void I2C_Stop(void){
    /* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
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

	I2C_SDA_H;	/* CPU�ͷ�SDA���� */
	I2C_Delay();
	I2C_SCL_H;	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
	I2C_Delay();
	if ((I2C_SDA_PORT->IDR&I2C_SDA_PIN) != 0)	/* CPU��ȡSDA����״̬ */
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

	/* �ȷ����ֽڵĸ�λbit7 */
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
			 I2C_SDA_H; // �ͷ�����
		}
		sendbyte <<= 1;	/* ����һ��bit */
		I2C_Delay();
	}
}


unsigned char I2C_ReadByte(void){
	uint8_t i;
	uint8_t value;

	/* ������1��bitΪ���ݵ�bit7 */
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


/* ����0��ʾ���� 1Ϊ���� */
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



