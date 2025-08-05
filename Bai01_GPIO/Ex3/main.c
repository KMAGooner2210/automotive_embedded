#include "stm32f10x.h"                  // Device header

void GPIO_Config(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA ,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}
void Delay_ms(uint32_t ms){
	uint32_t cycles = ms * 8000;
	for(uint32_t i = 0; i < cycles; i++){
		__NOP();
	}
}

uint8_t read_button(GPIO_TypeDef* GPIOx,uint16_t pin){
	if(GPIO_ReadInputDataBit(GPIOx,pin) == Bit_RESET){
		Delay_ms(20);
		if(GPIO_ReadInputDataBit(GPIOx,pin) == Bit_RESET)return 1;
	}
	return 0;
}

int main(void){
	GPIO_Config();
	uint8_t mode = 0; // 0: 1s, 1: 0.5s, 2: 0.25s, 3: tat
	while(1){
		if(read_button(GPIOA,GPIO_Pin_1)){
			mode = (mode + 1) % 4; //0->1->2->3->0
	    while(read_button(GPIOA,GPIO_Pin_1));
		}
    switch(mode){
			case 0: 
				GPIO_SetBits(GPIOA,GPIO_Pin_0);
			  Delay_ms(500);
			  GPIO_ResetBits(GPIOA,GPIO_Pin_0);
				Delay_ms(500);
				break;
			case 1:
				GPIO_SetBits(GPIOA,GPIO_Pin_0);
			  Delay_ms(250);
			  GPIO_ResetBits(GPIOA,GPIO_Pin_0);
				Delay_ms(250);
				break;	
 			case 2:
				GPIO_SetBits(GPIOA,GPIO_Pin_0);
			  Delay_ms(125);
			  GPIO_ResetBits(GPIOA,GPIO_Pin_0);
				Delay_ms(125);
				break;
			case 3:
				GPIO_ResetBits(GPIOA,GPIO_Pin_0);
			  break;
		}
	}
}