# FRDM-K64F
Educational BARE-METAL Project 

Test some basic funcitonality for NXP K64F in FRDM board, without libraries.

- GPIO IN / HW Interrupt: set pre-emptive priority (3), enter ISR, turn on red led, wait some ms, turn of led, clear flag,  exit ISR;

- PIT: interrupt with "high" pre-emptive priority (1), time for delay and state machine, increment internal counter every about 1 ms; 

- ADC: 16 bit read from external potentiometer, in polling mode;

- DAC: put in external pin ADC read value, truncated in 12 bit;

- LPTMR: set timer count-down, call ISR, "low" pre-emptive priority (4): enter ISR, turn on blue led, wait about 1 s, turn of led, clear                flag, exit ISR. Trigger HW interrupt to test the low priority level, so, red led turns on while blue led is on; 

- PWM: configure FTM for put in external pin a pwm signal;

- UART: set serial for command line interface, read data in ISR with higest pre-emptive priority (0, dafault), write same data in polling;

- I2C: serial sinc-communication with OLED 128 x 64, driver SSD1306. Every write data in ISR with pre-emptive priority (2). It 's strange      but Ack bit from OLED seems good only using ISR !! 

- Parallel 8080: parallel communication with OLED 128 x 128, driver SSD1327Z;

- Stepper: drive a little bipolar step-motor with L298N, only set the right sequence of GPIO;

- MAIN: for one time, turn on some pixel in OLED 128 x 64, enter in a state machine: 
  1- ADC and DAC, blue led on or off;
  2- UART read-write, spin step-motor. 


