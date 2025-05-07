#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

struct gpio {
    volatile uint32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR;
};

// Gpio setup, only need the one bus for ports,
#define RCC_AHB1ENR (*(volatile uint32_t *) (0x40023800 + 0x30))
#define RCC_APB1ENR (*(volatile uint32_t *) (0x40023800 + 0x40))

//nice setup to split the port and pin, making easier declaration in main.
#define GPIO(bank) ((struct gpio *)(0x40020000 + (0x400 * ((bank) - 'A'))))
#define PIN(bank, num) (((bank - 'A') << 8) | (num))
#define PINNO(pin) ((pin) & 0xFF)
#define PINBANK(pin) ((pin) >> 8)


// struct setup for easier pin setup in main.
typedef enum{
    GPIO_MODE_INPUT         = 0x00,
    GPIO_MODE_OUTPUT        = 0x01,
    GPIO_MODE_AF            = 0x02,
    GPIO_MODE_ANALOG        = 0x03,
} gpio_mode_t;

typedef enum{
    GPIO_OTYPE_PUSHPULL     = 0x00,
    GPIO_OTYPE_OPENDRAIN    = 0x01
} gpio_type_t;

typedef enum{
    GPIO_SPEED_LOW          = 0x00,
    GPIO_SPEED_MEDIUM       = 0x01,
    GPIO_SPEED_FAST         = 0x02,
    GPIO_SPEED_HIGH         = 0x03,
} gpio_speed_t;

typedef enum{
    GPIO_NO_PULL            = 0x00, 
    GPIO_PULL_UP            = 0x01,
    GPIO_PULL_DOWN          = 0x02,
} gpio_pupdr_t;

typedef struct{
    uint16_t        pin;
    gpio_mode_t     mode;
    gpio_type_t     otype;
    gpio_speed_t    speed;
    gpio_pupdr_t    pull;
} gpio_config_t;

void gpio_init_pin(gpio_config_t cfg);
void gpio_set_mode(uint16_t pin, uint8_t mode);
void gpio_set_af(uint16_t pin, uint8_t af);

#endif //GPIO_H