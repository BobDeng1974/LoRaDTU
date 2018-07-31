#ifndef __SOFT_I2C_H__
#define __SOFT_I2C_H__

//#include "stm32f7xx_hal.h"
#include "stm32f10x.h"
#include "bsp_i2c.h"
#include "sys.h"
#include "delay.h"
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Ack(void);
void I2C_NAck(void);
unsigned char I2C_WaitAck(void);
void I2C_SendByte(unsigned char sendbyte);
unsigned char I2C_ReadByte(void);
unsigned char I2C_CheckDevice(unsigned char address);

#endif /* __SOFT_I2C_H__ */
