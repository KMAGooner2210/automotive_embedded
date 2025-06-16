#include "stm32f10x.h"                  // Device header
#include<stdio.h>

volatile uint32_t counter = 0; //Bien dem cho ngat
volatile uint32_t second_counter = 0; //Bien dem so giay
volatile uint8_t flag_display = 0;
char buffer[50];

void RCC_Config(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
}


void TIM2_Config(void){
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;
	TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;
  
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(TIM2,ENABLE);
}

void UART1_Config(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure);
	
	USART_Cmd(USART1,ENABLE);
}

void UART1_SendString(char* str){
	while(*str){
		while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
		USART_SendData(USART1,*str++);
	}
}


void TIM2_IRQHandler(void) {
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
        counter++; // Tang bi?n d?m m?i 100ms
        
        // Ki?m tra n?u d? 1 giây (10 l?n ng?t 100ms)
        if (counter % 10 == 0) {
            second_counter++; // Tang bi?n d?m s? giây
            flag_display = 1; // Ð?t c? d? hi?n th? trong main
        }
        
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // Xóa c? ng?t
    }
}
int main(void){
	RCC_Config();
	TIM2_Config();
	UART1_Config();
	
	while(1){
	if (flag_display) {
            sprintf(buffer, "Counter: %lu\r\n", second_counter);
            UART1_SendString(buffer);
            flag_display = 0; // Reset c?
        }
    }
}
