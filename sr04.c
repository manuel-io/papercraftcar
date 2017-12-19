#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "include/uart.h"
#include "include/sr04.h"

extern uint16_t distance;
static uint8_t mode;
static void set_trigger(void);
static uint16_t get_echo(void);

static void
set_trigger()
{
  SR04_PORT |= (1 << TRIGGER);
  _delay_us(10);
  SR04_PORT &= ~(1 << TRIGGER);
}

static uint16_t
get_echo()
{
  uint16_t i = 0;

  while (IS_UNSET(SR04_PIN, ECHO))
  {
    /* wait to recive data
     * expected 8 cycle burst of ultrasound at 40 kHz
     */
  }

  while (IS_SET(SR04_PIN, ECHO)) {
    /* meassure */
    i++;
    _delay_us(1);
  }

  /* Formula: us / 58 = centimeters or us / 148 = inch,
   * or:
   * range = high level time * velocity (340ms) / 2
   */
  
  return i / (58 - SR04_DELAY);
}

void
sr04_transceive()
{
  switch (mode) {

    case SR04_TIMER1:
      set_trigger();
      break;
  
    default:
      set_trigger();
      distance = get_echo();
  }

  return;
}

void
sr04_init(uint8_t m)
{
  mode = m;
  switch (mode) {

    case SR04_TIMER1:
      MCUCR |= (1 << ISC01) | (1 << ISC00);
      GICR |= (1 << INTF0);
      SR04_DDR |= (1 << TRIGGER);
      break;

    default:
      SR04_DDR |= (1 << TRIGGER);
      SR04_DDR &= ~(1 << ECHO);

  }
  _delay_ms(50);
}

ISR(INT0_vect)
{
  if ((MCUCR & 3) == 3) {
    /* Interrupt: Rising Edge */

    /* Start Timer1: CPU 16 000 000 : 8 */
    TCCR1B |= (1 << CS11);

    MCUCR &= ~(1 << ISC00);
  } else {
    /* Interrupt: Falling Edge */

    /* Stop Timer1 */
    TCCR1B &= ~(1 << CS12) & ~(1 << CS11) & ~(1 << CS10);

    distance = TCNT1/116;

    /* Reset Timer1 */
    TCNT1 = 0;

    MCUCR |= (1 << ISC00);
  }
}
