#ifndef I2C_H
#define I2C_H

#define I2C_ACK      1
#define I2C_NACK     2
#define I2C_START    3
#define I2C_REPEATED 4


void i2c_init(void);
void i2c_write_byte(uint8_t, uint8_t, uint8_t);
uint8_t i2c_read_byte(uint8_t, uint8_t, uint8_t);
void i2c_read_bytes(uint8_t, uint8_t, uint8_t, uint8_t *, uint8_t);


#endif /* I2C_H */
