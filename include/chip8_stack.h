#ifndef CHIP8_STACK_H
#define CHIP8_STACK_H

#include "config.h"

struct chip8;

typedef struct chip8_stack {
  unsigned short stack[CHIP8_TOTAL_STACK_DEPTH];
} chip8_stack;

void chip8_stack_push(struct chip8 *chip8, unsigned short value);
unsigned short chip8_stack_pop(struct chip8 *chip8);

#endif
