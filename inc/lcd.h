#ifndef LCD_H
#define LCD_H

#include "stdint.h"

extern volatile uint8_t reset_requested;

void lcd_print(const char *str);
void lcd_send_data(uint8_t cmd);
void lcd_send_cmd(uint8_t cmd);
void lcd_print(const char *str);
void lcd_set_cursor(uint8_t col, uint8_t row);
void lcd_clear(void);
void lcd_init(void);
void lcd_print_char(char c);

#endif //LCD_H