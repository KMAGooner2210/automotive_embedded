#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"


#define DELAY_1_MS 1000

void delay(uint32_t time) {
  for (uint32_t i = 0; i < time; i++) {}
}

void delay_ms(uint32_t ms) {
  for (uint32_t i = 0; i < ms; i++) {
    delay(DELAY_1_MS);
  }
}

void RCC_Config() {
  // C?u h�nh clock cho GPIOA (s? d?ng APB2)
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
}

void GPIO_Config() {
  GPIO_InitTypeDef GPIO_InitStruct;

  // C?u h�nh c�c ch�n PA4, PA5, PA6, PA7 l� output (push-pull)
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz; // T?c d? 10MHz (t�y ch?nh)

  GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void knightRider(uint8_t loop) {
  uint16_t Ledval;

  for (int j = 0; j < loop; j++) {
    // S�ng t? tr�i sang ph?i
    for (int i = 0; i < 4; i++) {
      Ledval = 0x0010 << i;
      GPIOA->ODR &= ~(0x00F0); // T?t c�c LED t? PA4 d?n PA7
      delay_ms(500);          // Delay 0.5 gi�y sau khi t?t
      GPIOA->ODR |= Ledval;     // B?t LED mong mu?n
      delay_ms(2000);         // Delay 2 gi�y
    }
    // S�ng t? ph?i sang tr�i
    for (int i = 2; i >= 0; i--) {
      Ledval = 0x0010 << i;
      GPIOA->ODR &= ~(0x00F0); // T?t c�c LED t? PA4 d?n PA7
      delay_ms(500);          // Delay 0.5 gi�y sau khi t?t
      GPIOA->ODR |= Ledval;     // B?t LED mong mu?n
      delay_ms(2000);         // Delay 2 gi�y
    }
  }
}

int main() {
  RCC_Config();
  GPIO_Config();

  while (1) {
    knightRider(3); // Ch?y hi?u ?ng knightRider 3 l?n
  }
}