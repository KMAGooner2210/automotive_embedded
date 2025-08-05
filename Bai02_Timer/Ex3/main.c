#include "stm32f10x.h"                  // Device header

volatile uint8_t duty_cycle = 0;

void RCC_Config(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
}
void GPIO_Config(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

void TIM3_PWM_Config(void){
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM3,ENABLE);
	
	TIM_Cmd(TIM3,ENABLE);
}
	
void Delay_ms(uint32_t ms){
	volatile uint32_t i, j;
	for(i = 0;i < ms; i++)
		for(j = 0;j < 7200; j++);
	}

int main(void){
	RCC_Config();
	GPIO_Config();
	TIM3_PWM_Config();
	
	while(1){
		//Nut tang
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0) == 0){
			Delay_ms(50);
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0) == 0){
				if(duty_cycle < 100){
					duty_cycle += 10;
					TIM_SetCompare1(TIM3,duty_cycle);
				}
				while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0) == 0);
			}
	}
		
	  //Nut giam
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1) == 0){
		Delay_ms(50);
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1) == 0){
				if(duty_cycle > 0){
					duty_cycle -= 10;
					TIM_SetCompare1(TIM3,ENABLE);
				}
				while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1) == 0);
			}
		}
	}
}
	