#include "stm32f10x.h"


void Delay_ms(uint32_t ms){
	uint32_t cycles = ms * 8000;
	for(uint32_t i = 0; i < cycles; i++){
		__NOP();
	}
}
int main(void) {
   
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
  
    while (1) {
        
        GPIO_SetBits(GPIOA, GPIO_Pin_1);
        GPIO_ResetBits(GPIOA, GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4);
        Delay_ms(500); 
        
        GPIO_SetBits(GPIOA, GPIO_Pin_2);
        GPIO_ResetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4);
        Delay_ms(500);

       
        GPIO_SetBits(GPIOA, GPIO_Pin_3);
        GPIO_ResetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_4);
        Delay_ms(500);

        
        GPIO_SetBits(GPIOA, GPIO_Pin_4);
        GPIO_ResetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);
        Delay_ms(500);

        
        GPIO_ResetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4);
        Delay_ms(500);
    }
}
