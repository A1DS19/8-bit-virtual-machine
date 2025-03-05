#ifndef CHIP8_KEYBOARD_H 
#define CHIP8_KEYBOARD_H 

#include <stdbool.h>
#include "config.h"

typedef struct chip8_keyboard {
  bool keyboard[CHIP8_KEYBOARD_TOTAL_KEYS];
} chip8_keyboard;

int chip8_keyboard_map(const char *map, char key);
void chip8_keyboard_down(chip8_keyboard *keyboard, int key);
void chip8_keyboard_up(chip8_keyboard *keyboard, int key);
bool chip8_keyboard_is_down(chip8_keyboard *keyboard, int key);

#endif
