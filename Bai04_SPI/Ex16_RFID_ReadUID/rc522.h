#ifndef RC522_H
#define RC522_H

#include "stm32f10x.h"

// Trang thai
#define MI_OK 0
#define MI_NOTAGERR 1
#define MI_ERR 2

// Thanh ghi RC522
#define RC522_FIFODataReg 			0x09
#define RC522_CommandReg 				0x01
#define RC522_ComIEnReg 				0x02
#define RC522_DivIEnReg 				0x03
#define RC522_ComIrqReg 				0x04
#define RC522_DivIrqReg 				0x05
#define RC522_ErrorReg 					0x06
#define RC522_Status1Reg 				0x07
#define RC522_Status2Reg 				0x08
#define RC522_FIFOLevelReg 			0x0A
#define RC522_ControlReg 				0x0C
#define RC522_BitFramingReg 		0x0D
#define RC522_ModeReg 					0x11
#define RC522_TxModeReg 				0x12
#define RC522_RxModeReg 				0x13
#define RC522_TxControlReg 			0x14
#define RC522_TxAutoReg 				0x15
#define RC522_TxSelReg 					0x16
#define RC522_RxSelReg 					0x17
#define RC522_RxThresholdReg 		0x18
#define RC522_DemodReg 					0x19
#define RC522_MfTxReg 					0x1C
#define RC522_MfRxReg 					0x1D
#define RC522_SerialSpeedReg 		0x1F
#define RC522_CRCResultRegH 		0x21
#define RC522_CRCResultRegL 		0x22
#define RC522_ModWidthReg 			0x24
#define RC522_RFCfgReg 					0x26
#define RC522_GsNReg 						0x27
#define RC522_CWGsPReg 					0x28
#define RC522_ModGsPReg 				0x29
#define RC522_TModeReg 					0x2A
#define RC522_TPrescalerReg 		0x2B
#define RC522_TReloadRegH 			0x2C
#define RC522_TReloadRegL 			0x2D
#define RC522_TCounterValueRegH 0x2E
#define RC522_TCounterValueRegL 0x2F

// Lenh RC522
#define RC522_Idle 							0x00
#define RC522_Mem 							0x01
#define RC522_GenerateRandomID 	0x02
#define RC522_CalcCRC 					0x03
#define RC522_Transmit 					0x04
#define RC522_NoCmdChange 			0x07
#define RC522_Receive 					0x08
#define RC522_Transceive 				0x0C
#define RC522_MFAuthent 				0x0E
#define RC522_SoftReset 				0x0F

// Lenh PCD
#define PCD_Idle 								0x00
#define PCD_Auth 								0x0E
#define PCD_Receive 						0x08
#define PCD_Transmit 						0x04
#define PCD_Transceive 					0x0C
#define PCD_Reset 							0x0F
#define PCD_CalcCRC 						0x03
#define PCD_HALT 								0x50  

// Lenh MIFARE
#define PICC_REQIDL 						0x26
#define PICC_REQALL 						0x52
#define PICC_ANTICOLL 					0x93
#define PICC_SElECTTAG 					0x93
#define PICC_AUTHENT1A 					0x60
#define PICC_AUTHENT1B 					0x61
#define PICC_READ 							0x30
#define PICC_WRITE 							0xA0
#define PICC_DECREMENT 					0xC0
#define PICC_INCREMENT 					0xC1
#define PICC_RESTORE 						0xC2
#define PICC_TRANSFER 					0xB0
#define PICC_HALT 							0x50


void RC522_Config(void);
void RC522_Reset(void);
void RC522_WriteRegister(uint8_t addr, uint8_t val);
uint8_t RC522_ReadRegister(uint8_t addr);
void RC522_SetBitMask(uint8_t reg, uint8_t mask);
void RC522_ClearBitMask(uint8_t reg, uint8_t mask);
void RC522_AntennaOn(void);
void RC522_AntennaOff(void);
uint8_t RC522_ToCard(uint8_t command, uint8_t *sendData, uint8_t sendLen, uint8_t *backData, uint16_t *backLen);
uint8_t RC522_Request(uint8_t reqMode, uint8_t *TagType);
uint8_t RC522_Anticoll(uint8_t *serNum);
uint8_t RC522_Check(uint8_t *id, uint8_t *len);
#endif