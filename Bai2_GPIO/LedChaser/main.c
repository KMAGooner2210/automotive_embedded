#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

void delay(uint32_t time) {
  for ( uint32_t i = 0; i < time; i++){} 
}

void chaseLed(uint8_t loop) {
  uint16_t Ledval = 0x0010; //0x000010000
  for (int j = 0; j < loop; j++) {
    for (int i = 0; i < 4; i++) {
      GPIO_Write(GPIOC, Ledval);
      delay(500000);  
      Ledval = Ledval << 1; 
      if (Ledval > 0x0100) { 
        Ledval = 0x0010; 
      }
    }
  }
}

void RCC_Config() {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
}

void GPIO_Config() {
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOC, &GPIO_InitStruct);
}

int main() {
  RCC_Config();
  GPIO_Config();
  while (1) {
    chaseLed(3); 
  }
}
