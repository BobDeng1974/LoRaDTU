#ifndef  LORAUSART_H
#define  LORAUSART_H

#include "stm32f10x.h"

// 串口2-USART2
#define  LORA_USARTx                   USART2
#define  LORA_USART_CLK                RCC_APB1Periph_USART2
#define  LORA_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  LORA_USART_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  LORA_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  LORA_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  LORA_USART_TX_GPIO_PORT       GPIOA   
#define  LORA_USART_TX_GPIO_PIN        GPIO_Pin_2
#define  LORA_USART_RX_GPIO_PORT       GPIOA
#define  LORA_USART_RX_GPIO_PIN        GPIO_Pin_3

#define  LORA_USART_IRQ                USART2_IRQn
#define  LORA_USART_IRQHandler         USART2_IRQHandler

#define  LORA_DMA                       DMA1
#define  LORA_DMA_RX_CHANNEL            DMA1_Channel6
#define  LORA_DMA_TX_CHANNEL            DMA1_Channel7

#define  LORA_DMA_TX_IRQHandler         DMA1_Channel7_IRQHandler
#define  LORA_DMA_RX_IRQHandler         DMA1_Channel6_IRQHandler
#define  LORA_DMA_CLK                   RCC_AHBPeriph_DMA1
#define  LORA_DMA_APBxClkCmd            RCC_AHBPeriphClockCmd


#define  LORA_DMA_RX_IRQ                DMA1_Channel6_IRQn
#define  LORA_DMA_TX_IRQ                DMA1_Channel7_IRQn

#define  LORA_DMA_RX_TC                 DMA1_IT_TC6
#define  LORA_DMA_TX_TC                 DMA1_IT_TC7


#define  LoRaRxBufferSize 0x100

extern uint8_t  LoRaRxBuffer1[ LoRaRxBufferSize ];
extern uint8_t  LoRaRxBuffer2[ LoRaRxBufferSize ];
extern uint8_t  LoRaRXEvent ;
extern uint8_t  using_LoRaRxBuffer1 ;
extern uint8_t  receiveLength;


#endif /* LORAUSART_H */
