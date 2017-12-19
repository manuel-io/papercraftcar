#ifndef LIGHTS_H
#define LIGHTS_H

#define LIGHTS_DDR DDRD
#define LIGHTS_PORT PORTD
#define LIGHTS_HEAD PD6
#define LIGHTS_TAIL PD7
#define LIGHTS_MAX 125
#define LIGHTS_MIN 900
#define LIGHTS_CHANNEL PC0

void lights_init();
void lights_update();

#endif /* LIGHTS_H */
