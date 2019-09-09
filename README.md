# FRDM-K64F
Soft Real Time O.S. with Non-Preemptive First Come First Served Task Scheduling, Preemptive Prioritized Interrupt, "Educational" Project - NXP ARM Cortex M4F

Test some basic functionality for NXP K64F in FRDM board, without libraries.

ATTENTION!! There are many inconsistencies with K64F data sheet to set preemptive priority !! ... this code seems to work well !!

- GPIO IN / HW Interrupt: set pre-emptive priority (3), enter ISR, turn on red led, wait some ms, turn of led, clear flag,  exit ISR;

- PIT: interrupt with "high" pre-emptive priority (1), time for delay and state machine, increment internal counter every about 1 ms; 

- ADC: 16 bit read from external potentiometer, in polling mode;

- DAC: put in external pin ADC read value, truncated in 12 bit;

- LPTMR: set timer count-down, call ISR, "low" pre-emptive priority (4): enter ISR, turn on blue led, wait about 1 s, turn of led, clear                flag, exit ISR. Trigger HW interrupt to test the low priority level, so, red led turns on while blue led is on; 

- PWM: configure FTM for put in external pin a pwm signal;

- UART: set serial for command line interface, read data in ISR with higest pre-emptive priority (0, dafault), write same data in polling;

- I2C: serial sinc-communication with OLED 128 x 64, driver SSD1306. Every write data in ISR with pre-emptive priority (2). It 's strange      but Ack bit from OLED seems good only using ISR !! (...problem is solved adding a little delay before check the ACK!) 

- Parallel 8080: parallel communication with OLED 128 x 128, driver SSD1327Z;

- Stepper: drive a little bipolar step-motor with L298N, only set the right sequence of GPIO;

- SPI: AD9833 DDS with 2-wire SPI interface;

- FlexCAN: transit some frame to CAN-USB Converter, by SN65HVD230 transceiver (only TX from M4 to cli is operative... work in progress)

- MAIN: send some frame of CAN bus to a command line, just once, turn on some pixel in OLED 128 x 64 and OLED 128 x 128, draw picture in OLED 128 x 64, ADC and DAC blue led on or off in task1, UART command line read-write, one rotation of step-motor in task2.
  


