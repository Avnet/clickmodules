/* Private includes ----------------------------------------------------------*/
#include "air_quality_5.h"
#include "ads1015.h"


/* Private variables ---------------------------------------------------------*/
int user_r0_co = R0_CO;
int user_r0_no2 = R0_NO2;
int user_r0_nh3 = R0_NH3;



/*******************************************************************************************
* @brief  Initialize ADC of Air quality 5.
* @param  init:   Pointer to function taht initilizes the I2C hardware
* @param  i2c_rx: Pointer Function to read data.
* @param  i2c_tx: Pointer Function to write data.
* @retval None.
********************************************************************************************/
void init_gas_sensor(int(*init)(void),
	void(*i2c_rx)(uint8_t address, uint8_t* data_to_send, uint16_t length),
	void(*i2c_tx)(uint8_t address, uint8_t* data_to_send, uint16_t length),
	int(*read_pin)(void))
{
	open_air_quality5_click(init, i2c_rx, i2c_tx, read_pin);
}

/*******************************************************************************************
* @brief  Read ADC channel of a given gas type.
* @param  gas_type: Gas type to be read.
* @retval ADC conversion data.
********************************************************************************************/
uint16_t read_gas(uint8_t gas_type)
{
	return read_ADC_single_ended(gas_type);
}

/*******************************************************************************************
* @brief  Read the sensor's voltage of a given gas type.
* @param  gas_type: Gas type to be read.
* @retval Voltage of the gas.
********************************************************************************************/
float read_voltage(uint8_t gas_type)
{
	uint16_t data_adc;
	float voltage_adc;

	data_adc = read_ADC_single_ended(gas_type);

	voltage_adc = (float)data_adc * (float)LSB_VALUE_2_048;

	return voltage_adc;
}

/*******************************************************************************************
* @brief  Read the sensor's resistance of a given gas type.
* @param  gas_type: Gas type to be read.
* @retval Resistance of the gas sensor.
********************************************************************************************/
float read_resistance(uint8_t gas_type)
{
	float voltage_adc;
	float resistance;

	voltage_adc = read_voltage(gas_type);

	switch (gas_type)
	{
		case CH_NO2:
		{
			resistance = (float)(voltage_adc * RL_NO2 / (3.3 - voltage_adc));
		}
		break;
		case CH_NH3:
		{
			resistance = (float)(voltage_adc * RL_NH3 / (3.3 - voltage_adc));
		}
		break;
		case CH_CO:
		{
			resistance = (float)(voltage_adc * RL_CO / (3.3 - voltage_adc));
		}
		break;
		default:
		break;
	}
	
	return resistance;
}

/*******************************************************************************************
* @brief  Read the concentration (ppm) of a given gas type.
* @param  gas_type: Gas type to be read.
* @retval Concentration of the gas (ppm).
********************************************************************************************/
float read_concentration(uint8_t gas_type)
{
	float resistance;
	float concentration;

	resistance = read_resistance(gas_type);

	// Concentration is got from a trendline that you can see in the excel file
	switch (gas_type)
	{
		case CH_NO2:
		{
			concentration = (float)(0.1621*powf(resistance / user_r0_no2, (float)0.9816));
		}
		break;
		case CH_NH3:
		{
			concentration = (float)(0.5898*powf(resistance / user_r0_nh3, (float)-1.888));
		}
		break;
		case CH_CO:
		{
			concentration = (float)(4.8201*powf(resistance / user_r0_co, (float)-1.156));
		}
		break;
		default:
			break;
	}

	return concentration;
}
