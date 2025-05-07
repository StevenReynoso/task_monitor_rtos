#include "task_monitor.h"
#include "rtos.h"
#include "lcd.h"
#include "dht11.h"
#include "uart.h"
#include <stdint.h>

extern volatile dht11_data_t dht11_last_reading;
extern uint32_t task_run_count[MAX_TASKS];
extern Task task_list[MAX_TASKS];

void task_monitor_lcd(void) {
    static uint8_t state = 0;

    lcd_clear();

    if (state < MAX_TASKS) {
        // Show task info
        lcd_set_cursor(0, 0);
        lcd_print("T");
        lcd_print_char('0' + state);
        lcd_print(" R:");

        uint32_t run = task_run_count[state];
        lcd_print_char('0' + (run / 100) % 10);
        lcd_print_char('0' + (run / 10) % 10);
        lcd_print_char('0' + run % 10);

        lcd_set_cursor(0, 1);
        lcd_print("D:");
        uint32_t delay = task_list[state].delay;
        lcd_print_char('0' + (delay / 100) % 10);
        lcd_print_char('0' + (delay / 10) % 10);
        lcd_print_char('0' + delay % 10);
    } else {
        // Show DHT11 info
        lcd_set_cursor(0, 0);
        if (dht11_last_reading.valid) {
            lcd_print("H:");
            lcd_print_char('0' + (dht11_last_reading.humidity_int / 10));
            lcd_print_char('0' + (dht11_last_reading.humidity_int % 10));
            lcd_print(".");
            lcd_print_char('0' + (dht11_last_reading.humidity_dec % 10));
            lcd_print("%");

            lcd_set_cursor(0, 1);
            lcd_print("T:");
            lcd_print_char('0' + (dht11_last_reading.temp_int / 10));
            lcd_print_char('0' + (dht11_last_reading.temp_int % 10));
            lcd_print(".");
            lcd_print_char('0' + (dht11_last_reading.temp_dec % 10));
            lcd_print("C");
        } else {
            lcd_print("DHT11 ERROR");
            lcd_set_cursor(0, 1);
            lcd_print("Invalid Reading");
        }
    }

    state = (state + 1) % (MAX_TASKS + 1); // 0–MAX_TASKS, then DHT11
    task_list[3].delay = 1000; // 1 sec refresh
}