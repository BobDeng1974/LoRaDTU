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
#include "bsp_dht11.h"
#include "adc.h"
#include "RoutingTable.h"
//�������ȼ�
#define START_TASK_PRIO		1
//�����ջ��С	
#define START_STK_SIZE 		128  
//������
TaskHandle_t StartTask_Handler;
//������
void start_task(void *pvParameters);

//�������ȼ�
#define recv_TASK_PRIO		10
//�����ջ��С	
#define recv_STK_SIZE 		1024  
//������
TaskHandle_t recvTask_Handler;
//������
void recv_task(void *pvParameters);

//�������ȼ�
#define send_TASK_PRIO		10
//�����ջ��С	
#define send_STK_SIZE 		1024  
//������
TaskHandle_t sendTask_Handler;
//������
void send_task(void *pvParameters);





//Ӧ������
#define APP1_TASK_PRIO		4
#define APP1_STK_SIZE 		1024
TaskHandle_t APP1Task_Handler;
void APP1_task(void *pvParameters);



LoRaAddress WIFIaddress;





//Ӧ������
#define TEMP_TASK_PRIO		0
#define TEMP_STK_SIZE 		128
TaskHandle_t TEMPTask_Handler;
void TEMP_task(void *pvParameters);


//Ӧ������
#define HUMI_TASK_PRIO		0
#define HUMI_STK_SIZE 		128
TaskHandle_t HUMITask_Handler;
void HUMI_task(void *pvParameters);


//Ӧ������
#define EUMI_TASK_PRIO		0
#define EUMI_STK_SIZE 		128
TaskHandle_t EUMITask_Handler;
void EUMI_task(void *pvParameters);



//Ӧ������
#define ECC_TASK_PRIO		5
#define ECC_STK_SIZE 		512
TaskHandle_t ECCTask_Handler;
void ECC_task(void *pvParameters);







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
    create_adc();
    //create_bh1750();
    create_dht11();
    
    WIFIaddress.Address_H = 0x03;
    WIFIaddress.Address_L = 0x03;
    WIFIaddress.Channel = 0x03;
    
    printf("ϵͳ����");
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
    //������������
    xTaskCreate((TaskFunction_t )send_task,             
                (const char*    )"send_task",           
                (uint16_t       )send_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )send_TASK_PRIO,        
                (TaskHandle_t*  )&sendTask_Handler);   

    //������������
    xTaskCreate((TaskFunction_t )recv_task,             
                (const char*    )"recv_task",           
                (uint16_t       )recv_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )recv_TASK_PRIO,        
                (TaskHandle_t*  )&recvTask_Handler);               
    
    //�����¶ȷ�������
    xTaskCreate((TaskFunction_t )TEMP_task,             
                (const char*    )"TEMP_task",           
                (uint16_t       )TEMP_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )TEMP_TASK_PRIO,        
                (TaskHandle_t*  )&TEMPTask_Handler); 
    
    //����ʪ�ȷ�������
    xTaskCreate((TaskFunction_t )HUMI_task,             
                (const char*    )"HUMI_task",           
                (uint16_t       )HUMI_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )HUMI_TASK_PRIO,        
                (TaskHandle_t*  )&HUMITask_Handler);     
                
    //��������ʪ�ȷ�������
    xTaskCreate((TaskFunction_t )EUMI_task,             
                (const char*    )"EUMI_task",           
                (uint16_t       )EUMI_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )EUMI_TASK_PRIO,        
                (TaskHandle_t*  )&EUMITask_Handler);                
    
     //����app1����
    xTaskCreate((TaskFunction_t )APP1_task,             
                (const char*    )"app1_task",           
                (uint16_t       )APP1_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )APP1_TASK_PRIO,        
                (TaskHandle_t*  )&APP1Task_Handler);
    
     //����ECC����
    xTaskCreate((TaskFunction_t )ECC_task,             
                (const char*    )"ECC_task",           
                (uint16_t       )ECC_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )ECC_TASK_PRIO,        
                (TaskHandle_t*  )&ECCTask_Handler);                
                
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}



//send������
void send_task(void *pvParameters)
{

	while(1)
	{
		_send();
        
        vTaskDelay(10);
	}
}



//recv������
void recv_task(void *pvParameters)
{

	while(1)
	{
        _receive();
        
        vTaskDelay(10);                           
	}
}





void TEMP_task(void *pvParameters){
    while(1){
        device_dht11.Device_update(&device_dht11);
        
        DataPacket* packet = malloc(sizeof(DataPacket));
        packet->dataBytes.length = 7;
        packet->dataBytes.data = malloc(sizeof(uint8_t)*7);
        packet->source = localhost;
        packet->destination = WIFIaddress;
        packet->count = 0x10;
        strcpy((char*)packet->dataBytes.data,"temp:");
        packet->dataBytes.data[5] = DHT11.temp_int/10+0x30;
        packet->dataBytes.data[6] = DHT11.temp_int%10+0x30;
        Sender->send(packet);
        
        vTaskDelay(10000);
    }
}


void HUMI_task(void *pvParameters){
    while(1){
        device_dht11.Device_update(&device_dht11);
        
        DataPacket* packet1 = malloc(sizeof(DataPacket));
        packet1->dataBytes.length = 7;
        packet1->dataBytes.data = malloc(sizeof(uint8_t)*7);
        packet1->source = localhost;
        packet1->destination = WIFIaddress;
        packet1->count = 0x10;
        strcpy((char*)packet1->dataBytes.data,"humi:");
        packet1->dataBytes.data[5] = DHT11.humi_int/10+0x30;
        packet1->dataBytes.data[6] = DHT11.humi_int%10+0x30;
        Sender->send(packet1);
        
        vTaskDelay(10000);
    }
}

void EUMI_task(void *pvParameters){
    while(1){
        device_adc_pc4.Device_update(&device_adc_pc4);
        
        DataPacket* packet2 = malloc(sizeof(DataPacket));
        packet2->dataBytes.length = 7;
        packet2->dataBytes.data = malloc(sizeof(uint8_t)*7);
        packet2->source = localhost;
        packet2->destination = WIFIaddress;
        packet2->count = 0x10;
        strcpy((char*)packet2->dataBytes.data,"eumi:");
        packet2->dataBytes.data[5] = ADC_PC4/10+0x30;
        packet2->dataBytes.data[6] = ADC_PC4%10+0x30;
        Sender->send(packet2);
        
        vTaskDelay(10000);
    }
    
}






void APP1_task(void *pvParameters){
    
    
    while(1){
        
        DataPacket* packet = receiver->receive();
        
        if(packet != NULL){
            if(packet->dataBytes.length == 4){
                DataPacket* packet1 = malloc(sizeof(DataPacket));
                packet1->dataBytes.length = 7;
                packet1->dataBytes.data = malloc(sizeof(uint8_t)*7);
                packet1->source = localhost;
                packet1->destination = packet->source;
                packet->count = 0x10;
                device_adc_pc4.Device_update(&device_adc_pc4);
                device_dht11.Device_update(&device_dht11);
                if(strstr((const char*)packet->dataBytes.data,"temp")){
                   
                    strcpy((char*)packet1->dataBytes.data,"temp:");
                    packet1->dataBytes.data[5] = DHT11.temp_int/10+0x30;
                    packet1->dataBytes.data[6] = DHT11.temp_int%10+0x30;
                }else if(strstr((const char*)packet->dataBytes.data,"humi")){
                    strcpy((char*)packet1->dataBytes.data,"humi:");
                    packet1->dataBytes.data[5] = DHT11.humi_int/10+0x30;
                    packet1->dataBytes.data[6] = DHT11.humi_int%10+0x30;
                }else if(strstr((const char*)packet->dataBytes.data,"eumi")||
                         strstr((const char*)packet->dataBytes.data,"Eumi")){
                    strcpy((char*)packet1->dataBytes.data,"Eumi:");
                    packet1->dataBytes.data[5] = ADC_PC4/10+0x30;
                    packet1->dataBytes.data[6] = ADC_PC4%10+0x30;
                }else{
                    strcpy((char*)packet1->dataBytes.data,"error!!");
                }
                Sender->send(packet1);
            }else if(packet->dataBytes.length == 7){
                Usart_SendString(DEBUG_USARTx,"���յ����Ե�ַΪ��");
                vTaskDelay(50);
                printf("%2x %2x %2x ���豸���͵����ݡ�",packet->source.Address_H,packet->source.Address_L,packet->source.Channel);
                 vTaskDelay(50);
                Usart_SendArray(DEBUG_USARTx,packet->dataBytes.data,packet->dataBytes.length);
                 vTaskDelay(50);
                Usart_SendString(DEBUG_USARTx,"��\r\n");
            }
            destroyPacket(packet);
        }
        
        vTaskDelay(1000);
    }
}


void ECC_task(void *pvParameters){
    DataPacket* packet = NULL;
    Node* node = NULL;
	while(1)
	{
		
        //ÿ50ms����һ��ECC
        ECC->ECCList->iterator.reset(ECC->ECCList);
        while(ECC->ECCList->iterator.hasNext(ECC->ECCList)){
            node = ECC->ECCList->iterator.next(ECC->ECCList);
            packet = node->nodeData;
            if(--packet->time <= 0){
                ECC->ECCList->deleteByNode(ECC->ECCList,node);
                _LoRaSendData(packet);
            }
        }
        vTaskDelay(50);
	}


}


















