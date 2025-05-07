#include "rtos.h"
#include "dht11.h"
#include "gpio.h"
#include "uart.h"
#include <stdint.h>

volatile dht11_data_t dht11_last_reading;

// Simplified microsecond delay function
void dht11_delay_us(uint32_t us) {
    for(uint32_t i = 0; i < us * 10; i++) {
        __asm__ volatile ("nop");
    }
}

// Simplified GPIO access
static inline void gpio_output_mode(void) {
    gpio_set_mode(PIN('A', 4), GPIO_MODE_OUTPUT);
}

static inline void gpio_input_mode(void) {
    gpio_set_mode(PIN('A', 4), GPIO_MODE_INPUT);
}

static inline void gpio_set_high(void) {
    GPIO('A')->ODR |= (1 << 4);
}

static inline void gpio_set_low(void) {
    GPIO('A')->ODR &= ~(1 << 4);
}

static inline uint8_t gpio_read(void) {
    return (GPIO('A')->IDR & (1 << 4)) ? 1 : 0;
}


static uint8_t wait_for_pin_state(uint8_t state, uint32_t timeout_count) {
    while (gpio_read() != state) {
        if (--timeout_count == 0) return 0;
    }
    return 1;
}


void task_dht11(void) {
    uint8_t data[5] = {0}; // 5 bytes = 40 bits
    
    // Set data pin as output
    gpio_output_mode();
    
    // Send start signal
    gpio_set_low();          // Pull low
    dht11_delay_us(18000);   // Hold for 20ms (min 18ms)
    gpio_set_high();         // Pull high
    
    
    // Switch to input mode
    gpio_input_mode();
    
    // Wait for DHT11 to pull LOW (response start)
    if (!wait_for_pin_state(0, 500)) {
        uart_print("DHT11: No response (waiting for initial LOW)\r\n");
        dht11_last_reading.valid = 0;
        task_list[4].delay = 3000;
        return;
    }
    
    // Wait for DHT11 to pull HIGH
    if (!wait_for_pin_state(1, 500)) {
        uart_print("DHT11: No response (waiting for HIGH)\r\n");
        dht11_last_reading.valid = 0;
        task_list[4].delay = 3000;
        return;
    }
    
    // Wait for DHT11 to pull LOW again
    if (!wait_for_pin_state(0, 500)) {
        uart_print("DHT11: No response (waiting for LOW before data)\r\n");
        dht11_last_reading.valid = 0;
        task_list[4].delay = 3000;
        return;
    }
    
    uint32_t pulse_widths[40];  // Store for later inspection

    for (int i = 0; i < 40; i++) {
        if (!wait_for_pin_state(1, 100000)) {
            task_list[4].delay = 3000;
            return;
        }

        uint32_t pulse_width = 0;
        while (gpio_read()) {
            pulse_width++;
            if (pulse_width >= 400) break;
            __asm__ volatile ("nop");
        }

        pulse_widths[i] = pulse_width;

        if (pulse_width > 20) {
            data[i / 8] |= (1 << (7 - (i % 8)));
        }
    }

    // Check if the data makes sense
    // DHT11 typically reports humidity 20-90%, temperature 0-50°C
    if (data[0] > 0 && data[0] <= 100 && data[2] <= 50) {
        // Seems reasonable - check checksum
        uint8_t checksum = data[0] + data[1] + data[2] + data[3];
        
        uart_print("Checksum: ");
        uart_print_uint(checksum);
        uart_print(" | Received: ");
        uart_print_uint(data[4]);
        uart_print("\r\n");

        dht11_last_reading.humidity_int = data[0];
        dht11_last_reading.humidity_dec = data[1];
        dht11_last_reading.temp_int     = data[2];
        dht11_last_reading.temp_dec     = data[3];
        dht11_last_reading.checksum     = checksum;
        dht11_last_reading.valid        = 1;

        
        if (checksum == data[4]) {
            uart_print("** VALID READING **\r\n");
            uart_print_dht11_data(data[0], data[1], data[2], data[3]);
        } else {
            uart_print("DHT11: Checksum error\r\n");
            dht11_last_reading.valid = 0;
        }
    } else {
        uart_print("DHT11: Data out of expected range\r\n");
        dht11_last_reading.valid = 0;
    }
    
    // Schedule next reading
    task_list[4].delay = 3000;
}

void uart_print_dht11_data(uint8_t data0,uint8_t data1, uint8_t data2, uint8_t data3){
            uart_print("Humidity: ");
            uart_print_uint(data0);
            uart_print(".");
            uart_print_uint(data1);
            uart_print(" %\r\n");
            
            uart_print("Temperature: ");
            uart_print_uint(data2);
            uart_print(".");
            uart_print_uint(data3);
            uart_print(" °C\r\n");
}