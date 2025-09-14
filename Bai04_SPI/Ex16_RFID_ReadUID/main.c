/**--------------EX16: Doc UID module RFID RC522--------------**/
/**--------------KMAGooner2210 -- Mai Tung--------------**/
/**--------------So do noi chan--------------**/
/**		STM32F103	 --------------		RFID RC522**/
/**		3.3V	 		 --------------		3.3V**/
/**		GND     	 --------------		GND**/
/**		A4	     	 --------------		SDA**/
/**		A5	     	 --------------		SCK**/
/**		A7	     	 --------------		MOSI**/
/**		A6	     	 --------------		MISO**/
/**		A3	     	 --------------		RST**/


#include "stm32f10x.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
#include "rc522.h"
#include <stdio.h>


void USART1_Config(void);
void SPI1_Config(void);  
void USART1_SendChar(char c);
void USART1_SendString(char *str);

int main(void) {
    uint8_t uid[10];
    uint8_t uidLength;
    char buffer[20];
    uint8_t i; 
    volatile uint32_t j; 

    
    SPI1_Config();
    USART1_Config();
    
    RC522_Config();
  
    USART1_SendString("Dat the vao dung vi tri!\r\n");
    USART1_SendString("San sang doc the...\r\n");
    
    while (1) {
      
        if (RC522_Check(uid, &uidLength) == MI_OK) {
            USART1_SendString("Phat hien the! UID: ");
            
            
            for (i = 0; i < uidLength; i++) {
                sprintf(buffer, "%02X", uid[i]);
                USART1_SendString(buffer);
                if (i < uidLength - 1) {
                    USART1_SendString(":");
                }
            }
            USART1_SendString("\r\n");
            
            
            for (j = 0; j < 1000000; j++);
        }
    }
}


void USART1_Config(void) {
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
    
    //  TX (PA9), RX (PA10)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    
    USART_Cmd(USART1, ENABLE);
}


void SPI1_Config(void) {
    SPI_InitTypeDef SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA, ENABLE);
    
    //SCK(PA5), MISO(PA6), MOSI(PA7)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    //NSS (PA4) , RST (PA3)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);
    
    SPI_Cmd(SPI1, ENABLE);
}




void USART1_SendChar(char c) {
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, c);
}


void USART1_SendString(char *str) {
    while (*str) {
        USART1_SendChar(*str++);
    }
}