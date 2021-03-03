
// 基本定时器TIMx,x[6,7]定时应用
#include "stm32f10x.h"
#include "bsp_TiMbase.h"
#include "oled.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "sim_iic.h"
#include "motor.h"
  volatile uint32_t time = 0; // ms 计时变量 
  uint16_t second=0;
	uint16_t minute=0;
	short g[3]={0};
	float pitch,roll,yaw; 		//欧拉角
/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{
	/* led 端口配置 */ 
  
	delay_init(72);                 //延时初始化
	OLED_Init();
	OLED_Clear();
  MPU_Init();				            	//初始化MPU6050
  MiniBalance_PWM_Init(9999,71);
//	BASIC_TIM_Init();

	while(mpu_dmp_init())
	{
		OLED_ShowString(20,0,"MPU6050 Error");
		delay_ms(200);

	}
//	    OLED_ShowNum(20,0,mpu_dmp_init(),1,16);
  while(1)
  {
		Read_DMP();
		OLED_ShowAngle(15,6,Pitch,2,3,16);
//		PWMA=2000;
//		PWMB=2000;
//		AIN2=1;			AIN1=0;
//		BIN2=1;			BIN1=0;

      	
     			
  }
}
/*********************************************END OF FILE**********************/

