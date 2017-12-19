#ifndef ACCEL_H
#define ACCEL_H

void accel_init(void);

#define i2c_set_register(reg, val) i2c_write_byte(MPU60X0_SLAW, reg, val)
#define i2c_get_register(reg) i2c_read_byte(MPU60X0_SLAW,  MPU60X0_SLAR, reg)

#endif /* ACCEL_H */
