#include "chip8.h"

#include <assert.h>
#include <memory.h>

#include "chip8_stack.h"

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

  switch (final_4_bits) {
  // 8xy0 - ld vx, vy, vx = vy
  case 0x00:
    chip8->registers.V[x] = chip8->registers.V[y];
    break;
  }
}

static void chip8_exec_extended(chip8 *chip8, unsigned short opcode) {
  unsigned short nnn = opcode & 0x0fff;
  unsigned char x = (opcode >> 8) & 0x000f;
  unsigned char y = (opcode >> 4) & 0x000f;
  unsigned char kk = opcode & 0x00ff;

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
