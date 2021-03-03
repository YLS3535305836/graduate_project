#include "headfile.h"
#include "irq.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* ͨ�ö�ʱ��3�жϷ�����(�������ж�������) */
void TIM3_IRQHandler(void)   //TIM3�ж�
{	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
  {
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־ 	
		
		
		
		
	}
}


/* �߼���ʱ��1�жϷ�����(�������ж�������) */
void TIM1_UP_IRQHandler(void) 
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)//���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);//���TIMx���жϴ�����λ:TIM �ж�Դ 
		
		
			
	}	     
} 

/* ͨ�ö�ʱ��2�жϷ�����(�������ж�������) */
void TIM2_IRQHandler(void)   //TIM2�ж�
{	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM2�����жϷ������
	{
		 TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
		 
		 	
			

	}		 
 }


/* ͨ�ö�ʱ��4�жϷ�����(�������ж�������) */
void TIM4_IRQHandler(void)   //TIM4�ж�
{	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //���TIM4�����жϷ������
	{
		 TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
		 
					
			

	}		 
 }

/* ͨ�ö�ʱ��5�жϷ�����(�������ж�������) */
void TIM5_IRQHandler(void)   //TIM5�ж�
{	
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //���TIM5�����жϷ������
	{
		 TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
		 
					
			

	}		 
}


/* �ⲿ�ж�1�жϷ�����(�������ж�������) */ 
void EXTI1_IRQHandler(void)
{
	
	
	
	
	EXTI_ClearITPendingBit(EXTI_Line1);  //���LINE2�ϵ��жϱ�־λ  
}

/* �ⲿ�ж�2�жϷ�����(�������ж�������) */ 
void EXTI2_IRQHandler(void)
{

	
	
	
	EXTI_ClearITPendingBit(EXTI_Line2);  //���LINE2�ϵ��жϱ�־λ  
}

/* �ⲿ�ж�3�жϷ�����(�������ж�������) */ 
void EXTI3_IRQHandler(void)
{
	
	
	
	
	EXTI_ClearITPendingBit(EXTI_Line3);  //���LINE3�ϵ��жϱ�־λ  
}

/* �ⲿ�ж�4�жϷ�����(�������ж�������) */ 
void EXTI4_IRQHandler(void)
{
	
	
	
	
	EXTI_ClearITPendingBit(EXTI_Line4);  //���LINE4�ϵ��жϱ�־λ  
}


