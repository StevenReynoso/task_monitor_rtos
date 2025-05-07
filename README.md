# Task Monitor RTOS (Bare-Metal, No Libraries)

This project implements a cooperative real-time operating system (RTOS) on an STM32F4 microcontroller without using any external libraries (no HAL, no CMSIS). It serves as a task monitor system, displaying real-time information about task execution, system health, and live sensor data on an LCD1602 display.

## Overview

The system runs fully on bare-metal C and demonstrates the following:

- Cooperative task scheduler (non-preemptive)
- LCD1602 interface via GPIO (4-bit mode, with busy flag checking)
- UART logging interface using direct register access
- Push-button handling with debouncing
- DHT11 temperature/humidity sensor integration via single-wire protocol
- Real-time task monitoring with dynamic LCD updates

## Features

### DEMO
![Task_monitor_gif](https://github.com/user-attachments/assets/aa2ea48c-ba48-4b08-a74a-b65166db51af)


### RTOS (Bare-Metal)

- Custom task structure with `delay`, `ready`, and `func` fields
- Round-robin execution model
- SysTick-based timing (1 ms granularity)
- Per-task counters and delay management

### LCD Task Monitor

- Displays two tasks per screen refresh:
  - Task ID
  - Run count
  - Delay (ms)
- Automatically rotates through tasks every second
- Dedicated display for DHT11 sensor readings (temp/humidity)

### UART Debug Logging

- Fully manual UART2 driver (PA2, 115200 baud)
- Real-time logging of system events, sensor readings, and errors
- Serial monitor supported via USB/TTL serial adapter

### DHT11 Integration

- Bit-banged single-wire protocol
- Pulse-width-based decoding of 40-bit transmission
- Data validation via checksum
- Live display of humidity and temperature

### Button-Triggered Events

- GPIO input on PA9
- On press, resets task run counters and logs event over UART
- Flag-driven signaling to prevent race conditions

## Hardware

- **MCU**: STM32F446RE (or compatible STM32F4)
- **Display**: LCD1602 (parallel interface, 4-bit)
- **Sensor**: DHT11 temperature/humidity sensor
- **Interface**: USB UART (TTL serial, 115200 baud)
- **Button**: Connected to PA9 for reset trigger

## Build & Flash

Assumes `arm-none-eabi-gcc` toolchain is installed.

```bash
make
st-flash write build/main.bin 0x8000000
```
Project Goals

## This project was built as a hands-on exercise in:

  - Understanding low-level peripheral control on ARM Cortex-M
  - Writing a custom scheduler without relying on CMSIS or HAL
  - Building scalable embedded software from scratch
  - Interfacing with real-world sensors and displays using strict timing

## Future Improvements

  - Add I2C or SPI peripherals
  - Task priority handling
  - Preemptive scheduling
  - Low-power sleep mode
  - External EEPROM logging

