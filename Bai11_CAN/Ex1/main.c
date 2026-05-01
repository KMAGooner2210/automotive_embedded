/*
 * ==============================================================================
 * Project:      STM32 CAN Node 1 Transmitter
 * Description:  Khoi tao CAN1 tren chan mac dinh (PA11, PA12) o toc do 500 kbps ,
 *               truyen mot ban tin voi ID 0x111, va bao cao trang thai qua USART1
 *               qua USART1.
 * Author:       KMAGooner2210
 * Date:         01/05/2026
 * ==============================================================================
 * So do chan:
 *   PA12: CAN1_TX   -> TXD  SN65HVD230
 *   PA11: CAN1_RX   -> RXD SN65HVD230
 *   PA9:  USART1_TX -> RX  USB-to-Serial
 *   PA10: USART1_RX -> TX  USB-to-Serial
 * ==============================================================================
 */


#include "stm32f10x.h"                  // Device header
#include <stdio.h>



int fputc(int ch, FILE *f){
	USART_SendData(USART1, (uint8_t) ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {}
	return ch;
}


void GPIO_Config(void){
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	
	
	// PA9 (USART1_TX)
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// PA10 (USART1_RX)
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// PA11 (CAN_RX)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// PA12 (CAN_TX)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void UART_Config(void){
	USART_InitTypeDef USART_InitStructure;
	
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx;
	
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
}

void CAN_Config(void){
	CAN_InitTypeDef 						CAN_InitStructure;
	CAN_FilterInitTypeDef				CAN_FilterInitStructure;
	
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);
	
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;

	// Baudrate 500 kbps
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_14tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;
	CAN_InitStructure.CAN_Prescaler = 4;
	CAN_Init(CAN1, &CAN_InitStructure);
	
	// Cau hinh Filter 0 (Cho phep nhan tat co)
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
}

volatile uint32_t msTicks = 0;

void SysTick_Handler(void){
	msTicks++;
}

void SysTick_Init(void){
	if(SysTick_Config(SystemCoreClock / 1000)){
		while(1);
	}
}

int main(void){
	SystemInit();
	GPIO_Config();
	UART_Config();
	SysTick_Init();
	
	printf("\r\n--- STM32 CAN NODE 1 ---\r\n");
	CAN_Config();
	
	// Tao ban tin va gui
	CanTxMsg TxMessage;
	TxMessage.StdId = 0x111; 			// Standard ID = 0x111
	TxMessage.ExtId = 0x00;
	TxMessage.IDE = CAN_Id_Standard;
	TxMessage.RTR = CAN_RTR_Data;
	TxMessage.DLC = 5;						// Do dai 4 byte
	
	uint8_t counter = 0;
	uint32_t last_send_time = 0;
	
	while(1){
		if(msTicks - last_send_time >= 1000){
			last_send_time = msTicks;
			
			TxMessage.Data[0] = 'N';
			TxMessage.Data[1] = 'O';
			TxMessage.Data[2] = 'D';
			TxMessage.Data[3] = 'E';
			TxMessage.Data[4] = counter;
	
			// Day ban tin vao Mailbox de truyen di
	
			uint8_t mailbox = CAN_Transmit(CAN1, &TxMessage);
	
			// Kiem tra trang thai tra ve
	
			if(mailbox != CAN_TxStatus_NoMailBox){
				printf("Node 1 send (Mailbox %d), Data[4] = %d\r\n", mailbox, counter);
			} else {
				printf("Node 1 transmit failed (No empty mailbox)\r\n");
			}
			counter++;
		}
	}
}
	
	
	