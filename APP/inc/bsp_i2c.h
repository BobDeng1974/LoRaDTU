#ifndef __BSP_I2C_H__
#define __BSP_I2C_H__

//#include "stm32f7xx_hal.h"
#include "stm32f10x.h"
#define I2C_SCL_PORT 				GPIOA
#define I2C_SCL_PIN  				GPIO_Pin_12

#define I2C_SDA_PORT 				GPIOA
#define I2C_SDA_PIN  				GPIO_Pin_11

#define I2C_GPIO_CLK 				RCC_APB2Periph_GPIOA 	 //I2C_SCL_GPIO_CLK|I2C_SDA_GPIO_CLK
#define I2C_GPIO_CLK_CMD		RCC_APB2PeriphClockCmd

//���ŵ�ƽ����
#define I2C_SCL_H						GPIO_SetBits(I2C_SCL_PORT,I2C_SCL_PIN)     
#define I2C_SCL_L						GPIO_ResetBits(I2C_SCL_PORT,I2C_SCL_PIN) 
#define I2C_SDA_H						GPIO_SetBits(I2C_SDA_PORT,I2C_SDA_PIN)
#define I2C_SDA_L						GPIO_ResetBits(I2C_SDA_PORT,I2C_SDA_PIN)

////���ŵ�ƽ���
//#define I2C_SDA_READ				GPIO_ReadInputDataBit(I2C_SDA_PORT,I2C_SDA_PIN)  

//#define I2C_SCL_PORT 				GPIOH
//#define I2C_SCL_PIN  				GPIO_PIN_4

//#define I2C_SDA_PORT 				GPIOH
//#define I2C_SDA_PIN  				GPIO_PIN_5

//#define I2C_CLK_FUNC(); __HAL_RCC_GPIOH_CLK_ENABLE();
////IO��������
//#define SDA_IN()  {I2C_SDA_PORT->MODER&=~(3<<(5*2));I2C_SDA_PORT->MODER|=0<<5*2;}	//PH5����ģʽ
//#define SDA_OUT() {I2C_SDA_PORT->MODER&=~(3<<(5*2));I2C_SDA_PORT->MODER|=1<<5*2;} //PH5���ģʽ
////IO����
//#define IIC_SCL(n)  (n?HAL_GPIO_WritePin(I2C_SCL_PORT,I2C_SCL_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin(I2C_SCL_PORT,I2C_SCL_PIN,GPIO_PIN_RESET)) //SCL
//#define IIC_SDA(n)  (n?HAL_GPIO_WritePin(I2C_SDA_PORT,I2C_SDA_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin(I2C_SDA_PORT,I2C_SDA_PIN,GPIO_PIN_RESET)) //SDA
//#define READ_SDA    HAL_GPIO_ReadPin(I2C_SDA_PORT,I2C_SDA_PIN)  //����SDA

void I2C_GPIO_Config(void);

#endif /* __BSP_I2C_H__ */
