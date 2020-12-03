#ifndef VM_H
#define VM_H

#include <stdint.h>

void start();
void cleanup(int signal);

void set_pixel(int x, int y, uint8_t pixel);
void updateDisplay();
void setDisplayMode(int displayMode);

extern uint8_t *ram;
extern uint16_t reg[];
extern int displayMode;

#endif
