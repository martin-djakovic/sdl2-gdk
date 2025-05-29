#include <sdl2-gdk/performancetools.hpp>

namespace gdk {
static Uint64 last_time;
static Uint64 current_time = SDL_GetPerformanceCounter();

int calculateFPS() {
  last_time = current_time;
  current_time = SDL_GetPerformanceCounter();
  return 1.0f /
         ((current_time - last_time) / (float)SDL_GetPerformanceFrequency());
}
} // namespace gdk