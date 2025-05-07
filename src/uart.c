#include "uart.h"
#include "gpio.h"

void uart_init(void){
    RCC_APB1ENR |= (1 << 17);
    USART2->BRR = 16000000 / 115200;  // Assuming 16 MHz clock → 138 = 115200 baud
    USART2->CR1 |= (1 << 3) | (1 << 13);   // TE = Transmit enable // UE = USART enable
}

void uart_send_char(char c){
    while (!(USART2->SR & (1 << 7)));
    USART2->DR = c;
}

void uart_print(const char *str){
    while(*str){
        uart_send_char(*str++);
    }
}

void uart_print_uint(uint32_t num){
    char buf[10];
    int i = 0;

    if (num == 0){
        uart_send_char('0');
        return;
    }

    while(num > 0 && i < 10){
        buf[i++] = '0' + (num % 10);
        num /= 10;
    }

    while (i--){
        uart_send_char(buf[i]);
    }
}