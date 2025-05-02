#include "systick.h"
#include <stdint.h>

volatile uint32_t current_time = 0;
extern uint32_t SystemCoreClock;


void delay_us(uint32_t us){
    uint32_t ticks = us * (SystemCoreClock / 1000000);
    uint32_t start = SYST_CVR;

    while ((start - SYST_CVR) < ticks) {
        if (SYST_CVR > start) break;  // handles wrap-around
    }
}

void delay_ms(uint32_t ms) {
    uint32_t start = current_time;
    while ((current_time - start) < ms);
}

int SysTick_Config(uint32_t ticks){
    if(ticks > 0xFFFFFF){
        return 1;
    }

    SYST_RVR = ticks - 1;
    SYST_CVR = 0;
    SYST_CSR = 0x07;

    return 0;

}

void SysTick_Handler(void){
    current_time++;
}