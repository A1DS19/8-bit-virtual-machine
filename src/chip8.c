#include "chip8.h"

#include <memory.h>

void chip8_init(chip8 *chip8_) {
  // since this is initialized to a particular instance of chip8 I passed
  // sizeof(*chip8_) to be for that specific instance

  memset(chip8_, 0, sizeof(*chip8_));
}
