#include <avr/io.h>
#include <util/delay.h>

#include "include/i2c.h"
#include "include/mpu60x0.h"
#include "include/accel.h"

void
accel_init()
{
  /* Selects the full scale range of the accelerometer outputs.
   *
   * ± 16g */
  i2c_set_register(ACCEL_CONFIG, 0x18);
}
