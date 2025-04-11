#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_spi.h"              // Keil::Device:StdPeriph Drivers:SPI
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "kalman.h"

#define SPI1_NSS      GPIO_Pin_4
#define SPI1_SCK      GPIO_Pin_5
#define SPI1_MISO     GPIO_Pin_6
#define SPI1_MOSI     GPIO_Pin_7
#define SPI1_GPIO     GPIOA

void RCC_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1 | RCC_APB2Periph_SPI1 ,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
}

void GPIO_Config(){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SPI1_MISO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI1_GPIO,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SPI1_NSS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI1_GPIO,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SPI1_SCK | SPI1_MOSI;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // **S?a thành AF_PP**
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI1_GPIO,&GPIO_InitStructure);
}

void TIM_Config(){
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
  TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;
  TIM_TimeBaseInitStructure.TIM_Period = 0xFFFF;
  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
  TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	TIM_Cmd(TIM2,ENABLE);
}

void SPI_Config(){
	SPI_InitTypeDef SPI_InitStructure;
	
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	
	SPI_Init(SPI1,&SPI_InitStructure);
	SPI_Cmd(SPI1,ENABLE);
}
void ADC_Config(){
	ADC_InitTypeDef ADC_InitStructure;
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	
	ADC_Init(ADC1,&ADC_InitStructure);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_28Cycles5);
	ADC_Cmd(ADC1,ENABLE);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}
void SPI_SendBytes(uint16_t data){
	GPIO_ResetBits(SPI1_GPIO,SPI1_NSS);
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1,data);
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY) == SET);
	GPIO_SetBits(SPI1_GPIO,SPI1_NSS);
}
void Delay_ms(uint32_t time){
	TIM_SetCounter(TIM2,0);
	while(TIM_GetCounter(TIM2) < time*10);
}
uint16_t val = 0;
uint16_t final = 0;
int main(){
	RCC_Config();
	GPIO_Config();
	TIM_Config();
	SPI_Config();
	ADC_Config();
	SimpleKalmanFilter(0.1, 0.01, 1);
	while(1){
		val = ADC_GetConversionValue(ADC1);
		final = updateEstimate(val);
		SPI_SendBytes(final);
		Delay_ms(1000);
	}
}
