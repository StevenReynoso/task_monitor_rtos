#ifndef RTOS_H
#define RTOS_H

#include <stdint.h>

typedef struct {
    void (*func)(void);
    uint32_t delay;
    uint8_t ready;
} Task;

#define MAX_TASKS 5

extern uint32_t task_run_count[MAX_TASKS];
extern volatile uint8_t task_flags[MAX_TASKS];
extern Task task_list[MAX_TASKS];

void add_task(uint8_t index, void(*func)(void), uint32_t delay_ms);
void run_scheduler(void);
void update_task_delays(void);
void task_blink(void);
void task_heartbeat(void);
void task_button_check(void);
void task_monitor(void);

#endif //RTOS_H