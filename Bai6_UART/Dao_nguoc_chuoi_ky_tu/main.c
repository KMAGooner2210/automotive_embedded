#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART

#define MAX_BUFFER_SIZE 100

void RCC_Config(void);
void GPIO_Config(void);
void USART_Config(void);
void USART_Send_Char(char data);
void USART_Send_String(char* str);
void Reverse_String(char* str);
uint8_t USART_Receive_char(void);

void RCC_Config(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);
}

void GPIO_Config(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //chan TX
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //chan RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}
void USART_Config(void){
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART1,&USART_InitStructure);
	USART_Cmd(USART1,ENABLE);
}
void USART_Send_Char(char data){
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
	USART_SendData(USART1,data);
}
void USART_Send_String(char* str){
	while(*str){
		USART_Send_Char(*str++);
	}
}
uint8_t USART_Receive_Char(void){
	while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == RESET){}
  return USART_ReceiveData(USART1);
}
void Reverse_String(char* str){
	int start, end;
  char temp;
  int length = 0;
  
  while (str[length] != '\0') {
    length++;
  }

  start = 0;
  end = length - 1;

  while (start < end)
  {
    temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    start++;
    end--;
  }
}
int main(void)
{
  
  RCC_Config();
  GPIO_Config();
  USART_Config();

  char buffer[MAX_BUFFER_SIZE];
  uint8_t bufferIndex = 0;
  

  USART_Send_String("Nhap chuoi ky tu:\r\n");

  while (1)
  {
    
    uint8_t receivedChar = USART_Receive_Char();

    
    if (receivedChar == '\r' || receivedChar == '\n')
    {
      buffer[bufferIndex] = '\0'; 
      Reverse_String(buffer);    
      USART_Send_String("Chuoi dao nguoc: ");
      USART_Send_String(buffer); 
      USART_Send_String("\r\n");
      bufferIndex = 0;          
      for (int i = 0; i < MAX_BUFFER_SIZE; i++) {
          buffer[i] = '\0';
      }
    }
    else if (bufferIndex < MAX_BUFFER_SIZE - 1) 
    {
      buffer[bufferIndex++] = receivedChar; 
    }
  }
}
