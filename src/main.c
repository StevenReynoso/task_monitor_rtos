#include "gpio.h"
#include "lcd.h"
#include "systick.h"

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

    SysTick_Config(SystemCoreClock / 1000); 
    gpio_init_pin(lcd_cfg5);
    gpio_init_pin(lcd_cfg6);
    gpio_init_pin(lcd_cfg7);
    gpio_init_pin(lcd_cfg8);
    gpio_init_pin(RS_cfg);
    gpio_init_pin(En_cfg);
    gpio_init_pin(RW_cfg);

    lcd_init();
    lcd_clear();
    lcd_set_cursor(0,0);
    lcd_print("HAVING FUN WITH");
    lcd_set_cursor(0, 1);
    lcd_print("___BARE METAL___");

    while(1);

}