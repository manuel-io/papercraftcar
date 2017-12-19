#ifndef HCSR04_H
#define HCSR04_H

#define SR04_DDR DDRD
#define SR04_PORT PORTD
#define SR04_PIN PIND

#define SR04_TIMER1 1
#define SR04_POLL 2

#define SR04_DELAY 15

#define TRIGGER PD5
#define ECHO PD2

#define IS_SET(r, b) (r & (1 << b)) >> b == 1
#define IS_UNSET(r, b) (r & (1 << b)) >> b == 0

void sr04_init(uint8_t);
void sr04_transceive(void);

#endif /* HCSR04_H*/
