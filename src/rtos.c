#include <stdint.h>
#include "rtos.h"
#include "gpio.h"
#include "systick.h"
#include "uart.h"
#include "lcd.h"



volatile uint8_t reset_requested = 0;

volatile uint8_t task_flags[MAX_TASKS] = {0};
uint32_t task_run_count[MAX_TASKS] = {0};
Task task_list[MAX_TASKS];

void add_task(uint8_t index, void (*func)(void), uint32_t delay_ms){
    task_list[index].func = func;
    task_list[index].delay = delay_ms;
    task_list[index].ready = 1;
}

void rtos_set_flag(uint8_t task_id) {
    task_flags[task_id] = 1;
    uart_print("RTOS Flag Set\n");
}

uint8_t rtos_get_and_clear_flag(uint8_t task_id) {
    if (task_flags[task_id]) {
        task_flags[task_id] = 0;
        return 1;
    }
    return 0;
}

void run_scheduler(void){
    while(1){
        update_task_delays();  // ← This must be called every loop!
        for(int i = 0; i < MAX_TASKS; i++){
            if(task_list[i].ready){
                task_list[i].func();
                task_list[i].ready = 0;
                task_run_count[i]++;
            }
        }
    }
}

void update_task_delays(void){
    static uint32_t last_time = 0;
    if(current_time != last_time){
        last_time = current_time;

        for(int i = 0; i < MAX_TASKS; i++){
            task_list[i].delay--;
            if(task_list[i].delay == 0){
                task_list[i].ready = 1;
            }
        }
    }
}

void task_button_check(void){
    static uint8_t last_state = 1;
    uint8_t current_state = (GPIO('A')->IDR & (1 << 9)) ? 1 : 0;

    if (last_state == 1 && current_state == 0){
        reset_requested = 1;
        rtos_set_flag(2);
    }

    last_state = current_state;
    task_list[2].delay = 50;
}

void task_blink(void){
    lcd_clear();
    lcd_print("BLinking task");              // will need to look into lcd_puts, 
    task_list[0].delay = 1000;
}

void task_heartbeat(void){
    if (rtos_get_and_clear_flag(1)) {
    task_flags[1] = 0;  // Clear after reacting
    lcd_clear();
    lcd_print("Signal Received!");
    uart_print("Flag received by heartbeat task\r\n");
    }

    lcd_clear();
    lcd_print("Task 2 is Alive");
    task_list[1].delay = 2000;
}

void task_monitor(void){
    static int current = 0;

    lcd_clear();

    if(reset_requested){
        for(int i = 0; i < MAX_TASKS; i++){
            task_run_count[i] = 0;
        }

        lcd_clear();
        lcd_print("Counters Reset");
        uart_print("UART Button Pressed, Counter Reset\r\n");
        task_list[3].delay = 1000;
        reset_requested = 0;
        return;
    }

    for (int row = 0; row < 2; row++) {
        int index = (current + row) % MAX_TASKS;

        lcd_set_cursor(0, row);  // col, row

        lcd_print("T");
        lcd_print_char('0' + index);
        lcd_print(" R:");

        int run = task_run_count[index];
        lcd_print_char('0' + (run / 100) % 10);
        lcd_print_char('0' + (run / 10) % 10);
        lcd_print_char('0' + run % 10);
        
        lcd_print(" D:");
        
        int delay = task_list[index].delay;
        lcd_print_char('0' + (delay / 100) % 10);
        lcd_print_char('0' + (delay / 10) % 10);
        lcd_print_char('0' + delay % 10);
    }

    current = (current + 2) % MAX_TASKS;
    task_list[3].delay = 1000;
}