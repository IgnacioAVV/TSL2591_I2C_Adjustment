/*****************************************************************************
 * | File      	:   TSL2591.c
 * | Author      :   Waveshare team
 * | Function    :   TSL2591 driver
 * | Info        :
 *----------------
 * |	This version:   V1.0
 * | Date        :   2019-09-17
 * | Info        :   Basic version
 *
 ******************************************************************************/
#include "DEV_Config.h"
#include "TSL2591.h"

UBYTE TSL2591_Gain, TSL2591_Time;
/******************************************************************************
 function:	Read one byte of data to TSL2591 via I2C
 parameter:
 Addr: Register address
 Info:
 ******************************************************************************/
//UBYTE TSL2591_Read_Byte(TSL2591_t *sensor, UBYTE reg)
uint8_t TSL2591_Read_Byte(TSL2591_t *sensor, uint8_t reg) {
		uint8_t value;
		HAL_I2C_Mem_Read(sensor->i2c_handle, sensor->address, reg, 1, &value, 1,
				1000);
		return value;
//    address = address | COMMAND_BIT;

//	I2C_Read_Byte(sensor->i2c_handle, sensor->address, Addr);
//    return I2C_Read_Byte(sensor, Addr);

	}

	/******************************************************************************
	 function:	Read one word of data to TSL2591 via I2C
	 parameter:
	 Addr: Register address
	 Info:
	 ******************************************************************************/
//UWORD TSL2591_Read_Word(TSL2591_t *sensor, UBYTE Addr)
	uint16_t TSL2591_Read_Word(TSL2591_t *sensor, uint8_t reg) {
		uint8_t buffer[2];
		uint16_t value;
		HAL_I2C_Mem_Read(sensor->i2c_handle, sensor->address, reg, 1, buffer, 2,
				1000);
		value = (buffer[1] << 8) | buffer[0];
		return value;
//       address = address | COMMAND_BIT;
//	I2C_Read_Word(sensor->i2c_handle, sensor->address, Addr);
//    return I2C_Read_Word(Addr);
	}

	/******************************************************************************
	 function:	Send one byte of data to TSL2591 via I2C
	 parameter:
	 Addr: Register address
	 Value: Write to the value of the register
	 Info:
	 ******************************************************************************/
	void TSL2591_Write_Byte(TSL2591_t *sensor, UBYTE Addr, UBYTE Value) {
		Addr = Addr | COMMAND_BIT;
		I2C_Read_Byte(sensor->i2c_handle, sensor->address, Addr);

//    I2C_Write_Byte(Addr, Value);
	}

	/******************************************************************************
	 function:	Enable TSL2591
	 parameter:
	 Info:
	 ******************************************************************************/
	void TSL2591_Enable(TSL2591_t *sensor) {
		TSL2591_Write_Byte(sensor, COMMAND_BIT | ENABLE_REGISTER,
				ENABLE_POWERON | ENABLE_AEN);
	}
	/******************************************************************************
	 function:	Disable TSL2591
	 parameter:
	 Info:
	 ******************************************************************************/
	void TSL2591_Disable(TSL2591_t *sensor) {
		TSL2591_Write_Byte(sensor, COMMAND_BIT | ENABLE_REGISTER,
				ENABLE_POWEROFF);
	}
	/******************************************************************************
	 function:	Read TSL2591 gain
	 parameter:
	 Info:
	 ******************************************************************************/
	UBYTE TSL2591_Get_Gain(TSL2591_t *sensor) {
		/*************************************************
		 LOW_AGAIN           = (0X00)        (1x)
		 MEDIUM_AGAIN        = (0X10)        (25x)
		 HIGH_AGAIN          = (0X20)        (428x)
		 MAX_AGAIN           = (0x30)        (9876x)
		 *************************************************/
		UBYTE data;
		data = TSL2591_Read_Byte(sensor, CONTROL_REGISTER);
		TSL2591_Gain = data & 0x30;
		return data & 0x30;
	}

	/******************************************************************************
	 function:	Set the TSL2591 gain
	 parameter:
	 Info:
	 ******************************************************************************/
	void TSL2591_Set_Gain(TSL2591_t *sensor, UBYTE Gain) {
		UBYTE control = 0;
		if (Gain == LOW_AGAIN || Gain == MEDIUM_AGAIN || Gain == HIGH_AGAIN
				|| Gain == MAX_AGAIN) {
			control = TSL2591_Read_Byte(sensor, CONTROL_REGISTER);
			control &= 0xCf; //0b11001111
			control |= Gain;
			TSL2591_Write_Byte(sensor, CONTROL_REGISTER, control);
			TSL2591_Gain = Gain;
		} else {
			printf("Gain Parameter Error\r\n");
		}
	}

	/******************************************************************************
	 function:	Get the TSL2591 Integral Time
	 parameter:
	 Info:
	 ******************************************************************************/
	uint8_t TSL2591_Get_IntegralTime(TSL2591_t *sensor) {
		UBYTE control = 0;
		/************************************************************
		 ATIME_100MS = (0x00)      100 millis   MAX COUNT 36863
		 ATIME_200MS = (0x01)      200 millis   MAX COUNT 65535
		 ATIME_300MS = (0x02)      300 millis   MAX COUNT 65535
		 ATIME_400MS = (0x03)      400 millis   MAX COUNT 65535
		 ATIME_500MS = (0x04)      500 millis   MAX COUNT 65535
		 ATIME_600MS = (0x05)      600 millis   MAX COUNT 65535
		 ************************************************************/
		control = TSL2591_Read_Byte(sensor, CONTROL_REGISTER);
		TSL2591_Time = control & 0x07;
		return control & 0x07; //0b00000111
	}

	/******************************************************************************
	 function:	Set the TSL2591 Integral Time
	 parameter:
	 Info:
	 ******************************************************************************/
	void TSL2591_Set_IntegralTime(TSL2591_t *sensor, UBYTE Time) {
		UBYTE control = 0;
		if (Time < 0x06) {
			control = TSL2591_Read_Byte(sensor, CONTROL_REGISTER);
			control &= 0xf8; //0b11111000
			control |= Time;
			TSL2591_Write_Byte(sensor, CONTROL_REGISTER, control);
			TSL2591_Time = Time;
		} else {
			printf("Integral Time Parameter Error\r\n");
		}
	}

	/******************************************************************************
	 function:	Read channel data
	 parameter:
	 Info:
	 ******************************************************************************/
	int TSL2591_Read_Channel0(TSL2591_t *sensor) {
		return TSL2591_Read_Word(sensor, CHAN0_LOW);
	}

	int TSL2591_Read_Channel1(TSL2591_t *sensor) {
		return TSL2591_Read_Word(sensor, CHAN1_LOW);
	}

	/******************************************************************************
	 function:	TSL2591 Initialization
	 parameter:
	 Info:
	 ******************************************************************************/
	void TSL2591_Init(TSL2591_t *sensor, I2C_HandleTypeDef *i2c,
			uint8_t address) {
		sensor->i2c_handle = i2c;
		sensor->address = address;

		// Inicializa la comunicación I2C
		DEV_I2C_Init(i2c, address << 1);

		// Configura el sensor
		TSL2591_Enable(sensor);
		TSL2591_Write_Byte(sensor, PERSIST_REGISTER, 0x01);
		TSL2591_Disable(sensor);
	}
	/******************************************************************************
	 function:	Read TSL2591 data to convert to Lux value
	 parameter:
	 Info:
	 ******************************************************************************/
	float TSL2591_Read_Lux(TSL2591_t *sensor) {
		UWORD atime, max_counts, channel_0, channel_1;
		TSL2591_Enable(sensor);
		for (UBYTE i = 0; i < TSL2591_Time + 2; i++) {
			DEV_Delay_ms(100);
		}
//    if(DEV_Digital_Read(INT_PIN) == 1)
//            printf("INT 0\r\n");
//    else
//             printf("INT 1\r\n");
		channel_0 = TSL2591_Read_Channel0(sensor);
		channel_1 = TSL2591_Read_Channel1(sensor);
		TSL2591_Disable(sensor);
		TSL2591_Enable(sensor);
		TSL2591_Write_Byte(sensor, 0xE7, 0x13);
		TSL2591_Disable(sensor);

		atime = 100 * TSL2591_Time + 100;
		if (TSL2591_Time == ATIME_100MS) {
			max_counts = MAX_COUNT_100MS;
		} else {
			max_counts = MAX_COUNT;
		}
		UBYTE gain_t;
		if (channel_0 >= max_counts || channel_1 >= max_counts) {
			gain_t = TSL2591_Get_Gain(sensor);
			if (gain_t != LOW_AGAIN) {
				gain_t = ((gain_t >> 4) - 1) << 4;
				TSL2591_Set_Gain(sensor, gain_t);
				channel_0 = 0;
				channel_1 = 0;
				while (channel_0 <= 0 || channel_1 <= 0) {
					channel_0 = TSL2591_Read_Channel0(sensor);
					channel_1 = TSL2591_Read_Channel1(sensor);
				}
				DEV_Delay_ms(100);
			} else {
				printf("Numerical overflow!/r/n");
				return 0;
			}
		}
		double again;
		again = 1.0;
		if (TSL2591_Gain == MEDIUM_AGAIN) {
			again = 25.0;
		} else if (TSL2591_Gain == HIGH_AGAIN) {
			again = 428.0;
		} else if (TSL2591_Gain == MAX_AGAIN) {
			again = 9876.0;
		}
		double Cpl;
		UWORD lux1, lux2 = 0;

		Cpl = (atime * again) / LUX_DF;
		lux1 = (int) ((channel_0 - (2 * channel_1)) / Cpl);
		sensor->data_read1 = (float) lux1;
		sensor->data_read2 = (float) channel_1;
		// lux2 = ((0.6 * channel_0) - (channel_1)) / Cpl;
		// This is a two segment lux equation where the first
		// segment (Lux1) covers fluorescent and incandescent light
		// and the second segment (Lux2) covers dimmed incandescent light

		if (lux1 > lux2) {
			return lux1;
		} else {
			return lux2;
		}
	}

	/******************************************************************************
	 function:	Set the TSL2591 interrupt
	 parameter:
	 SET_LOW : Interrupt low threshold
	 SET_HIGH: Interrupt high threshold
	 Info:   Is the channel 0 AD data as a comparison
	 ******************************************************************************/
	void TSL2591_SET_InterruptThreshold(TSL2591_t *sensor, UWORD SET_LOW,
			UWORD SET_HIGH) {
		TSL2591_Enable(sensor);
		TSL2591_Write_Byte(sensor, AILTL_REGISTER, SET_LOW & 0xFF);
		TSL2591_Write_Byte(sensor, AILTH_REGISTER, SET_LOW >> 8);
		TSL2591_Write_Byte(sensor, AIHTL_REGISTER, SET_HIGH & 0xFF);
		TSL2591_Write_Byte(sensor, AIHTH_REGISTER, SET_HIGH >> 8);
		TSL2591_Write_Byte(sensor, NPAILTL_REGISTER, 0);
		TSL2591_Write_Byte(sensor, NPAILTH_REGISTER, 0);
		TSL2591_Write_Byte(sensor, NPAIHTL_REGISTER, 0xff);
		TSL2591_Write_Byte(sensor, NPAIHTH_REGISTER, 0xff);
		TSL2591_Disable(sensor);
	}

	/******************************************************************************
	 function:	Set the TSL2591 interrupt
	 parameter:
	 SET_LOW : Interrupt low threshold
	 SET_HIGH: Interrupt high threshold
	 Info:   Based on Lux as a comparison
	 Need to use the function TSL2591_Read_Lux() to update the data
	 ******************************************************************************/
	void TSL2591_SET_LuxInterrupt(TSL2591_t *sensor, UWORD SET_LOW,
			UWORD SET_HIGH) {
		double Cpl;
		double again;
		UWORD atime, channel_1;
		atime = 100 * TSL2591_Time + 100;
		again = 1.0;
		if (TSL2591_Gain == MEDIUM_AGAIN) {
			again = 25.0;
		} else if (TSL2591_Gain == HIGH_AGAIN) {
			again = 428.0;
		} else if (TSL2591_Gain == MAX_AGAIN) {
			again = 9876.0;
		}

		Cpl = (atime * again) / LUX_DF;
		channel_1 = TSL2591_Read_Channel1(sensor);

		SET_HIGH = (int) (Cpl * SET_HIGH) + 2 * channel_1 - 1;
		SET_LOW = (int) (Cpl * SET_LOW) + 2 * channel_1 + 1;

		TSL2591_Enable(sensor);
		TSL2591_Write_Byte(sensor, AILTL_REGISTER, SET_LOW & 0xFF);
		TSL2591_Write_Byte(sensor, AILTH_REGISTER, SET_LOW >> 8);

		TSL2591_Write_Byte(sensor, AIHTL_REGISTER, SET_HIGH & 0xFF);
		TSL2591_Write_Byte(sensor, AIHTH_REGISTER, SET_HIGH >> 8);

		TSL2591_Write_Byte(sensor, NPAILTL_REGISTER, 0);
		TSL2591_Write_Byte(sensor, NPAILTH_REGISTER, 0);

		TSL2591_Write_Byte(sensor, NPAIHTL_REGISTER, 0xff);
		TSL2591_Write_Byte(sensor, NPAIHTH_REGISTER, 0xff);
		TSL2591_Disable(sensor);
	}


