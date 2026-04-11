#ifndef __I2CLCD_H
#define __I2CLCD_H

#include "stm32l4xx_hal.h"

void lcd_init(void); // initialize lcd

void lcd_send_cmd(char cmd); // send command to the lcd

void lcd_send_data(char data); // send data to the lcd

void lcd_send_string(const char *str); // send string to the lcd

void lcd_write(const char *txt, uint8_t line, uint8_t column); // write string to specific line

void lcd_clear(void);

void lcd_clear_line(uint8_t line, uint8_t column);

void lcd_goto_line(uint8_t line, uint8_t column);

#endif /* __I2CLCD_H */