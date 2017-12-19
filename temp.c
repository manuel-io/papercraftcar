#include <avr/io.h>
#include <util/delay.h>

#include "include/i2c.h"
#include "include/mpu60x0.h"
#include "include/temp.h"

extern uint16_t temperature;

void
temp_init()
{
  /* The MPU-60X0 contains a 1024-byte FIFO register that is
   * accessible via the Serial Interface. The FIFO configuration
   * register determines which data is written into the FIFO. Possible
   * choices include gyro data, accelerometer data, temperature
   * readings, auxiliary sensor readings, and FSYNC input. A FIFO
   * counter keeps track of how many bytes of valid data are contained
   * in the FIFO. The FIFO register supports burst reads. The
   * interrupt function may be used to determine when new data is
   * available. For further information regarding the FIFO, please
   * refer to the MPU-6000/MPU-6050 Register Map and Register
   * Descriptions document.
   */
  i2c_set_register(FIFO_EN, TEMP_FIFO_EN);
}

void
temp_update()
{
  uint16_t value = 0;

  /* 16-bit unsigned value. Indicates the number of bytes stored in
   * the FIFO buffer. This number is in turn the number of bytes that
   * can be read from the FIFO buffer and it is directly proportional
   * to the number of samples available given the set of sensor data
   * bound to be stored in the FIFO (register 35 and 36). */

  value |= (i2c_get_register(FIFO_R_W) << 8);
  value |= (i2c_get_register(FIFO_R_W) & 0xff);

  /* Temperature in degrees C = (TEMP_OUT Register Value as a signed
   * quantity)/340 + 36.53
   */
  temperature = (int16_t)(value/340 + 36.53);
  
}
