/********************************* �����к�̫�������޹�˾ *******************************
* ʵ �� �� �����Գ���
* ʵ��˵�� ��ʹ�ú��������ң��������С���˶���ʵʱˢ����ʾ��·���Թ�״̬
* ʵ��ƽ̨ ����̫ARM��Ƭ��������
* ���ӷ�ʽ ����ο�interface.h�ļ�
* ע    �� ��ָ������������ͲŻ�ʹС����������ֹͣ����ָ���С����ֹͣ
* ��    �� ����̫���Ӳ�Ʒ�з���    QQ ��1909197536
* ��    �� ��http://shop120013844.taobao.com/
****************************************************************************************/

#include "stm32f10x.h"
#include "interface.h"
#include "LCD12864.h"
#include "IRCtrol.h"
#include "motor.h"
#include "uart.h"

//ȫ�ֱ�������
unsigned int speed_count=0;//ռ�ձȼ����� 50��һ����
char front_left_speed_duty=SPEED_DUTY;
char front_right_speed_duty=SPEED_DUTY;
char behind_left_speed_duty=SPEED_DUTY;
char behind_right_speed_duty=SPEED_DUTY;

unsigned char tick_5ms = 0;//5ms����������Ϊ�������Ļ�������
unsigned char tick_1ms = 0;//1ms����������Ϊ����Ļ���������
unsigned char tick_200ms = 0;//ˢ����ʾ
unsigned char tick_50ms = 0;//ˢ�¹��Թܼ��

char ctrl_comm = COMM_STOP;//����ָ��
unsigned char continue_time=0;
unsigned char bt_rec_flag=0;//�������Ʊ�־λ

int main(void)
{
	delay_init();
	GPIOCLKInit();
	UserLEDInit();
	LCD12864Init();
	IRCtrolInit();
	TIM2_Init();
	MotorInit();
	ServoInit();
	USART3Conf(9600);
	RedRayInit();

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
			}
			tick_50ms++;
			if(tick_50ms >= 10)
			{
				char void_l,search_l,search_m,search_r,void_r;
				search_m = 'M';
				search_r = 'M';
				search_l = 'M';
				void_l = 'M';
				void_r = 'M';
				tick_50ms = 0;
				if(SEARCH_M_IO == 1) search_m = '0';
				if(SEARCH_L_IO == 1) search_l = '0';
				if(SEARCH_R_IO == 1) search_r = '0';
				if(VOID_R_IO == 1) void_r = '0';
				if(VOID_L_IO == 1) void_l = '0';
				LCD12864ShowRedRay(void_l , search_l , search_m , search_r , void_r);
			}
			
			continue_time--;//200ms �޽���ָ���ͣ��
			if(continue_time == 0)
			{
				continue_time = 1;
				CarStop();
			}
			//do something
			if(bt_rec_flag == 1 | ir_rec_flag == 1)//���յ������ź�
			{
				bt_rec_flag = 0;
				ir_rec_flag = 0;
				switch(ctrl_comm)
				{
					case COMM_UP:    CarGo();break;
					case COMM_DOWN:  CarBack();break;
					case COMM_LEFT:  CarLeft();break;
					case COMM_RIGHT: CarRight();break;
					case COMM_STOP:  CarStop();break;
					default : break;
				}
				LCD12864WriteCommand(ctrl_comm);
			}
		}
		
 }
}

