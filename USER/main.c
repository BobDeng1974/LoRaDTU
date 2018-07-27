#include "sys.h"
#include "delay.h"
#include "bsp_usart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "LoRaUsart.h"
#include "LoRa.h"
#include "Sender.h"
#include "ECC.h"
#include "Receiver.h"
#include "LinkedList.h"

//�������ȼ�
#define START_TASK_PRIO		1
//�����ջ��С	
#define START_STK_SIZE 		128  
//������
TaskHandle_t StartTask_Handler;
//������
void start_task(void *pvParameters);

//�������ȼ�
#define TASK1_TASK_PRIO		2
//�����ջ��С	
#define TASK1_STK_SIZE 		1024  
//������
TaskHandle_t Task1Task_Handler;
//������
void task1_task(void *pvParameters);

//�������ȼ�
#define TASK2_TASK_PRIO		3
//�����ջ��С	
#define TASK2_STK_SIZE 		1024  
//������
TaskHandle_t Task2Task_Handler;
//������
void task2_task(void *pvParameters);


//Ӧ������
#define APP_TASK_PRIO		4
#define APP_STK_SIZE 		128
TaskHandle_t APPTask_Handler;
void APP_task(void *pvParameters);



int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4	 
	delay_init();	    				//��ʱ������ʼ��	 
	USART_Config();
	LoRaInit();
    ECCInit();
    SenderInit();
    ReceiverInit();


	
	//������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������              
    vTaskStartScheduler();          //�����������
}





//��ʼ����������
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���
    //����TASK1����
    xTaskCreate((TaskFunction_t )task1_task,             
                (const char*    )"task1_task",           
                (uint16_t       )TASK1_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )TASK1_TASK_PRIO,        
                (TaskHandle_t*  )&Task1Task_Handler);   

    //����TASK2����
    xTaskCreate((TaskFunction_t )task2_task,             
                (const char*    )"task2_task",           
                (uint16_t       )TASK2_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )TASK2_TASK_PRIO,        
                (TaskHandle_t*  )&Task2Task_Handler);               
                
	//����app����
    xTaskCreate((TaskFunction_t )APP_task,             
                (const char*    )"app_task",           
                (uint16_t       )APP_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )APP_TASK_PRIO,        
                (TaskHandle_t*  )&APPTask_Handler);   
                
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}



//task1������
void task1_task(void *pvParameters)
{

	while(1)
	{
		_send();
        vTaskDelay(1000);
	}
}



//task2������
void task2_task(void *pvParameters)
{

	while(1)
	{
        _receive();
        vTaskDelay(10);                           
	}
}

void APP_task(void *pvParameters){
    while(1){
        DataPacket* packet = receiver->receive();
        if(packet != NULL){
            Usart_SendArray(USART1,packet->dataBytes.data,packet->dataBytes.length);
        }
        vTaskDelay(1000);
    }
}

