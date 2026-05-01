#ifndef __MCP2515_H
#define __MCP2515_H

#include "stm32f10x.h"                  // Device header


// Ban tin CAN
typedef struct {
	uint16_t id;
	uint8_t dlc;
	uint8_t data[8];
} can_message_t;

#define MCP_RESET					0xC0
#define MCP_READ					0x03
#define MCP_WRITE					0x02
#define MCP_RTS_TX0				0x81
#define MCP_READ_STATUS		0xA0
#define MCP_BITMOD				0x05

#define MCP_CANCTRL				0x0F
#define MCP_CANSTAT				0x0E
#define MCP_CNF1					0x2A
#define MCP_CNF2					0x29
#define MCP_CNF3					0x28
#define MCP_TXB0CTRL			0x30
#define MCP_TXB0SIDH			0x31
#define MCP_TXB0SIDL			0x32
#define MCP_TXB0DLC				0x35

#define MCP_RXB0CTRL			0x60
#define MCP_RXB0SIDH			0x61
#define MCP_RXB0SIDL			0x62
#define MCP_RXB0DLC				0x65
#define MCP_CANINTF				0x2C


#define MODE_NORMAL 		0x00
#define MODE_LOOPBACK		0x40
#define MODE_CONFIG 		0x80

#define CAN_500KBPS			1
#define CAN_250KBPS			2

uint8_t mcp2515_config(uint8_t speed);
void mcp2515_reset(void);
uint8_t mcp2515_read_register(uint8_t address);
void mcp2515_write_register(uint8_t address, uint8_t data);
void mcp2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data);
uint8_t mcp2515_set_mode(uint8_t mode);
void mcp2515_send_message(can_message_t* msg);
uint8_t mcp2515_read_message(can_message_t* msg);


#endif