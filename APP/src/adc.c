/**
 *	@���ߣ���˼Դ
 *	@���ڣ�2017��12��22��22:42:20
 *	@���ܣ���ʼ�� stm32F767 �� ADC1 
 *	@�汾��1.0
 *	@�޸ģ�no
 *	@�ӿڣ�ADC1ͨ��5���豸��� 					device_adc_pa5
 *	@�ӿڣ�ADC1ͨ��5ת��������ʪ�Ⱥ����ֵ 		ADC_PA5
 *	@�ӿڣ�void create_adc(void);				��ʼ����������ʼ��ADC1����ʼ���豸���		
 *	@�ӿڣ�short adc_pa5_update(void *self);		�������ӣ�ֱ�ӵ����豸�����update����ָ���ȡ����
 *
 *
 *
 *	@ע������ṩ��ͳһ�ӿ��� PA5
 *
 *
 *
 */

#include "adc.h"

void adc1_Init(void);
static void ADC_DATA_Change(void);
static void ADCx_Mode_Config(void);
static void ADCx_GPIO_Config(void);



Device_node device_adc_pc4;
ADC_Data_TypeDef ADC_PC4 = 0x00;

//ADC_HandleTypeDef ADC1_handler;


void create_adc(void){
	adc1_Init();
	device(&device_adc_pc4,3,"ADC_PC4",&ADC_PC4,adc_pc4_update);
    
}



static void ADCx_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef 	DMA_InitStructure;
	
		// ��DMAʱ��
	RCC_AHBPeriphClockCmd(ADC_DMA_CLK, ENABLE);
	// ��λDMA������
	DMA_DeInit(ADC_DMA_CHANNEL);
	
	DMA_InitStructure.DMA_BufferSize = NUMBER_OF_CHANNEL;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_ConvertedValue;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(ADC_x->DR));
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	// ��ʼ��DMA
	DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);
	// ʹ�� DMA ͨ��
	DMA_Cmd(ADC_DMA_CHANNEL , ENABLE);
	//��ADCʱ��
	ADC_APBxClock_Cmd(ADC_CLK,ENABLE);
	//����ת��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	//ת������Ҷ���
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	//�����ⲿ����ת�����������
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	//ʹ�õ���ADC���ڵ�ģʽ
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	//ת��ͨ������
	ADC_InitStructure.ADC_NbrOfChannel = NUMBER_OF_CHANNEL;
	//ɨ��ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	
	ADC_Init(ADC_x, &ADC_InitStructure);
	// ����ADCʱ�ӣΪPCLK2��8��Ƶ����9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
		// ����ADC ͨ����ת��˳��Ͳ���ʱ��
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL1, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL2, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL3, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL4, 4, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL5, 5, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL6, 6, ADC_SampleTime_55Cycles5);
	// ʹ��ADC DMA ����
	ADC_DMACmd(ADC_x, ENABLE);
	// ����ADC ������ʼת��
	ADC_Cmd(ADC_x, ENABLE);
	// ��ʼ��ADC У׼�Ĵ���  
	ADC_ResetCalibration(ADC_x);
	// �ȴ�У׼�Ĵ�����ʼ�����
	while(ADC_GetResetCalibrationStatus(ADC_x));
	// ADC��ʼУ׼
	ADC_StartCalibration(ADC_x);
	// �ȴ�У׼���
	while(ADC_GetCalibrationStatus(ADC_x));
	// ����û�в����ⲿ����������ʹ���������ADCת�� 
	ADC_SoftwareStartConvCmd(ADC_x, ENABLE);
}



static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	ADC_GPIO_Clock_Cmd(ADC_GPIO_CLK,ENABLE);
	GPIO_InitStructure.GPIO_Pin=ADC_PIN;	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;	
	
	GPIO_Init(ADC_PORT, &GPIO_InitStructure);		
}


void adc1_Init(void){
    ADCx_GPIO_Config();
    ADCx_Mode_Config();
}


static void ADC_DATA_Change(void)
{
	ADC_ConvertedValueLocal[0] =(float) ADC_ConvertedValue[0]/4096*3.3;
	ADC_ConvertedValueLocal[1] =(float) ADC_ConvertedValue[1]/4096*3.3;
	ADC_ConvertedValueLocal[2] =(float) ADC_ConvertedValue[2]/4096*3.3;
	ADC_ConvertedValueLocal[3] =(float) ADC_ConvertedValue[3]/4096*3.3;
	ADC_ConvertedValueLocal[4] =(float) ADC_ConvertedValue[4]/4096*3.3;
	ADC_ConvertedValueLocal[5] =(float) ADC_ConvertedValue[5]/4096*3.3;
}


//��ȡָ��ͨ����ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
u16 Get_Adc_Average(u32 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
        ADC_DATA_Change();
		temp_val+=ADC_ConvertedValueLocal[ch];
		delay_ms(5);
	}
	return temp_val/times;
}


/**
 *  ʪ�ȴ�������Ӧ��������PC4
 *
 *
 *
 */ 
uint16_t get_e_humi(void){
	static unsigned int E_HUMI = 0;
	const float V[10] = {3.2604,2.7291,2.0746,1.8730,1.6220,1.4602,1.1138,0.9290,0.5900,0};
	static float value;
    ADC_DATA_Change();
    value = ADC_ConvertedValueLocal[4];
	for(int i = 0; i < 10; i++){
		if(value >= V[i]){
			E_HUMI = i*10;
			break;
		}
	}	
	return E_HUMI;
}



short adc_pc4_update(void *self){
	Device_node *this = self;
	if(strcmp(this->name,"ADC_PC4"))return ERROR;
	ADC_Data_TypeDef adc_data;
	
	adc_data = get_e_humi();
	if(adc_data != ADC_PC4){
		ADC_PC4 = adc_data;
		this->update_event = true;
	}
	return SUCCESS;
}

