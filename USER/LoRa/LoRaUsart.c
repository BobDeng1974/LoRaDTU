#include "LoRaUsart.h"
#include "String.h"
#include "stdlib.h"
#include "bsp_usart.h"
//串口缓冲区以及标志位
uint8_t  LoRaRxBuffer[ LoRaRxBufferSize ] = { 0 };
uint8_t  LoRaRXEvent = 0;
uint16_t  receiveLength  = 0;


/**
 *    配置 LoRa 的串口
 *    1.配置串口引脚
 *    
 *    3.配置向量表初始化
 */
void LoRaUsartConfig(int baudRate){
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

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
	GPIO_Init(LORA_USART_RX_GPIO_PORT, &GPIO_InitStructure);
    
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
    


    //usart  rxne
    NVIC_InitStructure.NVIC_IRQChannel = LORA_USART_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);
    

    
    USART_ITConfig(LORA_USARTx,USART_IT_RXNE,ENABLE);
    
    USART_Cmd(LORA_USARTx, ENABLE);
    
}




uint8_t ch;
_Bool flag = 0;
_Bool startFlag = 0;
void LORA_USART_IRQHandler(){

	if(USART_GetITStatus(LORA_USARTx,USART_IT_RXNE)!=RESET)
	{		
		ch = USART_ReceiveData(LORA_USARTx);

        if(flag == 0){
            if(startFlag == 1 && ch != 0x04 && ch != 0x1B){
                //如果正在接收并且当前字符不是帧结束符也不是转义符，那么直接接收
                LoRaRxBuffer[receiveLength++] = ch;
                return;
            }
            if(startFlag == 1 && ch == 0x04 ){
                //0x04前面不是0x1B，并且当前处于接收模式
                //判断为帧结束信号
                //产生一次更新信号并将当前的开始信号复位
                LoRaRxBuffer[receiveLength++] = ch;
                LoRaRXEvent = 1;
                startFlag = 0;
            }else if(LoRaRXEvent == 0 && ch == 0x01 ){
                //如果之前接收到的数据已取出并且接收到帧开始符
                //就将开始信号置位
                LoRaRxBuffer[receiveLength++] = ch;
                startFlag = 1;
            }else if(startFlag == 1 && ch == 0x1B){
                //如果前面没有转义符并且接收到了0x1B并且当前处于开始接收状态
                //那么，这个0x1B是转义符
                //需要丢弃当前这个转义符并将转义flag置位
                flag = 1;
            }
            return;
            
        }
        if(flag == 1 && startFlag == 1){
            //如果处于接收模式并且转义信号有效
            //那么应该收下当前这个数据
            LoRaRxBuffer[receiveLength++] = ch;
            flag = 0;
        }
        
	}
}









