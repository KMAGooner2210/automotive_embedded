#include "stm32f10x.h"                  // Device header
void Delay_ms(uint32_t ms){
	uint32_t cycles = ms * 8000;
	for(uint32_t i = 0; i < cycles; i++){
			__NOP();
	}
}
int main(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	while(1){
		for(int i =0; i < 4; i++){
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4 << i) == RESET){
				GPIO_SetBits(GPIOA,GPIO_Pin_4 << i);
				Delay_ms(50);
			}else{
			GPIO_ResetBits(GPIOA,GPIO_Pin_4 << i);
		}
	}
}
}
					
	