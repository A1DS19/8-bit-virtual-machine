#ifndef CHIP8_REGISTERS_H
#define CHIP8_REGISTERS_H

#include "config.h"

typedef struct chip8_registers{
  unsigned char V[CHIP8_TOTAL_DATA_REGISTERS];
  unsigned short I;
  unsigned char delay_timer;
  unsigned char sound_timer;
  unsigned short PC;
  unsigned char SP;
} chip8_registers;

#endif
