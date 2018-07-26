#include "LoRaUsart.h"
#include "String.h"
#include "stdlib.h"

//串口缓冲区以及标志位
uint8_t  LoRaRxBuffer1[ LoRaRxBufferSize ] = { 0 };
uint8_t  LoRaRxBuffer2[ LoRaRxBufferSize ] = { 0 };
uint8_t  LoRaRXEvent = 0;
uint8_t  using_LoRaRxBuffer1 = 1;
uint8_t  receiveLength = 0;
static void NVIC_Config(void);

/**
 *    配置 LoRa 的串口
 *    1.配置串口引脚
 *    2.配置串口DMA
 *    3.配置向量表初始化
 */
void LoRaUsartConfig(int baudRate){
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    
    //DMA DMA_InitStructure;
    // 打开串口GPIO的时钟
	LORA_USART_GPIO_APBxClkCmd(LORA_USART_GPIO_CLK, ENABLE);
	
	// 打开串口外设的时钟
	LORA_USART_APBxClkCmd(LORA_USART_CLK, ENABLE);

	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = LORA_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LORA_USART_TX_GPIO_PORT, &GPIO_InitStructure);

    // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = LORA_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(LORA_USART_TX_GPIO_PORT, &GPIO_InitStructure);
    
    // 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = baudRate;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(LORA_USARTx, &USART_InitStructure);
    USART_ITConfig(LORA_USARTx,USART_IT_IDLE,ENABLE);
    
    
    //设置劳拉的DMA接收
    DMA_Cmd(LORA_DMA_RX_CHANNEL,DISABLE);
    LORA_DMA_APBxClkCmd(LORA_DMA_CLK,ENABLE);
    
    DMA_InitStructure.DMA_BufferSize = LoRaRxBufferSize;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)LoRaRxBuffer1;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&LORA_USARTx->DR);
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    
    DMA_Init(LORA_DMA_RX_CHANNEL,&DMA_InitStructure);
    NVIC_Config();
    DMA_Cmd(LORA_DMA_RX_CHANNEL,ENABLE);//打开接收DMA
}


static void NVIC_Config(void){
    NVIC_InitTypeDef NVIC_InitStructure;

    //usart idle
    NVIC_InitStructure.NVIC_IRQChannel = LORA_USART_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);
    
    //dma rx
    DMA_ITConfig(LORA_DMA_RX_CHANNEL,DMA_IT_TC,ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = LORA_DMA_RX_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_Init(&NVIC_InitStructure);
    

    
}


void LORA_DMA_RX_IRQHandler(){
    if( DMA_GetITStatus(LORA_DMA_RX_TC) ){
        DMA_Cmd(LORA_DMA_RX_CHANNEL,DISABLE);//关闭接收DMA
        if(using_LoRaRxBuffer1 == 1){
            LORA_DMA_RX_CHANNEL->CMAR = (u32)LoRaRxBuffer2;
            using_LoRaRxBuffer1 = 0;
        }else{
            LORA_DMA_RX_CHANNEL->CMAR = (u32)LoRaRxBuffer1;
            using_LoRaRxBuffer1 = 1;
        }
        LoRaRXEvent = 1;
        receiveLength = LoRaRxBufferSize - LORA_DMA_RX_CHANNEL->CNDTR;
        LORA_DMA_RX_CHANNEL->CNDTR = LoRaRxBufferSize;
        DMA_ClearITPendingBit(LORA_DMA_RX_TC);
        DMA_Cmd(LORA_DMA_RX_CHANNEL,ENABLE);//打开接收DMA
    }

}

void LORA_USART_IRQHandler(){
    if( USART_GetITStatus(LORA_USARTx,USART_IT_IDLE) ){
         DMA_Cmd(LORA_DMA_RX_CHANNEL,DISABLE);//关闭接收DMA
        if(using_LoRaRxBuffer1 == 1){
            LORA_DMA_RX_CHANNEL->CMAR = (u32)LoRaRxBuffer2;
            using_LoRaRxBuffer1 = 0;
        }else{
            LORA_DMA_RX_CHANNEL->CMAR = (u32)LoRaRxBuffer1;
            using_LoRaRxBuffer1 = 1;
        }
        LoRaRXEvent = 1;
        receiveLength = LoRaRxBufferSize - LORA_DMA_RX_CHANNEL->CNDTR;
        LORA_DMA_RX_CHANNEL->CNDTR = LoRaRxBufferSize;
        DMA_ClearITPendingBit(LORA_DMA_RX_TC);
        DMA_Cmd(LORA_DMA_RX_CHANNEL,ENABLE);//打开接收DMA
    }
}

//void LORA_DMA_TX_IRQHandler(){
//    if(DMA_GetITStatus(LORA_DMA_TX_TC)){
//        DMA_ClearITPendingBit(LORA_DMA_TX_TC);
//    }
//}







