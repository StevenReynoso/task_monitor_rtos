#include "gpio.h"

void rcc_gpio_enr(uint8_t bank_num){
    RCC_AHB1ENR |= (1 << bank_num);
}


void gpio_init_pin(gpio_config_t cfg){
    struct gpio * gpio = GPIO(PINBANK(cfg.pin) + 'A');
    uint8_t gpio_pin = PINNO(cfg.pin);


    rcc_gpio_enr(PINBANK(cfg.pin));

    gpio->MODER &= ~(0x03 << (gpio_pin * 2));
    gpio->MODER |= ((cfg.mode & 0x03) << (gpio_pin * 2));

    gpio->OTYPER &= ~(1 << gpio_pin);
    gpio->OTYPER |= ((cfg.otype & 0x01) << gpio_pin);

    gpio->OSPEEDR &= ~(0x03 << (gpio_pin * 2));
    gpio->OSPEEDR |= ((cfg.speed & 0x03) << (gpio_pin * 2));

    gpio->PUPDR &= ~(0x03 << (gpio_pin * 2));
    gpio->PUPDR |= ((cfg.pull & 0x03) << (gpio_pin * 2));

}