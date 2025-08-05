#include "stm32f10x.h"                  // Device header


void Delay_ms(uint32_t ms){
	uint32_t cycles = ms * 8000;
	for(uint32_t i = 0; i < cycles; i++){
		__NOP();
	}
}

int main(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	while(1){
	for(int i = 0;i < 4; i++){
		GPIO_ResetBits(GPIOA,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);
		GPIO_SetBits(GPIOA,1 << i);
		Delay_ms(200);
	 }
  }
}