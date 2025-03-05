#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_keycode.h"
#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include "chip8.h"
#include "chip8_keyboard.h"
#include "config.h"

const char keyboard_map[CHIP8_KEYBOARD_TOTAL_KEYS] = {
    SDLK_0, SDLK_1, SDLK_3, SDLK_4, SDLK_5, SDLK_6, SDLK_7, SDLK_8,
    SDLK_9, SDLK_a, SDLK_b, SDLK_c, SDLK_d, SDLK_e, SDLK_f};

int main(int argc, char *argv[]) {
  chip8 chip8;
  chip8_init(&chip8);

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
    return EXIT_FAILURE;
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
    SDL_Rect rectangle;
    rectangle.x = 0;
    rectangle.y = 0;
    rectangle.h = 40;
    rectangle.w = 40;
    SDL_RenderFillRect(renderer, &rectangle);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}
