#ifndef GYRO_H
#define GYRO_H

void gyro_init(void);

/* To write the internal MPU-60X0 registers, the master transmits the
 * start condition (S), followed by the I2C address and the write bit
 * (0). At the 9th clock cycle (when the clock is high), the MPU-60X0
 * acknowledges the transfer. Then the master puts the register
 * address (RA) on the bus. After the MPU-60X0 acknowledges the
 * reception of the register address, the master puts the register
 * data onto the bus. This is followed by the ACK signal, and data
 * transfer may be concluded by the stop condition (P). */
#define i2c_set_register(reg, val) i2c_write_byte(MPU60X0_SLAW, reg, val)

/* To read the internal MPU-60X0 registers, the master sends a start
 * condition, followed by the I2C address and a write bit, and then
 * the register address that is going to be read. Upon receiving the
 * ACK signal from the MPU-60X0, the master transmits a start signal
 * followed by the slave address and read bit. As a result, the
 * MPU-60X0 sends an ACK signal and the data. The communication ends
 * with a not acknowledge (NACK) signal and a stop bit from master. */
#define i2c_get_register(reg) i2c_read_byte(MPU60X0_SLAW,  MPU60X0_SLAR, reg)

#endif /* GYRO_H */
