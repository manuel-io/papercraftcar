#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <avr/io.h>

#include "include/debug.h"
#include "include/i2c.h"
#include "include/gyro.h"
#include "include/accel.h"
#include "include/temp.h"
#include "include/temp.h"
#include "include/sr04.h"
#include "include/uart.h"
#include "include/lights.h"
#include "include/engine.h"

uint16_t volatile distance;
uint16_t volatile temperature;

int
main(void)
{
  DDRB |= (1 << 5);

  sei();

  debug_init(DEBUG_UART);

  lights_init();
  i2c_init();
  gyro_init();
  accel_init();
  temp_init();
  sr04_init(SR04_TIMER1);
  engine_init();

  PORTB |= (1 << 5);

  while (1) {

    lights_update();
    sr04_transceive();
    temp_update();

    for (uint8_t i = 0; i < 5; i++) {
      engine_update();
    }

    debug_update();
  }

  return 0;
}
