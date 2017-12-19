#include <avr/io.h>
#include <stdio.h>
#include "include/uart.h"
#include "include/adc.h"
#include "include/lights.h"

void
lights_init()
{
  LIGHTS_DDR |= (1 << LIGHTS_HEAD) | (1 << LIGHTS_TAIL);
}

void
lights_update()
{
  uint8_t dat;

  dat = adc_enable(LIGHTS_CHANNEL);

  if (dat > 30) {
    LIGHTS_PORT |= (1 << LIGHTS_HEAD) | (1 << LIGHTS_TAIL);
  } else {
    LIGHTS_PORT &= ~(1 << LIGHTS_HEAD) & ~(1 << LIGHTS_TAIL);
  }

  adc_disable();
}
