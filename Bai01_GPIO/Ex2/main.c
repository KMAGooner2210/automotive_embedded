#include "stm32f10x.h"

void GPIO_Config(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
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
		if(GPIO_ReadInputDataBit(GPIOx,pin) == Bit_RESET) return 1;
	}
	return 0;
}

int main(){
	GPIO_Config();
	
	uint32_t on_time = 50;
	uint32_t period = 100;
	
	while(1){
		//Kiem tra nut tang
		if(read_button(GPIOA,GPIO_Pin_1)){
			if(on_time < period) on_time += period / 10;
			while(read_button(GPIOA,GPIO_Pin_1));
		}
		
		//Kiem tra nut giam
		if(read_button(GPIOA,GPIO_Pin_2)){
			if(on_time > 0) on_time -= period / 10;
			while(read_button(GPIOA,GPIO_Pin_2));
		}
		if(on_time > 0){
			GPIO_SetBits(GPIOA,GPIO_Pin_0);
			Delay_ms(on_time);
		}
		if(on_time < period){
			GPIO_ResetBits(GPIOA, GPIO_Pin_0);
			Delay_ms(period - on_time);
		}
	}
}
	




