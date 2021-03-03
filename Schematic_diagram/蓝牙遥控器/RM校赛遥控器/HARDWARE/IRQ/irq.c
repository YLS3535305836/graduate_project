#include "headfile.h"
#include "irq.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* 通用定时器3中断服务函数(声明见中断向量表) */
void TIM3_IRQHandler(void)   //TIM3中断
{	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
  {
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志 	
		
		
		
		
	}
}


/* 高级定时器1中断服务函数(声明见中断向量表) */
void TIM1_UP_IRQHandler(void) 
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)//检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);//清除TIMx的中断待处理位:TIM 中断源 
		
		
			
	}	     
} 

/* 通用定时器2中断服务函数(声明见中断向量表) */
void TIM2_IRQHandler(void)   //TIM2中断
{	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM2更新中断发生与否
	{
		 TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx更新中断标志 
		 
		 	
			

	}		 
 }


/* 通用定时器4中断服务函数(声明见中断向量表) */
void TIM4_IRQHandler(void)   //TIM4中断
{	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //检查TIM4更新中断发生与否
	{
		 TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx更新中断标志 
		 
					
			

	}		 
 }

/* 通用定时器5中断服务函数(声明见中断向量表) */
void TIM5_IRQHandler(void)   //TIM5中断
{	
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //检查TIM5更新中断发生与否
	{
		 TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );  //清除TIMx更新中断标志 
		 
					
			

	}		 
}


/* 外部中断1中断服务函数(声明见中断向量表) */ 
void EXTI1_IRQHandler(void)
{
	
	
	
	
	EXTI_ClearITPendingBit(EXTI_Line1);  //清除LINE2上的中断标志位  
}

/* 外部中断2中断服务函数(声明见中断向量表) */ 
void EXTI2_IRQHandler(void)
{

	
	
	
	EXTI_ClearITPendingBit(EXTI_Line2);  //清除LINE2上的中断标志位  
}

/* 外部中断3中断服务函数(声明见中断向量表) */ 
void EXTI3_IRQHandler(void)
{
	
	
	
	
	EXTI_ClearITPendingBit(EXTI_Line3);  //清除LINE3上的中断标志位  
}

/* 外部中断4中断服务函数(声明见中断向量表) */ 
void EXTI4_IRQHandler(void)
{
	
	
	
	
	EXTI_ClearITPendingBit(EXTI_Line4);  //清除LINE4上的中断标志位  
}


