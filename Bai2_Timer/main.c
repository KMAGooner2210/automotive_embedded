#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

void delay_ms(uint16_t time){
	TIM_SetCounter(TIM2,0);
	while(TIM_GetCounter(TIM2) < time*10);
}
void RCC_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM2EN,ENABLE);
}
void GPIO_Config(){
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}
void TIM_Config(){
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStructure.TIM_Prescaler = 7200-1;
	TIM_InitStructure.TIM_Period = 0xFFFF;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM2,&TIM_InitStructure);
	TIM_Cmd(TIM2,ENABLE);
}
int main(){
	RCC_Config();
	GPIO_Config();
	TIM_Config();
	while(1){
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
		delay_ms(1000);
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		delay_ms(1000);
	}
}
