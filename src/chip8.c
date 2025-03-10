#include "chip8.h"

#include <assert.h>
#include <memory.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "SDL2/SDL_events.h"
#include "chip8_keyboard.h"
#include "chip8_memory.h"
#include "chip8_screen.h"
#include "chip8_stack.h"
#include "config.h"

const char chip8_default_char_set[] = {
    0xf0, 0x90, 0x90, 0x90, 0xf0, 0x20, 0x60, 0x20, 0x20, 0x70, 0xf0, 0x10,
    0xf0, 0x80, 0xf0, 0xf0, 0x10, 0xf0, 0x10, 0xf0, 0x90, 0x90, 0xf0, 0x10,
    0x10, 0xf0, 0x80, 0xf0, 0x10, 0xf0, 0xf0, 0x80, 0xf0, 0x90, 0xf0, 0xf0,
    0x10, 0x20, 0x40, 0x40, 0xf0, 0x90, 0xf0, 0x90, 0xf0, 0xf0, 0x90, 0xf0,
    0x10, 0xf0, 0xf0, 0x90, 0xf0, 0x90, 0x90, 0xe0, 0x90, 0xe0, 0x90, 0xe0,
    0xf0, 0x80, 0x80, 0x80, 0xf0, 0xe0, 0x90, 0x90, 0x90, 0xe0, 0xf0, 0x80,
    0xf0, 0x80, 0xf0, 0xf0, 0x80, 0xf0, 0x80, 0x80};

void chip8_init(chip8 *chip8_) {
  // since this is initialized to a particular instance of chip8 I passed
  // sizeof(*chip8_) to be for that specific instance

  memset(chip8_, 0, sizeof(*chip8_));
  memcpy(&chip8_->memory.memory, chip8_default_char_set,
         sizeof(chip8_default_char_set));
}

static void chip8_exec_extented_8(chip8 *chip8, unsigned short opcode) {
  unsigned char x = (opcode >> 8) & 0x000f;
  unsigned char y = (opcode >> 4) & 0x000f;
  unsigned char final_4_bits = opcode & 0x000f;
  unsigned short tmp = 0;

  switch (final_4_bits) {
  // 8xy0 - ld vx, vy, vx = vy
  case 0x00:
    chip8->registers.V[x] = chip8->registers.V[y];
    break;

  // 8xy1 - or Vx, Vy, performs Or
  case 0x01:
    chip8->registers.V[x] = chip8->registers.V[x] | chip8->registers.V[y];
    break;

  // 8xy2 - or Vx, Vy, performs and
  case 0x02:
    chip8->registers.V[x] = chip8->registers.V[x] & chip8->registers.V[y];
    break;

  // 8xy3 - xor Vx, Vy, performs XOR
  case 0x03:
    chip8->registers.V[x] = chip8->registers.V[x] ^ chip8->registers.V[y];
    break;

  // 8xy4 - add Vx with Vy
  case 0x04:
    tmp = chip8->registers.V[x] + chip8->registers.V[y];
    chip8->registers.V[0x0f] = false;

    if (tmp > 0xff) {
      chip8->registers.V[0x0f] = true;
    }

    chip8->registers.V[x] = tmp;
    break;

    // 8xy5 - sub Vx - Vy
  case 0x05:
    chip8->registers.V[0x0f] = false;
    if (chip8->registers.V[x] > chip8->registers.V[y]) {
      chip8->registers.V[0x0f] = true;
    }
    chip8->registers.V[x] = chip8->registers.V[x] - chip8->registers.V[y];
    break;

  // 8xy6 - SHR Vx {, Vy}
  case 0x06:
    chip8->registers.V[0x0f] = chip8->registers.V[x] & 0x01;
    chip8->registers.V[x] /= 2;
    break;

  // 8xy7 - SUBN Vx, Vy
  case 0x07:
    chip8->registers.V[0x0f] = chip8->registers.V[y] > chip8->registers.V[x];
    chip8->registers.V[x] = chip8->registers.V[y] - chip8->registers.V[x];
    break;

  // 8xy8 - SHL, Vx
  case 0x08:
    chip8->registers.V[0x0f] = chip8->registers.V[x] & 0b10000000;
    chip8->registers.V[x] *= 2;
    break;
  }
}

static char chip8_wait_for_keypress(chip8 *chip8) {
  SDL_Event event;
  while (SDL_WaitEvent(&event)) {
    if (event.type != SDL_KEYDOWN) {
      continue;
    }

    char c = event.key.keysym.sym;
    char chip8_key = chip8_keyboard_map(&chip8->keyboard, c);

    if (chip8_key != -1) {
      return chip8_key;
    }
  }

  return -1;
}

static void chip8_exec_extended_f(chip8 *chip8, unsigned short opcode) {
  unsigned char x = (opcode >> 8) & 0x000f;
  switch (opcode & 0x00ff) {
  case 0x07:
    chip8->registers.V[x] = chip8->registers.delay_timer;
    break;

  case 0x0A: {
    char pressed_key = chip8_wait_for_keypress(chip8);
    chip8->registers.V[x] = pressed_key;
    break;
  }

  case 0x15:
    chip8->registers.delay_timer = chip8->registers.V[x];
    break;

  case 0x18:
    chip8->registers.sound_timer = chip8->registers.V[x];
    break;

  case 0x1e:
    chip8->registers.I += chip8->registers.V[x];
    break;

  case 0x29:
    chip8->registers.I = chip8->registers.V[x] * CHIP8_DEFAULT_SPRITE_HEIGHT;
    break;

  case 0x33: {
    unsigned char hundreds = chip8->registers.V[x] / 100;
    unsigned char tens = chip8->registers.V[x] / 10 % 10;
    unsigned char units = chip8->registers.V[x] % 10;

    chip8_memory_set(&chip8->memory, chip8->registers.I, hundreds);
    chip8_memory_set(&chip8->memory, chip8->registers.I + 1, tens);
    chip8_memory_set(&chip8->memory, chip8->registers.I + 2, units);

    break;
  }

  case 0x55: {
    for (int i = 0; i <= x; i++) {
      chip8_memory_set(&chip8->memory, chip8->registers.I + 1,
                       chip8->registers.V[i]);
    }
    break;
  }

  case 0x65: {
    for (int i = 0; i <= x; i++) {
      chip8->registers.V[i] =
          chip8_memory_get(&chip8->memory, chip8->registers.I + 1);
    }
    break;
  }
  }
}

static void chip8_exec_extended(chip8 *chip8, unsigned short opcode) {
  unsigned short nnn = opcode & 0x0fff;
  unsigned char x = (opcode >> 8) & 0x000f;
  unsigned char y = (opcode >> 4) & 0x000f;
  unsigned char kk = opcode & 0x00ff;
  unsigned char n = opcode & 0x000f;

  switch (opcode & 0xf000) {
  // jmp to location nnn
  case 0x1000:
    chip8->registers.PC = nnn;
    break;

  // call subroutine, at location nnn
  case 0x2000:
    chip8_stack_push(chip8, chip8->registers.PC);
    chip8->registers.PC = nnn;
    break;

  // SE Vx, byte - 3xSkip next instruction if Vx=kk
  case 0x3000:
    if (chip8->registers.V[x] == kk) {
      chip8->registers.PC += 2;
    }
    break;

    // SE Vx, byte - 3xSkip next instruction if Vx!=kk
  case 0x4000:
    if (chip8->registers.V[x] != kk) {
      chip8->registers.PC += 2;
    }
    break;

  // 5xy0 - SE, vx, vy, skip the next isntruction if vx = vy
  case 0x5000:
    if (chip8->registers.V[x] == chip8->registers.V[y]) {
      chip8->registers.PC += 2;
    }
    break;

  // LD Vx, byte = kk
  case 0x6000:
    chip8->registers.V[x] = kk;
    break;

  // 7xkk - add Vx byte, set vx += kk
  case 0x7000:
    chip8->registers.V[x] += kk;
    break;

  case 0x8000:
    chip8_exec_extented_8(chip8, opcode);
    break;

  // 8xkk - SNE
  case 0x9000:
    if (chip8->registers.V[x] != chip8->registers.V[y]) {
      chip8->registers.PC += 2;
    }
    break;

  // 0XAnnn
  case 0xA000:
    chip8->registers.I = nnn;
    break;

  // 0xB000
  case 0xB000:
    chip8->registers.PC = nnn + chip8->registers.V[0x00];
    break;

  // Cxkk - RND
  case 0xC000:
    srand(clock());
    chip8->registers.V[x] = (rand() % 255) & kk;
    break;

  // Dxkk - draw sprites instruction
  case 0xD000: {
    const char *sprite = &chip8->memory.memory[chip8->registers.I];
    chip8->registers.V[0x0f] =
        chip8_screen_draw_sprite(&chip8->screen, chip8->registers.V[x],
                                 chip8->registers.V[y], sprite, n);
    break;
  }
  // Exkk - SKP Vx, skip instruction if key is Vx
  case 0xE000: {
    switch (opcode & 0x0ff) {
    case 0x9e:
      if (chip8_keyboard_is_down(&chip8->keyboard, chip8->registers.V[x])) {
        chip8->registers.PC += 2;
      }
      break;

    case 0xa1:
      if (!chip8_keyboard_is_down(&chip8->keyboard, chip8->registers.V[x])) {
        chip8->registers.PC += 2;
      }
      break;
    }
    break;
  }

  case 0XF000:
    chip8_exec_extended_f(chip8, opcode);
    break;
  }
}

void chip8_exec(chip8 *chip8, unsigned short opcode) {
  switch (opcode) {
  // clear screen
  case 0x00E0:
    chip8_screen_clear(&chip8->screen);
    break;

  // return from subroutine
  case 0x00EE:
    chip8->registers.PC = chip8_stack_pop(chip8);
    break;

  default:
    chip8_exec_extended(chip8, opcode);
  }
}

void chip8_load(chip8 *chip8_, const char *buf, size_t size) {
  assert(size + CHIP8_PROGRAM_LOAD_ADDR < CHIP8_MEMORY_SIZE);
  memcpy(&chip8_->memory.memory[CHIP8_PROGRAM_LOAD_ADDR], buf, size);
  chip8_->registers.PC = CHIP8_PROGRAM_LOAD_ADDR;
}
