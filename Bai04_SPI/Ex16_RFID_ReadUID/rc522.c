#include "rc522.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"


#define RC522_CS_PORT GPIOA
#define RC522_CS_PIN GPIO_Pin_4
#define RC522_RST_PORT GPIOA
#define RC522_RST_PIN GPIO_Pin_3


void delay_ms(uint32_t nCount) {
    volatile uint32_t i, j;  
    for (i = 0; i < nCount; i++) {
        for (j = 0; j < 1000; j++);
    }
}

/**
 * @brief  Ghi gia tri vao thanh ghi cua RC522 qua SPI
 * @param	 addr Dia chi thanh ghi
 * @param	 val Gia tri can ghi
**/
void RC522_WriteRegister(uint8_t addr, uint8_t val) {
    GPIO_ResetBits(RC522_CS_PORT, RC522_CS_PIN);
    
    
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, (addr << 1) & 0x7E);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    SPI_I2S_ReceiveData(SPI1);
    
    
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, val);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    SPI_I2S_ReceiveData(SPI1);
    
    GPIO_SetBits(RC522_CS_PORT, RC522_CS_PIN);
}

/**
 * @brief  Doc gia tri tu thanh ghi cua RC522 qua SPI
 * @param	 addr Dia chi thanh ghi
 * @return Gia tri doc duoc tu thanh ghi	 
**/

uint8_t RC522_ReadRegister(uint8_t addr) {
    uint8_t val;
    
    GPIO_ResetBits(RC522_CS_PORT, RC522_CS_PIN);
    
    // G?i d?a ch? (bit 7 = 1 cho d?c)
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, ((addr << 1) & 0x7E) | 0x80);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    SPI_I2S_ReceiveData(SPI1);
    
    // Nh?n d? li?u (g?i giá tr? gi? d? nh?n d? li?u)
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, 0x00);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    val = SPI_I2S_ReceiveData(SPI1);
    
    GPIO_SetBits(RC522_CS_PORT, RC522_CS_PIN);
    
    return val;
}

/**
 * @brief Dat bit mask cho thanh ghi cua RC522
 * @param reg Dia chi thanh ghi
 * @param mask Gia tri mask can dat
**/
void RC522_SetBitMask(uint8_t reg, uint8_t mask) {
    uint8_t tmp;
    tmp = RC522_ReadRegister(reg);
    RC522_WriteRegister(reg, tmp | mask);
}

/**
 * @brief Xoa bit mask cho thanh ghi cua RC522
 * @param reg Dia chi thanh ghi
 * @param mask Gia tri mask can xoa
**/
void RC522_ClearBitMask(uint8_t reg, uint8_t mask) {
    uint8_t tmp;
    tmp = RC522_ReadRegister(reg);
    RC522_WriteRegister(reg, tmp & (~mask));
}

/**
 * @brief Bat antenna cua RC522
**/

void RC522_AntennaOn(void) {
    uint8_t temp;
    temp = RC522_ReadRegister(RC522_TxControlReg);
    if (!(temp & 0x03)) {
        RC522_SetBitMask(RC522_TxControlReg, 0x03);
    }
}

/**
 * @brief Tat antenna cua RC522
**/
void RC522_AntennaOff(void) {
    RC522_ClearBitMask(RC522_TxControlReg, 0x03);
}

/**
 * @brief Reset module RC522
**/
void RC522_Reset(void) {
    RC522_WriteRegister(RC522_CommandReg, PCD_Reset);
}
/**
 * @brief Cau hinh module RC522
**/
void RC522_Config(void) {
    GPIO_SetBits(RC522_RST_PORT, RC522_RST_PIN);
    delay_ms(10);
    
    RC522_Reset();
    
    RC522_WriteRegister(RC522_TModeReg, 0x8D);      
    RC522_WriteRegister(RC522_TPrescalerReg, 0x3E); 
    RC522_WriteRegister(RC522_TReloadRegL, 30);
    RC522_WriteRegister(RC522_TReloadRegH, 0);
    RC522_WriteRegister(RC522_TxAutoReg, 0x40);     
    RC522_WriteRegister(RC522_ModeReg, 0x3D);       
    
    RC522_AntennaOn();
}


/**
 * @brief Gui lenh va du lieu den the RFID va nhan phan hoi
 * @param command Lenh PCD gui den the
 * @param sendData Mang du lieu gui di
 * @param sendLen Do dai du lieu gui
 * @param backData Mang nhan du lieu phan hoi
 * @param backLen Con tro den do dai phan hoi nhan duoc
 * @return Trang thai thuc hien
**/
uint8_t RC522_ToCard(uint8_t command, uint8_t *sendData, uint8_t sendLen, uint8_t *backData, uint16_t *backLen) {
    uint8_t status = MI_ERR;
    uint8_t irqEn = 0x00;
    uint8_t waitIRq = 0x00;
    uint8_t lastBits;
    uint8_t n;
    uint16_t i;
    
    switch (command) {
        case PCD_Auth:
            irqEn = 0x12;
            waitIRq = 0x10;
            break;
        case PCD_Transceive:
            irqEn = 0x77;
            waitIRq = 0x30;
            break;
        default:
            break;
    }
    
    RC522_WriteRegister(RC522_ComIEnReg, irqEn | 0x80);
    RC522_ClearBitMask(RC522_ComIrqReg, 0x80);
    RC522_SetBitMask(RC522_FIFOLevelReg, 0x80);
    
    RC522_WriteRegister(RC522_CommandReg, PCD_Idle);
    
    
    for (i = 0; i < sendLen; i++) {
        RC522_WriteRegister(RC522_FIFODataReg, sendData[i]);
    }
    
  
    RC522_WriteRegister(RC522_CommandReg, command);
    if (command == PCD_Transceive) {
        RC522_SetBitMask(RC522_BitFramingReg, 0x80);
    }
    
   
    i = 4000; 
    do {
        n = RC522_ReadRegister(RC522_ComIrqReg);
        i--;
    } while ((i != 0) && !(n & 0x01) && !(n & waitIRq));
    
    RC522_ClearBitMask(RC522_BitFramingReg, 0x80);
    
    if (i != 0) {
        if (!(RC522_ReadRegister(RC522_ErrorReg) & 0x1B)) {
            status = MI_OK;
            if (n & irqEn & 0x01) {
                status = MI_NOTAGERR;
            }
            
            if (command == PCD_Transceive) {
                n = RC522_ReadRegister(RC522_FIFOLevelReg);
                lastBits = RC522_ReadRegister(RC522_ControlReg) & 0x07;
                if (lastBits) {
                    *backLen = (n - 1) * 8 + lastBits;
                } else {
                    *backLen = n * 8;
                }
                
                if (n == 0) {
                    n = 1;
                }
                if (n > 16) {
                    n = 16;
                }
                
                
                for (i = 0; i < n; i++) {
                    backData[i] = RC522_ReadRegister(RC522_FIFODataReg);
                }
            }
        } else {
            status = MI_ERR;
        }
    }
    
    return status;
}

/**
 * @brief Gui yeu cau den the RFID de phat hien the
 * @param reqMode Che do yeu cau (PICC_REQUIDL/PICC_REQALL)
 * @param TagType Mang nhan loai the
 * @return Trang thai thuc hien
**/

uint8_t RC522_Request(uint8_t reqMode, uint8_t *TagType) {
    uint8_t status;
    uint16_t backBits;
    
    RC522_WriteRegister(RC522_BitFramingReg, 0x07);
    
    TagType[0] = reqMode;
    status = RC522_ToCard(PCD_Transceive, TagType, 1, TagType, &backBits);
    
    if ((status != MI_OK) || (backBits != 0x10)) {
        status = MI_ERR;
    }
    
    return status;
}


/**
 * @brief Thuc hien anti-collision de lay UID cua the
 * @param serNum Mang nhan serial number(UID) cua the
 * @return Trang thai thuc hien
**/

uint8_t RC522_Anticoll(uint8_t *serNum) {
    uint8_t status;
    uint8_t i;
    uint8_t serNumCheck = 0;
    uint16_t unLen;
    
    RC522_WriteRegister(RC522_BitFramingReg, 0x00);
    
    serNum[0] = PICC_ANTICOLL;
    serNum[1] = 0x20;
    status = RC522_ToCard(PCD_Transceive, serNum, 2, serNum, &unLen);
    
    if (status == MI_OK) {
        // Ki?m tra serial number
        for (i = 0; i < 4; i++) {
            serNumCheck ^= serNum[i];
        }
        if (serNumCheck != serNum[i]) {
            status = MI_ERR;
        }
    }
    
    return status;
}

/**
 * @brief Kiem tra va lay UID cua the
 * @param id Mang nhan UID
 * @param len Con tro den do dai UID
 * @return Trang thai thuc hien
*/

uint8_t RC522_Check(uint8_t *id, uint8_t *len) {
    uint8_t status;
    uint8_t TagType[2];
    uint16_t backBits;
    
    status = RC522_Request(PICC_REQIDL, TagType);
    if (status == MI_OK) {
        status = RC522_Anticoll(id);
        *len = 5; // UID dài 4 byte + checksum
    }
    
  
    
    return status;
}