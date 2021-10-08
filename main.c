#include "delay.h"
#include "sys.h"
#include "usart.h"	
#include "ADC.h"
#include "LedBeep.h"
#include "WDG.h"

/***************************************************************
Copyright  Elsintech Co., Ltd. All rights reserved.
�ļ���		: main .c
����	  	: �Ų���
�汾	   	: V1.0
����	   	: main���������߳�
����	   	: ��
��վ 	   	: www.elsintech.com
��־	   	: ����V1.0 2020/6/23 �Ų��ȴ���
***************************************************************/
extern u16 adc_value[9];
extern u8 leave_flag;

u8 done_flag[9] = {0};
u8 doing_flag[9] = {0};
u8 standard_set_flag  = 1; //Ϊ1ʱУ׼ Ϊ0ʱ��У׼
u16 standard_value[9];

 int main(void)
{

	u8 wdg_flag = 1; //Ϊ1ʱ��ʼ���������Ź�
	u8 t = 0;
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
//	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	Adc_Init(); 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	DMA_Cmd(DMA1_Channel1, ENABLE); //����DMAͨ��
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
			
			get_adc();	//��ȡADֵ 
			
			standard_set(standard_value, &standard_set_flag); //�趨У׼ֵ		
			
			if(standard_set_flag != 1)	//��У׼���� ��ʼ����
			{
//				if (wdg_flag)	//�������Ź�
//				{
//					IWDG_Init(4,625); //1.6*625 = 1s
//					wdg_flag = 0;
//				}
				check(standard_value, doing_flag);	//������λ�����			
				LedBeep_Control(adc_value, done_flag);	//LED BEEP����
				
				if(check_leave(done_flag))
					BEEP = 1;
				
//				IWDG_Feed();
			}
			

				
		}

}







	


	
	










