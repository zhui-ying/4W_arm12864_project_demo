/********************************* 深圳市航太电子有限公司 *******************************
* 实 验 名 ：测试程序
* 实验说明 ：使用红外或蓝牙遥控来控制小车运动，实时刷新显示五路光电对管状态
* 实验平台 ：航太ARM单片机开发板
* 连接方式 ：请参考interface.h文件
* 注    意 ：指令必须连续发送才会使小车动作，若停止发送指令，则小车会停止
* 作    者 ：航太电子产品研发部    QQ ：1909197536
* 店    铺 ：http://shop120013844.taobao.com/
****************************************************************************************/

#include "stm32f10x.h"
#include "interface.h"
#include "LCD12864.h"
#include "IRCtrol.h"
#include "motor.h"
#include "uart.h"

//全局变量定义
unsigned int speed_count=0;//占空比计数器 50次一周期
char front_left_speed_duty=SPEED_DUTY;
char front_right_speed_duty=SPEED_DUTY;
char behind_left_speed_duty=SPEED_DUTY;
char behind_right_speed_duty=SPEED_DUTY;

unsigned char tick_5ms = 0;//5ms计数器，作为主函数的基本周期
unsigned char tick_1ms = 0;//1ms计数器，作为电机的基本计数器
unsigned char tick_200ms = 0;//刷新显示
unsigned char tick_50ms = 0;//刷新光电对管检测

char ctrl_comm = COMM_STOP;//控制指令
unsigned char continue_time=0;
unsigned char bt_rec_flag=0;//蓝牙控制标志位

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
			
			continue_time--;//200ms 无接收指令就停车
			if(continue_time == 0)
			{
				continue_time = 1;
				CarStop();
			}
			//do something
			if(bt_rec_flag == 1 | ir_rec_flag == 1)//接收到红外信号
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

