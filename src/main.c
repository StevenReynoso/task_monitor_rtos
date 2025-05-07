#include "gpio.h"
#include "uart.h"
#include "lcd.h"
#include "systick.h"
#include "rtos.h"
#include "task_monitor.h"
#include "dht11.h"

uint32_t SystemCoreClock = 16000000;

int main(void){
    //                 D7 is also BF (Busy Flag)
    //bit manipulation sheet D7 | D6 | D5 | D4 | R/W | E | RS | , documentation says DB4 - DB7 on LCD is for 4 or 8 bit transfer, Not DB0-DB3
    //     All Port A -      8     7    6    5   10    1   0 
    gpio_config_t lcd_cfg5 = {                      
        .pin = PIN('A', 5),                         
        .mode = GPIO_MODE_OUTPUT,                   
        .otype = GPIO_OTYPE_PUSHPULL,               
        .speed = GPIO_SPEED_HIGH,                  
        .pull = GPIO_NO_PULL                        
    };

    gpio_config_t lcd_cfg6 = {                      
        .pin = PIN('A', 6),                         
        .mode = GPIO_MODE_OUTPUT,                   
        .otype = GPIO_OTYPE_PUSHPULL,               
        .speed = GPIO_SPEED_HIGH,                  
        .pull = GPIO_NO_PULL                        
    };
    gpio_config_t lcd_cfg7 = {                      
        .pin = PIN('A', 7),                         
        .mode = GPIO_MODE_OUTPUT,                   
        .otype = GPIO_OTYPE_PUSHPULL,               
        .speed = GPIO_SPEED_HIGH,                  
        .pull = GPIO_NO_PULL                        
    };
    gpio_config_t lcd_cfg8 = {          
        .pin = PIN('A', 8),                         
        .mode = GPIO_MODE_OUTPUT,                   
        .otype = GPIO_OTYPE_PUSHPULL,               
        .speed = GPIO_SPEED_HIGH,                  
        .pull = GPIO_NO_PULL                        
    };

    gpio_config_t btn_cfg = {          
        .pin = PIN('A', 9),                         
        .mode = GPIO_MODE_INPUT,                   
        .otype = GPIO_OTYPE_PUSHPULL,               
        .speed = GPIO_SPEED_HIGH,                  
        .pull = GPIO_PULL_UP                        
    };

    gpio_config_t uart_cfg = {
        .pin = PIN('A', 2),
        .mode = GPIO_MODE_AF,
        .otype = GPIO_OTYPE_PUSHPULL,
        .speed = GPIO_SPEED_HIGH,
        .pull = GPIO_NO_PULL
    };

    gpio_config_t RS_cfg = {                      
        .pin = PIN('A', 0),                         
        .mode = GPIO_MODE_OUTPUT,                   
        .otype = GPIO_OTYPE_PUSHPULL,               
        .speed = GPIO_SPEED_HIGH,                  
        .pull = GPIO_NO_PULL                        
    };

    gpio_config_t En_cfg = {                      
        .pin = PIN('A', 1),                         
        .mode = GPIO_MODE_OUTPUT,                   
        .otype = GPIO_OTYPE_PUSHPULL,               
        .speed = GPIO_SPEED_HIGH,                  
        .pull = GPIO_NO_PULL                        
    };

    gpio_config_t RW_cfg = {                      
        .pin = PIN('A', 10),                         
        .mode = GPIO_MODE_OUTPUT,                   
        .otype = GPIO_OTYPE_PUSHPULL,               
        .speed = GPIO_SPEED_HIGH,                  
        .pull = GPIO_NO_PULL                       
    };

    gpio_config_t dht_cfg = {
        .pin = PIN('A', 4),
        .mode = GPIO_MODE_OUTPUT,
        .otype = GPIO_OTYPE_PUSHPULL,
        .speed = GPIO_SPEED_HIGH,
        .pull = GPIO_NO_PULL
    };

    SysTick_Config(SystemCoreClock / 1000);
    
    gpio_init_pin(lcd_cfg5);
    gpio_init_pin(lcd_cfg6);
    gpio_init_pin(lcd_cfg7);
    gpio_init_pin(lcd_cfg8);
    gpio_init_pin(btn_cfg);
    gpio_init_pin(uart_cfg);
    gpio_init_pin(RS_cfg);
    gpio_init_pin(En_cfg);
    gpio_init_pin(RW_cfg);
    gpio_init_pin(dht_cfg);
    
    uart_init();
    gpio_set_af(uart_cfg.pin, 7);


    lcd_init();
    lcd_clear();

    uart_print("UART initialized!\r\n");
    
    add_task(0, task_blink, 0);
    add_task(1, task_heartbeat, 1000);
    add_task(2, task_button_check, 0);
    add_task(3, task_monitor, 250);
    add_task(4, task_dht11, 5000);
    task_list[3].func = task_monitor_lcd;

    run_scheduler();  // n


    while(1);

}