/********************************* �����к�̫�������޹�˾ *******************************
* ʵ �� �� ���������ʵ��
* ʵ��˵�� ��ʹ�ú������������С���ƶ�
* ʵ��ƽ̨ ����̫ARM��Ƭ��������
* ���ӷ�ʽ ����ο�interface.h�ļ�
* ע    �� ��1.���������ӵĲ��죬����ת��ʱ��Ҫ����ʵ����Ҫ���ڲ��ٵĲ�ֵ
*            2.����ʹ�ò�������鱾��Ϊ������
* ��    �� ����̫���Ӳ�Ʒ�з���    QQ ��1909197536
* ��    �� ��http://shop120013844.taobao.com/
****************************************************************************************/

#include "stm32f10x.h"
#include "interface.h"
#include "LCD12864.h"
#include "IRCtrol.h"
#include "motor.h"
#include "UltrasonicCtrol.h"

//ȫ�ֱ�������
unsigned int speed_count=0;//ռ�ձȼ����� 50��һ����
char front_left_speed_duty=SPEED_DUTY;
char front_right_speed_duty=SPEED_DUTY;
char behind_left_speed_duty=SPEED_DUTY;
char behind_right_speed_duty=SPEED_DUTY;

unsigned char tick_5ms = 0;//5ms����������Ϊ�������Ļ�������
unsigned char tick_1ms = 0;//1ms����������Ϊ����Ļ���������
unsigned char tick_200ms = 0;//ˢ����ʾ

char ctrl_comm = COMM_STOP;//����ָ��
char ctrl_comm_last = COMM_STOP;//��һ�ε�ָ��
unsigned char continue_time=0;

void BarrierProc(void)
{
	char void_l,void_r;
	void_l = VOID_L_IO;
	void_r = VOID_R_IO;
	
	if(void_r == BARRIER_Y || void_l == BARRIER_Y)
	{
		CarBack();
		Delayms(500);
		if(void_r == BARRIER_Y && void_l == BARRIER_Y)
		{
			CarBack();
			Delayms(300);			
			CarRight();
			Delayms(400);
			CarGo();
			Delayms(100);
			return;
		}
		
			CarRight();
			Delayms(600);		
			CarGo();
			Delayms(100);
			return;		
	}else
	{
		CarGo();
	}
}

int main(void)
{
	delay_init();
	GPIOCLKInit();
	UserLEDInit();
	LCD12864Init();
//	IRCtrolInit();
	TIM2_Init();
	MotorInit();
	//UltraSoundInit();
	RedRayInit();
	ServoInit();

 while(1)
 {	 
	 		if(tick_5ms >= 5)
		{
			tick_5ms = 0;
			tick_200ms++;
			if(tick_200ms >= 40)
			{
				tick_200ms = 0;
				LEDToggle(LED_PIN);
				LCD12864WriteDistance(distance_cm);//���¾���
			}
//			continue_time--;//200ms �޽���ָ���ͣ��
//			if(continue_time == 0)
//			{
//				continue_time = 1;
//				CarStop();
//			}
			//do something
			BarrierProc();
//			if(ctrl_comm_last != ctrl_comm)//���յ������ź�
//			{
//				ctrl_comm_last = ctrl_comm;
//				switch(ctrl_comm)
//				{
//					case COMM_UP:    CarGo();break;
//					case COMM_DOWN:  CarBack();break;
//					case COMM_LEFT:  CarLeft();break;
//					case COMM_RIGHT: CarRight();break;
//					case COMM_STOP:  CarStop();break;
//					default : break;
//				}
//				LCD12864WriteCommand(ctrl_comm);
//			}
		}
		
 }
}

