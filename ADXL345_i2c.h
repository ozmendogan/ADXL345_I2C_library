/*
 * ADXL345_i2c.h
 *
 *  Created on: Feb 25, 2023
 *      Author: dogan
 */

#ifndef INC_ADXL345_I2C_H_
#define INC_ADXL345_I2C_H_

#define ADXL_ADDRESS 0xA6

#include "stm32f4xx_hal.h"

extern I2C_HandleTypeDef hi2c1;


void values(int16_t *x, int16_t *y, int16_t *z );
void Scan_I2C_Address();
void ADXL_Write(uint8_t reg, uint8_t value);
void ADXL_Read(uint8_t reg, uint8_t numberOfBytes);
void ADXL_Init();




#endif /* INC_ADXL345_I2C_H_ */
