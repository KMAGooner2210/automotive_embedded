#include "stm32f10x.h"
#include "ili9225.h"
#include "fonts.h"
#include <string.h>

// Function prototypes
void SystemClock_Config(void);
void GPIO_Config(void);
void Delay_ms(uint32_t ms);
void ILI9225_Demo(void);
// Global variables
ILI9225_GPIO_t lcd_gpio;

int main(void) {
    // System initialization
    SystemClock_Config();
    GPIO_Config();
    
    // Configure LCD GPIO pins - S?A L?I CHO ÐÚNG
    lcd_gpio.rst_port = GPIOA;
    lcd_gpio.rst_pin = GPIO_Pin_1;
    lcd_gpio.rs_port = GPIOA;
    lcd_gpio.rs_pin = GPIO_Pin_2;
    lcd_gpio.cs_port = GPIOA;
    lcd_gpio.cs_pin = GPIO_Pin_3;
    lcd_gpio.led_port = GPIOA;
    lcd_gpio.led_pin = GPIO_Pin_4;
    
    // Initialize LCD
    ILI9225_Init(&lcd_gpio);
    
    // Demo applications
    ILI9225_Demo();
    
    while (1) {
        // Nh?p nháy LED onboard d? bi?t chuong trình dang ch?y
        GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13)));
        Delay_ms(500);
    }
}

void ILI9225_Demo(void) {
    // Clear screen with black background
    ILI9225_FillScreen(COLOR_BLACK);
    Delay_ms(500);
    
    // Draw colorful rectangles
    ILI9225_FillRectangle(10, 10, 50, 50, COLOR_RED);
    ILI9225_FillRectangle(60, 10, 100, 50, COLOR_GREEN);
    ILI9225_FillRectangle(110, 10, 150, 50, COLOR_BLUE);
    Delay_ms(1000);
    
    // Clear and draw text
    ILI9225_FillScreen(COLOR_BLACK);
    
    // Draw text with different fonts and colors
    ILI9225_DrawString(10, 10, "STM32F103 + ILI9225", COLOR_WHITE, COLOR_BLACK, Terminal6x8);
    ILI9225_DrawString(10, 30, "Hello World!", COLOR_YELLOW, COLOR_BLACK, Terminal11x16);
    ILI9225_DrawString(10, 60, "TFT Display", COLOR_CYAN, COLOR_BLACK, Terminal12x16);
    
    Delay_ms(2000);
    
    // Draw geometric shapes
    ILI9225_FillScreen(COLOR_BLACK);
    
    // Draw circles
    ILI9225_DrawCircle(50, 50, 30, COLOR_RED);
    ILI9225_FillCircle(120, 50, 25, COLOR_GREEN);
    
    // Draw lines
    ILI9225_DrawLine(10, 100, 166, 100, COLOR_BLUE);
    ILI9225_DrawLine(88, 80, 88, 120, COLOR_BLUE);
    
    // Draw rectangles
    ILI9225_DrawRectangle(20, 130, 70, 180, COLOR_YELLOW);
    ILI9225_FillRectangle(100, 130, 150, 180, COLOR_MAGENTA);
    
    Delay_ms(2000);
    
    // Color gradient demo
    ILI9225_FillScreen(COLOR_BLACK);
    ILI9225_DrawString(40, 10, "Color Gradient", COLOR_WHITE, COLOR_BLACK, Terminal6x8);
    
    for (uint16_t i = 0; i < 176; i++) {
        uint16_t color = ILI9225_ColorFromRGB(i * 255 / 176, (176 - i) * 255 / 176, 128);
        ILI9225_DrawLine(i, 30, i, 80, color);
    }
    
    Delay_ms(2000);
    
    // Final screen with information
    ILI9225_FillScreen(COLOR_NAVY);
    ILI9225_DrawString(20, 20, "ILI9225 TFT Display", COLOR_WHITE, COLOR_NAVY, Terminal11x16);
    ILI9225_DrawString(30, 50, "Resolution:", COLOR_YELLOW, COLOR_NAVY, Terminal6x8);
    
    char resolution[20];
    sprintf(resolution, "%dx%d", ILI9225_GetWidth(), ILI9225_GetHeight());
    ILI9225_DrawString(100, 50, resolution, COLOR_WHITE, COLOR_NAVY, Terminal6x8);
    
    ILI9225_DrawString(30, 70, "STM32F103C8T6", COLOR_CYAN, COLOR_NAVY, Terminal6x8);
    ILI9225_DrawString(30, 90, "SPI Interface", COLOR_GREEN, COLOR_NAVY, Terminal6x8);
    
    ILI9225_DrawString(40, 150, "Demo Complete!", COLOR_WHITE, COLOR_NAVY, Terminal12x16);
}

void SystemClock_Config(void) {
    // Enable HSE and wait for ready
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY));
    
    // Configure PLL: HSE * 9 = 72 MHz
    RCC->CFGR |= RCC_CFGR_PLLMULL9;
    RCC->CFGR |= RCC_CFGR_PLLSRC;
    
    // Configure flash latency
    FLASH->ACR |= FLASH_ACR_LATENCY_2;
    
    // Enable PLL and wait for ready
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY));
    
    // Switch to PLL
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while (!(RCC->CFGR & RCC_CFGR_SWS_PLL));
    
    // Set APB1 prescaler to 2 (36 MHz)
    RCC->CFGR |= RCC_CFGR_PPRE1_2;
    
    // Enable clocks for used peripherals
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;
    RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
}

void GPIO_Config(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // Enable clock for GPIOA, GPIOB, GPIOC
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
    
    // Configure LED pin (PC13 - onboard LED)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    // Configure SPI2 pins: PB13(SCK), PB15(MOSI)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // Configure control pins: PA1(RST), PA2(RS/DC), PA3(CS), PA4(LED)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void Delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms; i++) {
        for (uint32_t j = 0; j < 7200; j++) {
            __NOP();
        }
    }
}

// Simple printf function for debugging
int _write(int file, char *ptr, int len) {
    int i;
    for (i = 0; i < len; i++) {
        ITM_SendChar(*ptr++);
    }
    return len;
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
    while (1) {
    }
}
#endif