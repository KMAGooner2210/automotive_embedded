#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_flash.h"            // Keil::Device:StdPeriph Drivers:Flash
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART
#include "stdio.h"

#define TX_Pin   GPIO_Pin_9


void RCC_Config(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
}

void USART_Config(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = TX_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_Mode = USART_Mode_Tx;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_Init(USART1,&USART_InitStructure);
	USART_Cmd(USART1,ENABLE);
}

//Ham gui 1 ky tu
void UART_SendChar(char c){
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	USART_SendData(USART1,c);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
}

//Ham gui 1 chuoi
void UART_SendString(const char * str){
	while(*str != '\0'){
		UART_SendChar(*str);
	  str++;
	}
}

//Ham xoa 1 trang FLASH
void Flash_Erase(uint32_t page_address){
	
	if(page_address < 0x08000000 || page_address > 0x0801FFFF){
		UART_SendString("Error: Invalid page address!\r\n");
		return;
   }
	
	 FLASH_Unlock();
	 FLASH_Status status = FLASH_ErasePage(page_address);
	 FLASH_Lock();
	 
	 if(status == FLASH_COMPLETE){
		 UART_SendString("Page erased successfully\r\n");
	 }else{
		 UART_SendString("Error: Page erased failed\r\n");
	 }
 }

//Ham ghi 2 byte(half-word) vao FLASH
void Flash_WriteInt(uint32_t address,uint16_t value){
	if(address < 0x08000000 || address > 0x0801FFFF || (address % 2 != 0)){
		UART_SendString("Error: Invalid addres for half-word\r\n");
		return;
  }
	
	FLASH_Unlock();
	FLASH_Status status = FLASH_ProgramHalfWord(address,value);
	FLASH_Lock();
	
	if(status == FLASH_COMPLETE) {
		UART_SendString("Write half-word successfully\r\n");
	}else{
		UART_SendString("Error: Write half-word failed\r\n");
	}
}

//Ham ghi nhieu byte vao FLASH
void Flash_WriteNumByte(uint32_t address,uint8_t *data, uint32_t num){
	if(address < 0x08000000 | address > 0x0801FFFF || (address + num -1) > 0x0801FFFF){
		UART_SendString("Error: Invalid addresss range\r\n");
		return;
	}
	
	FLASH_Unlock();
	for(uint32_t i = 0; i < num; i += 2){
		uint16_t half_word = 0;
    
		half_word = data[i];
		if(i + 1 < num){
			half_word |= (uint16_t)data[i+1] << 8;
		}else{
			half_word |= 0xFF << 8;
		}
		
		FLASH_Status status = FLASH_ProgramHalfWord(address+i,half_word);
		if(status != FLASH_COMPLETE){
		   UART_SendString("Error: Write Byte failed \r\n");
       FLASH_Lock();
       return;			
		}
	}
  FLASH_Lock();
	UART_SendString("Write bytes successfully\r\n");
}

//Ham doc nhieu byte tu FLASH
void Flash_ReadNumByte(uint32_t address,uint32_t num){
	if(address < 0x08000000 || address > 0x0801FFFF || (address + num - 1) > 0x0801FFFF){
		UART_SendString("Error: Invalid address range\r\n");
		return;
	}
	
	char buffer[50];
	UART_SendString("Data read: ");
	for(uint32_t i = 0; i < num; i++){
		buffer[i] = *(uint8_t *)(address + i);
		sprintf(buffer, "0x%02X ",buffer[i]);
		UART_SendString(buffer);
	}
	UART_SendString("\r\n");
}

uint8_t array[14]={5,2,3,1,4,9,6,5,10, 20, 11, 23, 25, 19};
uint8_t buffer[14];

int main(void){
	RCC_Config();
	USART_Config();
	
	Flash_Erase(0x0800FC00);
	
	//Ghi 14 byte tu mang array
	Flash_WriteNumByte(0x08010000,array,14);
	
	//Doc 12 byte vao buffer vao buffer va hien thi
	Flash_ReadNumByte(0x08010000,12);
	
	//Ghi 2 byte (0xAAAA) tai 0X08010020
	Flash_WriteInt(0x08010020,0xAAAA);
	
	while(1){}

}
