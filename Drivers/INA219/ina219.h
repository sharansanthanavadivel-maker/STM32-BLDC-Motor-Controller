#ifndef INA219_H
#define INA219_H
#include "stm32f4xx_hal.h"
#include <stdbool.h>
typedef struct {
    I2C_HandleTypeDef *hi2c;
    uint8_t address;
    float current_lsb_a;
    float shunt_ohms;
} INA219_t;
bool INA219_Init(INA219_t *dev, I2C_HandleTypeDef *hi2c, uint8_t address,
                 float current_lsb_a, float shunt_ohms);
bool INA219_ReadCurrentA(INA219_t *dev, float *current_a);
#endif
