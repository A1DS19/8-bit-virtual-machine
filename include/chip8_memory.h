#ifndef CHIP8_MEMORY_H
#define CHIP8_MEMORY_H

#include "config.h"

typedef struct chip8_memory {
  char memory[CHIP8_MEMORY_SIZE];
} chip8_memory;

void chip8_memory_set(chip8_memory *memory, int index, unsigned char value);
unsigned char chip8_memory_get(chip8_memory *memory, int index);

#endif

