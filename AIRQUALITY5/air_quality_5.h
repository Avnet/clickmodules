#pragma once

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <math.h>

/* Private define ------------------------------------------------------------*/
// Channel where sensors are connected to
#define CH_NO2 0
#define CH_NH3 1
#define CH_CO  2

// Load resistance
#define RL_NO2 15000
#define RL_NH3 1000000
#define RL_CO  1000000

// Sensing resistance in clean air (you have to determine these values in an area considered as clean air)
#define R0_NO2  14400
#define R0_NH3  440000 
#define R0_CO   620000

// Less significative bit value with PGA at 2.048 V
#define LSB_VALUE_2_048 0.001



/********* Function prototypes **********/

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
	int(*read_pin)(void));

/*******************************************************************************************
* @brief  Read ADC channel of a given gas type.
* @param  gas_type: Gas type to be read.
* @retval ADC conversion data.
********************************************************************************************/
uint16_t read_gas(uint8_t gas_type);

/*******************************************************************************************
* @brief  Read the sensor's voltage of a given gas type.
* @param  gas_type: Gas type to be read.
* @retval Voltage of the gas.
********************************************************************************************/
float read_voltage(uint8_t gas_type);

/*******************************************************************************************
* @brief  Read the sensor's resistance of a given gas type.
* @param  gas_type: Gas type to be read.
* @retval Resistance of the gas sensor.
********************************************************************************************/
float read_resistance(uint8_t gas_type);

/*******************************************************************************************
* @brief  Read the concentration (ppm) of a given gas type.
* @param  gas_type: Gas type to be read.
* @retval Concentration of the gas (ppm).
********************************************************************************************/
float read_concentration(uint8_t gas_type);

