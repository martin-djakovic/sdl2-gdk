#ifdef __linux__
#include <sdl2-gdk/init.hpp>

#elif _WIN32
#include <init.hpp>

#elif __APPLE__
#include <init.hpp>

#else
#error "FATAL ERROR: Operating system not supported"

#endif

#include <cstdio>

namespace gdk {
void initAll() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    printf("Fatal error initializing SDL: %s\n", SDL_GetError());
  }

  if (TTF_Init() != 0) {
    printf("Fatal error initializing TTF: %s\n", TTF_GetError());
  }

  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    printf("Fatal error initializing Mixer: %s\n", Mix_GetError());
  }
}

void quitAll() {
  SDL_Quit();
  TTF_Quit();
  Mix_Quit();
}
} // namespace gdk
