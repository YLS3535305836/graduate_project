#include "headfile.h"


/* ������ľ�ֵ�˲��������뽫���������Ԫ������Ϊ������ADCͨ�������Ļ���ͬʱȥdma.h���ٴθ��� */
volatile u16 ADCConvertedValue[30];   //��·ADC�ڴ�

/* ��·DMA-ADC��ʼ�� */
void dma_init()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  DMA_InitTypeDef DMA_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE); //APB2���ߣ�GPIOA|ADC1ʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//AHB���ߣ�DMA1ʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//ADC1ʱ��6��Ƶ��12MHz
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1 | GPIO_Pin_2; //ADC1ͨ����ʼ����ch1|ch2|ch3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                        //ģ������
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* DMA1���� */
	DMA_DeInit(DMA1_Channel1);                                         //DMA1����Ϊȱʡֵ
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(ADC1->DR));//ADC1���ݼĴ�����ַ
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADCConvertedValue;     //�ڴ��ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                 //���䷽��:���� -> �ڴ�
  DMA_InitStructure.DMA_BufferSize = 30;                             //���䳤��
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;   //�������:�ر�
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;            //�ڴ����:��
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//���ݿ�ȣ�16λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//���ݿ��:16λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                    //ѭ��ģʽ
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;            //���ȼ�:��
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                       //�ڴ���ڴ�:��
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);                       //��ʼ��DMA1ͨ��1
	
	DMA_Cmd(DMA1_Channel1, ENABLE);                                    //ʹ��DMA1ͨ��1
	
	/* ADC1���� */
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                 //����ģʽ������ģʽ
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;                       //ģʽ����ͨ��ɨ��
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                 //����ģʽ������ת��
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//�������
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;             //ADC�����Ҷ���
  ADC_InitStructure.ADC_NbrOfChannel = 3;                            //ͨ��������3��
  ADC_Init(ADC1, &ADC_InitStructure);                                //��ʼ��ADC1
	
	/* ����ָ��ADC�Ĺ�����ͨ��������ͨ����Ӧ��ת��˳��Ͳ���ʱ�� */
  ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5 );//����ʱ�䣺21.0us
  ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_239Cycles5); //����ʱ�䣺21.0us
  ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 3, ADC_SampleTime_239Cycles5); //����ʱ�䣺21.0us
	
	ADC_DMACmd(ADC1, ENABLE);                                          //ʹ��ADC1��DMA����
  ADC_Cmd(ADC1, ENABLE);                                             //ʹ��ADC1
	
	ADC_ResetCalibration(ADC1);                                        //У�鸴λ
  while(ADC_GetResetCalibrationStatus(ADC1));                        //�ȴ���λ���

  ADC_StartCalibration(ADC1);                                        //��ʼADC1У׼
  while(ADC_GetCalibrationStatus(ADC1));                             //�ȴ�У�����
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);                            //�����������ADC1	
}

  /* ��ֵ�˲� */
/* ����adc2=Get_ADC(2)�� */ //�õ�"ת��˳��"�еڶ���ADCͨ����10��ƽ��ֵ
u16 Get_ADC(u8 num)    //num:�˲�����
{
  uint8_t i;  
  u16 sum = 0;  
	for(i=0;i<10;i++)
	 {
	  sum+= ADCConvertedValue[3*i+(num-1)];
	 }
	return (sum/10);
}


//�õ�ҡ��X�����
u8 Get_X_Value(void)
{
	u16 Xvalue=0;
	Xvalue = Get_ADC(1);
	return (u8)( ((float)Xvalue/(float)4095)*255 );
}

//�õ�ҡ��Y�����
u8 Get_Y_Value(void)
{
	u16 Yvalue=0;
	Yvalue = Get_ADC(2);
	return (u8)( ((float)Yvalue/(float)4095)*255 );
}


