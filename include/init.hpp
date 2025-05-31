#ifndef INIT_HPP
#define INIT_HPP

#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <sdl2-gdk/apiprefix.h>

#elif _WIN32
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <apiprefix.h>

#elif __APPLE__
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <apiprefix.h>

#else
#error "FATAL ERROR: Operating system not supported"

#endif

namespace gdk {
/**
 * @brief Initialize all sdl2-gdk dependencies
 */
void API_PREFIX initAll();

/**
 * @brief Quit all sdl2-gdk dependencies
 */
void API_PREFIX quitAll();
} // namespace gdk

#endif
