#ifdef __linux__
#include <SDL2/SDL.h>

#elif _WIN32
#include <SDL.h>

#elif __APPLE__
#include <SDL.h>

#else
#error "FATAL ERROR: Operating system not supported"

#endif

namespace gdk {
/**
 * @brief Calculates FPS. Must be called every frame to get accurate result.
 * Best to call on beginning or end of main game loop
 */
int calculateFPS();
} // namespace gdk