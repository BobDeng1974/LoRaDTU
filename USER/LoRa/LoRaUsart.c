#include "LoRaUsart.h"
#include "String.h"
#include "stdlib.h"
#include "bsp_usart.h"
//���ڻ������Լ���־λ
uint8_t  LoRaRxBuffer[ LoRaRxBufferSize ] = { 0 };
uint8_t  LoRaRXEvent = 0;
uint16_t  receiveLength  = 0;


/**
 *    ���� LoRa �Ĵ���
 *    1.���ô�������
 *    
 *    3.�����������ʼ��
 */
void LoRaUsartConfig(int baudRate){
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // �򿪴���GPIO��ʱ��
	LORA_USART_GPIO_APBxClkCmd(LORA_USART_GPIO_CLK, ENABLE);
	
	// �򿪴��������ʱ��
	LORA_USART_APBxClkCmd(LORA_USART_CLK, ENABLE);

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = LORA_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LORA_USART_TX_GPIO_PORT, &GPIO_InitStructure);

    // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = LORA_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(LORA_USART_RX_GPIO_PORT, &GPIO_InitStructure);
    
    // ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = baudRate;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    
    // ��ɴ��ڵĳ�ʼ������
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
                //������ڽ��ղ��ҵ�ǰ�ַ�����֡������Ҳ����ת�������ôֱ�ӽ���
                LoRaRxBuffer[receiveLength++] = ch;
                return;
            }
            if(startFlag == 1 && ch == 0x04 ){
                //0x04ǰ�治��0x1B�����ҵ�ǰ���ڽ���ģʽ
                //�ж�Ϊ֡�����ź�
                //����һ�θ����źŲ�����ǰ�Ŀ�ʼ�źŸ�λ
                LoRaRxBuffer[receiveLength++] = ch;
                LoRaRXEvent = 1;
                startFlag = 0;
            }else if(LoRaRXEvent == 0 && ch == 0x01 ){
                //���֮ǰ���յ���������ȡ�����ҽ��յ�֡��ʼ��
                //�ͽ���ʼ�ź���λ
                LoRaRxBuffer[receiveLength++] = ch;
                startFlag = 1;
            }else if(startFlag == 1 && ch == 0x1B){
                //���ǰ��û��ת������ҽ��յ���0x1B���ҵ�ǰ���ڿ�ʼ����״̬
                //��ô�����0x1B��ת���
                //��Ҫ������ǰ���ת�������ת��flag��λ
                flag = 1;
            }
            return;
            
        }
        if(flag == 1 && startFlag == 1){
            //������ڽ���ģʽ����ת���ź���Ч
            //��ôӦ�����µ�ǰ�������
            LoRaRxBuffer[receiveLength++] = ch;
            flag = 0;
        }
        
	}
}









