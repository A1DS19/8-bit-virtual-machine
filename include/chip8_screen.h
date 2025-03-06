#ifndef CHIP8_SCREEN_H
#define CHIP8_SCREEN_H

#include <stdbool.h>
#include "config.h"

typedef struct chip8_screen {
  bool pixels[CHIP8_SCREEN_HEIGHT][CHIP8_SCREEN_WIDTH];
} chip8_screen;

void chip8_screen_set(chip8_screen *screen, int x, int y);
bool chip8_screen_is_set(chip8_screen *screen, int x, int y);

#endif 
