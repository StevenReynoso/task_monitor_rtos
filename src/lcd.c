#include "gpio.h"
#include <stdint.h>

static void lcd_send_nibble(uint8_t nibble){
    GPIO('A')->ODR &= ~((1 << 5) | (1 << 6) | (1 << 7) | (1 << 8));

    if (nibble & 0x01) GPIO('A')->ODR |= (1 << 5);
    if (nibble & 0x02) GPIO('A')->ODR |= (1 << 6);
    if (nibble & 0x04) GPIO('A')->ODR |= (1 << 7);
    if (nibble & 0x08) GPIO('A')->ODR |= (1 << 8);

}

void delay_us(uint32_t us) {
    for (volatile uint32_t i = 0; i < us * 16; i++) {
        __asm__("nop");
    }
}

void delay_ms(uint32_t ms) {
    while (ms--) delay_us(1000);
}

static void lcd_pulse_enable(void) {
    GPIO('A')->ODR |= (1 << 1);
    delay_us(1);
    GPIO('A')->ODR &= ~(1 << 1);
    delay_us(100);
}

void lcd_send_cmd(uint8_t cmd) {
    GPIO('A')->ODR &= ~(1 << 0);  // RS = 0 for command

    lcd_send_nibble(cmd >> 4);
    lcd_pulse_enable();
    lcd_send_nibble(cmd & 0x0F);
    lcd_pulse_enable();
}

void lcd_send_data(uint8_t data) {
    GPIO('A')->ODR |= (1 << 0);  // RS = 1 for data

    lcd_send_nibble(data >> 4);
    lcd_pulse_enable();
    lcd_send_nibble(data & 0x0F);
    lcd_pulse_enable();
}

void lcd_print(const char *str) {
    while (*str) {
        lcd_send_data(*str++);
    }
}

void lcd_clear(void) {
    lcd_send_cmd(0x01);
    delay_ms(2);
}

void lcd_set_cursor(uint8_t col, uint8_t row) {
    uint8_t address = (row == 0) ? col : (0x40 + col);
    lcd_send_cmd(0x80 | address);
}


void lcd_init(void) {
    delay_ms(50);                                                   // wait 15ms or more after vdd reaches 4.5v, wait 4.1 ms or more, wait 100 micro seconds, end of initialization

    lcd_send_nibble(0x03); // 0b0011
    lcd_pulse_enable();
    delay_ms(5);

    lcd_send_nibble(0x03);
    lcd_pulse_enable();
    delay_us(150);

    lcd_send_nibble(0x03);
    lcd_pulse_enable();
    delay_us(150);

    lcd_send_nibble(0x02); 
    lcd_pulse_enable();
    delay_us(150);

    // Now you're in 4-bit mode — proceed normally
    lcd_send_cmd(0x28);  // Function set: 4-bit, 2 line, 5x8 font
    lcd_send_cmd(0x0C);  // Display ON, cursor OFF
    lcd_send_cmd(0x06);  // Entry mode: increment
    lcd_send_cmd(0x01);  // Clear display
    delay_ms(2);
}
