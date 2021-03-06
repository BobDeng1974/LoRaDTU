/**
  ******************************************************************************
  * @file    bsp_usart.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   重定向c库printf函数到usart端口
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火STM32 F103-指南者 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
	
#include "LoRa_usart.h"

uint8_t receiveBuffer[2048];
uint8_t* preceiveBuffer = receiveBuffer;
uint8_t receiveEvent = 0;


 /**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 嵌套向量中断控制器组选择 */
  //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
  
  /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = LoRa_USART_IRQ;
  /* 抢断优先级*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 子优先级 */
  //NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

 /**
  * @brief  USART GPIO 配置,工作参数配置
  * @param  无
  * @retval 无
  */
void LoRa_USART_Config(int baudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// 打开串口GPIO的时钟
	LoRa_USART_GPIO_APBxClkCmd(LoRa_USART_GPIO_CLK, ENABLE);
	
	// 打开串口外设的时钟
	LoRa_USART_APBxClkCmd(LoRa_USART_CLK, ENABLE);

	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = LoRa_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LoRa_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = LoRa_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(LoRa_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
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
	USART_Init(LoRa_USARTx, &USART_InitStructure);
	
//	// 串口中断优先级配置
//	NVIC_Configuration();
//	
//	// 使能串口接收中断
//	USART_ITConfig(LoRa_USARTx, USART_IT_RXNE, ENABLE);	
	
	// 使能串口
	USART_Cmd(LoRa_USARTx, ENABLE);	    
}



/*****************  发送一个字节 **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/****************** 发送8位的数组 ************************/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
  uint8_t i;
	
	for(i=0; i<num; i++)
  {
	    /* 发送一个字节数据到USART */
	    Usart_SendByte(pUSARTx,array[i]);	
  
  }
	/* 等待发送完成 */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

/*****************  发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* 等待发送完成 */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/*****************  发送一个16位数 **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	/* 取出高八位 */
	temp_h = (ch&0XFF00)>>8;
	/* 取出低八位 */
	temp_l = ch&0XFF;
	
	/* 发送高八位 */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
	/* 发送低八位 */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}




/////重定向c库函数printf到串口，重定向后可使用printf函数
//int fputc(int ch, FILE *f)
//{
//		/* 发送一个字节数据到串口 */
//		USART_SendData(DEBUG_USARTx, (uint8_t) ch);
//		
//		/* 等待发送完毕 */
//		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);		
//	
//		return (ch);
//}

/////重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
//int fgetc(FILE *f)
//{
//		/* 等待串口输入数据 */
//		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

//		return (int)USART_ReceiveData(DEBUG_USARTx);
//}










//    _Bool flag = 0;
//    _Bool startFlag = 0;
//// 串口中断服务函数
//void DEBUG_USART_IRQHandler(void)
//{
//    uint8_t ch;
//	if(USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET)
//	{		
//		ch = USART_ReceiveData(DEBUG_USARTx);

//        if(flag == 0){
//            if(startFlag == 1 && ch != 0x04 && ch != 0x1B){
//                //如果正在接收并且当前字符不是帧结束符也不是转义符，那么直接接收
//                *preceiveBuffer++ = ch;
//                goto out;
//            }
//            if(startFlag == 1 && ch == 0x04 ){
//                //0x04前面不是0x1B，并且当前处于接收模式
//                //判断为帧结束信号
//                //产生一次更新信号并将当前的开始信号复位
//                *preceiveBuffer++ = ch;
//                receiveEvent = 1;
//                startFlag = 0;
//            }else if(receiveEvent == 0 && ch == 0x01 ){
//                //如果之前接收到的数据已取出并且接收到帧开始符
//                //就将开始信号置位
//                *preceiveBuffer++ = ch;
//                startFlag = 1;
//            }else if(startFlag == 1 && ch == 0x1B){
//                //如果前面没有转义符并且接收到了0x1B并且当前处于开始接收状态
//                //那么，这个0x1B是转义符
//                //需要丢弃当前这个转义符并将转义flag置位
//                flag = 1;
//            }
//            
//            goto out;
//        }
//        if(flag == 1 && startFlag == 1){
//            //如果处于接收模式并且转义信号有效
//            //那么应该收下当前这个数据
//            *preceiveBuffer++ = ch;
//            flag = 0;
//        }
//        
//	}
//    out:;
//}

