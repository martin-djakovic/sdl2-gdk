#ifdef __linux__
#include <sdl2-gdk/performancetools.hpp>

#elif _WIN32
#include <performancetools.hpp>

#elif __APPLE__
#include <performancetools.hpp>

#else
#error "FATAL ERROR: Operating system not supported"

#endif

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