#include <avr/io.h>
#include <util/delay.h>
#include "include/engine.h"

void
engine_init()
{
  ENGINE_DDR |= (1 << MOTOR1_IN) | (1 << MOTOR1_OUT) | \
                (1 << MOTOR2_IN) | (1 << MOTOR2_OUT);
  ENGINE_PORT |= (1 << MOTOR2_IN);
}

void
engine_update()
{
}
