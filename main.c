#include "delay.h"
#include "sys.h"
#include "usart.h"	
#include "ADC.h"
#include "LedBeep.h"
#include "WDG.h"

/***************************************************************
Copyright  Elsintech Co., Ltd. All rights reserved.
文件名		: main .c
作者	  	: 张博钊
版本	   	: V1.0
描述	   	: main函数，主线程
其他	   	: 无
网站 	   	: www.elsintech.com
日志	   	: 初版V1.0 2020/6/23 张博钊创建
***************************************************************/
extern u16 adc_value[9];
extern u8 leave_flag;

u8 done_flag[9] = {0};
u8 doing_flag[9] = {0};
u8 standard_set_flag  = 1; //为1时校准 为0时不校准
u16 standard_value[9];

 int main(void)
{

	u8 wdg_flag = 1; //为1时初始化开启看门狗
	u8 t = 0;
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
//	uart_init(115200);	 	//串口初始化为115200
	Adc_Init(); 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	DMA_Cmd(DMA1_Channel1, ENABLE); //启动DMA通道
	TIM3_Int_Init(999,7199);
//	app_ADC1DMA_enable();
	LedBeepKey_Init();
	 
	while(1) 
		{
			t= KEY_Scan(0);
			if(t == 1)
			{
				Reset(&standard_set_flag, done_flag);
			}	
			
			if(standard_set_flag)
			{
				delay_ms(200);
				delay_ms(300);
				delay_ms(200);
				delay_ms(300);
			}
			
			get_adc();	//获取AD值 
			
			standard_set(standard_value, &standard_set_flag); //设定校准值		
			
			if(standard_set_flag != 1)	//若校准正常 开始工作
			{
//				if (wdg_flag)	//开启看门狗
//				{
//					IWDG_Init(4,625); //1.6*625 = 1s
//					wdg_flag = 0;
//				}
				check(standard_value, doing_flag);	//检查各个位置情况			
				LedBeep_Control(adc_value, done_flag);	//LED BEEP控制
				
				if(check_leave(done_flag))
					BEEP = 1;
				
//				IWDG_Feed();
			}
			

				
		}

}







	


	
	










