#ifndef DHT11_H
#define DHT11_H

#include <stdint.h>

typedef struct
{
    uint8_t humidity_int;
    uint8_t humidity_dec;
    uint8_t temp_int;
    uint8_t temp_dec;
    uint8_t checksum;
    uint8_t valid;
    
}dht11_data_t;

extern volatile dht11_data_t dht11_last_reading;

void task_dht11(void);
void dht11_delay_us(uint32_t us);
void uart_print_dht11_data(uint8_t data0,uint8_t data1, uint8_t data2, uint8_t data3);

#endif //DHT11_H