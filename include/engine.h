#ifndef ENGINE_H
#define ENGINE_H

#define ENGINE_DDR  DDRB
#define ENGINE_PORT PORTB
#define MOTOR1_IN   PB0
#define MOTOR1_OUT  PB1
#define MOTOR2_IN   PB2
#define MOTOR2_OUT  PB3

void engine_init(void);
void engine_update(void);

#endif /* ENGINE_H */
