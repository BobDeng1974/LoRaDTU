#ifndef __BSP_BH1750_H_
#define __BSP_BH1750_H_
//#include "stm32f7xx_hal.h"
#include "stm32f10x.h"
#include "bsp_i2c.h"
#include "soft_i2c.h"
//#include "debug_usart1.h"
#include "FreeRTOS.h"
#include "task.h"
#include "common.h"
#include "device.h"
#define ADDRESS 		(0x46)
#define BH1750_MODE (0x20)
typedef unsigned int BH1750_Data_TypeDef;


extern BH1750_Data_TypeDef BH1750;
extern Device_node device_bh1750;



void create_bh1750(void);
short BH1750_GetData(void* self);
#endif /* __BSP_BH1750_H_ */
