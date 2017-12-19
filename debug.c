#include <avr/io.h>

#include <stdio.h>

#include "include/uart.h"
#include "include/debug.h"
#include "include/mpu60x0.h"

extern uint16_t distance;
extern uint16_t temperature;

static void (*init)();
static void (*message)(uint8_t, uint8_t *);
static void debug_uart(uint8_t, uint8_t *);

static void
debug_uart(uint8_t sc, uint8_t *msg)
{
  uint8_t output[80];

  switch(sc) {

    case 1:
      uart_putstr(msg);
      break;

    case 2:
      sprintf((char *)output, "Distance: %dcm\r", distance);
      uart_putstr(output);

      sprintf((char *)output, "Temperature: %d C\r", (int16_t)(temperature));
      uart_putstr(output);
      break;
  }
}

void
debug_init(uint8_t t)
{
  switch (t) {
    case DEBUG_UART:
      init = uart_init;
      message = debug_uart;
    break;

    case DEBUG_LOG:
      message = NULL;
    break;

    default:
      init = uart_init;
      message = debug_uart;
  }
  
  init();
}

void
debug_message(char *str)
{
  message(1, (uint8_t *)str);
}

void
debug_update()
{
  message(2, (uint8_t *)"");
}

