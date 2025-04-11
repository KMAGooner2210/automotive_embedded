#include "stm32f10x.h"
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
void RCC_Config(void);
void GPIO_Config(void);
void USART_Config(void);
void USART_Send_Char(USART_TypeDef* USARTx,uint16_t Data);

void RCC_Config(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);
}
	
void GPIO_Config(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //chan Tx 9.3.8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //chan Rx 9.3.8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}
void USART_Config(void){
	USART_InitTypeDef USART_InitStructure;
  USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_Mode = USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	USART_Init(USART1,&USART_InitStructure);
	USART_Cmd(USART1,ENABLE);
}
void USART_Send_Char(USART_TypeDef* USARTx, uint16_t Data){
	while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET){}
		USART_SendData(USARTx,Data);
	}
int main(void){
	RCC_Config();
	GPIO_Config();
	USART_Config();
	USART_Send_Char(USART1,'T');
	while(1){
		
	}
}
