#ifndef SYSTICK_H

#include <stdint.h>

extern volatile uint32_t current_time;

#define SYST_CSR (*(volatile uint32_t *) (0xE000E010))
#define SYST_RVR (*(volatile uint32_t *) (0xE000E014))
#define SYST_CVR (*(volatile uint32_t *) (0xE000E018))


int SysTick_Config(uint32_t ticks);
void SysTick_Handler(void);
void delay_ms(uint32_t ms);
void delay_us(uint32_t us);
uint32_t millis(void);

#endif //SYSTICK_H