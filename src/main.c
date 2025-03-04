#include <stdio.h>
#include <stdlib.h>

#include "SDL2/SDL.h"

int main(int argc, char *argv[]) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  // Create a window
  SDL_Window *window =
      SDL_CreateWindow("Chip 8 VM",             // Window title
                       SDL_WINDOWPOS_UNDEFINED, // Initial x position (centered)
                       SDL_WINDOWPOS_UNDEFINED, // Initial y position (centered)
                       640,                     // Width
                       320,                     // Height
                       SDL_WINDOW_SHOWN         // Flags (show the window)
      );

  if (!window) {
    fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
    SDL_Quit();
    return EXIT_FAILURE;
  }

  printf("Window created successfully!\n");

  // Event loop
  int running = 1; // Flag to keep the program running
  SDL_Event event;

  while (running) {
    // Process events
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        // Exit the loop when the user closes the window
        running = 0;
      }
    }

    // Add rendering code here if needed
  }

  // Clean up
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}
