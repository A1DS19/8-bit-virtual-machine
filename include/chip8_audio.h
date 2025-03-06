#ifndef CHIP8_AUDIO_H 
#define CHIP8_AUDIO_H

#include "SDL2/SDL_stdinc.h"

void audio_callback(void *userdata, Uint8 *stream, int len);
#endif
