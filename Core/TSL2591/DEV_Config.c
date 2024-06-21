/*****************************************************************************
* | File      	:   DEV_Config.c
* | Author      :   Waveshare team
* | Function    :   Hardware underlying interface
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2019-09-17
* | Info        :   Basic version
*
******************************************************************************/
#include "DEV_Config.h"

#define MAX_I2C_DEVICES 2

uint8_t DEV_SPI_Device = 0, DEV_I2C_Device = 0;
uint8_t I2C_ADDR[MAX_I2C_DEVICES];

/******************************************************************************
function:	GPIO Function initialization and transfer
parameter:
Info:
******************************************************************************/
void DEV_GPIO_Mode(GPIO_TypeDef* GPIO_Port, UWORD Pin, UWORD mode)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = Pin;

    if(mode == GPIO_MODE_INPUT || mode == 0){
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    } else if(mode == GPIO_MODE_OUTPUT_PP || mode == 1){
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    } else {
        GPIO_InitStruct.Mode = mode;
    }

    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIO_Port, &GPIO_InitStruct);
}

/**
 * delay x ms
**/
void DEV_Delay_ms(UDOUBLE xms)
{
    HAL_Delay(xms);
}

void GPIO_Config(void)
{
    // Configuración de pines GPIO según sea necesario
}

/******************************************************************************
function:	SPI Function initialization and transfer
parameter:
Info:
******************************************************************************/
void DEV_SPI_Init()
{
#if DEV_SPI 
    DEV_SPI_Device = 1;  // Activación del dispositivo SPI
#endif
}

void DEV_SPI_WriteByte(uint8_t Value)
{
#if DEV_SPI 
    // Implementación de escritura por SPI según tus necesidades
#endif
}

void DEV_SPI_Write_nByte(uint8_t *pData, uint32_t Len)
{
#if DEV_SPI 
    // Implementación de escritura de múltiples bytes por SPI según tus necesidades
#endif
}

/******************************************************************************
function:	I2C Function initialization and transfer
parameter:
Info:
******************************************************************************/
void DEV_I2C_Init(I2C_HandleTypeDef *i2c, uint8_t address)
{
#if DEV_I2C
    // Aquí puedes inicializar un nuevo dispositivo I2C
    if (DEV_I2C_Device < MAX_I2C_DEVICES)
    {
        I2C_ADDR[DEV_I2C_Device] = address;
        DEV_I2C_Device++;
    }
#endif
}


void I2C_Write_Byte(I2C_HandleTypeDef *i2c, uint8_t device_index, uint8_t Cmd, uint8_t value)
{
#if DEV_I2C
    if (device_index < DEV_I2C_Device) {
        uint8_t Buf[1] = {0};
        Buf[0] = value;
        HAL_I2C_Mem_Write(i2c, I2C_ADDR[device_index], Cmd, I2C_MEMADD_SIZE_8BIT, Buf, 1, 0x20);
    }
#endif
}

int I2C_Read_Byte(I2C_HandleTypeDef *i2c, uint8_t device_index, uint8_t Cmd)
{
#if DEV_I2C
    if (device_index < DEV_I2C_Device) {
        uint8_t Buf[1] = {0};
        HAL_I2C_Mem_Read(i2c, I2C_ADDR[device_index], Cmd, I2C_MEMADD_SIZE_8BIT, Buf, 1, 0x20);
        return Buf[0];
    }
#endif
    return 0;  // Manejar errores apropiadamente si es necesario
}

int I2C_Read_Word(I2C_HandleTypeDef *i2c, uint8_t device_index, uint8_t Cmd)
{
#if DEV_I2C
    if (device_index < DEV_I2C_Device) {
        uint8_t Buf[2] = {0, 0};
        HAL_I2C_Mem_Read(i2c, I2C_ADDR[device_index], Cmd, I2C_MEMADD_SIZE_8BIT, Buf, 2, 0x20);
        return ((Buf[1] << 8) | (Buf[0] & 0xFF));  // Retornar la palabra leída desde el dispositivo I2C
    }
#endif
    return 0;  // Manejar errores apropiadamente si es necesario
}

UBYTE DEV_ModuleInit(I2C_HandleTypeDef *i2c, uint8_t address)
{
    GPIO_Config();  // Configurar pines GPIO si es necesario
    DEV_I2C_Init(i2c, address);  // Inicializar el primer dispositivo I2C
    return 0;  // Retornar el estado de inicialización (éxito o falla)
}

void DEV_ModuleExit(void)
{
    // Implementar acciones de salida del módulo si es necesario
}
