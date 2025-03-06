#ifndef CONFIG_H
#define CONFIG_H 

#define CHIP8_MEMORY_SIZE 4096
#define CHIP8_SCREEN_WIDTH 64
#define CHIP8_SCREEN_HEIGHT 32
#define CHIP8_SCREEN_TITLE "chip8 vm"
#define CHIP8_WINDOW_MULTIPLIER 10
#define CHIP8_WINDOW_MW ((CHIP8_WINDOW_MULTIPLIER) * (CHIP8_SCREEN_WIDTH))
#define CHIP8_WINDOW_MH ((CHIP8_WINDOW_MULTIPLIER) * (CHIP8_SCREEN_HEIGHT))
#define CHIP8_TOTAL_DATA_REGISTERS 16
#define CHIP8_TOTAL_STACK_DEPTH 16
#define CHIP8_KEYBOARD_TOTAL_KEYS 16
#define CHIP8_CHARACTER_SET_LOAD_ADDR 0x00

#endif
