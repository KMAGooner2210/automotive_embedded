#include "stm32f10x.h"

// H�m delay don gi?n
void Delay_ms(uint32_t ms) {
    uint32_t i;
    for (i = 0; i < ms * 8000; i++);
}

// H�m c?u h�nh GPIO
void GPIO_Config(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    // B?t clock cho GPIOA
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // C?u h�nh PA1 l�m output push-pull
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

// H�m t?o hi?u ?ng fading
void LED_Fading(void) {
    uint16_t i;
    const uint16_t max_duty = 100; // Chu k? PWM (don v? t�y �)
    
    // S�ng d?n
    for (i = 0; i <= max_duty; i++) {
        GPIO_SetBits(GPIOA, GPIO_Pin_1);   // B?t LED
        Delay_ms(i);                       // Th?i gian b?t tang d?n
        GPIO_ResetBits(GPIOA, GPIO_Pin_1); // T?t LED
        Delay_ms(max_duty - i);            // Th?i gian t?t gi?m d?n
    }
    
    // T?t d?n
    for (i = 0; i <= max_duty; i++) {
        GPIO_SetBits(GPIOA, GPIO_Pin_1);   // B?t LED
        Delay_ms(max_duty - i);            // Th?i gian b?t gi?m d?n
        GPIO_ResetBits(GPIOA, GPIO_Pin_1); // T?t LED
        Delay_ms(i);                       // Th?i gian t?t tang d?n
    }
}

int main(void) {
    // C?u h�nh GPIO
    GPIO_Config();

    while (1) {
        // G?i h�m fading li�n t?c
        LED_Fading();
    }
}