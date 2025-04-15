#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC

#define LED_GPIO_PORT          GPIOC
#define LED_GPIO_PIN           GPIO_Pin_13
#define LED_GPIO_CLK           RCC_APB2Periph_GPIOC

void GPIO_Config(void);
void USART_Config(void);
void RCC_Config(void);
uint8_t USART_Receive_Char(void);

void RCC_Config(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1 | LED_GPIO_CLK,ENABLE);
}
void GPIO_Config(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = LED_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LED_GPIO_PORT ,&GPIO_InitStructure);
}

void USART_Config(void){
	
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure);
	
	USART_Cmd(USART1,ENABLE);
}
uint8_t USART_Receive_Char(void){
	while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == RESET);
	return (uint8_t)USART_ReceiveData(USART1);
}
int main(void){
	RCC_Config();
	GPIO_Config();
	USART_Config();
	while(1)
	{
		uint8_t receivedChar = USART_Receive_Char();
		if(receivedChar == '1')
			{
			GPIO_SetBits(LED_GPIO_PORT,LED_GPIO_PIN);
			}
    else if(receivedChar == '0'){
      GPIO_ResetBits(LED_GPIO_PORT,LED_GPIO_PIN);
		}
	}
}
