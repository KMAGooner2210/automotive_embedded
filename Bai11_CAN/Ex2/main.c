/*
 * ==============================================================================
 * Project:      STM32 CAN Node 2 (SPI + MCP2515)
 * Description:  Khoi tao giao tiep SPI1 voi module MCP2515 o toc do CAN 500 kbps.
 *               Kiem tra hoat dong bang che do Loopback hoac Normal Mode,
 *               gui/nhan ban tin CAN va bao cao trang thai, du lieu qua USART1.
 * Author:       KMAGooner2210
 * Date:         01/05/2026
 * ==============================================================================
 * So do chan:
 *   [SPI1 Giao tiep MCP2515]
 *   PA4:  SPI1_NSS   -> CS   (MCP2515)
 *   PA5:  SPI1_SCK   -> SCK  (MCP2515)
 *   PA6:  SPI1_MISO  -> SO   (MCP2515)
 *   PA7:  SPI1_MOSI  -> SI   (MCP2515)
 *
 *   [USART1 Giao tiep PC]
 *   PA9:  USART1_TX  -> RX   (USB-to-Serial)
 *   PA10: USART1_RX  -> TX   (USB-to-Serial)
 *
 *
 *   *Luu y: Cap nguon 5V cho VCC cua module MCP2515 khi giao tiep thuc te.
 * ==============================================================================
 */


#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <string.h>
#include "mcp2515.h"

void SPI1_Config(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
    // THAY Ð?I 1: B?t thêm Clock cho AFIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1 | RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32; 
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE);
    

    SPI_I2S_ReceiveData(SPI1); 
}

int fputc(int ch, FILE *f){
	USART_SendData(USART1, (uint8_t)ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	return ch;
}

void USART1_Config(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; // TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; // RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
}	

int main(void){
	SystemInit();
	USART1_Config();
	SPI1_Config();
	
	printf("\r\n--- STM32 MCP2515 NODE 2 ---\r\n");
	
	if(mcp2515_config(CAN_500KBPS) == 0){
		printf("MCP2515 Init OK\r\n");
	} else {
		printf("MCP2515 Init FAILED\r\n");
		while(1);
	}
	
	if(mcp2515_set_mode(MODE_LOOPBACK) == 0){
		printf("MCP2515 set to Loopback Mode\r\n");
	}else{
		printf("MCP2515 set mode FAILED\r\n");
		while(1);
	}
	
	can_message_t tx_msg, rx_msg;
	
    tx_msg.id = 0x222;
    tx_msg.dlc = 5;
    tx_msg.data[0] = 'H';
    tx_msg.data[1] = 'E';
    tx_msg.data[2] = 'L';
    tx_msg.data[3] = 'L';
    tx_msg.data[4] = 'O';
	
	printf("Sending message with ID 0x%X...\r\n", tx_msg.id);
	mcp2515_send_message(&tx_msg);
	

    uint32_t timeout = 0;
    while((mcp2515_read_register(MCP_TXB0CTRL) & 0x08) == 0x08) {
        timeout++;
        if(timeout > 100000) {
            printf(">> ERROR: TX Timeout! Message stuck in TX buffer.\r\n");
            break; 
        }
    }

    if (timeout <= 100000) {
        printf("Transmission complete. Checking RX Buffer...\r\n");
        
 
        if(mcp2515_read_message(&rx_msg)){
            printf("Message received from RX Buffer.\r\n");
            
            if(rx_msg.id == tx_msg.id && rx_msg.dlc == tx_msg.dlc && 
                 memcmp(rx_msg.data, tx_msg.data, tx_msg.dlc) == 0)
            {
                printf(">> MCP2515 Loopback OK <<\r\n");
            } else {
                printf(">> MCP2515 Loopback FAILED: Data mismatch!\r\n");
            }
        } else {
            printf(">> MCP2515 Loopback FAILED: No message in RX buffer!\r\n");
        }
    }

    while(1) {}
		}
