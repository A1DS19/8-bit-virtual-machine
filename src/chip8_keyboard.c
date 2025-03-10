#include "chip8_keyboard.h"

#include <assert.h>

void chip8_keyboard_set_map(chip8_keyboard *keyboard, const char *map) {
  keyboard->keyboard_map = map;
}

static void chip8_keyboard_is_bounds(int key) {
  assert(key >= 0 && key < 0x0f);
}

int chip8_keyboard_map(chip8_keyboard *keyboard, char key) {
  for (int i = 0; i < CHIP8_KEYBOARD_TOTAL_KEYS; i++) {
    if (keyboard->keyboard_map[i] == key) {
      return i;
    }
  }

  return -1;
}

void chip8_keyboard_down(chip8_keyboard *keyboard, int key) {
  keyboard->keyboard[key] = true;
}

void chip8_keyboard_up(chip8_keyboard *keyboard, int key) {
  keyboard->keyboard[key] = false;
}

bool chip8_keyboard_is_down(chip8_keyboard *keyboard, int key) {
  return keyboard->keyboard[key];
}
