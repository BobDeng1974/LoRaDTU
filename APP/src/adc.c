/**
 *	@作者：杨思源
 *	@日期：2017年12月22日22:42:20
 *	@功能：初始化 stm32F767 的 ADC1 
 *	@版本：1.0
 *	@修改：no
 *	@接口：ADC1通道5的设备句柄 					device_adc_pa5
 *	@接口：ADC1通道5转换成土壤湿度后的数值 		ADC_PA5
 *	@接口：void create_adc(void);				初始化函数，初始化ADC1并初始化设备句柄		
 *	@接口：short adc_pa5_update(void *self);		可以无视，直接调用设备对象的update函数指针获取更新
 *
 *
 *
 *	@注意事项：提供的统一接口是 PA5
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
	
		// 打开DMA时钟
	RCC_AHBPeriphClockCmd(ADC_DMA_CLK, ENABLE);
	// 复位DMA控制器
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
	// 初始化DMA
	DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);
	// 使能 DMA 通道
	DMA_Cmd(ADC_DMA_CHANNEL , ENABLE);
	//打开ADC时钟
	ADC_APBxClock_Cmd(ADC_CLK,ENABLE);
	//连续转换模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	//转换结果右对齐
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	//不用外部触发转换，软件开启
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	//使用单个ADC属于单模式
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	//转换通道数量
	ADC_InitStructure.ADC_NbrOfChannel = NUMBER_OF_CHANNEL;
	//扫描模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	
	ADC_Init(ADC_x, &ADC_InitStructure);
	// 配置ADC时钟Ｎ狿CLK2的8分频，即9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
		// 配置ADC 通道的转换顺序和采样时间
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL1, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL2, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL3, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL4, 4, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL5, 5, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL6, 6, ADC_SampleTime_55Cycles5);
	// 使能ADC DMA 请求
	ADC_DMACmd(ADC_x, ENABLE);
	// 开启ADC ，并开始转换
	ADC_Cmd(ADC_x, ENABLE);
	// 初始化ADC 校准寄存器  
	ADC_ResetCalibration(ADC_x);
	// 等待校准寄存器初始化完成
	while(ADC_GetResetCalibrationStatus(ADC_x));
	// ADC开始校准
	ADC_StartCalibration(ADC_x);
	// 等待校准完成
	while(ADC_GetCalibrationStatus(ADC_x));
	// 由于没有采用外部触发，所以使用软件触发ADC转换 
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


//获取指定通道的转换值，取times次,然后平均 
//times:获取次数
//返回值:通道ch的times次转换结果平均值
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
 *  湿度传感器对应的引脚是PC4
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

