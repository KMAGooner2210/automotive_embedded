#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_spi.h"              // Keil::Device:StdPeriph Drivers:SPI
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART
#include <stdio.h>
#define SPI1_NSS      GPIO_Pin_4
#define SPI1_SCK      GPIO_Pin_5
#define SPI1_MISO     GPIO_Pin_6
#define SPI1_MOSI     GPIO_Pin_7

#define USART1_TX     GPIO_Pin_9
//#define USART1_RX     GPIO_Pin_10


// Bien toan cuc cho ngat Timer
volatile uint32_t delay_counter = 0;

void RCC_Config(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1 | RCC_APB2Periph_USART1,ENABLE);
}

void TIM_Config(void){
TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;
TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
TIM_TimeBaseInitStructure.TIM_Period = 9;
TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    
    // C?u hình NVIC cho Timer 2
NVIC_InitTypeDef NVIC_InitStructure;
NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);
    
TIM_Cmd(TIM2,ENABLE);
}
void TIM_IRQHandler(void){
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET){
		 TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
		 if (delay_counter > 0) {
            delay_counter--;
		 }
	 }
}

void Delay_ms(uint32_t time){
	delay_counter = time;
	TIM_SetCounter(TIM2,0);
	while(TIM_GetCounter(TIM2)< delay_counter);

}



void GPIO_Config(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//NSS
	GPIO_InitStructure.GPIO_Pin = SPI1_NSS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//SCK,MOSI
	GPIO_InitStructure.GPIO_Pin= SPI1_SCK | SPI1_MOSI;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//MISO
	GPIO_InitStructure.GPIO_Pin = SPI1_MISO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//UART TX
	GPIO_InitStructure.GPIO_Pin = USART1_TX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
}

void SPI_Config(void){
	SPI_InitTypeDef SPI_InitStructure;
	
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	
	SPI_Init(SPI1,&SPI_InitStructure);
	SPI_Cmd(SPI1,ENABLE);
}

void USART_Config(void){
	USART_InitTypeDef USART_InitStructure;
	
	USART_InitStructure.USART_Mode = USART_Mode_Tx ;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	
	USART_Init(USART1,&USART_InitStructure);
	USART_Cmd(USART1,ENABLE);
}
	
//Ham chuyen huong printf qua UART
int __io_putchar(int ch){
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
	USART_SendData(USART1,(uint8_t)ch);
	return ch;
}
int main(void){
	RCC_Config();
	GPIO_Config();
	TIM_Config();
	SPI_Config();
	USART_Config();
	
	uint8_t tx_data = 0x55;
	uint8_t rx_data = 0x00;
	
	printf("Master Started\r\n");
	while(1){
		GPIO_ResetBits(GPIOA,SPI1_NSS);
		
		SPI_I2S_SendData(SPI1,tx_data);
		while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET);
		
		while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET);
		rx_data = SPI_I2S_ReceiveData(SPI1);
		
		GPIO_SetBits(GPIOA,SPI1_NSS);
		// In d? li?u nh?n du?c
        printf("Master Received: 0x%02X\r\n", rx_data);

        Delay_ms(1000); 
	}
}


