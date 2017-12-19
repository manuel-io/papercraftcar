#include <avr/io.h>
#include <util/delay.h>
#include "include/adc.h"

/*
 * The ATmega8 features a 10-bit successive approximation ADC. The ADC
 * is connected to an 8- channel Analog Multiplexer which allows eight
 * single-ended voltage inputs constructed from the pins of Port C.
 * The single-ended voltage inputs refer to 0V (GND).
 *
 * The ADC converts an analog input voltage to a 10-bit digital value
 * through successive approximation. The minimum value represents
 * GND and the maximum value represents the voltage on the AREF pin
 * minus 1 LSB. Optionally, AVCC or an internal 2.56V reference
 * voltage may be connected to the AREF pin by writing to the REFSn
 * bits in the ADMUX Register. The internal voltage reference may thus
 * be decoupled by an external capacitor at the AREF pin to improve
 * noise immunity.
 *
 * The analog input channel is selected by writing to the MUX bits in
 * ADMUX. Any of the ADC input pins, as well as GND and a fixed
 * bandgap voltage reference, can be selected as single ended inputs
 * to the ADC. The ADC is enabled by setting the ADC Enable bit, ADEN
 * in ADCSRA. Voltage reference and input channel se lections will
 * not go into effect until ADEN is set. The ADC does not consume
 * power when ADEN is cleared, so it is recommended to switch off the
 * ADC before entering power saving sleep modes. The ADC generates a
 * 10-bit result which is presented in the ADC Data Registers, ADCH
 * and ADCL. By default, the result is presented right adjusted, but
 * can optionally be presented left adjusted by setting the ADLAR bit
 * in ADMUX.
 *
 * =========================================================================
 *
 * ADMUX: ADC Multiplexer Selection Register
 * ------------------------------------------------------------
 * | 7     | 6     | 5     | 4    | 3    | 2    | 1    | 0    |
 * | REFS1 | REFS0 | ADLAR |      | MUX3 | MUX2 | MUX1 | MUX0 |
 * ------------------------------------------------------------
 *
 * ------------------------------------------------------------
 * | REFS1 | REFS0 | Voltage Reference Selection              |
 * ------------------------------------------------------------
 * |     0 |     0 | AREF, Internal Vref turned off           |
 * |     0 |     1 | AVCC with external capacitor at AREF pin |
 * |     1 |     0 | Reserved                                 |
 * |     1 |     1 | Internal 2.56V Voltage Reference         |
 * |       |       | with external capacitor at AREF pin      |
 * ------------------------------------------------------------
 *
 * ADLAR: The ADLAR bit affects the presentation of the ADC conversion
 * result in the ADC Data Register.
 *
 * --------------------------------
 * | MUX3..0 | Single Ended Input |
 * --------------------------------
 * | 0000    | ADC0               |
 * | 0001    | ADC1               |
 * | 0010    | ADC2               |
 * | 0011    | ADC3               |
 * | 0100    | ADC4               |
 * | 0101    | ADC5               |
 * | 0110    | ADC6               |
 * | 0111    | ADC7               |
 * |         |                    |
 * | 1110    | 1.30V (VBG)        |
 * | 1111    | 0V (GND)           |
 * --------------------------------
 *
 * =========================================================================
 *
 * ADCSRA: ADC Control and Status Register A
 * ------------------------------------------------------------
 * | 7    | 6    | 5    | 4    | 3    | 2     | 1     | 0     |
 * | ADEN | ADSC | ADFR | ADIF | ADIE | ADPS2 | ADPS1 | ADPS0 | 
 * ------------------------------------------------------------
 *
 * ADEN: Writing this bit to one enables the ADC. By writing it to
 * zero, the ADC is turned off. Turning the ADC off while a conversion
 * is in progress, will terminate this conversion.
 *
 * ADSC: In Single Conversion mode, write this bit to one to start
 * each conversion. In Free Running mode, write this bit to one to
 * start the first conversion.
 *
 * ADFR: When this bit is set (one) the ADC operates in Free Running
 * mode. In this mode, the ADC samples and updates the Data Registers
 * continuously.  Clearing th is bit (zero) will terminate Free
 * Running mode.
 *  
 * ADIF: This bit is set when an ADC conversion completes and the Data
 * Registers are updated. The ADC Conversion Complete Interrupt is
 * executed if the ADIE bit and the I-bit in SREG are set. ADIF is
 * cleared by hardware when executing th e corresponding interrupt
 * Handling Vector. Alter- natively, ADIF is cleared by writing a
 * logical one to the flag.
 *
 * ADIE: When this bit is written to one and the I-bit in SREG is set,
 * the ADC Conversion Complete Interrupt is activated.
 *
 * ------------------------------
 * | ADPS2..0 | Division Factor |
 * ------------------------------
 * | 000      | 2               |
 * | 001      | 2               |
 * | 010      | 4               |
 * | 011      | 8               |
 * | 100      | 16              |
 * | 101      | 32              |
 * | 110      | 64              |
 * | 111      | 128             |
 * ------------------------------
 *
 */

static void adc_init(uint8_t);
static uint8_t adc_single(void);

static void
adc_init(uint8_t channel) {
  ADMUX = (1 << REFS0) | (1 << ADLAR) | (channel & 0x7);
  ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

static uint8_t
adc_single() {
  /* A single conversion is started by writing a logical one to the
   * ADC Start Conversion bit, ADSC. This bit stays high as long as
   * the conversion is in progress and will be cleared by hardware
   * when the conversion is completed.
   */
  ADCSRA |= (1 << ADSC);
  while (!(ADCSRA & (1 << ADIF)));
  /* The conversion result can be found in the 
   * ADC Result Registers (ADCL, ADCH).
   */
  ADCSRA |= (1 << ADIF);
  return(ADCH);
}

void
adc_disable()
{
  ADCSRA &= ~(1 << ADEN);
}

uint8_t
adc_enable(uint8_t channel)
{
  adc_init(channel);
  return adc_single();
}
