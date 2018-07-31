/**
 *	@���ߣ���˼Դ
 *	@���ڣ�2017��12��22��23:11:35
 *	@���ܣ���ʼ�� dht11 ���ṩһ��ͳһ�ӿ�
 *	@�汾��1.0
 *	@�޸ģ�no
 *	@�ӿڣ�Device_node device_dht11;								���������
 *	@�ӿڣ�DHT11_Data_TypeDef DHT11;								���»�ȡ�������ݽ��ᱣ��������ṹ������
 *	@�ӿڣ�void create_dht11(void)								��ʼ������Ͷ�����
 *	@�ӿڣ�short DHT11_Read_TempAndHumidity(void *self)			�������ӣ�ֱ�ӵ����豸�����update����ָ���ȡ����
 *	
 *
 *	@ע�����	��������Ϊ PE6 
 *
 *
 *
 *
 */





#include "bsp_dht11.h"
DHT11_Data_TypeDef DHT11;
Device_node device_dht11;

static void DHT11_GPIO_Config( void );
static void DHT11_Mode_IPU( void );
static void DHT11_Mode_Out_PP( void );
static unsigned char DHT11_ReadByte( void );


void create_dht11(void){
	DHT11_Init();
	device(&device_dht11,1,"DHT11",&DHT11,DHT11_Read_TempAndHumidity);
}


 /**
  * @brief  DHT11 ��ʼ������
  * @param  ��
  * @retval ��
  */
void DHT11_Init ( void )
{
	DHT11_GPIO_Config ();
	
	macDHT11_Dout_1;               // ����GPIOE6
}


/*
 * ��������DHT11_GPIO_Config
 * ����  ������DHT11�õ���I/O��
 * ����  ����
 * ���  ����
 */
static void DHT11_GPIO_Config ( void )
{	
//	__HAL_RCC_GPIOE_CLK_ENABLE();
//	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
//	GPIO_InitTypeDef GPIO_InitStruct;
//    
//    
//    
//	GPIO_InitStruct.Pin=macDHT11_Dout_GPIO_PIN;			//Pe6
//	GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;		//�������
//	GPIO_InitStruct.Pull=GPIO_NOPULL;			
//	GPIO_InitStruct.Speed=GPIO_SPEED_FAST;		//����
//	HAL_GPIO_Init(macDHT11_Dout_GPIO_PORT,&GPIO_InitStruct);	   	//��ʼ��PE6	  
	GPIO_InitTypeDef GPIO_InitStruct;
    
    RCC_APB2PeriphClockCmd(DHT11_CLK,ENABLE);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Pin = macDHT11_Dout_GPIO_PIN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(macDHT11_Dout_GPIO_PORT,&GPIO_InitStruct);
}


/*
 * ��������DHT11_Mode_IPU
 * ����  ��ʹDHT11-DATA���ű�Ϊ��������ģʽ
 * ����  ����
 * ���  ����
 */
static void DHT11_Mode_IPU(void)
{
//    GPIO_InitTypeDef GPIO_InitStruct;
//    GPIO_InitStruct.GPIO_Pin = macDHT11_Dout_GPIO_PIN;
//    /*ѡ��Ҫ���Ƶ�macDHT11_Dout_GPIO_PORT����*/	
//    GPIO_InitStruct.Pin = macDHT11_Dout_GPIO_PIN;
//    GPIO_InitStruct.Pull = GPIO_PULLUP;
//    /*��������ģʽΪ��������ģʽ*/ 
//    GPIO_InitStruct.Mode = GPIO_MODE_INPUT ; 

//    /*���ÿ⺯������ʼ��macDHT11_Dout_GPIO_PORT*/
//    HAL_GPIO_Init(macDHT11_Dout_GPIO_PORT, &GPIO_InitStruct);	 
	GPIO_InitTypeDef GPIO_InitStruct;
    
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin = macDHT11_Dout_GPIO_PIN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(macDHT11_Dout_GPIO_PORT,&GPIO_InitStruct);    
	
}


/*
 * ��������DHT11_Mode_Out_PP
 * ����  ��ʹDHT11-DATA���ű�Ϊ�������ģʽ
 * ����  ����
 * ���  ����
 */
static void DHT11_Mode_Out_PP(void)
{
// 	GPIO_InitTypeDef GPIO_InitStruct;

//	 	/*ѡ��Ҫ���Ƶ�macDHT11_Dout_GPIO_PORT����*/															   
//  	GPIO_InitStruct.Pin = 	macDHT11_Dout_GPIO_PIN;

//	/*��������ģʽΪͨ���������*/
//  	GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;		//�������   

//	/*������������Ϊ50MHz */   
//  	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

//	/*���ÿ⺯������ʼ��macDHT11_Dout_GPIO_PORT*/
//  	HAL_GPIO_Init(macDHT11_Dout_GPIO_PORT, &GPIO_InitStruct);	 	 
	
    
    GPIO_InitTypeDef GPIO_InitStruct;
    
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Pin = macDHT11_Dout_GPIO_PIN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(macDHT11_Dout_GPIO_PORT,&GPIO_InitStruct);   
    
}


/* 
 * ��DHT11��ȡһ���ֽڣ�MSB����
 */
static unsigned char DHT11_ReadByte ( void )
{
	uint8_t i, temp=0;
	

	for(i=0;i<8;i++)    
	{	  
		/*ÿbit��50us�͵�ƽ���ÿ�ʼ����ѯֱ���ӻ����� ��50us �͵�ƽ ����*/  
		while(macDHT11_Dout_IN()==GPIO_PIN_RESET);

		/*DHT11 ��26~28us�ĸߵ�ƽ��ʾ��0������70us�ߵ�ƽ��ʾ��1����
		 *ͨ����� x us��ĵ�ƽ��������������״ ��x ���������ʱ 
		 */
		delay_us(40); //��ʱx us �����ʱ��Ҫ��������0������ʱ�伴��	   	  

		if(macDHT11_Dout_IN()==GPIO_PIN_SET)/* x us����Ϊ�ߵ�ƽ��ʾ���ݡ�1�� */
		{
			/* �ȴ�����1�ĸߵ�ƽ���� */
			while(macDHT11_Dout_IN()==GPIO_PIN_SET);

			temp|=(uint8_t)(0x01<<(7-i));  //�ѵ�7-iλ��1��MSB���� 
		}
		else	 // x us��Ϊ�͵�ƽ��ʾ���ݡ�0��
		{			   
			temp&=(uint8_t)~(0x01<<(7-i)); //�ѵ�7-iλ��0��MSB����
		}
	}
	
	return temp;
	
}


/*
 * һ�����������ݴ���Ϊ40bit����λ�ȳ�
 * 8bit ʪ������ + 8bit ʪ��С�� + 8bit �¶����� + 8bit �¶�С�� + 8bit У��� 
 */
short DHT11_Read_TempAndHumidity(void *self)
{  
	Device_node *this = self;
	if(strcmp(this->name,"DHT11"))return ERROR;
	DHT11_Data_TypeDef *DHT11_Data_update = this->pvalue;
	DHT11_Data_TypeDef DHT11_Data;
	uint8_t return_flag = ERROR;
	uint8_t timeout = 50;
	/*���ģʽ*/
	DHT11_Mode_Out_PP();
	/*��������*/
	macDHT11_Dout_0;
	/*��ʱ18ms*/
	delay_ms(20);
	taskENTER_CRITICAL();
	/*�������� ������ʱ30us*/
	macDHT11_Dout_1; 

	delay_us(30);  //��ʱ30us

	/*������Ϊ���� �жϴӻ���Ӧ�ź�*/ 
	DHT11_Mode_IPU();

	/*�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������*/   
	if(macDHT11_Dout_IN()==GPIO_PIN_RESET)     
	{
		/*��ѯֱ���ӻ����� ��80us �͵�ƽ ��Ӧ�źŽ���*/  
		while(macDHT11_Dout_IN()==GPIO_PIN_RESET && --timeout)delay_us(5);		
		if(timeout == 0){
			return_flag = 16;
			goto ret;
		}else timeout = 50;
		/*��ѯֱ���ӻ������� 80us �ߵ�ƽ �����źŽ���*/
		while(macDHT11_Dout_IN()==GPIO_PIN_SET && --timeout)delay_us(5);
		if(timeout == 0){
			return_flag = 16;
			goto ret;
		}
		/*��ʼ��������*/   
		DHT11_Data.humi_int= DHT11_ReadByte();

		DHT11_Data.humi_deci= DHT11_ReadByte();

		DHT11_Data.temp_int= DHT11_ReadByte();

		DHT11_Data.temp_deci= DHT11_ReadByte();

		DHT11_Data.check_sum= DHT11_ReadByte();


		/*��ȡ���������Ÿ�Ϊ���ģʽ*/
		DHT11_Mode_Out_PP();
		/*��������*/
		macDHT11_Dout_1;

		/*����ȡ�������Ƿ���ȷ*/
		if(DHT11_Data.check_sum == DHT11_Data.humi_int + DHT11_Data.humi_deci + DHT11_Data.temp_int+ DHT11_Data.temp_deci){
			return_flag = SUCCESS;
			if(DHT11_Data.humi_int 	!= DHT11_Data_update->humi_int || 
			   DHT11_Data.humi_deci != DHT11_Data_update->humi_deci||
			   DHT11_Data.temp_deci != DHT11_Data_update->temp_deci||
			   DHT11_Data.temp_int 	!= DHT11_Data_update->temp_int
			){ 
				DHT11_Data_update->humi_int 	= DHT11_Data.humi_int ; 
				DHT11_Data_update->humi_deci	= DHT11_Data.humi_deci;
				DHT11_Data_update->temp_deci 	= DHT11_Data.temp_deci;
				DHT11_Data_update->temp_int 	= DHT11_Data.temp_int;
				this->update_event = true;
			}			
		}
	}
ret:
	taskEXIT_CRITICAL();
	return return_flag;	
}

