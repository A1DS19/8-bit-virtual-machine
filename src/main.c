#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_keycode.h"
#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include "chip8.h"
#include "chip8_audio.h"
#include "chip8_keyboard.h"
#include "chip8_screen.h"
#include "config.h"

const char keyboard_map[CHIP8_KEYBOARD_TOTAL_KEYS] = {
    SDLK_0, SDLK_1, SDLK_3, SDLK_4, SDLK_5, SDLK_6, SDLK_7, SDLK_8,
    SDLK_9, SDLK_a, SDLK_b, SDLK_c, SDLK_d, SDLK_e, SDLK_f};

int main(int argc, char *argv[]) {
  SDL_AudioSpec desired_spec;
  SDL_zero(desired_spec);
  desired_spec.freq = SAMPLE_RATE;
  desired_spec.format = AUDIO_S16SYS;
  desired_spec.channels = 1;
  desired_spec.samples = 4096;
  desired_spec.callback = audio_callback;

  chip8 chip8;
  chip8_init(&chip8);
  chip8.registers.sound_timer = 3;

  chip8_screen_draw_sprite(&chip8.screen, 0, 0, &chip8.memory.memory[0x00], 5);

  if (SDL_Init(SDL_INIT_EVERYTHING)) {
    fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  if (SDL_OpenAudio(&desired_spec, NULL) < 0) {
    fprintf(stderr, "Could not open audio: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  SDL_Window *window =
      SDL_CreateWindow(CHIP8_SCREEN_TITLE,      // Window title
                       SDL_WINDOWPOS_UNDEFINED, // Initial x position (centered)
                       SDL_WINDOWPOS_UNDEFINED, // Initial y position (centered)
                       CHIP8_WINDOW_MW,         // Width
                       CHIP8_WINDOW_MH,         // Height
                       SDL_WINDOW_SHOWN         // Flags (show the window)
      );

  if (!window) {
    fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
    SDL_Quit();
    return EXIT_FAILURE;
  }

  int running = 1;
  SDL_Event event;
  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);

  while (running) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        running = 0;
        break;

      case SDL_KEYDOWN: {
        char key = event.key.keysym.sym;
        int vkey = chip8_keyboard_map(keyboard_map, key);
        if (vkey != -1) {
          chip8_keyboard_down(&chip8.keyboard, vkey);
        }
      } break;

      case SDL_KEYUP: {
        char key = event.key.keysym.sym;
        int vkey = chip8_keyboard_map(keyboard_map, key);
        if (vkey != -1) {
          chip8_keyboard_up(&chip8.keyboard, vkey);
        }

      } break;
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);

    for (int x = 0; x < CHIP8_SCREEN_WIDTH; x++) {
      for (int y = 0; y < CHIP8_SCREEN_HEIGHT; y++) {
        if (chip8_screen_is_set(&chip8.screen, x, y)) {
          SDL_Rect rectangle;
          rectangle.x = x * CHIP8_WINDOW_MULTIPLIER;
          rectangle.y = y * CHIP8_WINDOW_MULTIPLIER;
          rectangle.h = CHIP8_WINDOW_MULTIPLIER;
          rectangle.w = CHIP8_WINDOW_MULTIPLIER;
          SDL_RenderFillRect(renderer, &rectangle);
        }
      }
    }

    SDL_RenderPresent(renderer);

    if (chip8.registers.delay_timer > 0) {
      usleep(100000);
      chip8.registers.delay_timer -= 1;
    }

    if (chip8.registers.sound_timer > 0) {
      SDL_PauseAudio(0);
      sleep(1);
      chip8.registers.sound_timer -= 1;
    } else {
      SDL_PauseAudio(1);
    }
  }

  SDL_DestroyWindow(window);
  SDL_CloseAudio();

  SDL_Quit();

  return EXIT_SUCCESS;
}
