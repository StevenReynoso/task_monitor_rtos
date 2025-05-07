#include "gpio.h"

void gpio_set_mode(uint16_t pin, uint8_t mode){
    struct gpio *gpio = GPIO('A' + PINBANK(pin));

    uint8_t gpio_pin = PINNO(pin);

    gpio->MODER &= ~(0x3u << (gpio_pin * 2));
    gpio->MODER |= ((mode & 0x3u) << (gpio_pin * 2));
}

// static cause we only call this in gpio_init_pin.
static void rcc_gpio_enr(uint8_t bank_num){
    RCC_AHB1ENR |= (1 << bank_num);
}


void gpio_init_pin(gpio_config_t cfg){
    struct gpio *gpio = GPIO(PINBANK(cfg.pin) + 'A');
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

void gpio_set_af(uint16_t pin, uint8_t af) {
    uint8_t bank = PINBANK(pin);
    uint8_t gpio_pin = PINNO(pin);

    // Compute address of AFR[0] or AFR[1]
    volatile uint32_t *AFR = (volatile uint32_t *)(0x40020000 + (0x400 * bank) + 0x20 + ((gpio_pin / 8) * 4));
    uint8_t shift = (gpio_pin % 8) * 4;

    *AFR &= ~(0xF << shift);
    *AFR |=  (af  << shift);
}