#ifndef CHIP8_H
#define CHIP8_H

#include "chip8_memory.h"
#include "chip8_registers.h"
#include "chip8_stack.h"
#include "chip8_keyboard.h"
#include "chip8_screen.h"
#include <stddef.h>

typedef struct chip8 {
  chip8_memory memory;
  chip8_registers registers;
  chip8_stack stack;
  chip8_keyboard keyboard;
  chip8_screen screen;
} chip8;

void chip8_init(chip8 *chip8_);
void chip8_exec(chip8 *chip8_, unsigned short opcode);
void chip8_load(chip8 *chip8_, const char* buf, size_t size);

#endif 
