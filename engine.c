#include <avr/io.h>
#include <util/delay.h>
#include "include/engine.h"

void
engine_init()
{
  ENGINE_DDR |= (1 << MOTOR1_IN) | (1 << MOTOR1_OUT) | \
                (1 << MOTOR2_IN) | (1 << MOTOR2_OUT);
}

void
engine_update()
{
  ENGINE_PORT |= (1 << MOTOR2_IN);
  _delay_us(1000);
  ENGINE_PORT &= ~(1 << MOTOR2_IN);
  _delay_us(20000 - 1000);
}
