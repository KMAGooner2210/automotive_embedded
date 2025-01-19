#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

void delay(uint32_t time) {
  for ( uint32_t i = 0; i < time; i++){} 
}



void RCC_Config() {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_GPIOA , ENABLE);
}

void GPIO_Config() {
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOC, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
}

int main() {
  RCC_Config();
  GPIO_Config();
  while (1) {
    if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 0){
			while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 0){}
				if(GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_13)){
					GPIO_ResetBits(GPIOC,GPIO_Pin_13);
				}else{
						GPIO_SetBits(GPIOC,GPIO_Pin_13);
    }
    }
    }
		}
