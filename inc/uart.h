#ifndef UART_H
#define UART_H

#include <stdint.h>

#define USART2 ((USART_t *) 0x40004400)

typedef struct{
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GTPR;
} USART_t;

void uart_print(const char *str);
void uart_init(void);
void uart_print_uint(uint32_t num);

#endif //UART_H