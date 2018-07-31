#ifndef __ADC_H_
#define __ADC_H_
#include "sys.h"
#include "delay.h"
//#include "stm32f7xx_hal.h"
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "common.h"
#include "device.h"
#include "string.h"
typedef unsigned short  ADC_Data_TypeDef;
extern Device_node device_adc_pc4;
extern ADC_Data_TypeDef ADC_PC4;

//extern ADC_HandleTypeDef ADC1_handler;

void create_adc(void);
short adc_pa5_update(void *self);




//使用ADC1，采集IO口不能为复用
#define ADC_APBxClock_Cmd             RCC_APB2PeriphClockCmd
#define ADC_CLK						  RCC_APB2Periph_ADC1
#define ADC_GPIO_Clock_Cmd		      RCC_APB2PeriphClockCmd
#define ADC_GPIO_CLK				  RCC_APB2Periph_GPIOC
#define ADC_PORT					  GPIOC
//ADC1 GPIOC   时钟函数 时钟宏

//转换通道  个数 引脚  channel
#define NUMBER_OF_CHANNEL			6

#define ADC_PIN1					GPIO_Pin_0
#define ADC_CHANNEL1                ADC_Channel_10

#define ADC_PIN2					GPIO_Pin_1
#define ADC_CHANNEL2                ADC_Channel_11

#define ADC_PIN3					GPIO_Pin_2
#define ADC_CHANNEL3                ADC_Channel_12

#define ADC_PIN4					GPIO_Pin_3
#define ADC_CHANNEL4                ADC_Channel_13

#define ADC_PIN5					GPIO_Pin_4
#define ADC_CHANNEL5                ADC_Channel_14

#define ADC_PIN6					GPIO_Pin_5
#define ADC_CHANNEL6                ADC_Channel_15

#define    ADC_PIN					((ADC_PIN1)|(ADC_PIN2)|(ADC_PIN3)|(ADC_PIN4)|(ADC_PIN5)|(ADC_PIN6))

//ADC1对应DMA1通道1		//DMA宏
#define	ADC_x						ADC1
#define ADC_DMA_CHANNEL				DMA1_Channel1
#define ADC_DMA_CLK                 RCC_AHBPeriph_DMA1

//ADC采集到的数据
static __IO uint16_t ADC_ConvertedValue[NUMBER_OF_CHANNEL]={0};
// 局部变量，用于保存转换计算后的电压值 	 
static float ADC_ConvertedValueLocal[NUMBER_OF_CHANNEL];

uint16_t get_e_humi(void);
void create_adc(void);
short adc_pc4_update(void *self);
//void adc1_Init(void);
//u16 Get_Adc(u32 ch);  
//u16 Get_Adc_Average(u32 ch,u8 times);
//uint16_t get_e_humi(void);
#endif /* __ADC_H_ */
