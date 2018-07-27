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

//任务优先级
#define START_TASK_PRIO		1
//任务堆栈大小	
#define START_STK_SIZE 		128  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);

//任务优先级
#define TASK1_TASK_PRIO		2
//任务堆栈大小	
#define TASK1_STK_SIZE 		1024  
//任务句柄
TaskHandle_t Task1Task_Handler;
//任务函数
void task1_task(void *pvParameters);

//任务优先级
#define TASK2_TASK_PRIO		3
//任务堆栈大小	
#define TASK2_STK_SIZE 		1024  
//任务句柄
TaskHandle_t Task2Task_Handler;
//任务函数
void task2_task(void *pvParameters);


//应用任务
#define APP_TASK_PRIO		4
#define APP_STK_SIZE 		128
TaskHandle_t APPTask_Handler;
void APP_task(void *pvParameters);



int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4	 
	delay_init();	    				//延时函数初始化	 
	USART_Config();
	LoRaInit();
    ECCInit();
    SenderInit();
    ReceiverInit();


	
	//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    vTaskStartScheduler();          //开启任务调度
}





//开始任务任务函数
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区
    //创建TASK1任务
    xTaskCreate((TaskFunction_t )task1_task,             
                (const char*    )"task1_task",           
                (uint16_t       )TASK1_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )TASK1_TASK_PRIO,        
                (TaskHandle_t*  )&Task1Task_Handler);   

    //创建TASK2任务
    xTaskCreate((TaskFunction_t )task2_task,             
                (const char*    )"task2_task",           
                (uint16_t       )TASK2_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )TASK2_TASK_PRIO,        
                (TaskHandle_t*  )&Task2Task_Handler);               
                
	//创建app任务
    xTaskCreate((TaskFunction_t )APP_task,             
                (const char*    )"app_task",           
                (uint16_t       )APP_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )APP_TASK_PRIO,        
                (TaskHandle_t*  )&APPTask_Handler);   
                
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}



//task1任务函数
void task1_task(void *pvParameters)
{

	while(1)
	{
		_send();
        vTaskDelay(1000);
	}
}



//task2任务函数
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

