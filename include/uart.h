#ifndef UART_H
#define UART_H

#define IS_SET(r, b) (r & (1 << b)) >> b == 1
#define IS_UNSET(r, b) (r & (1 << b)) >> b == 0

void uart_init(void);
uint8_t uart_getc(void);
void uart_putc(uint8_t);
void uart_putstr(uint8_t *);

#endif /* UART_H */
