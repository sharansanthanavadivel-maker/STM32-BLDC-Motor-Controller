#include "ina219.h"
#define INA219_REG_CONFIG       0x00
#define INA219_REG_CALIBRATION  0x05
#define INA219_REG_CURRENT      0x04

static bool write16(INA219_t *d, uint8_t reg, uint16_t value) {
    uint8_t b[3] = {reg, (uint8_t)(value >> 8), (uint8_t)value};
    return HAL_I2C_Master_Transmit(d->hi2c, d->address, b, 3, 100) == HAL_OK;
}
static bool read16(INA219_t *d, uint8_t reg, int16_t *value) {
    uint8_t r = reg, b[2];
    if (HAL_I2C_Master_Transmit(d->hi2c, d->address, &r, 1, 100) != HAL_OK) return false;
    if (HAL_I2C_Master_Receive(d->hi2c, d->address, b, 2, 100) != HAL_OK) return false;
    *value = (int16_t)((uint16_t)b[0] << 8 | b[1]);
    return true;
}
bool INA219_Init(INA219_t *d, I2C_HandleTypeDef *hi2c, uint8_t address,
                 float current_lsb_a, float shunt_ohms) {
    if (!d || !hi2c || current_lsb_a <= 0.0f || shunt_ohms <= 0.0f) return false;
    d->hi2c = hi2c; d->address = address;
    d->current_lsb_a = current_lsb_a; d->shunt_ohms = shunt_ohms;
    /* Calibration = 0.04096 / (current_LSB * Rshunt). */
    uint32_t cal = (uint32_t)(0.04096f / (current_lsb_a * shunt_ohms));
    if (cal > 65535U) cal = 65535U;
    return write16(d, INA219_REG_CONFIG, 0x399FU) &&
           write16(d, INA219_REG_CALIBRATION, (uint16_t)cal);
}
bool INA219_ReadCurrentA(INA219_t *d, float *current_a) {
    int16_t raw;
    if (!d || !current_a || !read16(d, INA219_REG_CURRENT, &raw)) return false;
    *current_a = (float)raw * d->current_lsb_a;
    return true;
}
