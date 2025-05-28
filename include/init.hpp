#ifndef INIT_HPP
#define INIT_HPP

#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#elif _WIN32
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#elif __APPLE__
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#else
#error "FATAL ERROR: Operating system not supported"

#endif

namespace gdk {
/**
 * @brief Initialize all sdl2-gdk dependencies
 */
void initAll();

/**
 * @brief Quit all sdl2-gdk dependencies
 */
void quitAll();
} // namespace gdk

#endif
