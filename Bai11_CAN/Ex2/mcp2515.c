#include "mcp2515.h"

#define CS_PORT			GPIOA
#define CS_PIN			GPIO_Pin_4

#define CS_LOW()		do { GPIO_ResetBits(CS_PORT, CS_PIN); for(volatile int j=0; j<10; j++); } while(0)
#define CS_HIGH()		do { for(volatile int j=0; j<10; j++); GPIO_SetBits(CS_PORT, CS_PIN); } while(0)



uint8_t SPI1_SendRecv(uint8_t byte){
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, byte);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPI1);
}

void mcp2515_reset(void){
	CS_LOW();
	SPI1_SendRecv(MCP_RESET);
	CS_HIGH();
	for(volatile int i = 0; i < 10000; i++);
}

uint8_t mcp2515_read_register(uint8_t address){
	uint8_t data;
	CS_LOW();
	SPI1_SendRecv(MCP_READ);
	SPI1_SendRecv(address);
	data = SPI1_SendRecv(0x00);		// Gui dummy byte de nhan du lieu
	CS_HIGH();
	return data;
}

void mcp2515_write_register(uint8_t address, uint8_t data){
	CS_LOW();
	SPI1_SendRecv(MCP_WRITE);
	SPI1_SendRecv(address);
	SPI1_SendRecv(data);
	CS_HIGH();
}

void mcp2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data){
	CS_LOW();
	SPI1_SendRecv(MCP_BITMOD);
	SPI1_SendRecv(address);
	SPI1_SendRecv(mask);
	SPI1_SendRecv(data);
	CS_HIGH();
}

uint8_t mcp2515_set_mode(uint8_t mode){
	mcp2515_bit_modify(MCP_CANCTRL, 0xE0, mode);
	for(volatile int i = 0; i < 10000; i++);
	if((mcp2515_read_register(MCP_CANSTAT) & 0xE0) == mode){
		return 0;
	}
	return 1;
}

uint8_t mcp2515_config(uint8_t speed){
	mcp2515_reset();
	if(mcp2515_set_mode(MODE_CONFIG) != 0) return 1;
	if(speed == CAN_500KBPS){
		mcp2515_write_register(MCP_CNF1, 0x00);
		mcp2515_write_register(MCP_CNF2, 0x90);
		mcp2515_write_register(MCP_CNF3, 0x02);
	}else if(speed == CAN_250KBPS){
		mcp2515_write_register(MCP_CNF1, 0x01);
		mcp2515_write_register(MCP_CNF2, 0x90);
		mcp2515_write_register(MCP_CNF3, 0x02);
	} else {
		return 2;
	}
	
	// Cau hinh de buffer nhan moi tin nhan
	mcp2515_write_register(MCP_RXB0CTRL, 0x60);
	return 0;
}

void mcp2515_send_message(can_message_t* msg) {
    // 1. Ghi ID
    mcp2515_write_register(MCP_TXB0SIDH, (uint8_t)(msg->id >> 3));
    mcp2515_write_register(MCP_TXB0SIDL, (uint8_t)(msg->id << 5));
    // 2. Ghi DLC
    mcp2515_write_register(MCP_TXB0DLC, msg->dlc);
    // 3. Ghi d? li?u
    for (int i = 0; i < msg->dlc; i++) {
        mcp2515_write_register(MCP_TXB0SIDH + 5 + i, msg->data[i]);
    }
    
    // --- CÁCH G?I M?I CHU?N XÁC HON ---
    // Tr?c ti?p set bit TXREQ (bit th? 3, giá tr? 0x08) c?a thanh ghi di?u khi?n Buffer truy?n 0
    mcp2515_bit_modify(MCP_TXB0CTRL, 0x08, 0x08);
}

uint8_t mcp2515_read_message(can_message_t* msg){
	if((mcp2515_read_register(MCP_CANINTF) & 0x01) == 0){
		return 0;
	}
	
	// Doc ID
	
	uint8_t id_h = mcp2515_read_register(MCP_RXB0SIDH);
	uint8_t id_l = mcp2515_read_register(MCP_RXB0SIDL);
	msg->id = (id_h << 3) | (id_l >> 5);
	
	// Doc DLC
	
	msg->dlc = mcp2515_read_register(0x65) & 0x0F;
	
	// Doc du lieu
	
	for(int i = 0; i < msg->dlc; i++){
		msg->data[i] = mcp2515_read_register(0x66 + i);
	}
	mcp2515_bit_modify(MCP_CANINTF, 0x01, 0x00);
	return 1;
}

