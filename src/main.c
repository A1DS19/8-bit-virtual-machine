#include <stdio.h>
#include <stdlib.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include "chip8.h"

int main(int argc, char *argv[]) {
  chip8 chip8;
  chip8.registers.V[0] = 100;
  printf("%us\n", chip8.registers.V[0]);
  chip8_memory_set(&chip8.memory, 50, 'z');
  printf("%c\n", chip8_memory_get(&chip8.memory, 10));

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

  printf("Window created successfully!\n");

  int running = 1;
  SDL_Event event;
  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);

  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        printf("goodbye");
        running = 0;
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
