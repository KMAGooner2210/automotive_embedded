#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_spi.h"              // Keil::Device:StdPeriph Drivers:SPI
#include "stm32f10x_dma.h"              // Keil::Device:StdPeriph Drivers:DMA

#define SPI1_GPIO       GPIOA
#define SPI1_NSS        GPIO_Pin_4
#define SPI1_SCK        GPIO_Pin_5
#define SPI1_MISO       GPIO_Pin_6
#define SPI1_MOSI       GPIO_Pin_7

#define MIN_PULSE_WIDTH 1000
#define MAX_PULSE_WIDTH 2000

void RCC_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1 ,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
}

void GPIO_Config(){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SPI1_NSS | SPI1_MOSI | SPI1_SCK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI1_GPIO,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SPI1_MISO; // PA6 - MISO (Slave Output)
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // Correct for Slave Output
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SPI1_GPIO,&GPIO_InitStructure);
}

void TIM_Config(){
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;  //1 us dem len 1 lan
	TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;  //chu ky xung la 20ms
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0xFFFF;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);
	TIM_Cmd(TIM2,ENABLE);
}
uint16_t buffer;

void DMA_Config(){
	DMA_InitTypeDef DMA_InitStructure;
	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStructure.DMA_BufferSize = 50;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)buffer;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	
	DMA_Init(DMA1_Channel2,&DMA_InitStructure);
	DMA_Cmd(DMA1_Channel2,ENABLE);
	
}
void SPI_Config(){
	SPI_InitTypeDef SPI_InitStructure;
	
	SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI1,&SPI_InitStructure);
	SPI_Cmd(SPI1,ENABLE);
	SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Rx,ENABLE);
}

void Servo_Angle(uint16_t angle){
	if(angle > 180) angle = 180;
	uint8_t pulseWidth = MIN_PULSE_WIDTH + ( MAX_PULSE_WIDTH - MIN_PULSE_WIDTH)*angle/180;
	TIM_SetCompare1(TIM2,pulseWidth);
}

int main(){
	RCC_Config();
	GPIO_Config();
	TIM_Config();
	DMA_Config();
	SPI_Config();
	while(1){
		if(DMA_GetFlagStatus(DMA1_FLAG_TC2) != RESET){
			DMA_ClearFlag(DMA1_FLAG_TC2);
			uint16_t angle = (buffer*180)/4096;
			Servo_Angle(angle);
		}
	}
}

			