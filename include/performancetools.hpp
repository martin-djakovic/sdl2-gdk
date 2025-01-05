#include <SDL2/SDL.h>

namespace gdk {
/**
 * @brief Calculates FPS. Must be called every frame to get accurate result.
 * Best to call on beginning or end of main game loop
 */
int calculateFPS();
} // namespace gdk