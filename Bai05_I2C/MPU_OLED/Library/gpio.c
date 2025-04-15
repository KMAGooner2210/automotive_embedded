#include "stm32f10x.h"                  // Device header

#include "gpio.h"



void GPIO_Config(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure; // No need to initialize here in SPL style

  

  
  GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET); // Reset PC13 output using SPL, Bit_RESET is equivalent to GPIO_PIN_RESET

  /*Configure GPIO pin : PC13 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; // Configure for Pin 13
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // Output Push Pull mode (equivalent to GPIO_MODE_OUTPUT_PP)
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // GPIO speed (equivalent to GPIO_SPEED_FREQ_LOW, but using 50MHz for better practice)
  GPIO_Init(GPIOC, &GPIO_InitStructure); // Initialize GPIOC with the configuration

}