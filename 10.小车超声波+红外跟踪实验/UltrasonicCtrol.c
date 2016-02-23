#include "UltrasonicCtrol.h"
#include "interface.h"
#include "stm32f10x.h"

volatile unsigned char status = 0;//����ǰ״̬��0,���� 1 ���ʹ����źţ�2 �ȴ��źŷ���,3 
unsigned int dis_count = 0;//������ʱ
volatile unsigned int distance_cm = 0;//��ǰ����
unsigned char t2_full_count = 0;//������������������
static unsigned int ultick_5ms = 0;//5ms������

void Time4Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    TIM_TimeBaseStructure.TIM_Period = 65535;
    TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);//72M / 72 = 1us
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
}

//�ⲿ�ж����� ������,����û�õ��жϼ�����ʽ
void UltraSoundInit(void)	
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	EXTI_InitTypeDef 	EXTI_InitStructure;//����һ���ⲿ�ж���صĽṹ��
	NVIC_InitTypeDef NVIC_InitStructure; //����һ���жϵĽṹ��
	
	GPIO_InitStructure.GPIO_Pin = Echo_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����GPIO�˿��ٶ�
	GPIO_Init(IRIN_GPIO , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Trig_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����GPIO�˿��ٶ�
	GPIO_Init(Trig_GPIO , &GPIO_InitStructure);

	GPIO_EXTILineConfig(Echo_PORTSOURCE , Echo_PINSOURCE);//��GPIO�����ó��ⲿ�ж� 
	EXTI_InitStructure.EXTI_Line = Echo_EXITLINE;//����Ӧ��GPIO�����ӵ��ж�����
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�ж��¼����ͣ��½���
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//ѡ��ģʽ���ж���
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;//ʹ�ܸ��ж�
	EXTI_Init(&EXTI_InitStructure);//�����úõĲ���д��Ĵ���
			
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	 //�׼�Ϊ0������Ƕ��
	NVIC_InitStructure.NVIC_IRQChannel = 	Echo_IRQCH;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//�����ȼ�0�����
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	//�����ȼ������
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//ʹ�ܸ�ģ���ж�
	NVIC_Init(&NVIC_InitStructure);	//�жϳ�ʼ�������ṹ�嶨�������ִ��
	
	Time4Init();
	Trig_RESET;
}

void Distance(void)
{
	ultick_5ms++;
	if(ultick_5ms >= 15)
	{
		ultick_5ms = 0;
		GetDistanceDelay();
	}
}

//��ʱ�ķ�ʽ��ȡ����ֵ
void GetDistanceDelay(void)
{
		//���ʹ����ź�
		Trig_SET;
		status = 1;
	TIM_SetCounter(TIM4, 0);
	TIM_Cmd(TIM4, ENABLE);//�򿪶�ʱ��
		while(TIM_GetCounter(TIM4) < 11);//��ʱ����10us
		status = 2;
		Trig_RESET;
		TIM_SetCounter(TIM4, 0);
	
		while(Echo == 0)//�ȴ������ź���ʼλ��
		{
			if(TIM_GetCounter(TIM4) > 60000)//��Ӧʱ�䳬��������Χ 
			{
				status = 0;
				distance_cm = 0;//ʧ�ܺ�ͺ���
				TIM_Cmd(TIM4, DISABLE);//�رն�ʱ��
				return;//����ʧ��
			}
		}
		TIM_SetCounter(TIM4, 0);
		while(Echo == 1)//��ʼ���㳤��
		{
			int count = TIM_GetCounter(TIM4);
			if(count > 60000)//��Ӧʱ�䳬��������Χ 
			{
				status = 0;
				distance_cm = 0;//ʧ�ܺ�ͺ���
				TIM_Cmd(TIM4, DISABLE);//�رն�ʱ��
				return;//����ʧ��
			}
		}
		dis_count = TIM_GetCounter(TIM4);
		TIM_Cmd(TIM4, DISABLE);//�رն�ʱ��
		distance_cm = (unsigned int)(((long)(dis_count) * 34)/2000);//����340m/s
		status = 0;//׼���´η���		
}



