#include <avr/io.h>

#include "include/i2c.h"
#include "include/debug.h"

static void i2c_start(uint8_t, uint8_t);
static void i2c_stop(void);
static void i2c_wait(uint8_t);
static void i2c_repeated(uint8_t);
static uint8_t i2c_read(uint8_t);
static void i2c_write(uint8_t);

/* Master Transmitter Mode*/
static void
i2c_wait(uint8_t addr)
{
  while (1) {
 
    /* Communication on the I2C bus starts when the master puts the
     * START condition (S) on the bus, which is defined as a HIGH to
     * LOW transition of the SDA line while SCL line is HIG H */
    TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);  
    while (!(TWCR & (1 << TWINT)));
    
    /* A START condition has been transmitted */
    if ((TWSR & 0xf8) != 0x08) {
      continue;
    }

    TWDR = addr;
    TWCR = (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1<<TWINT)));

    switch (TWSR & 0xf8) {

      /* SLA+W has been transmitted; ACK has been received */
      case 0x18:
        //debug_message("SLA+W ACK\r");
        return;
        break;

      /* SLA+W has been transmitted; NOT ACK has been received */
      case 0x20:
        //debug_message("SLA+W NACK\r");
        break;
    }
    
    i2c_stop();
    continue;
  }
}

/* Master Receiver Mode */
static void
i2c_repeated(uint8_t addr)
{
  TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
  while (!(TWCR & (1 << TWINT)));

  /* A repeated START condition has been transmitted */
  if ((TWSR & 0xf8) != 0x10) {
    return;
  }
  
  /* SLA+R will be transmitted */
  TWDR = addr;
  TWCR = (1 << TWEN) | (1 << TWINT);
  while (!(TWCR & (1 << TWINT)));
  
  switch (TWSR & 0xf8) {
  
    /* SLA+R has been transmitted; ACK has been received */
    case 0x40:
      //debug_message("REPEATED ACK\r");
      break;
    
    /* SLA+R has been transmitted; NOT ACK has been received */
    case 0x48:
      //debug_message("REPEATED NACK\r");
      break;
  }
}

static uint8_t
i2c_read(uint8_t handshake)
{
  switch (handshake) {

    case I2C_ACK:
      TWCR |= (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
      break;

    case I2C_NACK:
      TWCR |= (1 << TWEN) | (1 << TWINT);
      break;

    default:
      TWCR |= (1 << TWEN) | (1 << TWINT);
  }

  while (!(TWCR & (1 << TWINT)));

  switch (TWSR & 0xf8) {

    /* Arbitration lost in SLA+R or NOT ACK bit */
    case 0x38:
      //debug_message("ERR ACK\r");
      break;

    /* Data byte has been received; ACK has been returned */
    case 0x50:
      //debug_message("READ ACK\r");
      break;

    /* Data byte has been received; NOT ACK has been returned */
    case 0x58:
      //debug_message("READ NACK\r");
      break;
  }

  return TWDR;
}

static void
i2c_write(uint8_t dat)
{
  TWDR = dat;
  TWCR = (1 << TWEN) | (1 << TWINT);
  while (!(TWCR & (1 << TWINT)));

  switch (TWSR & 0xf8) {
 
    /* Data byte has been transmitted; ACK has been received */
    case 0x28:
      //debug_message("WRITE ACK\r");
      break;

    /* Data byte has been transmitted; NOT ACK has been received */
    case 0x30:
      //debug_message("WRITE NACK\r");
      break;

    /* Arbitration lost in SLA+W or data bytes */
    case 0x38:
      //debug_message("WRITE ERR\r");
      break;
  }
}

static void
i2c_start(uint8_t typ, uint8_t addr)
{
  if (typ == I2C_REPEATED) {
    i2c_repeated(addr);
    return;
  }
  
  i2c_wait(addr);
}

static void
i2c_stop()
{
  /* The bus is considered to be busy until the master puts a STOP
   * condition (P) on the bus, which is defined as a LOW to HIGH
   * transition on the SDA line while SCL is HIGH */
  TWCR |= (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);
  while (TWCR & (1 << TWSTO));
}

void
i2c_init()
{
  TWBR = 0x8;
  TWSR &= ~(1 << TWPS1) & ~(1 << TWPS0);
}

void
i2c_write_byte(uint8_t slaw, uint8_t reg, uint8_t val)
{
  /* I2C data bytes are defined to be 8 bits long. Each byte transferred
   * must be followed by an acknowledge (ACK) signal.
   *
   * After beginning communications with the START condition (S), the
   * master sends a 7-bit slave address followed by an 8th bit, the
   * read/write bit. The read/write bit indicates whether the master
   * is receiving data fromor  is  writing  to  the  slave device.
   * Then, the master releases the SDA line and waits for the
   * acknowledgesignal (ACK) from the slave device. */
  i2c_start(I2C_START, slaw);
  i2c_write(reg);
  i2c_write(val);
  i2c_stop();
}

uint8_t
i2c_read_byte(uint8_t slaw, uint8_t slar, uint8_t reg)
{
  uint8_t val;
  i2c_start(I2C_START, slaw);
  i2c_write(reg);
  i2c_start(I2C_REPEATED, slar);
  val = i2c_read(I2C_NACK);
  i2c_stop();

  return val;
}

void
i2c_read_bytes(uint8_t slaw, uint8_t slar, uint8_t reg, uint8_t *dat, uint8_t s)
{
  i2c_start(I2C_START, slaw);
  i2c_write(reg);
  i2c_start(I2C_REPEATED, slar);

  for (uint8_t i = 0; i < (s-1); i++) {
    dat[i] = i2c_read(I2C_ACK);
  }

  dat[s-1] = i2c_read(I2C_NACK); 
  i2c_stop();
}
