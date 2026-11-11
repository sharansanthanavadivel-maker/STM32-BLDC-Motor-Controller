#ifndef SSD1306_H
#define SSD1306_H
#include "stm32f4xx_hal.h"
#include <stdbool.h>
bool SSD1306_Init(I2C_HandleTypeDef *hi2c, uint8_t address);
void SSD1306_ShowStatus(const char *line1, const char *line2);
#endif
