#ifndef MPU60X0_H
#define MPU60X0_H

#define MPU60X0_ADDRESS 0x68

#define MPU60X0_SLAW ((MPU60X0_ADDRESS << 1) | 0)
#define MPU60X0_SLAR ((MPU60X0_ADDRESS << 1) | 1) 


/* This register specifies the divider from the gyroscope output rate
 * used to generate the Sample Rate for the MPU-60X0.
 */
#define SMPLRT_DIV     0x19

/* This register is used to trigger gyroscope self-test and configure
 * the gyroscopes’ full scale range.
 */
#define GYRO_CONFIG    0x1b

/* This register is used to trigger accelerometer self test and
 * configure the accelerometer full scale range. This register also
 * configures the Digital High Pass Filter (DHPF).
 */
#define ACCEL_CONFIG   0x1c

/* This register configures the external Frame Synchronization (FSYNC)
 * pin sampling and the Digital Low Pass Filter (DLPF) setting for
 * both the gyroscopes and accelerometers.
 */
#define CONFIG         0x1a

/* ############################################################# */
/* This register determines which sensor measurements are loaded into
 * the FIFO buffer.
 */
#define FIFO_EN        0x23

#define ACCEL_FIFO_EN  0x08
#define ZG_FIFO_EN     0x10
#define YG_FIFO_EN     0x20
#define XG_FIFO_EN     0x40
#define TEMP_FIFO_EN   0x80
/* ############################################################# */

/* ############################################################# */
/* This register enables interrupt generation by interrupt sources.
 */
#define INT_ENABLE     0x38

#define DATA_RDY_EN    0x01
#define FIFO_OFLOW_EN  0x10
/* ############################################################# */

/* ############################################################# */
/* This register shows the interrupt status of each interrupt
 * generation source. Each bit will clear after the register is read.
 */
#define INT_STATUS     0x3a

#define DATA_RDY_INT   0x01
#define I2C_MST_INT    0x08
#define FIFO_OFLOW_INT 0x10
/* ############################################################# */

/* These registers store the most recent accelerometer measurements.
 * Accelerometer measurements are written to these registers at the
 * Sample Rate as defined in Register 25.  The accelerometer
 * measurement registers, along with the temperature measurement
 * registers, gyroscope measurement registers, and external sensor
 * data registers, are composed of two sets of registers: an internal
 * register set and a user-facing read register set.  The data within
 * the accelerometer sensors’ internal register set is always updated
 * at the Sample Rate. Meanwhile, the user-facing read register set
 * duplicates the internal register set’s data values whenever the
 * serial interface is idle. This guarantees that a burst read of
 * sensor registers will read measurements from the same sampling
 * instant. Note that if burst reads are not used, the user is
 * responsible for ensuring a set of single byte reads correspond to a
 * single sampling instant by checking the Data Ready interrupt.
 */
#define ACCEL_XOUT_H   0x3b
#define ACCEL_XOUT_L   0x3c
#define ACCEL_YOUT_H   0x3d
#define ACCEL_YOUT_L   0x3e
#define ACCEL_ZOUT_H   0x3f
#define ACCEL_ZOUT_L   0x40

/* These registers store the most recent temperature sensor
 * measurement.  Temperature measurements are written to these
 * registers at the Sample Rate as defined in Register 25.  These
 * temperature measurement registers, along with the accelerometer
 * measurement registers, gyroscope measurement registers, and
 * external sensor data registers, are composed of two sets of
 * registers: an internal register set and a user-facing read register
 * set.  The data within the temperature sensor’s internal register
 * set is always updated at the Sample Rate.  Meanwhile, the
 * user-facing read register set duplicates the internal register
 * set’s data values whenever the serial interface is idle. This
 * guarantees that a burst read of sensor registers will read
 * measurements from the same sampling instant. Note that if burst
 * reads are not used, the user is responsible for ensuring a set of
 * single byte reads correspond to a single sampling instant by
 * checking the Data Ready interrupt.*/
#define TEMP_OUT_H     0x41
#define TEMP_OUT_L     0x42

/* These registers store the most recent gyroscope measurements.
 * Gyroscope measurements are written to these registers at the Sample
 * Rate as defined in Register 25.  These gyroscope measurement
 * registers, along with the accelerometer measurement registers,
 * temperature measurement registers, and external sensor data
 * registers, are composed of two sets of registers: an internal
 * register set and a user-facing read register set.  The data within
 * the gyroscope sensors’ internal register set is always updated at
 * the Sample Rate.  Meanwhile, the user-facing read register set
 * duplicates the internal register set’s data values whenever the
 * serial interface is idle. This guarantees that a burst read of
 * sensor registers will read measurements from the same sampling
 * instant. Note that if burst reads are not used, the user is
 * responsible for ensuring a set of single byte reads correspond to a
 * single sampling instant by checking the Data Ready interrupt.*/
#define GYRO_XOUT_H    0x43
#define GYRO_XOUT_L    0x44
#define GYRO_YOUT_H    0x45
#define GYRO_YOUT_L    0x46
#define GYRO_ZOUT_H    0x47
#define GYRO_ZOUT_L    0x48

/* ############################################################# */
/* This register allows the user to enable and disable the FIFO
 * buffer, I2C Master Mode, and primary I2C interface. The FIFO
 * buffer, I2C Master, sensor signal paths and sensor registers can
 * also be reset using this register.
 */
#define USER_CTRL      0x6a

/* This bit resets the FIFO buffer when set to 1 while FIFO_EN equals
 * 0. This bit automatically clears to 0 after the reset has been
 * triggered. */
#define FIFO_RESET     0x04

/* ############################################################# */

/* This register allows the user to configure the power mode and clock
 * source. It also provides a bit for resetting the entire device, and
 * a bit for disabling the temperature sensor.
 */
#define PWR_MGMT_1     0x6b

/* These registers keep track of the number of samples currently in
 * the FIFO buffer.
 */
#define FIFO_COUNT_H   0x72
#define FIFO_COUNT_L   0x73

/* This register is used to read and write data from the FIFO buffer.
 * Data is written to the FIFO in order of register number (from
 * lowest to highest). If all the FIFO enable flags (see below) are
 * enabled and all External Sensor Data registers (Registers 73 to 96)
 * are associated with a Slave device, the contents of registers 59
 * through 96 will be written in order at the Sample Rate.  The
 * contents of the sensor data registers (Registers 59 to 96) are
 * written into the FIFO buffer when their corresponding FIFO enable
 * flags are set to 1 in FIFO_EN (Register 35). An additional flag for
 * 2 the sensor data registers associated with I C Slave 3 can be
 * found in I2C_MST_CTRL (Register 36). If the FIFO buffer has
 * overflowed, the status bit FIFO_OFLOW_INT is automatically set to
 * 1. This bit is located in INT_STATUS (Register 58). When the FIFO
 * buffer has overflowed, the oldest data will be lost and new data
 * will be written to the FIFO. If the FIFO buffer is empty, reading
 * this register will return the last byte that was previously read
 * from the FIFO until new data is available. The user should check
 * FIFO_COUNT to ensure that the FIFO buffer is not read when empty.
 */
#define FIFO_R_W       0x74

/* This register is used to verify the identity of the device. The
 * contents of WHO_AM_I are the upper 6 bits of the MPU-60X0’s 7-bit
 * I2C address. The least significant bit of the MPU-60X0’s I2C
 * address is determined by the value of the AD0 pin. The value of the
 * AD0 pin is not reflected in this register. The default value of
 * the register is 0x68.
 */
#define WHO_AM_I       0x75

#endif /* MPU60X0_H */
