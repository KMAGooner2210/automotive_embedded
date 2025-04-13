#include "i2c.h"

void I2C1_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	
	I2C_InitTypeDef I2C_InitStructure;
	
	I2C_InitStructure.I2C_ClockSpeed = 100000;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	
	I2C_Init(I2C1,&I2C_InitStructure);
	I2C_Cmd(I2C1,ENABLE);
}
void I2C2_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	
	I2C_InitTypeDef I2C_InitStructure;
	
	I2C_InitStructure.I2C_ClockSpeed = 400000;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	
	I2C_Init(I2C2,&I2C_InitStructure);
	I2C_Cmd(I2C2,ENABLE);
}
uint8_t I2C_WriteByteToSlave(uint8_t I2C_Addr, uint8_t reg, uint8_t data)
{
    __IO uint32_t timeout = 1000;

    // Start condition
    I2C_GenerateSTART(I2C2, ENABLE);

    timeout = 1000;
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if ((timeout--) == 0) return 1;
    }

    // Send slave address for write
    I2C_Send7bitAddress(I2C2, I2C_Addr, I2C_Direction_Transmitter);

    timeout = 1000;
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if ((timeout--) == 0) return 1;
    }

    // Send register address
    I2C_SendData(I2C2, reg);

    timeout = 1000;
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
    {
        if ((timeout--) == 0) return 1;
    }

    // Send data
    I2C_SendData(I2C2, data);

    timeout = 1000;
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        if ((timeout--) == 0) return 1;
    }

    // Stop condition
    I2C_GenerateSTOP(I2C2, ENABLE);
    return 0;
}

uint8_t WriteByteFromSlave(uint8_t I2C_Addr,uint8_t reg,uint8_t data){
	
	__IO uint32_t timeout = 1000;
	
	//Start condition
	I2C_GenerateSTART(I2C2,ENABLE);
	
	timeout = 1000;
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT))
		{
		if((timeout--) == 0) return 1;
    }
		
	/* Send slave address for write */
  I2C_Send7bitAddress(I2C2,I2C_Addr,I2C_Direction_Transmitter);
		
	timeout = 1000;
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((timeout--) == 0) return 1;
	}
	
	// send register address
	I2C_SendData(I2C2,reg);
	
	timeout = 1000;
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING))
	{
		if((timeout--) == 0) return 1;
	}
	
	I2C_SendData(I2C2,data);
	
	timeout = 1000;
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((timeout--) == 0) return 1;
	}
	
	//Stop condition
	I2C_GenerateSTOP(I2C2,ENABLE);
	return 0;
}

uint8_t I2C_ReadByteFromSlave(uint8_t I2C_Addr, uint8_t reg, uint8_t* buffer) {
    __IO uint32_t timeout = 1000;

    I2C_GenerateSTART(I2C2, ENABLE);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT)) {
        if ((timeout--) == 0) return 1;
    }

    I2C_Send7bitAddress(I2C2, I2C_Addr, I2C_Direction_Transmitter);
    timeout = 1000;
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
        if ((timeout--) == 0) return 1;
    }

    I2C_SendData(I2C2, reg);
    timeout = 1000;
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
        if ((timeout--) == 0) return 1;
    }

    I2C_GenerateSTART(I2C2, ENABLE);
    timeout = 1000;
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT)) {
        if ((timeout--) == 0) return 1;
    }

    I2C_Send7bitAddress(I2C2, I2C_Addr, I2C_Direction_Receiver);
    timeout = 1000;
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {
        if ((timeout--) == 0) return 1;
    }

    I2C_AcknowledgeConfig(I2C2, DISABLE); // T?t ACK tru?c khi nh?n byte cu?i
    timeout = 1000;
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED)) {
        if ((timeout--) == 0) return 1;
    }

    *buffer = I2C_ReceiveData(I2C2);
    I2C_GenerateSTOP(I2C2, ENABLE);
    I2C_AcknowledgeConfig(I2C2, ENABLE); // B?t l?i ACK cho l?n sau
    return 0;
}
	
uint8_t I2C_ReadMultiByteFromSlave(uint8_t device,uint8_t reg,uint8_t length,uint8_t* data)
{
	for(uint8_t i = 0;i < length;i++)
	 {
		  if(I2C_ReadByteFromSlave(device,reg+i,&data[i]))
				{
				  return 1;
				}
			}
	 return 0;
		}

uint8_t I2C_WriteMultiByteToSlave(uint8_t device,uint8_t reg,uint8_t length,uint8_t* data)
{
	for(uint8_t i=0;i< length;i++)
	  {
    	if(I2C_WriteByteToSlave(device,reg+i,data[i]))
			{
				return 1;
			}
		}
		return 0;
}

int16_t MPU6050_GetData(uint8_t regAddr) {
    uint8_t Data_H, Data_L;
    if (I2C_ReadByteFromSlave(MPU_ADDRESS, regAddr, &Data_H) ||
        I2C_ReadByteFromSlave(MPU_ADDRESS, regAddr + 1, &Data_L)) {
        return 0; // Tr? v? 0 n?u l?i
    }
    return (int16_t)((Data_H << 8) | Data_L);
}
void MPU6050_Configuration(){
	I2C_WriteByteToSlave(MPU_ADDRESS,MPU6050_PWR_MGMT_1,0x00);
	I2C_WriteByteToSlave(MPU_ADDRESS,MPU6050_SMPLRT_DIV,0x07);
	I2C_WriteByteToSlave(MPU_ADDRESS,MPU6050_CONFIG,0x06);
	I2C_WriteByteToSlave(MPU_ADDRESS,MPU6050_GYRO_CONFIG,0x18);
	I2C_WriteByteToSlave(MPU_ADDRESS,MPU6050_ACCEL_CONFIG,0x01);
}
