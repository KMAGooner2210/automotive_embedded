#include "stm32f10x.h"                  // Device header


// B?ng mã LED 7 thanh cho s? 0-9 (Common Anode)
const uint8_t LED7Segment[10] = {
    0xC0, // 0: 11000000
    0xF9, // 1: 11111001
    0xA4, // 2: 10100100
    0xB0, // 3: 10110000
    0x99, // 4: 10011001
    0x92, // 5: 10010010
    0x82, // 6: 10000010
    0xF8, // 7: 11111000
    0x80, // 8: 10000000
    0x90  // 9: 10010000
};


void Delay_ms(uint32_t ms) {
    uint32_t i;
    for (i = 0; i < ms * 8000; i++);
}


void GPIO_Config(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}


void Display7Segment(uint8_t number) {
    GPIO_Write(GPIOA, LED7Segment[number]);
}

int main(void) {
    uint8_t number = 0; 

    GPIO_Config();

    Display7Segment(number);

    while (1) {
       
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0) { 
            Delay_ms(50); 
            if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0) {
                if (number < 9) {
                    number++;
                } else {
                    number = 0; // Quay vòng v? 0
                }
                Display7Segment(number);
                while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0); 
            }
        }

        // Nút gi?m (PB1)
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0) { 
            Delay_ms(50); 
            if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0) {
                if (number > 0) {
                    number--;
                } else {
                    number = 9; // Quay vòng v? 9
                }
                Display7Segment(number);
                while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0); // Ch? th? nút
            }
        }
    }
}
