/*
 * Minimal display transport placeholder.
 * Implement font rendering or integrate a tested SSD1306 library before deployment.
 * This driver initializes the panel and currently sends no text graphics.
 */
#include "ssd1306.h"
static I2C_HandleTypeDef *s_i2c;
static uint8_t s_addr;
static bool cmd(uint8_t c) {
    uint8_t b[2] = {0x00, c};
    return HAL_I2C_Master_Transmit(s_i2c, s_addr, b, 2, 100) == HAL_OK;
}
bool SSD1306_Init(I2C_HandleTypeDef *hi2c, uint8_t address) {
    if (!hi2c) return false;
    s_i2c = hi2c; s_addr = address;
    const uint8_t init_seq[] = {0xAE,0x20,0x00,0x40,0xA1,0xC8,0x81,0x7F,
        0xA6,0xA8,0x3F,0xD3,0x00,0xD5,0x80,0xD9,0xF1,0xDA,0x12,
        0xDB,0x40,0x8D,0x14,0xAF};
    for (unsigned i=0; i<sizeof(init_seq); i++) if (!cmd(init_seq[i])) return false;
    return true;
}
void SSD1306_ShowStatus(const char *line1, const char *line2) {
    /* Text rendering intentionally not implemented in this starter driver.
       Integrate a font-enabled SSD1306 library to display these strings. */
    (void)line1; (void)line2;
}
