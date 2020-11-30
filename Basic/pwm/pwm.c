#include "sys.h"
#include "pwm.h"

void TIM2_PWM_Init(u16 arr, u16 psc)
{
	GPIO_InitTypeDef gpio;
	TIM_OCInitTypeDef oc;
	TIM_TimeBaseInitTypeDef timebase;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//使能TIM3
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //定时器对应的输出IO端口在PA组。
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	gpio.GPIO_Pin = GPIO_Pin_0;	//TIM_CH3, 是固定的，用户不能随意修改，因此无宏定义
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);
	
	timebase.TIM_Period = arr;	//设置自动重装载值
	timebase.TIM_Prescaler = psc;	//预分频系数
	timebase.TIM_CounterMode = TIM_CounterMode_Up;	//计数器向上溢出
	timebase.TIM_ClockDivision = TIM_CKD_DIV1;	//时钟的分频因子
	TIM_TimeBaseInit(TIM2, &timebase);
	
	oc.TIM_OCMode = TIM_OCMode_PWM1;	// PWM模式1: CNT < CCR 时输出有效电平
	oc.TIM_OCPolarity = TIM_OCPolarity_High;//设置极性-有效电平为：高电平
	oc.TIM_OutputState = TIM_OutputState_Enable;	//输出使能
	TIM_OC1Init(TIM2, &oc);	//OC3指的是通道号3
	
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM2, ENABLE);
}