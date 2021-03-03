#include "headfile.h"


/* 若想更改均值滤波基数，请将这里的数组元素数改为基数与ADC通道个数的积，同时去dma.h中再次更改 */
volatile u16 ADCConvertedValue[30];   //三路ADC内存

/* 多路DMA-ADC初始化 */
void dma_init()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  DMA_InitTypeDef DMA_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE); //APB2总线：GPIOA|ADC1时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//AHB总线：DMA1时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//ADC1时钟6分频，12MHz
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1 | GPIO_Pin_2; //ADC1通道初始化：ch1|ch2|ch3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                        //模拟输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* DMA1配置 */
	DMA_DeInit(DMA1_Channel1);                                         //DMA1设置为缺省值
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(ADC1->DR));//ADC1数据寄存器地址
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADCConvertedValue;     //内存地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                 //传输方向:外设 -> 内存
  DMA_InitStructure.DMA_BufferSize = 30;                             //传输长度
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;   //外设递增:关闭
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;            //内存递增:打开
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//数据宽度：16位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//数据宽度:16位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                    //循环模式
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;            //优先级:高
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                       //内存→内存:否
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);                       //初始化DMA1通道1
	
	DMA_Cmd(DMA1_Channel1, ENABLE);                                    //使能DMA1通道1
	
	/* ADC1配置 */
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                 //工作模式：独立模式
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;                       //模式：多通道扫描
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                 //工作模式：连续转换
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//软件触发
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;             //ADC数据右对齐
  ADC_InitStructure.ADC_NbrOfChannel = 3;                            //通道个数：3个
  ADC_Init(ADC1, &ADC_InitStructure);                                //初始化ADC1
	
	/* 设置指定ADC的规则组通道，设置通道对应的转化顺序和采样时间 */
  ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5 );//采样时间：21.0us
  ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_239Cycles5); //采样时间：21.0us
  ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 3, ADC_SampleTime_239Cycles5); //采样时间：21.0us
	
	ADC_DMACmd(ADC1, ENABLE);                                          //使能ADC1的DMA功能
  ADC_Cmd(ADC1, ENABLE);                                             //使能ADC1
	
	ADC_ResetCalibration(ADC1);                                        //校验复位
  while(ADC_GetResetCalibrationStatus(ADC1));                        //等待复位完成

  ADC_StartCalibration(ADC1);                                        //开始ADC1校准
  while(ADC_GetCalibrationStatus(ADC1));                             //等待校验完成
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);                            //启动软件触发ADC1	
}

  /* 均值滤波 */
/* 例：adc2=Get_ADC(2)； */ //得到"转换顺序"中第二个ADC通道的10次平均值
u16 Get_ADC(u8 num)    //num:滤波对象
{
  uint8_t i;  
  u16 sum = 0;  
	for(i=0;i<10;i++)
	 {
	  sum+= ADCConvertedValue[3*i+(num-1)];
	 }
	return (sum/10);
}


//得到摇杆X轴分量
u8 Get_X_Value(void)
{
	u16 Xvalue=0;
	Xvalue = Get_ADC(1);
	return (u8)( ((float)Xvalue/(float)4095)*255 );
}

//得到摇杆Y轴分量
u8 Get_Y_Value(void)
{
	u16 Yvalue=0;
	Yvalue = Get_ADC(2);
	return (u8)( ((float)Yvalue/(float)4095)*255 );
}


