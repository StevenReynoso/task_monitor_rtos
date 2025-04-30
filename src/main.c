#include "gpio.h"
#include "lcd.h"


int main(void){

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
    
    //might have to change these or manually do it?
    gpio_config_t RS_cfg = {                      
        .pin = PIN('A', 0),                         
        .mode = GPIO_MODE_OUTPUT,                   
        .otype = GPIO_OTYPE_PUSHPULL,               
        .speed = GPIO_SPEED_HIGH,                  
        .pull = GPIO_NO_PULL                        
    };

    gpio_config_t En_cfg = {                      
        .pin = PIN('A', 1),                         
        .mode = GPIO_MODE_OUTPUT,                   // does this matter for enable pin?
        .otype = GPIO_OTYPE_PUSHPULL,               
        .speed = GPIO_SPEED_HIGH,                  
        .pull = GPIO_NO_PULL                        
    };

    
    gpio_init_pin(lcd_cfg5);
    gpio_init_pin(lcd_cfg6);
    gpio_init_pin(lcd_cfg7);
    gpio_init_pin(lcd_cfg8);
    gpio_init_pin(RS_cfg);
    gpio_init_pin(En_cfg);

    lcd_init();
    lcd_clear();
    lcd_set_cursor(0,0);
    lcd_print("HAVING FUN WITH");
    lcd_set_cursor(0, 1);
    lcd_print("___BARE METAL___");

    while(1);

}