#include <stdint.h>
#include "ads1015.h"


/* Private variables*/
static uint8_t m_bit_shift;

/* Function prototypes*/
static void writeRegister(uint8_t i2cAddress, uint8_t reg, uint16_t value);
static void (*I2C_rx)(uint8_t address, uint8_t* data_to_send, uint16_t length);
static void (*I2C_tx)(uint8_t address, uint8_t* data_to_send, uint16_t length);
static int(*Read_pin)(void);


/*******************************************************************************************
* @brief  Initialize ADC.
* @param  None.
* @retval None.
********************************************************************************************/
void ads1015_init(void)
{
	m_bit_shift = 4;

	writeRegister(ADS1015_ADDR, ADS1015_REG_POINTER_HITHRESH, 0x8000);
	writeRegister(ADS1015_ADDR, ADS1015_REG_POINTER_LOWTHRESH, 0x0000);
}

/*******************************************************************************************
* @brief  Initialize ADC of Air quality 5.
* @param  init:   Pointer to function taht initilizes the I2C hardware
* @param  i2c_rx: Pointer Function to read data.
* @param  i2c_tx: Pointer Function to write data.
* @retval None.
********************************************************************************************/
void open_air_quality5_click(int(*init)(void),
	void(*i2c_rx)(uint8_t address, uint8_t* data_to_send, uint16_t length),
	void(*i2c_tx)(uint8_t address, uint8_t* data_to_send, uint16_t length),
	int(*read_pin)(void))
{
	init();
	I2C_rx = i2c_rx;
	I2C_tx = i2c_tx;
	Read_pin = read_pin;

	ads1015_init();
	return;
}

/*******************************************************************************************
* @brief  Checks the conversion ready flag.
* @param  None.
* @retval 1: if convertion has finished, 0 otherwise.
********************************************************************************************/
static int is_ADC_ready(void)
{
	return Read_pin();
}

/*******************************************************************************************
* @brief  Writes 16-bits to the specified destination register.
* @param  i2cAdress: IC I2C address.
* @param  reg:       register address to write.
* @param  value:     value to be written into the register.
* @retval None.
********************************************************************************************/
static void writeRegister(uint8_t i2cAddress, uint8_t reg, uint16_t value)
{
	uint8_t data_to_send[3];
	data_to_send[0] = reg;
	data_to_send[1] = (uint8_t)(value >> 8);
	data_to_send[2] = (uint8_t)(value & 0xff);
	I2C_tx(i2cAddress, data_to_send, sizeof(data_to_send));
}

/*******************************************************************************************
* @brief  Reads 16-bits from the specified register.
* @param  i2cAdress: IC I2C address.
* @param  reg:       address to be read.
* @retval 16 bits read value.
********************************************************************************************/
static uint16_t readRegister(uint8_t i2cAddress, uint8_t reg) 
{
	uint8_t data_received[2];

	I2C_tx(i2cAddress, &reg, sizeof(reg));

	I2C_rx(i2cAddress, data_received, sizeof(data_received));

	return (uint16_t)((data_received[0]<<8)|data_received[1]);
}

/*******************************************************************************************
* @brief  Gets a single-ended ADC reading from the specified channel.
* @param  channel: the channel to be read.
* @retval 16 bits ADC conversion.
********************************************************************************************/

uint16_t read_ADC_single_ended(uint8_t channel)
{
	uint16_t config;

	if (channel > 3)
	{
		return 0;
	}

	config = ADS1015_REG_CONFIG_CQUE_1CONV | // Enable conversion ready pin
		ADS1015_REG_CONFIG_CLAT_NONLAT |  // Non-latching (default val)
		ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
		ADS1015_REG_CONFIG_CMODE_TRAD |   // Traditional comparator (default val)
		ADS1015_REG_CONFIG_DR_1600SPS |   // 1600 samples per second (default)
		ADS1015_REG_CONFIG_MODE_SINGLE |  // Single-shot mode (default)
		ADS1015_REG_CONFIG_PGA_2_048V;    //

	switch (channel)
	{
		case 0:
		{
			config |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
		}
		break;
		case 1:
		{
			config |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
		}
		break;
		case 2:
		{
			config |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
		}
		break;
		case 3:
		{
			config |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
		}
		break;
		default:
		{

		}
		break;
	}

	config |= ADS1015_REG_CONFIG_OS_SINGLE;


	writeRegister(ADS1015_ADDR, ADS1015_REG_POINTER_CONFIG, config);

	while (is_ADC_ready());

	return readRegister(ADS1015_ADDR, ADS1015_REG_POINTER_CONVERT) >> 4;
}


/*******************************************************************************************
* @brief  Gets the last conversion result.
* @param  None.
* @retval Last ADC conversion result.
********************************************************************************************/

int16_t get_last_conversion_result(void)
{
	uint16_t res = readRegister(ADS1015_ADDR, ADS1015_REG_POINTER_CONVERT) >> m_bit_shift;

	if (m_bit_shift == 0)
	{
		return (int16_t)res;
	}
	else
	{
		// Shift 12-bit results right 4 bits for the ADS1015,
		// making sure we keep the sign bit intact
		if (res > 0x07FF)
		{
			// negative number - extend the sign to 16th bit
			res |= 0xF000;
		}
		return (int16_t)res;
	}
}

