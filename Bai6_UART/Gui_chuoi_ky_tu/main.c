#include "stm32f10x.h"
void GPIO_Config(void);
void USART_Config(void);
void USART_PutChar(char ch);
void USART_PutString(char *str);

void GPIO_Config(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}
void USART_Config(void){
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1,ENABLE);
}
void USART_PutChar(char ch){
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET){}
		USART_SendData(USART1, ch);
	}
void USART_PutString(char *str){
	while(*str){
		USART_PutChar(*str++);
	}
}
int main (void){
	GPIO_Config();
	USART_Config();
	USART_PutString("I love KMA!\r\n");
	while(1){
		//vong lap giu chuong trinh khong ket thuc sau khi gui chuoi ky tu
	}
}
