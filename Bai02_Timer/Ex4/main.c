#include "stm32f10x.h"

void GPIO_Config(void);
void TIM3_Config(void);
void Delay_ms(uint32_t ms);

volatile uint16_t frequencies[] = {500, 1000, 2000}; 
volatile uint8_t freq_index = 0; 

int main(void) {
   
    GPIO_Config();
    TIM3_Config();
    
    while (1) {
        // Nút nh?n PB0 d? chuy?n t?n s?
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0) {
            Delay_ms(50); // Ch?ng d?i phím
            if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0) {
                freq_index = (freq_index + 1) % 3; 
                uint16_t period = (72000000 / frequencies[freq_index]) - 1; 
                TIM_SetAutoreload(TIM3, period); 
                TIM_SetCompare1(TIM3, period / 2); 
                while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0); 
            }
        }
    }
}

void GPIO_Config(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // Kích ho?t clock cho GPIOA và GPIOB
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
    
    // C?u hình PA6 (Output Compare - TIM3_CH1)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // Alternate Function Push-Pull
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // C?u hình PB0 là Input pull-up
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // Input Pull-Up
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void TIM3_Config(void) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
    // Kích ho?t clock cho TIM3
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
    // C?u hình Timer 3
    TIM_TimeBaseStructure.TIM_Period = (72000000 / frequencies[0]) - 1; // T?n s? ban d?u 500Hz
    TIM_TimeBaseStructure.TIM_Prescaler = 0; // Không chia t?n s?
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    
    // C?u hình Output Compare Channel 1 (Toggle mode)
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = TIM_TimeBaseStructure.TIM_Period / 2; // Duty cycle 50%
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);
    
    // Kích ho?t preload cho Channel 1
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable); // Toggle mode không c?n preload
    TIM_ARRPreloadConfig(TIM3, ENABLE);
    
    // Kh?i d?ng Timer 3
    TIM_Cmd(TIM3, ENABLE);
}

void Delay_ms(uint32_t ms) {
    volatile uint32_t i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 7200; j++); // Delay thô cho 72MHz
}