#ifndef CHIP8_H
#define CHIP8_H

#include "chip8_memory.h"
#include "chip8_registers.h"
#include "chip8_stack.h"

typedef struct chip8 {
  chip8_memory memory;
  chip8_registers registers;
  chip8_stack stack;
} chip8;

#endif 
