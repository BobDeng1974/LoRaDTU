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
#include "bh1750.h"
#include "bsp_dht11.h"
#include "adc.h"

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
#define TASK2_TASK_PRIO		10
//�����ջ��С	
#define TASK2_STK_SIZE 		1024  
//������
TaskHandle_t Task2Task_Handler;
//������
void task2_task(void *pvParameters);


//Ӧ������
#define APP_TASK_PRIO		0
#define APP_STK_SIZE 		128
TaskHandle_t APPTask_Handler;
void APP_task(void *pvParameters);



int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4	 
	delay_init();	    				//��ʱ������ʼ��	 
	USART_Config();
    LoRaUsartConfig(115200);
	LoRaInit();
    ECCInit();
    SenderInit();
    ReceiverInit();
    //Usart_SendArray(USART2,"helloworld",10);
    printf("ϵͳ����");
	Usart_SendString(USART2,"LoRa��ʼ��");
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
        vTaskDelay(10);
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
    create_adc();
    create_bh1750();
    create_dht11();
    while(1){
//        DataPacket* packet = receiver->receive();
//        if(packet != NULL){
//            Usart_SendArray(USART1,packet->dataBytes.data,packet->dataBytes.length);
//            destroyPacket(packet);  
//        }
        
        
        
        if(device_dht11.Device_update(&device_dht11) == SUCCESS &&device_dht11.update_event == true){
//            DHT11_Data_TypeDef data = *((DHT11_Data_TypeDef*)device_dht11.pvalue);
            
			printf("�¶����ݣ�%d\n",DHT11.temp_int);
            printf("ʪ�����ݣ�%d%%\n",DHT11.humi_int);
			device_dht11.update_event = false;		
		}
        if(device_adc_pc4.Device_update(&device_adc_pc4) == SUCCESS &&device_adc_pc4.update_event == true){
			
			printf("����ʪ�ȣ�%d%%\n",ADC_PC4);
			device_adc_pc4.update_event = false;		
		}
        
        
        
        
        vTaskDelay(1000);
    }
}

