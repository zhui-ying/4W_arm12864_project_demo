/********************************* �����к�̫�������޹�˾ *******************************
* ʵ �� �� ������������Զ�����ʵ��
* ʵ��˵�� ����������ģ����ڶ����ת���ϣ�ͨ��ת�����������ȡǰ��������Լ��ұߵ��ϰ������
* ʵ��ƽ̨ ����̫ARM��Ƭ��������
* ���ӷ�ʽ ����ο�interface.h�ļ�
* ע    �� �������ת�ǶȽ��鲻Ҫʹ��0���180�㣬������΢���м�һ�㣬����ѡȡ20���160 �㣬���������ת����ȷ
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

unsigned char duoji_count=0;
unsigned char zhuanjiao = 11;

void DuojiMid()
{
	zhuanjiao = 11;
	Delayms(150);//��ʱ1s
}

void DuojiRight()
{
	zhuanjiao = 5;
	Delayms(150);//��ʱ1s
}

void DuojiLeft()
{
	zhuanjiao = 18;
	Delayms(150);//��ʱ1s
}

///��ȡ��������ľ���,����ǰ�������Ϊ��ǰ
void GetAllDistance(unsigned int *dis_left,unsigned int *dis_right,unsigned int *dis_direct)
{
	CarStop();
	GetDistanceDelay();
	*dis_direct = distance_cm;
	
	DuojiRight();
	Delayms(100);
	GetDistanceDelay();//��ȡ�ұ߾���
	*dis_right = distance_cm;
	
	DuojiMid();
	DuojiLeft();
	Delayms(100);
	GetDistanceDelay();//��ȡ��߾���
	*dis_left = distance_cm;
	
	DuojiMid();//��λ
}

void BarrierProc()
{
		if(distance_cm < 10)//ǰ�����ϰ���
	{
		unsigned int dis_left;//��߾���
		unsigned int dis_right;//�ұ߾���
		unsigned int dis_direct;//�ұ߾���
		if(distance_cm < 8)
		{
			CarBack();
			Delayms(400);
		}
		
		while(1)
		{
			GetAllDistance(&dis_left,&dis_right,&dis_direct);
			if(dis_direct < 5)
			{
				CarBack();
				Delayms(300);
				continue;
			}
			else if((dis_left < 5) || (dis_right < 5))
			{
				CarBack();
				Delayms(300);
				continue;
			}
			else if(dis_direct >= dis_left && dis_direct >= dis_right)//ǰ��������Զ
			{
				CarGo();
				Delayms(600);
				return;
			}
			else if(dis_left <= dis_right)//��ת
			{
				CarRight();
				Delayms(500);
			}
			else if(dis_right < dis_left)
			{
				CarLeft();
				Delayms(500);
			}
		}
	}
	else
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
	//IRCtrolInit();
	TIM2_Init();
	MotorInit();
	UltraSoundInit();
	//RedRayInit();
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
				//LCD12864WriteDistance(distance_cm);//���¾���
			}
//			continue_time--;//200ms �޽���ָ���ͣ��
//			if(continue_time == 0)
//			{
//				continue_time = 1;
//				CarStop();
//			}
			//do something
			Distance();//�������
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

