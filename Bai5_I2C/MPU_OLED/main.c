
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_i2c.h"
#include "i2c.h"
#include "ssd1306.h"
#include "fonts.h"
#include "stdio.h"
#include "gpio.h"
#include "math.h"
void SystemClock_Config(void);
void SysTick_Init(void);
void Delay_ms(uint32_t ms);

// Hàm g? l?i: Nh?p nháy LED trên PC13 n?u có l?i
void ErrorBlink(uint8_t times) {
    for (int i = 0; i < times; i++) {
        GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
        Delay_ms(500);
        GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
        Delay_ms(500);
    }
}
int main(void)
{
    
    SystemClock_Config();
    SysTick_Init();  

    GPIO_Config();  
    I2C1_Config();  
    I2C2_Config();  

    
    SSD1306_Config();  
	  // Kh?i t?o OLED và ki?m tra l?i
    if (!SSD1306_Config()) {
        // N?u kh?i t?o th?t b?i, nh?p nháy LED 10 l?n và d?ng chuong trình
        ErrorBlink(10);
        while (1); // D?ng chuong trình
    }
		//
    MPU6050_Configuration();  
    //
		// Ki?m tra MPU6050
    uint8_t test_Who_Am_I = 0;
    if (I2C_ReadByteFromSlave(MPU_ADDRESS, MPU6050_WHO_AM_I, &test_Who_Am_I) != 0 || test_Who_Am_I != 0x68) {
        // N?u MPU6050 không ph?n h?i dúng, nh?p nháy LED 5 l?n
        ErrorBlink(5);
        while (1);
    }
		
		
		//
    /*
    uint8_t test_Who_Am_I = 0;
    I2C_ReadByteFromSlave(MPU_ADDRESS, MPU6050_WHO_AM_I, &test_Who_Am_I);
    if (test_Who_Am_I != 0x68) {
        for (int i = 0; i < 5; i++) {
            GPIO_WriteBit(GPIOC, GPIO_Pin_13, (GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13) == Bit_SET) ? Bit_RESET : Bit_SET); // Toggle PC13
            Delay_ms(1000);
        }
    }
*/
    
    char buf[12];
    char temp[10];
    SSD1306_GotoXY(0, 0);
    SSD1306_Puts("accelX:", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY(56, 0);
    SSD1306_Puts("0", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY(0, 10);
    SSD1306_Puts("accelY:", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY(56, 10);
    SSD1306_Puts("0", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY(0, 20);
    SSD1306_Puts("accelZ:", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY(56, 20);
    SSD1306_Puts("0", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY(0, 30);
    SSD1306_Puts("angleX:", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY(56, 30);
    SSD1306_Puts("0", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY(0, 40);
    SSD1306_Puts("angleY:", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY(56, 40);
    SSD1306_Puts("0", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY(0, 50);
    SSD1306_Puts("angleZ:", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY(56, 50);
    SSD1306_Puts("0", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_UpdateScreen();

    
    while (1)
    {
        SSD1306_GotoXY(56, 0);
        SSD1306_Puts("          ", &Font_7x10, SSD1306_COLOR_WHITE);
        SSD1306_GotoXY(56, 10);
        SSD1306_Puts("          ", &Font_7x10, SSD1306_COLOR_WHITE);
        SSD1306_GotoXY(56, 20);
        SSD1306_Puts("          ", &Font_7x10, SSD1306_COLOR_WHITE);
        SSD1306_GotoXY(56, 30);
        SSD1306_Puts("          ", &Font_7x10, SSD1306_COLOR_WHITE);
        SSD1306_GotoXY(56, 40);
        SSD1306_Puts("          ", &Font_7x10, SSD1306_COLOR_WHITE);
        SSD1306_GotoXY(56, 50);
        SSD1306_Puts("          ", &Font_7x10, SSD1306_COLOR_WHITE);
			  //
			  // ACCELX (gia t?c tr?c X, hi?n th? du?i d?ng s? th?c)
        float accelX = MPU6050_GetData(MPU_ACCEL_XOUT_H) / 16384.0;
        snprintf(buf, sizeof(buf), "%.2f", accelX); // Ð?nh d?ng s? th?c v?i 2 ch? s? th?p phân
        SSD1306_GotoXY(56, 0);
        SSD1306_Puts(buf, &Font_7x10, SSD1306_COLOR_WHITE);

        // ACCELY (gia t?c tr?c Y)
        float accelY = MPU6050_GetData(MPU_ACCEL_YOUT_H) / 16384.0;
        snprintf(buf, sizeof(buf), "%.2f", accelY);
        SSD1306_GotoXY(56, 10);
        SSD1306_Puts(buf, &Font_7x10, SSD1306_COLOR_WHITE);

        // ACCELZ (gia t?c tr?c Z)
        float accelZ = MPU6050_GetData(MPU_ACCEL_ZOUT_H) / 16384.0;
        snprintf(buf, sizeof(buf), "%.2f", accelZ);
        SSD1306_GotoXY(56, 20);
        SSD1306_Puts(buf, &Font_7x10, SSD1306_COLOR_WHITE);

        // GYROX (con quay tr?c X, hi?n th? du?i d?ng s? nguyên)
        int16_t gyroX = (MPU6050_GetData(MPU_GYRO_XOUT_H) + 19)/16.4; // Bù l?ch
        snprintf(buf, sizeof(buf), "%d", gyroX);
        SSD1306_GotoXY(56, 30);
        SSD1306_Puts(buf, &Font_7x10, SSD1306_COLOR_WHITE);

        // GYROY (con quay tr?c Y)
        int16_t gyroY = (MPU6050_GetData(MPU_GYRO_YOUT_H) - 143)/16.4; // Bù l?ch
        snprintf(buf, sizeof(buf), "%d", (int)gyroY);
        SSD1306_GotoXY(56, 40);
        SSD1306_Puts(buf, &Font_7x10, SSD1306_COLOR_WHITE);

        // GYROZ (con quay tr?c Z)
        int16_t gyroZ = (MPU6050_GetData(MPU_GYRO_ZOUT_H) + 20)/16.4; // Bù l?ch
        snprintf(buf, sizeof(buf), "%d", gyroZ);
        SSD1306_GotoXY(56, 50);
        SSD1306_Puts(buf, &Font_7x10, SSD1306_COLOR_WHITE);

     
        float temperature = MPU6050_GetData(MPU_TEMP_OUT_H) / 340.0 + 36.53;
        snprintf(temp, sizeof(temp), "%.2f", temperature);
        SSD1306_GotoXY(84, 40); // V? trí hi?n th? nhi?t d?
        SSD1306_Puts(temp, &Font_7x10, SSD1306_COLOR_WHITE);
			
			 
        SSD1306_UpdateScreen();
        Delay_ms(50);
    }
}


void SystemClock_Config(void)
{
    
    RCC_DeInit();

    
    RCC_HSEConfig(RCC_HSE_ON);
    while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);

   
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9); 
    RCC_PLLCmd(ENABLE);
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

    
    RCC_HCLKConfig(RCC_SYSCLK_Div1);  
    RCC_PCLK1Config(RCC_HCLK_Div2);    
    RCC_PCLK2Config(RCC_HCLK_Div1);    

    
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    while (RCC_GetSYSCLKSource() != 0x08);

    
    FLASH_SetLatency(FLASH_Latency_2);
}


void SysTick_Init(void)
{
    
    if (SysTick_Config(SystemCoreClock / 1000)) {
        while (1); 
    }
}


static volatile uint32_t tick_count = 0;
void Delay_ms(uint32_t ms)
{
    tick_count = 0;
    while (tick_count < ms);
}

void SysTick_Handler(void)
{
    tick_count++;
}
