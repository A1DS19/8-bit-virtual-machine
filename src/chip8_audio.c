#include "chip8_audio.h"

#include "config.h"

void audio_callback(void *userdata, Uint8 *stream, int len) {
  Sint16 *buffer = (Sint16 *)stream;
  int length = len / 2; // 2 bytes per sample for AUDIO_S16SYS
  static double phase = 0.0;
  double phase_increment = 2.0 * M_PI * FREQUENCY / SAMPLE_RATE;

  for (int i = 0; i < length; i++, phase += phase_increment) {
    buffer[i] = (Sint16)(AMPLITUDE * sin(phase));
    if (phase >= 2.0 * M_PI) {
      phase -= 2.0 * M_PI;
    }
  }
}
