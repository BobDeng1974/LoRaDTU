#include "bsp_i2c.h"

void I2C_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	I2C_GPIO_CLK_CMD(I2C_GPIO_CLK,ENABLE);
	
	//SCL&SDA开漏输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C_SCL_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;
	GPIO_Init(I2C_SDA_PORT,&GPIO_InitStructure);	
}

//void I2C_GPIO_Config(void){
//	GPIO_InitTypeDef GPIO_Initure;
//    
//    I2C_CLK_FUNC();   //使能GPIOH时钟
//    
//    //PH4,5初始化设置
//    GPIO_Initure.Pin=I2C_SCL_PIN|I2C_SDA_PIN;
//    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
//    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
//    GPIO_Initure.Speed=GPIO_SPEED_FAST;     //快速
//    HAL_GPIO_Init(I2C_SCL_PORT,&GPIO_Initure);
//    
//    IIC_SDA(1);
//    IIC_SCL(1); 
//}
