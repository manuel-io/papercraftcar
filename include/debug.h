#ifndef DEBUG_H
#define DEBUG_H

#define DEBUG_UART 1
#define DEBUG_LOG 2

void debug_init(uint8_t);
void debug_update(void);
void debug_message(char *);

#endif /* DEBUG_H */
